#ifndef _SAVE_H_
#define _SAVE_H_

#include "def.h"
#include <stdio.h>

int save_to_file(s_directory *root, char *path_to_target);
int save_directory(s_directory *dir,FILE *file_to_target);
int save_file(s_file *file,FILE *file_to_target);
int puts_type_value(void *file,FILE *file_to_target);

int default_file_name(char *file_name);

#endif //_SAVE_H_
