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
#include <libio/shared/shared.h>

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    print_fd(STDERR_FILENO, "No file specified");
    exit(EXIT_FAILURE);
  }
  const pid_t pid = getpid();
  int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0600);
  shared_handle_t shared = shared_consumer_open(argv[0], 4096);
  if (shared == NULL) {
    print_fd(STDERR_FILENO, "%d: Error accessing shared memory\n", pid);
    exit(EXIT_FAILURE);
  }

  if (file == -1) {
    print_fd(STDERR_FILENO, "%d: Error opening file %s\n", pid, argv[1]);
    exit(EXIT_FAILURE);
  }

  print_fd(STDOUT_FILENO, "%d: opened file %s\n", getpid(), argv[1]);
  char* str;;
  ssize_t bytes = 0;
  while ((str = shared_get_str(shared)) != NULL) {

    if (str[0] == '\n') {
      break;
    }


    str[strlen(str) - 1] = '\0'; // remove newline
    print_fd(STDOUT_FILENO, "%d: got: %s\n", pid, str);
    if (print_fd(file, "%s ", str) == -1) {
      print_fd(STDERR_FILENO, "%d: Error writing to file\n", pid);
      exit(EXIT_FAILURE);
    }
  }
  const char term = '\0';
  write(file, &term, sizeof(term));
  close(file);
  shared_close(shared);
  return 0;
}
