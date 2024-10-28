/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 26.10.2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <libconfig/config.h>

#include <libio/io.h>

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    print_fd(STDERR_FILENO, "No file specified");
    exit(EXIT_FAILURE);
  }
  const pid_t pid = getpid();
  int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0600);

  if (file == -1) {
    print_fd(STDERR_FILENO, "%d: Error opening file %s\n", pid, argv[1]);
    exit(EXIT_FAILURE);
  }

  print_fd(STDOUT_FILENO, "%d: opened file %s\n", getpid(), argv[1]);
  char buffer[MAX_LINE_LENGTH];
  ssize_t bytes = 0;
  while ((bytes = read(STDIN_FILENO, buffer, MAX_LINE_LENGTH)) > 0) {
    if (bytes == -1) {
      print_fd(STDERR_FILENO, "%d: Error reading from pipe\n", pid);
      exit(EXIT_FAILURE);
    }
    if (buffer[0] == '\n') {
      break;
    }


    buffer[bytes - 1] = '\0'; // remove newline
    print_fd(STDOUT_FILENO, "%d: got: %s\n", pid, buffer);
    if (write(file, buffer, bytes - 1) != bytes - 1) {
      print_fd(STDERR_FILENO, "%d: Error writing to file\n", pid);
      exit(EXIT_FAILURE);
    }
  }
  const char term = '\0';
  write(file, &term, sizeof(term));
  close(file);
  return 0;
}
