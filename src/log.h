#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__
#include <stdint.h>

#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "singleton.h"
#include "utils.h"

namespace sylar {

class LoggerManager;

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_LEVEL(logger, level)                                                                               \
  if (logger->getLevel() <= level)                                                                                   \
  sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, __FILE__, __LINE__, 0,                 \
                                                               sylar::GetThreadId(), sylar::GetFiberId(), time(0)))) \
      .getSS()

#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...)                                                                 \
  if (logger->getLevel() <= level)                                                                                   \
  sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, __FILE__, __LINE__, 0,                 \
                                                               sylar::GetThreadId(), sylar::GetFiberId(), time(0)))) \
      .getEvent()                                                                                                    \
      ->format(fmt, __VA_ARGS__)

#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)

#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()
#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)

class Logger;

class LogLevel {
 public:
  enum Level { UNKNOW = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, FATAL = 5 };
  static const char* ToString(LogLevel::Level level);
  static LogLevel::Level FromString(const std::string& str);
};

class LogEvent {
 public:
  using ptr = std::shared_ptr<LogEvent>;
  LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char* file, int32_t line, uint32_t elapse,
           uint32_t thread_id, uint32_t fiber_id, uint64_t time);
  const char* getFile() const { return m_file; }
  int32_t getLine() const { return m_line; }
  uint32_t getElapse() const { return m_elapse; }
  uint32_t getThreadId() const { return m_threadId; }
  uint32_t getFiberId() const { return m_fiberId; }
  uint64_t getTime() const { return m_time; }
  const std::string getContent() const { return m_ss.str(); }
  std::shared_ptr<Logger> getLogger() const { return m_logger; }
  LogLevel::Level getLevel() const { return m_level; }

  std::stringstream& getSS() { return m_ss; }

  void format(const char* fmt, ...);
  void format(const char* fmt, va_list al);

 private:
  std::shared_ptr<Logger> m_logger;
  LogLevel::Level m_level;
  const char* m_file = nullptr;
  int32_t m_line = 0;
  uint32_t m_elapse = 0;
  uint32_t m_threadId = 0;
  uint32_t m_fiberId = 0;
  uint32_t m_time;
  std::stringstream m_ss;
};

class LogEventWrap {
 public:
  LogEventWrap(LogEvent::ptr event);
  ~LogEventWrap();
  std::stringstream& getSS();
  std::shared_ptr<LogEvent> getEvent();

 private:
  LogEvent::ptr m_event;
};

class LogFormatter {
 public:
  using ptr = std::shared_ptr<LogFormatter>;

  LogFormatter(const std::string& pattern);

  std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

 public:
  class FormatItem {
   public:
    using ptr = std::shared_ptr<FormatItem>;
    virtual ~FormatItem() {}
    virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level,
                        LogEvent::ptr event) = 0;
  };
  void init();

  bool isError() const { return m_error; }

  const std::string getPattern() const { return m_pattern; }

 private:
  std::string m_pattern;
  std::vector<FormatItem::ptr> m_items;
  bool m_error = false;
};

// 日志输出地
class LogAppender {
  friend class Logger;

 public:
  using ptr = std::shared_ptr<LogAppender>;
  virtual ~LogAppender(){};

  virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

  virtual std::string toYamlString() = 0;

  void setFormatter(LogFormatter::ptr val);

  LogFormatter::ptr getFormatter() const { return m_formatter; }
  LogLevel::Level getLevel() { return m_level; }
  void setLevel(LogLevel::Level val) { m_level = val; }

 protected:
  LogLevel::Level m_level = LogLevel::DEBUG;
  bool m_hasFormatter = false;
  LogFormatter::ptr m_formatter;
};

class StdoutLogAppender : public LogAppender {
 public:
  using ptr = std::shared_ptr<StdoutLogAppender>;
  void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

  std::string toYamlString() override;

 private:
};

class FileLogAppender : public LogAppender {
 public:
  using ptr = std::shared_ptr<FileLogAppender>;
  FileLogAppender(const std::string& filename);
  void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

  bool reopen();
  std::string toYamlString() override;

 private:
  std::string m_filename;
  std::ofstream m_filestream;
};
// 日志器
class Logger : public std::enable_shared_from_this<Logger> {
  friend class LoggerManager;

 public:
  using ptr = std::shared_ptr<Logger>;

  Logger(const std::string& name = "root");
  void log(LogLevel::Level level, const LogEvent::ptr event);

  void debug(LogEvent::ptr event);
  void info(LogEvent::ptr event);
  void warn(LogEvent::ptr event);
  void error(LogEvent::ptr event);
  void fatal(LogEvent::ptr event);

  void addAppender(LogAppender::ptr appender);
  void delAppender(LogAppender::ptr appender);
  void clearAppender();
  LogLevel::Level getLevel() const { return m_level; }
  const std::string& getName() const { return m_name; }
  void setLevel(LogLevel::Level val) { m_level = val; }

  void setFormatter(LogFormatter::ptr val);
  void setFormatter(const std::string& val);
  LogFormatter::ptr getFormatter();

  std::string toYamlString();

 private:
  std::string m_name;
  LogLevel::Level m_level;
  std::list<LogAppender::ptr> m_appenders;
  LogFormatter::ptr m_formatter;
  Logger::ptr m_root;
};

class LoggerManager {
 public:
  LoggerManager();
  Logger::ptr getLogger(const std::string& name);
  std::string toYamlString();

  void init();

  Logger::ptr getRoot() const { return m_root; }

 private:
  std::map<std::string, Logger::ptr> m_loggers;
  Logger::ptr m_root;
};

using LoggerMgr = Singleton<LoggerManager>;

}  // namespace sylar
#endif