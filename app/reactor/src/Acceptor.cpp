#include "unistd.h"
#include "Acceptor.h"
#include "Logger.h"

static int createNonblockingFd()
{
  int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  if (sockfd < 0)
    LOG_FATAL("Fail to create socket fd");
  return sockfd;
}

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listen_addr, bool reuseport)
    : loop_(loop),
      acceptSocket_(createNonblockingFd()),
      acceptChannel_(loop, acceptSocket_.getFd()),
      listening_(false)
{
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.setReusePort(reuseport);
  acceptSocket_.bindAddress(listen_addr);
  acceptChannel_.setReadCallback(std::bind(&Acceptor::handleAccept, this));
}

Acceptor::~Acceptor()
{
  acceptChannel_.disableAll();
  acceptChannel_.remove();
}

void Acceptor::listen()
{
  listening_ = true;
  acceptSocket_.listen();
  // register the accept channel in Poller
  // so that the new connection could be detected
  acceptChannel_.enableReading();
}

void Acceptor::handleAccept()
{
  InetAddress peeraddr(0);
  int conn_fd = acceptSocket_.accept(peeraddr);
  if (conn_fd > 0)
  {
    if (newConnectionCallback_)
      newConnectionCallback_(conn_fd, peeraddr);
    else
      ::close(conn_fd);
  }
  else
  {
    LOG_ERROR("Fail to accept new connection");
    if (errno == EMFILE)
      LOG_ERROR("Socket fd reaches limit");
  }
}