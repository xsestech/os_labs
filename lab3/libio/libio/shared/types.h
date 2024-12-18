/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 12.12.2024
 */
 
#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#if defined(__APPLE__)
#include <sys/posix_shm.h>
#define SHARED_MAX_NAME_LEN PSHMNAMLEN
#else
#include <sys/shm.h>
#include <linux/limits.h>
#define SHARED_MAX_NAME_LEN PATH_MAX
#endif


#define SHARED_READ_PERMISSIONS O_RDWR
#define SHARED_WRITE_PERMISSIONS O_CREAT | O_RDWR
#define SHARED_ACCESS_PERMISSIONS 0666
#define SHARED_DEFAULT_PROTECTIONS PROT_READ | PROT_WRITE

typedef struct {
 char name[SHARED_MAX_NAME_LEN + 1];
 size_t size;
 sem_t* sem;
 int fd;
 char* mem_ptr;
 bool is_producer;
} shared_t;

typedef shared_t* shared_handle_t;

