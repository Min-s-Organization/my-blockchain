#pragma once

#include <functional>
#include <unordered_map>
#include <atomic>

#include "noncopyable.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "EventLoopThreadPool.h"
#include "Callback.h"
#include "TcpConnection.h"
#include "Logger.h"

class TcpServer : noncopyable
{
public:
  using ThreadInitCallback = std::function<void(EventLoop *)>;

  enum Option
  {
    DisableReusePort,
    EnableReusePort,
  };

  TcpServer(EventLoop *loop, const InetAddress &listenAddr, const std::string &nameArg, Option option = DisableReusePort);
  ~TcpServer();

  void setThreadNum(int numThreads);
  void start();

  void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; };
  void setThreadInitCallback(const ThreadInitCallback &cb) { threadInitCallback_ = cb; };
  void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; };
  void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; };

private:
  using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;

  EventLoop *loop_;
  const std::string ipPort_;
  const std::string name_;
  std::unique_ptr<Acceptor> acceptor_;
  std::shared_ptr<EventLoopThreadPool> threadPool_;
  ThreadInitCallback threadInitCallback_;

  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;

  std::atomic_int started_;
  int nextConnId_;
  ConnectionMap connections_;

  void newConnection(int sockfd, const InetAddress &peerAddr);
  void removeConnection(const TcpConnectionPtr &conn);
  void removeConnectionInLoop(const TcpConnectionPtr &conn);
};