#include <iostream>
#include <thread>

#include "../src/log.h"
#include "../src/utils.h"
using namespace sylar;

int main()
{
  auto logger = std::make_shared<sylar::Logger>();
  logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

  auto event =
      std::make_shared<sylar::LogEvent>(logger, sylar::LogLevel::DEBUG, __FILE__, __LINE__, 0, sylar::GetThreadId(),
                                        sylar::GetFiberId(), time(0), sylar::Thread::GetName());
  auto file_appender = std::make_shared<sylar::FileLogAppender>("./log.txt");

  auto fmt = std::make_shared<LogFormatter>("%d%T%p%T%m%n");
  file_appender->setFormatter(fmt);
  file_appender->setLevel(LogLevel::ERROR);
  logger->addAppender(file_appender);

  logger->log(sylar::LogLevel::DEBUG, event);

  SYLAR_LOG_DEBUG(logger) << "test macro";
  SYLAR_LOG_ERROR(logger) << "test error macro";

  SYLAR_LOG_FMT_DEBUG(logger, "test macro fmt error %s", "aa");

  auto i = sylar::LoggerMgr::GetInstance()->getLogger("xx");
  SYLAR_LOG_INFO(i) << "xxx";
  return 0;
}