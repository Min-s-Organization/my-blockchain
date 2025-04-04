#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"
#include <sys/epoll.h>

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false)
{
}

Channel::~Channel()
{
}

void Channel::tie(const std::shared_ptr<void> &obj)
{
  tie_ = obj;
  tied_ = true;
}

/**
 * Update the channel in the poller via EventLoop.
 */
void Channel::update()
{
  loop_->updateChannel(this);
}

void Channel::remove()
{
  loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
  if (tied_)
  {
    std::shared_ptr<void> guard = tie_.lock();
    if (guard)
    {
      handleEventWithGuard(receiveTime);
    }
  }
  else
  {
    handleEventWithGuard(receiveTime);
  }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
  if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
  {
    if (closeCallback_)
    {
      closeCallback_();
    }
  }

  if (revents_ & EPOLLERR)
  {
    if (errorCallback_)
    {
      errorCallback_();
    }
  }

  if (revents_ & (EPOLLIN | EPOLLPRI))
  {
    if (readCallback_)
    {
      readCallback_(receiveTime);
    }
  }

  if (revents_ & EPOLLOUT)
  {
    if (writeCallback_)
    {
      writeCallback_();
    }
  }
}