#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>

#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"

class EventLoop;

class Acceptor
{
public:
  using NewConnectionCallback = std::function<void(int sockfd, const InetAddress &)>;

  Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport);
  ~Acceptor();
  // Set callback for new connections
  void setNewConnectionCallback(const NewConnectionCallback &cb) { newConnectionCallback_ = cb; }
  // Check if currently listening
  bool isListening() const { return listening_; }
  // Listen on local port
  void listen();

private:
  void handleAccept(); // Handle new connection events

  EventLoop *loop_;
  Socket acceptSocket_;
  Channel acceptChannel_;
  NewConnectionCallback newConnectionCallback_;
  bool listening_;
};

#endif