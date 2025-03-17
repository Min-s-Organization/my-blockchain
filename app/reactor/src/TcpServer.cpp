#include "TcpServer.h"
#include "Logger.h"
#include "TcpConnection.h"

#include <cstring>

TcpServer::TcpServer(EventLoop *loop, const InetAddress &listenAddr, const std::string &name, Option option)
    : loop_(loop),
      ipPort_(listenAddr.toIpPort()),
      name_(name),
      acceptor_(new Acceptor(loop, listenAddr, option == EnableReusePort)),
      threadPool_(new EventLoopThreadPool(loop, name)),
      started_(0),
      nextConnId_(1)
{
  if (loop == nullptr)
  {
    LOG_FATAL("Loop is nullptr");
  }

  acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
}

void TcpServer::setThreadNum(int numThreads)
{
  threadPool_->setThreadNum(numThreads);
}

void TcpServer::start()
{
  if (started_.fetch_add(1) == 0)
  {
    threadPool_->start(threadInitCallback_);
    loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
  }
}

void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr)
{
  EventLoop *ioLoop = threadPool_->getNextLoop();
  char buf[64] = {0};
  snprintf(buf, sizeof(buf), "-%s#%d", ipPort_.c_str(), nextConnId_);
  ++nextConnId_;
  std::string connName = name_ + buf;

  LOG_INFO("TcpServer::newConnection[%s] - new connection [%s] from %s", name_.c_str(), connName.c_str(), peerAddr.toIpPort().c_str());

  sockaddr_in local;
  ::memset(&local, 0, sizeof(local));
  socklen_t len = sizeof(local);
  if (::getsockname(sockfd, (sockaddr *)&local, &len) < 0)
  {
    LOG_ERROR("TcpServer::newConnection getsockname error");
  }
  InetAddress localAddr(local);

  TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
  connections_[connName] = conn;
  conn->setConnectionCallback(connectionCallback_);
  conn->setMessageCallback(messageCallback_);
  conn->setWriteCompleteCallback(writeCompleteCallback_);
  conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));

  ioLoop->runInLoop(std::bind(&TcpConnection::establishConnection, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
  loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn)
{
  LOG_INFO("TcpServer::removeConnectionInLoop[%s] - connection %s", name_.c_str(), conn->getName().c_str());

  size_t n = connections_.erase(conn->getName());
  (void)n;
  EventLoop *ioLoop = conn->getLoop();
  ioLoop->queueInLoop(std::bind(&TcpConnection::destroyConnection, conn));
}