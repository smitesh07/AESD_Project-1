#include <stdio.h>

#define LOG_DO(log_file_ptr,message, level, ...)                                        \
  fprintf(log_file_ptr,"%5" PRIu32 ":%s:%s: " message "\n", loggerGetTimestamp(), level, \
         __func__, ##__VA_ARGS__)

void main()
{
    char *file_name = "test";
    FILE *file_ptr = fopen((char *)file_name,"w");
    LOG_DO(file_ptr,"HELLO %d", 12);
}
