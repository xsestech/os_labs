#include <stdlib.h>
#include <unistd.h>
#if defined(__clang__)
#include <sys/syslimits.h>
#else
#include <limits.h>
#endif

#include <sys/wait.h>
#include <libio/io.h>

#define MAX_LINE_LENGTH 1024
#define PIPE_WRITE_END 1
#define PIPE_READ_END 0

typedef struct {
  pid_t pid;
  char name[50];
  char file_path[PATH_MAX];
  int channel[2];
} child_t;

pid_t start_child_process(char *exec_path, child_t *child) {
  if (pipe(child->channel) == -1) {
    print_fd(STDERR_FILENO, "Error during pipe creation for %s", child->name);
    return -1;
  }

  pid_t fork_pid = fork();

  if (fork_pid == -1) {
    print_fd(STDERR_FILENO, "Error during creating process for %s\n", child->name);
    return -1;
  }

  if (fork_pid == 0) {
    // We are child
    pid_t child_pid = getpid();

    printf("%s: child pid %d\n", exec_path, child_pid);

    if (dup2(child->channel[PIPE_READ_END],STDIN_FILENO) == -1) {
      print_fd(STDERR_FILENO, "Error during dup2 for %s\n", child->name);
      return -1;
    }
    close(child->channel[PIPE_WRITE_END]);
    const char *args[] = {
      child->name,
      child->file_path,
      NULL,
    };

    const int32_t status = execv(exec_path, args);
    if (status != 0) {
      print_fd(STDERR_FILENO, "Error executing %s in %s", exec_path, child->name);
      return -1;
    }
  } else {
    const pid_t parent_pid = getppid();
    child->pid = fork_pid;
    print_fd(STDOUT_FILENO, "Parent %d: created child with pid %d\n", parent_pid, fork_pid);
  }
  return fork_pid;
}

int main(void) {
  char *exec_path = getenv("CHILD_EXEC_PATH");
  if (exec_path == NULL) {
    print_fd(STDERR_FILENO, "%s: environment variable not set\n", exec_path);
    exit(EXIT_FAILURE);
  }

  child_t child[2] = {
    {0, "child1", "", {0, 0}},
    {0, "child2", "", {0, 0}},
  };
  const size_t child_len = sizeof(child) / sizeof(child[0]);
  for (int i = 0; i < child_len; i++) {
    ssize_t read_bytes = reads_fd(STDIN_FILENO, child[i].file_path, PATH_MAX);
    if (read_bytes == -1) {
      print_fd(STDERR_FILENO, "Error: Failure during reading file path");
      exit(EXIT_FAILURE);
    }
    child[i].file_path[read_bytes - 1] = '\0';
    // @todo: realpath
    // char temp_path[PATH_MAX];
    // if (!realpath(child[i].file_path, temp_path)) {
    //   print_fd(STDERR_FILENO, "Error: Failure during realpath for %s", child[i].file_path);
    //   exit(EXIT_FAILURE);
    // }
    // strcpy(child[i].name, temp_path);
  }

  for (int i = 0; i < child_len; i++) {
    const pid_t status = start_child_process(exec_path, &child[i]);
    if (status == -1) {
      exit(EXIT_FAILURE);
    }
  }

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
  for (int i = 0; i < child_len; i++) {
    close(child[i].channel[PIPE_WRITE_END]);
    int child_status;
    waitpid(child[i].pid, &child_status, 0);
    print_fd(STDOUT_FILENO, "Child %d: exit status %d\n", child[i].pid, child_status);
  }
  return 0;
}
