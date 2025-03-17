#pragma once

#include "noncopyable.h"
#include "Timestamp.h"
#include "Channel.h"
#include "CurrentThread.h"
#include <atomic>
#include <functional>
#include <vector>
#include <mutex>

class Poller;

class EventLoop : public noncopyable
{
public:
  using Functor = std::function<void()>;

  EventLoop();
  ~EventLoop();

  void loop();
  void quit();

  Timestamp pollReturnTime() const { return pollReturnTime_; }

  void runInLoop(Functor &&cb);
  void queueInLoop(Functor &&cb);

  void wakeup();

  void updateChannel(Channel *channel);
  void removeChannel(Channel *channel);
  bool hasChannel(Channel *channel);

  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

private:
  void handleRead();
  void callPendingFunctors();

  using ChannelList = std::vector<Channel *>;
  ChannelList activeChannels_;

  std::atomic_bool looping_;
  std::atomic_bool quit_;

  const pid_t threadId_;
  Timestamp pollReturnTime_;
  std::unique_ptr<Poller> poller_;

  int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;

  std::atomic_bool callingPendingFunctors_;
  std::vector<Functor> pendingFunctors_;
  std::mutex mutex_;
};