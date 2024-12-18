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
 return child;
}

pid_t start_child_process(char *exec_path, child_t *child) {
 shared_handle_t shared = shared_producer_open(child->name, PROC_MEM_SIZE);
 if (shared == NULL) {
  print_fd(STDERR_FILENO, "Error during pipe creation for %s", child->name);
  return -1;
 }
 child->shared = shared;

 pid_t fork_pid = fork();

 if (fork_pid == -1) {
  print_fd(STDERR_FILENO, "Error during creating process for %s\n", child->name);
  return -1;
 }

 if (fork_pid == 0) {
  // We are child
  pid_t child_pid = getpid();

  printf("%s: child pid %d\n", exec_path, child_pid);
  const char *args[] = {
   child->name,
   child->file_path,
   NULL,
 };
  const int32_t status = execv(exec_path, args);

  if (status != 0) {
   print_fd(STDERR_FILENO, "Error executing %s in %s, status %d \n", exec_path, child->name, status);
   return -1;
  }
  return 0;
 }
 const pid_t parent_pid = getppid();
 child->pid = fork_pid;
 print_fd(STDOUT_FILENO, "Parent %d: created child with pid %d\n", parent_pid, fork_pid);
 return fork_pid;
}

void close_child_process(const child_t child) {
 shared_close(child.shared);
 int child_status;
 waitpid(child.pid, &child_status, 0);
 print_fd(STDOUT_FILENO, "Child %d: exit status %d\n", child.pid, child_status);
}
