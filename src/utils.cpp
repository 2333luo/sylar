#include "utils.h"

#include <execinfo.h>

#include "Fiber.h"
#include "log.h"

namespace sylar {

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

pid_t GetThreadId() { return syscall(SYS_gettid); }
u_int32_t GetFiberId() { return sylar::Fiber::GetFiberId(); }

void Backtrace(std::vector<std::string>& bt, int size, int skip)
{
  void** array = (void**)malloc(sizeof(void*) * size);
  size_t s = backtrace(array, size);

  char** string = backtrace_symbols(array, s);
  if (string == NULL) {
    SYLAR_LOG_ERROR(g_logger) << "backtrace_synbols error";
    return;
  }

  for (size_t i = skip; i < s; ++i) {
    bt.push_back(string[i]);
  }
  free(string);
  free(array);
}

std::string BacktraceToString(int size, int skip, const std::string& prefix)
{
  std::vector<std::string> bt;
  Backtrace(bt, size, skip);
  std::stringstream ss;
  for (size_t i = 0; i < bt.size(); ++i) {
    ss << prefix << bt[i] << std::endl;
  }
  return ss.str();
}

}  // namespace sylar