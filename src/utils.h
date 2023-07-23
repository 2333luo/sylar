#ifndef __UTIL_H__
#define __UTIL_H__

#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

namespace sylar {
pid_t GetThreadId();
u_int32_t GetFiberId();

void Backtrace(std::vector<std::string>& bt, int size, int skip = 2);
std::string BacktraceToString(int size, int skip, const std::string& prefix);

}  // namespace sylar

#endif
