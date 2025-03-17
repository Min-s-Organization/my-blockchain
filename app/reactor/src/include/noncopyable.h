#pragma once

/**
 * This class is used to prevent copy and assignment of the class.
 * It is a base class for all classes that should not be copied.
 */
class noncopyable
{
public:
  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;

protected:
  noncopyable() = default;
  ~noncopyable() = default;
};
