#include "Buffer.h"
#include <sys/uio.h>
#include <unistd.h>

ssize_t Buffer::readFd(int fd, int *save_errno)
{
  char extra_buffer[1024 * 64] = {0};

  struct iovec vec[2];
  const size_t writable = writableSize();

  vec[0].iov_base = beginWrite();
  vec[0].iov_len = writable;
  vec[1].iov_base = extra_buffer;
  vec[1].iov_len = sizeof(extra_buffer);

  const int iovcnt = (writable < sizeof(extra_buffer)) ? 2 : 1;
  const ssize_t n = ::readv(fd, vec, iovcnt);
  if (n < 0)
    *save_errno = errno;
  else if (n < writable)
    writeIndex_ += n;
  else
  {
    writeIndex_ = buffer_.size();
    append(extra_buffer, n - writable);
  }
  return n;
}

ssize_t Buffer::writeFd(int fd, int *save_errno)
{
  ssize_t n = ::write(fd, peek(), readableSize());
  if (n < 0)
    *save_errno = errno;
  return n;
}