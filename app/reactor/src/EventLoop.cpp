#include "EventLoop.h"
#include "Logger.h"
#include "Poller.h"
#include "Channel.h"
#include <unistd.h>
#include <sys/eventfd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

thread_local EventLoop *t_loopInThisThread = nullptr;

const int PollTimeoutMs = 10000;

int createEventfd()
{
  int eventFd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (eventFd < 0)
  {
    LOG_FATAL("Create eventfd error: %s", strerror(errno));
  }
  return eventFd;
}

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      threadId_(CurrentThread::tid()),
      poller_(Poller::newDefaultPoller(this)),
      wakeupFd_(createEventfd()),
      wakeupChannel_(new Channel(this, wakeupFd_)),
      callingPendingFunctors_(false)
{
  LOG_DEBUG("EventLoop created %p in thread %d", this, threadId_);
  if (t_loopInThisThread)
  {
    LOG_FATAL("EventLoop %p exists in this thread %d", t_loopInThisThread, threadId_);
  }
  else
  {
    t_loopInThisThread = this;
  }

  wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
  wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
  wakeupChannel_->disableAll();
  wakeupChannel_->remove();
  ::close(wakeupFd_);
  t_loopInThisThread = nullptr;
}

void EventLoop::handleRead()
{
  uint64_t data = 1;
  ssize_t n = ::read(wakeupFd_, &data, sizeof data);
  if (n != sizeof data)
  {
    LOG_ERROR("EventLoop::handleRead() reads %ld bytes instead of %ld", (long)n, (long)sizeof(data));
  }
}

void EventLoop::loop()
{
  LOG_INFO("EventLoop %p start looping", this);

  looping_ = true;
  quit_ = false;

  while (!quit_)
  {
    activeChannels_.clear();
    pollReturnTime_ = poller_->poll(PollTimeoutMs, &activeChannels_);
    for (Channel *channel : activeChannels_)
    {
      channel->handleEvent(pollReturnTime_);
    }
    callPendingFunctors();
  }

  LOG_INFO("EventLoop %p stop looping", this);
  looping_ = false;
}

void EventLoop::quit()
{
  quit_ = true;
  if (!isInLoopThread())
  {
    wakeup();
  }
}

void EventLoop::runInLoop(Functor &&cb)
{
  if (isInLoopThread())
  {
    cb();
  }
  else
  {
    queueInLoop(std::move(cb));
  }
}
void EventLoop::queueInLoop(Functor &&cb)
{
  {
    std::unique_lock<std::mutex> lock(mutex_);
    pendingFunctors_.emplace_back(cb);
  }
  if (!isInLoopThread() || callingPendingFunctors_)
  {
    wakeup();
  }
}

void EventLoop::wakeup()
{
  uint64_t data = 1;
  ssize_t n = ::write(wakeupFd_, &data, sizeof data);
  if (n != sizeof data)
  {
    LOG_ERROR("EventLoop::wakeup() writes %ld bytes instead of %ld", (long)n, (long)sizeof(data));
  }
}

void EventLoop::updateChannel(Channel *channel)
{
  poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
  poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel)
{
  return poller_->hasChannel(channel);
}

void EventLoop::callPendingFunctors()
{
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;

  {
    std::unique_lock<std::mutex> lock(mutex_);
    functors.swap(pendingFunctors_);
  }
  for (const Functor &functor : functors)
  {
    functor();
  }
  callingPendingFunctors_ = false;
}
