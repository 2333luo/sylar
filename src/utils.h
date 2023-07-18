#ifndef __UTIL_H__
#define __UTIL_H__

#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace sylar {
pid_t GetThreadId();
u_int32_t GetFiberId();

}  // namespace sylar

#endif
