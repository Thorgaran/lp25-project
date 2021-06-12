#ifndef _DEF_H_
#define _DEF_H_

#include <linux/limits.h>
#include <time.h>
//#include <openssl/md5.h>
#include <stdint.h>

typedef enum {DIRECTORY=0, REGULAR_FILE=1, OTHER_TYPE=2} e_type;

typedef struct _file {
    e_type file_type;
    char name[NAME_MAX+1];
    time_t mod_time;
    uint64_t file_size;
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


#endif // _DEF_H_
