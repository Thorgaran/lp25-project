#include "scan.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>


s_directory *process_dir(char *path)
{
	
	s_directory *resulting_dir=(s_directory*)malloc(sizeof(s_directory));
	
	//Saving basic information
	strcpy(resulting_dir->name,basename(path));
	
	struct stat st_dir;
	if(stat(path,&st_dir)==-1)
	{
		fprintf(stderr,"Error : stat() failed !\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		resulting_dir->mod_time = st_dir.st_mtime;
	}
	

	//Processing this dir's content recursively
	DIR *descripteur = opendir(path);
	if(!descripteur)
	{
		perror("Error ");
		exit(EXIT_FAILURE);
	}
	
	struct dirent *dirent;
	dirent = readdir(descripteur);
	while ( dirent!=NULL ) {
		if(strcmp(dirent->d_name,".")!=0 && strcmp(dirent->d_name,"..")!=0)
		{
			//Storing path in order to free it after
			char * newPath = mergePath(path,dirent->d_name);
			
			struct stat st_subdir;
			if(stat(newPath,&st_subdir)==-1)
			{
				fprintf(stderr,"Error : stat() failed !\n");
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
	closedir(descripteur); 
	return resulting_dir;
}



s_file *process_file(char *path)
{
	s_file *resulting_file=(s_file*)malloc(sizeof(s_file));
	
	//Saving basic information
	strcpy(resulting_file->name,basename(path));
	
	struct stat st_file;
	if(stat(path,&st_file)==-1)
	{
		fprintf(stderr,"Error : stat() failed !\n");
		exit(EXIT_FAILURE);
	}
	else
	{
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
	}
	return resulting_file;
}


char *mergePath(char *path,char* fileName)
{
	//+2 because we will add "/" between and "\0" at the end
	char *newPath = (char *)malloc(sizeof(char)*(strlen(path)+strlen(fileName)+2));
	
	if(newPath==NULL)
	{
		fprintf(stderr,"Error : Memory not available !");
		exit(EXIT_FAILURE);
	}
	
	strcpy(newPath,path);
	strcat(newPath,"/");
	strcat(newPath,fileName);
	//strcat add automatically the null terminated byte 
	
	return newPath;
}
