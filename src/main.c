#include <stdio.h>
#include <inttypes.h>
#include "log.h"

void main()
{
    char *file_name = "test";
    FILE *file_ptr = fopen((char *)file_name,"w");
    LOG_INFO(file_ptr, "HELLO");
    fclose(file_ptr);
}
