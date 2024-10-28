#include <stdlib.h>
#include <unistd.h>


#include <sys/wait.h>
#include <libio/io.h>
#include <parent/processes.h>

#define MAX_LINE_LENGTH 1024




int main(void) {
  // Get child exec path from environment variable
  const char *env_var_name = "CHILD_EXEC_PATH";
  char *exec_path = getenv(env_var_name);
  if (exec_path == NULL) {
    print_fd(STDERR_FILENO, "%s: environment variable %s not set\n", exec_path, env_var_name);
    exit(EXIT_FAILURE);
  }

  child_t child[2] = {
    create_empty_child("child1"),
    create_empty_child("child2"),
  };
  const size_t child_len = sizeof(child) / sizeof(child[0]);

  // Read file path from stdin
  for (int i = 0; i < child_len; i++) {
    ssize_t read_bytes = reads_fd(STDIN_FILENO, child[i].file_path, PATH_MAX);
    if (read_bytes == -1) {
      print_fd(STDERR_FILENO, "Error: Failure during reading file path");
      exit(EXIT_FAILURE);
    }
    child[i].file_path[read_bytes - 1] = '\0'; // Remove trailing newline
  }

  // Start child processes
  for (int i = 0; i < child_len; i++) {
    const pid_t status = start_child_process(exec_path, &child[i]);
    if (status == -1) {
      exit(EXIT_FAILURE);
    }
    if (status == 0) {
      // Child process
      exit(EXIT_SUCCESS);
    }
  }

  // Send to child processes
  char line[MAX_LINE_LENGTH];
  int line_number = 1;
  while (reads_fd(STDIN_FILENO, line, MAX_LINE_LENGTH) > 0) {
    if (line[0] == '\n') {
      // Close on empty line
      for (int i = 0; i < child_len; i++) {
        const ssize_t bytes = write_str(child[i].channel[PIPE_WRITE_END], line);
        if (bytes == -1) {
          print_fd(STDERR_FILENO, "Error: Failure during writing to pipe for %s\n", child[i].name);
          exit(EXIT_FAILURE);
        }
      }
      break;
    }
    child_t current_child = child[line_number % child_len];
    const ssize_t bytes = write_str(current_child.channel[PIPE_WRITE_END], line);
    if (bytes == -1) {
      print_fd(STDERR_FILENO, "Error: Failure during writing to pipe for %s\n", current_child.name);
      exit(EXIT_FAILURE);
    }
    line_number++;
  }
  // Close all pipes and wait for children
  for (int i = 0; i < child_len; i++) {
    close_child_process(child[i]);
  }
  return 0;
}
