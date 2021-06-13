#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "scan.h"
#include "tree.h"

s_directory *process_dir(char *path)
{	
	s_directory *resulting_dir=(s_directory*)malloc(sizeof(s_directory));
	
	//Setting initial child's pointers
	resulting_dir->next_dir = NULL;
	resulting_dir->subdirs = NULL;
	
	//Saving basic information
	strcpy(resulting_dir->name, basename(path));
	
	struct stat st_dir;
	if(stat(path, &st_dir)==-1)
	{
		fprintf(stderr, "Error: stat() failed for %s! %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}

	resulting_dir->mod_time = st_dir.st_mtime;
	
	//Processing this dir's content recursively
	DIR *descripteur = opendir(path);
	if(descripteur == NULL)
	{
		fprintf(stderr, "Error while opening the directory %s. %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	struct dirent *dirent;
	dirent = readdir(descripteur);
	while ( dirent!=NULL ) {
		//Ignore current and parent dirs
		if(strcmp(dirent->d_name,".")!=0 && strcmp(dirent->d_name,"..")!=0)
		{
			//Storing path in order to free it after
			char *newPath = merge_path(path,dirent->d_name);
			
			struct stat st_subdir;
			if(stat(newPath,&st_subdir)==-1)
			{
				fprintf(stderr, "Error: stat() failed for %s! %s\n", path, strerror(errno));
				exit(EXIT_FAILURE);
			}
			else
			{
				if(S_ISDIR(st_subdir.st_mode))
				{
					append_subdir(process_dir(newPath),resulting_dir);
				}
				else
				{
					append_file(process_file(newPath),resulting_dir);
				}
			}
			printf("%s\n", dirent->d_name);
			free(newPath);
		}
		dirent = readdir(descripteur);
	}

	if (closedir(descripteur) == -1)
	{
		fprintf(stderr, "Error while closing the directory %s. %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}

	return resulting_dir;
}



s_file *process_file(char *path)
{
	s_file *resulting_file=(s_file*)malloc(sizeof(s_file));
	
	//Saving basic information
	strcpy(resulting_file->name, basename(path));
	
	struct stat st_file;
	if(stat(path,&st_file)==-1)
	{
		fprintf(stderr, "Error: stat() failed for %s! %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}

	resulting_file->mod_time = st_file.st_mtime;
	resulting_file->file_size = st_file.st_size;
	if(S_ISREG(st_file.st_mode))
	{
		resulting_file->file_type = REGULAR_FILE;
	}
	else
	{
		resulting_file->file_type = OTHER_TYPE;
	}

	// MD5 todo

	return resulting_file;
}
