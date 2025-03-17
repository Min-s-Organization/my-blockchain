#pragma once

#include "noncopyable.h"

#include <memory>
#include <functional>
#include <sys/epoll.h>

class EventLoop;
class Timestamp;

/**
 * The channel class includes file descriptor and related events.
 */
class Channel : public noncopyable
{
public:
  using EventCallback = std::function<void()>;
  using ReadEventCallback = std::function<void(Timestamp)>;

  Channel(EventLoop *loop, int fd);
  ~Channel();

  void handleEvent(Timestamp receiveTime);

  void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
  void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
  void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

  void tie(const std::shared_ptr<void> &);

  int fd() const { return fd_; }
  int events() const { return events_; }
  void set_revents(int revt) { revents_ = revt; }

  bool isNoneEvent() const { return events_ == NoneEvent; }
  bool isWriting() const { return events_ & WriteEvent; }
  bool isReading() const { return events_ & ReadEvent; }

  void enableReading()
  {
    events_ |= ReadEvent;
    update();
  }
  void disableReading()
  {
    events_ &= ~ReadEvent;
    update();
  }
  void enableWriting()
  {
    events_ |= WriteEvent;
    update();
  }
  void disableWriting()
  {
    events_ &= ~WriteEvent;
    update();
  }
  void disableAll()
  {
    events_ = NoneEvent;
    update();
  }

  int index() { return index_; }
  void set_index(int idx) { index_ = idx; }

  EventLoop *ownerLoop() { return loop_; }
  void remove();

private:
  static const int NoneEvent = 0;
  static const int ReadEvent = EPOLLIN | EPOLLPRI;
  static const int WriteEvent = EPOLLOUT;

  EventLoop *loop_;
  int fd_;
  int events_;  // Channel's interested events
  int revents_; // Events that occurred
  int index_;

  std::weak_ptr<void> tie_;
  bool tied_;

  void update();
  void handleEventWithGuard(Timestamp receiveTime);

  ReadEventCallback readCallback_;
  EventCallback writeCallback_;
  EventCallback closeCallback_;
  EventCallback errorCallback_;
};