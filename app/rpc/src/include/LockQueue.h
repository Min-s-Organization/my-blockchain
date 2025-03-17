#ifndef LOCK_QUEUE_H
#define LOCK_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class LockQueue
{
public:
  void push(const T &item)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(item);
    cond_variable_.notify_one();
  }
  T pop()
  {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty())
    {
      cond_variable_.wait(lock);
    }
    T item = queue_.front();
    queue_.pop();
    return item;
  }

private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_variable_;
};

#endif
