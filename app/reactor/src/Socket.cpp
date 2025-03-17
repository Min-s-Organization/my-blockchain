#include "Socket.h"

#include <sys/socket.h>
#include <unistd.h>
#include "Logger.h"

Socket::~Socket()
{
  close(sockfd_);
}

void Socket::bindAddress(const InetAddress &localaddr)
{
  int ret = bind(sockfd_, (struct sockaddr *)localaddr.getSockAddr(), sizeof(sockaddr_in));
  if (ret != 0)
  {
    LOG_FATAL("Fail to bind the address");
  }
}

void Socket::listen()
{
  int ret = ::listen(sockfd_, 1024);
  if (ret != 0)
  {
    LOG_FATAL("Fail to listen");
  }
}

int Socket::accept(InetAddress &peeraddr)
{
  socklen_t len = sizeof(peeraddr);
  int conn_fd = ::accept4(sockfd_, (sockaddr *)peeraddr.getSockAddr(), &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (conn_fd < 0)
  {
    LOG_FATAL("Fail to accept");
  }
  return conn_fd;
}

void Socket::setReuseAddr(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

void Socket::setReusePort(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
}

void Socket::setKeepAlive(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
}

void Socket::shutdownWrite()
{
  if (::shutdown(sockfd_, SHUT_WR) < 0)
  {
    LOG_ERROR("Socket::shutdownWrite");
  }
}