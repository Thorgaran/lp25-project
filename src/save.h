#ifndef _SAVE_H_
#define _SAVE_H_

#include "def.h"
#include <stdio.h>

int save_to_file(s_directory *root, char *path_to_target);
int save_directory(void *dir_data,FILE *file_to_target);
int save_reg_file(void *r_file_data,FILE *file_to_target);
int save_other_file(void *o_file_data,FILE *file_to_target);

int default_file_name(char *file_name);

int process_save_dir(s_directory *root, FILE *file_to_target, int depth);

#endif //_SAVE_H_
