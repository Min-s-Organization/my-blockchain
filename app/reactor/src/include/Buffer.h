#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <cstddef>
#include <string>

class Buffer
{
public:
  static constexpr size_t PrependSize = 8;
  static constexpr size_t InitialBufferSize = 1024;

  explicit Buffer(size_t initSize = InitialBufferSize)
      : buffer_(PrependSize + initSize),
        readIndex_(PrependSize),
        writeIndex_(PrependSize)
  {
  }

  size_t readableSize() const { return writeIndex_ - readIndex_; }
  size_t writableSize() const { return buffer_.size() - writeIndex_; }
  size_t prependableSize() const { return readIndex_; }

  const char *peek() const { return begin() + readIndex_; }
  char *beginWrite() { return begin() + writeIndex_; }
  const char *beginWrite() const { return begin() + writeIndex_; }

  void retrieve(size_t len)
  {
    if (len < readableSize())
    {
      readIndex_ += len;
    }
    else
    {
      retrieveAll();
    }
  }

  void retrieveAll()
  {
    readIndex_ = PrependSize;
    writeIndex_ = PrependSize;
  }

  std::string retrieveAllAsString()
  {
    return retrieveAsString(readableSize());
  }

  std::string retrieveAsString(size_t len)
  {
    std::string result(peek(), len);
    retrieve(len);
    return result;
  }

  void ensureWritableSize(size_t len)
  {
    if (writableSize() < len)
    {
      makeSpace(len);
    }
  }

  void append(char *data, size_t len)
  {
    ensureWritableSize(len);
    std::copy(data, data + len, beginWrite());
    writeIndex_ += len;
  }

  ssize_t readFd(int fd, int *saveErrno);
  ssize_t writeFd(int fd, int *saveErrno);

private:
  std::vector<char> buffer_;
  size_t readIndex_;
  size_t writeIndex_;

  char *begin() { return &*buffer_.begin(); }
  const char *begin() const { return &*buffer_.begin(); }

  void makeSpace(size_t len)
  {
    if (len + PrependSize > writableSize() + prependableSize())
    {
      buffer_.resize(writeIndex_ + len);
    }
    else
    {
      // move the readable buffer to the front
      std::copy(
          begin() + readIndex_,
          begin() + writeIndex_,
          begin() + PrependSize);
      readIndex_ = PrependSize;
      writeIndex_ = readIndex_ + readableSize();
    }
  }
};

#endif