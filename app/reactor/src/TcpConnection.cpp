#include "TcpConnection.h"
#include "Logger.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"

#include <sys/socket.h>
#include <unistd.h>

TcpConnection::TcpConnection(EventLoop *loop, const std::string &name, int sockfd, const InetAddress &localAddr, const InetAddress &peerAddr)
    : loop_(loop),
      name_(name),
      state_(Connecting),
      reading_(true),
      localAddr_(localAddr),
      peerAddr_(peerAddr),
      socket_(new Socket(sockfd)),
      channel_(new Channel(loop, sockfd)),
      highWaterMark_(64 * 1024 * 1024)
{
  if (loop == nullptr)
  {
    LOG_FATAL("Loop is nullptr");
  }

  channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
  channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
  channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
  channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));

  LOG_INFO("TcpConnection::ctor[%s] at fd=%d", name_.c_str(), sockfd);

  socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
  LOG_INFO("TcpConnection::dtor[%s] at fd=%d", name_.c_str(), socket_->getFd());
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
  int err = 0;
  ssize_t n = inputBuffer_.readFd(channel_->fd(), &err);
  if (n > 0)
  {
    messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
  }
  else if (n == 0)
  {
    handleClose();
  }
  else
  {
    LOG_ERROR("TcpConnection::handleRead");
    handleError();
  }
}

void TcpConnection::handleWrite()
{
  if (channel_->isWriting())
  {
    ssize_t n = outputBuffer_.writeFd(channel_->fd(), nullptr);
    if (n > 0)
    {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.readableSize() == 0)
      {
        channel_->disableWriting();
        if (writeCompleteCallback_)
        {
          loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
        }
        if (state_ == Disconnecting)
        {
          shutdownInLoop();
        }
      }
    }
    else
    {
      LOG_ERROR("TcpConnection::handleWrite");
    }
  }
  else
  {
    LOG_ERROR("TcpConnection fd=%d can't be written", channel_->fd());
  }
}

void TcpConnection::handleClose()
{
  LOG_INFO("TcpConnection::handleClose state=%d", static_cast<int>(state_));
  setState(Disconnected);
  channel_->disableAll();

  TcpConnectionPtr connPtr(shared_from_this());
  if (connectionCallback_)
  {
    connectionCallback_(connPtr);
  }
  if (closeCallback_)
  {
    closeCallback_(connPtr);
  }
}

void TcpConnection::handleError()
{
  int optval;
  socklen_t optlen = sizeof optval;
  int err = getsockopt(channel_->fd(), SOL_SOCKET, SO_ERROR, &optval, &optlen);
  if (err < 0)
  {
    LOG_ERROR("TcpConnection::handleError");
  }
  else
  {
    LOG_ERROR("TcpConnection::handleError name=%s - SO_ERROR=%d", name_.c_str(), optval);
  }
}

void TcpConnection::send(const void *message, size_t len)
{
  if (state_ == Connected)
  {
    if (loop_->isInLoopThread())
    {
      sendInLoop(message, len);
    }
    else
    {
      loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, message, len));
    }
  }
}

void TcpConnection::sendInLoop(const void *message, size_t len)
{
  ssize_t nwrote = 0;
  size_t remaining = len;
  bool faultError = false;

  if (state_ == Disconnected)
  {
    LOG_ERROR("disconnected, give up writing");
    return;
  }

  if (!channel_->isWriting() && outputBuffer_.readableSize() == 0)
  {
    nwrote = ::write(channel_->fd(), message, len);
    if (nwrote >= 0)
    {
      remaining = len - nwrote;
      if (remaining == 0 && writeCompleteCallback_)
      {
        loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
      }
    }
    else
    {
      nwrote = 0;
      if (errno != EWOULDBLOCK)
      {
        LOG_ERROR("TcpConnection::sendInLoop");
        if (errno == ECONNRESET || errno == ENOTCONN)
        {
          faultError = true;
        }
      }
    }
  }

  if (!faultError && remaining > 0)
  {
    size_t oldLen = outputBuffer_.readableSize();
    if (oldLen + remaining >= highWaterMark_ && oldLen < highWaterMark_ && highWaterMarkCallback_)
    {
      loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
    }
    outputBuffer_.append((char *)message + nwrote, remaining);
    if (!channel_->isWriting())
    {
      channel_->enableWriting();
    }
  }
}

void TcpConnection::establishConnection()
{
  setState(Connected);
  channel_->tie(shared_from_this());
  channel_->enableReading();

  connectionCallback_(shared_from_this());
}

void TcpConnection::destroyConnection()
{
  if (state_ == Connected)
  {
    setState(Disconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());
  }
  channel_->remove();
}

void TcpConnection::shutdown()
{
  if (state_ == Connected)
  {
    setState(Disconnecting);
    loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
  }
}

void TcpConnection::shutdownInLoop()
{
  if (!channel_->isWriting())
  {
    socket_->shutdownWrite();
  }
}
