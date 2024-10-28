/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 28.10.2024
 */
#ifndef PROCESSES_H
#define PROCESSES_H

#include <unistd.h>
#include <libio/io.h>
#if defined(__clang__)
#include <sys/syslimits.h>
#else
#include <limits.h>
#endif


#define PIPE_WRITE_END 1
#define PIPE_READ_END 0

typedef struct {
 pid_t pid;
 char name[50];
 char file_path[PATH_MAX];
 int channel[2];
} child_t;

child_t create_empty_child(const char* name);

/**
 * Creates child process
 * @param exec_path path to executable
 * @param child pointer to child structure
 * @return 0 if we are child process or child pid
 */
pid_t start_child_process(char *exec_path, child_t *child);

/**
 * Close child process
 * @param child child structure
 */
void close_child_process(child_t child);

#endif //PROCESSES_H
