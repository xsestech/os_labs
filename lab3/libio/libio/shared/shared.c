/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 12.12.2024
 */
#include "shared.h"
#include <libio/io.h>
#include <errno.h>
shared_handle_t shared_open(char *name, size_t size, bool is_producer) {
  if (name == NULL || strlen(name) > SHARED_MAX_NAME_LEN) {
    return NULL;
  }
  int rw_perms = SHARED_READ_PERMISSIONS;
  if (is_producer) rw_perms = SHARED_WRITE_PERMISSIONS;
  int fd = shm_open(name, rw_perms, SHARED_ACCESS_PERMISSIONS);
  if (fd < 0) {
    print_fd(STDERR_FILENO, "file descr\n");
    return NULL;
  }
  ftruncate(fd, size);

  char *mem_ptr = mmap(0, size, SHARED_DEFAULT_PROTECTIONS, MAP_SHARED, fd, 0);
  if ((caddr_t) mem_ptr == (caddr_t) -1) {
    print_fd(STDERR_FILENO, " mmap errno: %s \n", strerror(errno));
    close(fd);
    return NULL;
  }

  char data_ready_name[SHARED_MAX_NAME_LEN];
  char buffer_empty_name[SHARED_MAX_NAME_LEN];
  snprintf(data_ready_name, sizeof(data_ready_name), "%s_data", name);
  snprintf(buffer_empty_name, sizeof(buffer_empty_name), "%s_buff", name);

  sem_t *data_ready = sem_open(data_ready_name, O_CREAT, SHARED_ACCESS_PERMISSIONS, 0);
  if (data_ready == SEM_FAILED) {
    print_fd(STDERR_FILENO, " sem \n");
    close(fd);
    munmap(mem_ptr, size);
    return NULL;
  }
  sem_t *buffer_empty = sem_open(buffer_empty_name, O_CREAT, SHARED_ACCESS_PERMISSIONS, 1);
  if (buffer_empty == SEM_FAILED) {
    sem_close(data_ready);
    print_fd(STDERR_FILENO, " sem \n");
    close(fd);
    munmap(mem_ptr, size);
    return NULL;
  }

  shared_handle_t shared = malloc(sizeof(shared_t));
  assert(shared);
  strcpy(shared->name, name);
  shared->is_producer = is_producer;
  shared->size = size;
  shared->fd = fd;
  shared->mem_ptr = mem_ptr;
  shared->buffer_empty = buffer_empty;
  shared->data_ready = data_ready;
  return shared;
}
shared_handle_t shared_producer_open(char *name, size_t size) {
  return shared_open(name, size, true);
}
shared_handle_t shared_consumer_open(char *name, size_t size) {
  return shared_open(name, size, false);
}
char *shared_get_str(shared_handle_t shared) {
  if (sem_wait(shared->data_ready) != 0) {
    return NULL;
  }

  char *str = strdup(shared->mem_ptr);
  if (!str) {
    sem_post(shared->data_ready);
    return NULL;
  }
  if (sem_post(shared->buffer_empty) != 0) {
    free(str);
    return NULL;
  }
  return str;
}

int shared_put_str(shared_handle_t shared, char *str) {
  if (strlen(str) > shared->size) return -1;
  if (sem_wait(shared->buffer_empty) != 0) {
    return -1;
  }
  strcpy(shared->mem_ptr, str);
  return sem_post(shared->data_ready);
}

void shared_close(shared_handle_t shared) {
  munmap(shared->mem_ptr, shared->size);
  close(shared->fd);
  sem_close(shared->data_ready);
  sem_close(shared->buffer_empty);
  unlink(shared->name);
  free(shared);
}
