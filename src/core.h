#ifndef _CORE_H_
#define _CORE_H_

#include <linux/limits.h>
#include <time.h>
#include <openssl/md5.h>
#include <stdint.h>

// This file contains the few things that are needed by the whole program

typedef enum {DIRECTORY=0, REGULAR_FILE=1, OTHER_TYPE=2} e_type;

typedef struct _file {
    e_type file_type;
    char name[NAME_MAX+1];
    time_t mod_time;
    uint64_t file_size;
    unsigned char md5sum[MD5_DIGEST_LENGTH];
    struct _file *next_file;
} s_file;

typedef struct _directory {
    char name[NAME_MAX+1];
    time_t mod_time;
    struct _directory *subdirs;
    s_file *files;
    struct _directory *next_dir;
} s_directory;

char *merge_path(char *path, char* file_name);

#endif // _CORE_H_
