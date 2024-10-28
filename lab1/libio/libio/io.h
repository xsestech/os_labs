/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 27.10.2024
 */
#ifndef IO_H
#define IO_H

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define IO_MAX_STR_LEN 1024

ssize_t print_fd(int fd, char *fmt, ...);
ssize_t write_str(const int fd, const char *buff);
ssize_t reads_fd(const int fd, char *buff, const size_t buff_size);


#endif //IO_H
