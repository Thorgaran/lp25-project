#ifndef _TREE_H_
#define _TREE_H_

#include <linux/limits.h>
#include <time.h>

typedef enum {DIRECTORY, REGULAR_FILE, LINK, FIFO, OTHER_TYPE} e_type;

typedef struct _file {
    e_type file_type;
    char name[NAME_MAX+1];
    time_t mod_time;
    //uint64_t file_size;
    //u_char md5sum[MD5_DIGEST_LENGTH];
    struct _file *next_file;
} s_file;

typedef struct _directory {
    char name[NAME_MAX+1];
    time_t mod_time;
    struct _directory *subdirs;
    s_file *files;
    struct _directory *next_dir;
} s_directory;

int append_subdir(s_directory *child, s_directory *parent);
int append_file(s_file *child, s_directory *parent);
void clear_files(s_directory *parent);
void clear_subdirs(s_directory *parent);

#endif // _TREE_H_
