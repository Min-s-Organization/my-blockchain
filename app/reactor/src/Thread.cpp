#include "Thread.h"
#include "CurrentThread.h"
#include <semaphore.h>

std::atomic<int> Thread::numCreated_{0};

Thread::Thread(ThreadFunc func, const std::string &name)
    : started_(false), joined_(false), thread_(nullptr), tid_(0), func_(std::move(func)), name_(name)
{
  setDefaultName();
}

void Thread::setDefaultName()
{
  int num = ++numCreated_;
  if (name_.empty())
  {
    char buf[32];
    snprintf(buf, sizeof buf, "Thread%d", num);
    name_ = buf;
  }
}

Thread::~Thread()
{
  if (started_ && !joined_)
  {
    thread_->detach();
  }
}

void Thread::start()
{
  started_ = true;
  sem_t sem;
  sem_init(&sem, false, 0);
  thread_ = std::shared_ptr<std::thread>(new std::thread([&]()
                                                         {
    tid_ = CurrentThread::tid();
    sem_post(&sem);
    func_(); }));
  sem_wait(&sem);
}

void Thread::join()
{
  joined_ = true;
  thread_->join();
}
