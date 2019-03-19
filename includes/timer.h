#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>

void initTimer(pid_t threadID, uint64_t nanosec, void (*callbackFunction()));