#ifndef _SAVE_H_
#define _SAVE_H_

#include "core.h"
#include <stdio.h>

int save_to_file(s_directory *root, char *output_path);
int save_directory(s_directory *dir, FILE *output_file);
int save_reg_file(s_file *file, FILE *output_file);
int save_other_file(s_file *file, FILE *output_file);

int default_file_name(char *file_name);

int process_save_dir(s_directory *parent, FILE *output_file, int depth);

#endif //_SAVE_H_
