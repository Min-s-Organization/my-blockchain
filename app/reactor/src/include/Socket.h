#ifndef SOCKET_H
#define SOCKET_H

#include "InetAddress.h"

class Socket
{
public:
  explicit Socket(int sockfd)
      : sockfd_(sockfd)
  {
  }
  ~Socket();

  int getFd() const { return sockfd_; }
  void bindAddress(const InetAddress &localaddr);
  void listen();
  int accept(InetAddress &peeraddr);
  void setReuseAddr(bool on);
  void setReusePort(bool on);

  void setKeepAlive(bool on);
  void shutdownWrite();

private:
  const int sockfd_;
};

#endif