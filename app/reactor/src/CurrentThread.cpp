#include "CurrentThread.h"
#include <unistd.h>
#include <sys/syscall.h>

namespace CurrentThread
{
  thread_local int t_cachedTid = 0;

  void cacheTid()
  {
    if (t_cachedTid == 0)
      t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
  }
}