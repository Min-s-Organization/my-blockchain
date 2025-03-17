#ifndef MY_PROJECT_LOGGER_H
#define MY_PROJECT_LOGGER_H

#include <string>
#include <iostream>
#include "LockQueue.h"

enum class LogLevel
{
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
};

class Logger
{
private:
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(Logger &&) = delete;

  LogLevel current_level_ = LogLevel::INFO;
  LockQueue<std::string> lock_queue_;

public:
  static Logger &instance();
  Logger();
  ~Logger() = default;

  void log(LogLevel level, std::string filepath, std::string funcname, int line, std::string msg);
  void setLogLevel(LogLevel level) { current_level_ = level; }
  LogLevel getLogLevel() const { return current_level_; }
};

template <typename... Args>
void logMsg(LogLevel level, std::string filepath, std::string funcname, int line, const char *format, Args... args)
{
  Logger &logger = Logger::instance();

  if constexpr (sizeof...(args) > 0)
  {
    char buffer[1024];
    std::snprintf(buffer, sizeof(buffer), format, args...);
    logger.log(level, filepath, funcname, line, buffer);
  }
  else
  {
    logger.log(level, filepath, funcname, line, format);
  }
}

#define LOG_DEBUG(format, ...)                                                    \
  do                                                                              \
  {                                                                               \
    logMsg(LogLevel::DEBUG, __FILE__, __func__, __LINE__, format, ##__VA_ARGS__); \
  } while (0)

#define LOG_INFO(format, ...)                                                    \
  do                                                                             \
  {                                                                              \
    logMsg(LogLevel::INFO, __FILE__, __func__, __LINE__, format, ##__VA_ARGS__); \
  } while (0)

#define LOG_WARN(format, ...)                                                    \
  do                                                                             \
  {                                                                              \
    logMsg(LogLevel::WARN, __FILE__, __func__, __LINE__, format, ##__VA_ARGS__); \
  } while (0)

#define LOG_ERROR(format, ...)                                                    \
  do                                                                              \
  {                                                                               \
    logMsg(LogLevel::ERROR, __FILE__, __func__, __LINE__, format, ##__VA_ARGS__); \
  } while (0)

#define LOG_FATAL(format, ...)                                                    \
  do                                                                              \
  {                                                                               \
    logMsg(LogLevel::FATAL, __FILE__, __func__, __LINE__, format, ##__VA_ARGS__); \
  } while (0)

#endif