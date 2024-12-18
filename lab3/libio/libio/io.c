/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 27.10.2024
 */

#include <libio/io.h>

ssize_t print_fd(const int fd, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buff[IO_MAX_STR_LEN];
  size_t len = vsnprintf(buff, IO_MAX_STR_LEN - 1, fmt, args);
  const ssize_t writen_bytes = write(fd, buff, len);
  va_end(args);
  return writen_bytes;
}
ssize_t write_str(const int fd, const char *buff) {
  return write(fd, buff, strlen(buff));
}

ssize_t reads_fd(const int fd, char *buff, const size_t buff_size) {
  ssize_t read_bytes = 0;
  return read(fd, buff, buff_size);
}
