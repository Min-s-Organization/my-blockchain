#pragma once

#include <string>

#include "noncopyable.h"

#define LOG_INFO(format, ...)                          \
  do                                                   \
  {                                                    \
    Logger &logger = Logger::instance();               \
    logger.setLogLevel(INFO);                          \
    char buf[1024] = {0};                              \
    snprintf(buf, sizeof(buf), format, ##__VA_ARGS__); \
    logger.log(buf, __FILE__, __LINE__);               \
  } while (0)

#define LOG_WARN(format, ...)                          \
  do                                                   \
  {                                                    \
    Logger &logger = Logger::instance();               \
    logger.setLogLevel(WARN);                          \
    char buf[1024] = {0};                              \
    snprintf(buf, sizeof(buf), format, ##__VA_ARGS__); \
    logger.log(buf, __FILE__, __LINE__);               \
  } while (0)

#define LOG_ERROR(format, ...)                         \
  do                                                   \
  {                                                    \
    Logger &logger = Logger::instance();               \
    logger.setLogLevel(ERROR);                         \
    char buf[1024] = {0};                              \
    snprintf(buf, sizeof(buf), format, ##__VA_ARGS__); \
    logger.log(buf, __FILE__, __LINE__);               \
  } while (0)

#define LOG_FATAL(format, ...)                         \
  do                                                   \
  {                                                    \
    Logger &logger = Logger::instance();               \
    logger.setLogLevel(FATAL);                         \
    char buf[1024] = {0};                              \
    snprintf(buf, sizeof(buf), format, ##__VA_ARGS__); \
    logger.log(buf, __FILE__, __LINE__);               \
    exit(-1);                                          \
  } while (0)

#ifdef ENABLE_DEBUG
#define LOG_DEBUG(format, ...)                         \
  do                                                   \
  {                                                    \
    Logger &logger = Logger::instance();               \
    logger.setLogLevel(DEBUG);                         \
    char buf[1024] = {0};                              \
    snprintf(buf, sizeof(buf), format, ##__VA_ARGS__); \
    logger.log(buf, __FILE__, __LINE__);               \
  } while (0)
#else
#define LOG_DEBUG(format, ...)
#endif

enum LogLevel
{
  INFO,
  WARN,
  ERROR,
  FATAL,
  DEBUG
};

class Logger : public noncopyable
{
public:
  static Logger &instance();
  void setLogLevel(LogLevel level);
  void log(std::string msg, const char *file, int line);

private:
  int logLevel_;
  Logger();
};