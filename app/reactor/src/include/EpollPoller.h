#pragma once

#include "Poller.h"

#include <sys/epoll.h>

class EpollPoller : public Poller
{
public:
  EpollPoller(EventLoop *loop);
  ~EpollPoller() override;

  Timestamp poll(int timeoutMs, ChannelList *activeChannels) override;
  void updateChannel(Channel *channel) override;
  void removeChannel(Channel *channel) override;

private:
  static const int InitEventListSize = 16;
  using EventList = std::vector<epoll_event>;

  void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
  void update(int operation, Channel *channel);

  int epollfd_;
  EventList events_;
};