#include <iostream>

#include "Logger.h"
#include "Timestamp.h"

Logger::Logger()
{
  logLevel_ = INFO;
}

Logger &Logger::instance()
{
  static Logger logger;
  return logger;
}

void Logger::setLogLevel(LogLevel level)
{
  logLevel_ = level;
}

void Logger::log(std::string msg, const char *file, int line)
{
  switch (logLevel_)
  {
  case INFO:
    std::cout << "[INFO]  ";
    break;
  case WARN:
    std::cout << "[WARN]  ";
    break;
  case ERROR:
    std::cout << "[ERROR] ";
    break;
  case FATAL:
    std::cout << "[FATAL] ";
    break;
  case DEBUG:
    std::cout << "[DEBUG] ";
    break;
  }
  std::cout << Timestamp::now().toString() << " " << msg << " " << file << ":" << line << std::endl;
}
