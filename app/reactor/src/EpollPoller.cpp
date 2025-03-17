#include "EpollPoller.h"
#include "Logger.h"
#include "Channel.h"
#include "Timestamp.h"
#include <cstring>
#include <unistd.h>

const int NewChannel = -1;
const int ActiveChannel = 1;
const int DeleteChannel = 2;

EpollPoller::EpollPoller(EventLoop *loop)
    : Poller(loop), epollfd_(::epoll_create1(EPOLL_CLOEXEC)), events_(InitEventListSize)
{
  if (epollfd_ < 0)
  {
    LOG_FATAL("epoll_create error: %s", strerror(errno));
  }
}

EpollPoller::~EpollPoller()
{
  ::close(epollfd_);
}

Timestamp EpollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
  LOG_DEBUG("fd total count: %d", channels_.size());

  int numEvents = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
  Timestamp now(Timestamp::now());
  if (numEvents > 0)
  {
    LOG_DEBUG("%d events happened", numEvents);
    fillActiveChannels(numEvents, activeChannels);
    if (numEvents == events_.size())
    {
      events_.resize(events_.size() * 2);
    }
  }
  else if (numEvents == 0)
  {
    LOG_DEBUG("%d events happened", numEvents);
  }
  else
  {
    LOG_ERROR("epoll_wait error: %s", strerror(errno));
  }
  return now;
}

void EpollPoller::updateChannel(Channel *channel)
{
  const int index = channel->index();
  LOG_DEBUG("epoll_ctl op=%d, fd=%d, events=%d", index, channel->fd(), channel->events());
  if (index == NewChannel || index == DeleteChannel)
  {
    if (index == NewChannel)
    {
      int fd = channel->fd();
      channels_[fd] = channel;
    }
    channel->set_index(ActiveChannel);
    update(EPOLL_CTL_ADD, channel);
  }
  else
  {
    int fd = channel->fd();
    if (channel->isNoneEvent())
    {
      update(EPOLL_CTL_DEL, channel);
      channel->set_index(DeleteChannel);
    }
    else
    {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}

void EpollPoller::removeChannel(Channel *channel)
{
  int fd = channel->fd();
  channels_.erase(fd);

  int index = channel->index();
  if (index == ActiveChannel)
  {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->set_index(NewChannel);
}

void EpollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
  for (int i = 0; i < numEvents; ++i)
  {
    Channel *channel = static_cast<Channel *>(events_[i].data.ptr);
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}

void EpollPoller::update(int operation, Channel *channel)
{
  epoll_event event;
  memset(&event, 0, sizeof(event));
  int fd = channel->fd();
  event.events = channel->events();
  event.data.fd = fd;
  event.data.ptr = channel;

  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
  {
    if (operation == EPOLL_CTL_DEL)
    {
      LOG_ERROR("epoll_ctl del error: %s", strerror(errno));
    }
    else
    {
      LOG_FATAL("epoll_ctl add/mod error: %s", strerror(errno));
    }
  }
}