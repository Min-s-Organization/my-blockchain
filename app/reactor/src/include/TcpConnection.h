#pragma once

#include "noncopyable.h"
#include "InetAddress.h"
#include "Callback.h"
#include "Buffer.h"

#include <atomic>
#include <memory>
#include <string>

class EventLoop;
class Socket;
class Channel;

class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
  enum State
  {
    Disconnected,
    Disconnecting,
    Connecting,
    Connected,
  };

  TcpConnection(EventLoop *loop, const std::string &name, int sockfd, const InetAddress &localAddr, const InetAddress &peerAddr);
  ~TcpConnection();

  EventLoop *getLoop() const { return loop_; }
  const std::string &getName() const { return name_; }
  const InetAddress &getLocalAddress() const { return localAddr_; }
  const InetAddress &getPeerAddress() const { return peerAddr_; }

  bool isConnected() const { return state_ == Connected; }

  void establishConnection();
  void destroyConnection();

  void send(const void *message, size_t len);
  void shutdown();

  void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
  void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
  void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }
  void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }
  void setHighWaterMarkCallback(const HighWaterMarkCallback &cb, size_t highWaterMark)
  {
    highWaterMarkCallback_ = cb;
    highWaterMark_ = highWaterMark;
  }

  void setState(State s) { state_ = s; }

private:
  EventLoop *loop_;

  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;

  std::string name_;
  std::atomic_int state_;
  bool reading_;

  const InetAddress localAddr_;
  const InetAddress peerAddr_;

  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;
  CloseCallback closeCallback_;
  HighWaterMarkCallback highWaterMarkCallback_;

  size_t highWaterMark_;

  Buffer inputBuffer_;
  Buffer outputBuffer_;

  void handleRead(Timestamp receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();

  void sendInLoop(const void *message, size_t len);
  void shutdownInLoop();
};
