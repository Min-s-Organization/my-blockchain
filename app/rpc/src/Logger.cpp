#include <Logger.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <sys/stat.h>

Logger &Logger::instance()
{
  static Logger instance;
  return instance;
}

Logger::Logger()
{
  std::thread write_log_task([&]()
                             {
    while (true)
    {
      time_t now = time(nullptr);
      tm *now_tm = localtime(&now);

      // create log directory if not exists
      struct stat st;
      if (stat("log", &st) == -1)
      {
        mkdir("log", 0777);
      }

      char filename[128];
      sprintf(filename, "log/log_%d_%d_%d", now_tm->tm_year + 1900, now_tm->tm_mon + 1, now_tm->tm_mday);

      FILE *file = fopen(filename, "a");
      if (file == nullptr)
      {
        printf("open file %s failed\n", filename);
        exit(1);
      }

      std::string msg = lock_queue_.pop();
      fputs(msg.c_str(), file);
      fclose(file);
    } });
  write_log_task.detach();
}

void Logger::log(LogLevel logLevel, std::string filepath, std::string funcname, int line, std::string msg)
{
  std::string level = "INFO ";
  switch (logLevel)
  {
  case LogLevel::DEBUG:
    level = "DEBUG";
    break;
  case LogLevel::INFO:
    level = "INFO ";
    break;
  case LogLevel::WARN:
    level = "WARN ";
    break;
  case LogLevel::ERROR:
    level = "ERROR";
    break;
  case LogLevel::FATAL:
    level = "FATAL";
    break;
  }

  // generate current time
  // format: Sun Feb 16 17:05:45.123 2025
  auto now = std::chrono::system_clock::now();
  std::time_t time = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
  std::string timeStr = std::ctime(&time);
  timeStr = timeStr.substr(0, timeStr.length() - 1);
  timeStr = timeStr.substr(0, timeStr.length() - 5) + "." + std::to_string(ms.count()).substr(0, 2) + " " + timeStr.substr(timeStr.length() - 4);
  // get thread id and convert to string
  std::stringstream ss;
  ss << std::this_thread::get_id();
  std::string threadID = ss.str();
  // extract file name from path
  std::string filename = filepath.substr(filepath.find_last_of("/") + 1);

  lock_queue_.push(timeStr + " " + level + " " + threadID + " - " + msg + " " + filename + ":" + std::to_string(line) + "(" + funcname + ")\n");
  printf("%s %s %s - %s %s:%d(%s)\n", timeStr.c_str(), level.c_str(), threadID.c_str(), msg.c_str(), filename.c_str(), line, funcname.c_str());
}