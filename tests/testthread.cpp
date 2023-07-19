#include <unistd.h>

#include <iostream>
#include <mutex>

#include "../src/log.h"
#include "../src/thread.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int cnt = 0;
sylar::RWMutex mutex;

void fun1()
{
  SYLAR_LOG_INFO(g_logger) << "name: " << sylar::Thread::GetName()
                           << " this name: " << sylar::Thread::GetThis()->getName() << " id: " << sylar::GetThreadId()
                           << " this id: " << sylar::Thread::GetThis()->getId();
  for (int i = 0; i < 100000; ++i) {
    sylar::RWMutex::WriteLock lock(mutex);
    ++cnt;
  }
}

void fun2() {}

int main(int argc, char** argv)
{
  SYLAR_LOG_INFO(g_logger) << "Thread test begin";
  std::vector<sylar::Thread::ptr> threads;
  for (int i = 0; i < 5; ++i) {
    sylar::Thread::ptr thr(new sylar::Thread(&fun1, "name_" + std::to_string(i)));
    threads.push_back(thr);
  }

  for (int i = 0; i < 5; ++i) {
    threads[i]->join();
  }
  SYLAR_LOG_INFO(g_logger) << "thread test end";
  SYLAR_LOG_INFO(g_logger) << cnt;
  return 0;
}