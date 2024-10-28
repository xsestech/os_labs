/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 28.10.2024
 */
#include <parent/processes.h>

child_t create_empty_child(const char* name) {
 child_t child;
 strncpy(child.name, name, sizeof(child.name));
 child.pid = -1;
 child.channel[0] = -1;
 child.channel[1] = -1;
 return child;
}

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
  // Redirect pipe output to child stdin, before execv
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
  close(child->channel[PIPE_READ_END]);
  if (status != 0) {
   print_fd(STDERR_FILENO, "Error executing %s in %s", exec_path, child->name);
   return -1;
  }
  return 0;
 }
 // We are parent
 close(child->channel[PIPE_READ_END]);
 const pid_t parent_pid = getppid();
 child->pid = fork_pid;
 print_fd(STDOUT_FILENO, "Parent %d: created child with pid %d\n", parent_pid, fork_pid);
 return fork_pid;
}
void close_child_process(const child_t child) {
 close(child.channel[PIPE_WRITE_END]);
 int child_status;
 waitpid(child.pid, &child_status, 0);
 print_fd(STDOUT_FILENO, "Child %d: exit status %d\n", child.pid, child_status);
}
