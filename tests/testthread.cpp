#include <unistd.h>

#include <iostream>
#include <mutex>

#include "../src/log.h"
#include "../src/thread.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int cnt = 0;
sylar::RWMutex rwmutex;

void fun1()
{
  while (1)
    SYLAR_LOG_INFO(g_logger) << "name: " << sylar::Thread::GetName()
                             << " this name: " << sylar::Thread::GetThis()->getName() << " id: " << sylar::GetThreadId()
                             << " this id: " << sylar::Thread::GetThis()->getId();
}

sylar::Mutex mutex;

void fun2()
{
  SYLAR_LOG_INFO(g_logger) << "mutex test:";
  for (int i = 0; i < 100000; ++i) {
    sylar::Mutex::Lock lock(mutex);
    ++cnt;
  }
}

void fun3()
{
  while (true) {
    SYLAR_LOG_INFO(g_logger) << "fun3 "
                             << "=======================================================";
  }
}

void fun4()
{
  while (true) {
    SYLAR_LOG_INFO(g_logger) << "fun4 "
                             << "*******************************************************";
  }
}

int main(int argc, char** argv)
{
  SYLAR_LOG_INFO(g_logger) << "Thread test begin";
  std::vector<sylar::Thread::ptr> threads;
  for (int i = 0; i < 2; ++i) {
    sylar::Thread::ptr thr(new sylar::Thread(&fun3, "name_" + std::to_string(i)));
    sylar::Thread::ptr thr2(new sylar::Thread(&fun4, "name_" + std::to_string(i + 1)));
    threads.push_back(thr);
  }

  for (size_t i = 0; i < threads.size(); ++i) {
    threads[i]->join();
  }
  SYLAR_LOG_INFO(g_logger) << "thread test end";
  SYLAR_LOG_INFO(g_logger) << cnt;
  return 0;
}