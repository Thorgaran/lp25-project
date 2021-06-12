#include "save.h"
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#define NAME_LENGTH 25

int save_to_file(s_directory *root, char *path_to_target)
{
	int errno;
	DIR *target = opendir(".filescanner");
	
	//Case where ".filescaner" doesnt exist
	if(target==NULL && errno == ENOENT)
	{
		if(mkdir(".filescanner",0755)==-1)
		{
			fprintf(stderr,"Error : Couldn't create directory !");
			return -1;
		}
		target = opendir(".filescanner");
	}
	
	//Generating the default file name to target
	char file_name[NAME_LENGTH];
	if(default_file_name(file_name)==-1)
	{
		fprintf(stderr,"Error : Failure during generation of default file name !");
		return -1;
	}
	
	//Creating the file to target
	FILE *file_targeted = NULL;
	file_targeted = fopen(file_name,"w");
	if(file_targeted == NULL)
	{
		fprintf(stderr,"Error : Couldn't open file !");
		return -1;
	}
	
	process_dir(root, file_targeted);
	
	fclose(file_targeted);
	closedir(target);
	return 0;
}

int save_directory(void *dir_data,FILE *file_to_target)
{
	s_directory *dir = dir_data;

	//Getting time information
	struct tm * timeinfo;
	timeinfo = localtime ( &dir->mod_time );

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%F-%T", timeinfo);

	if (fprintf(file_to_target,"%d\t%s\t%s\n",0,buffer,dir->name) == 0)
	{
		return -1;
	}
	return 0;
}

int save_reg_file(void *r_file_data,FILE *file_to_target)
{
	s_file *file = r_file_data;
	
	//Getting time informations
	struct tm * timeinfo;
	timeinfo = localtime ( &file->mod_time );

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%F-%T", timeinfo);

	if (fprintf(file_to_target,"\t%d\t%s\t" PRIu64 "\t%s\t%s\n",1,buffer,file->file_size,file->md5sum,file->name) == 0)
	{
		return -1;
	} 
	return 0;
}

int save_other_file(void *o_file_data,FILE *file_to_target)
{
	s_file *file = o_file_data;

	struct tm * timeinfo;
	timeinfo = localtime ( &file->mod_time );

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%F-%T", timeinfo);

	if (fprintf(file_to_target,"%d\t%s\t%s\n",2,buffer,file->name) == 0)
	{
		return -1;
	} 
	return 0;
} 

int default_file_name(char *file_name)
{
	time_t rawtime;
	struct tm * timeinfo;

	//Getting time informations
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	//Converting time informations to a string
	if(strftime(file_name,NAME_LENGTH,"%F-%T",timeinfo)==0)
	{
		return -1;
	}
	strcat(file_name,".scan");
	
	return 0;
}

int process_dir(s_directory *root, FILE *file_to_target)
{
	s_directory temp = root;

	if (save_directory(&temp,file_to_target) != 0)
	{
		printf("Error : Missing directory information");
		return -1;
	}	

	//Process all the files
	while (temp->files != NULL)
	{
		if (temp->files->file_type == REGULAR_FILE)
		{
			if (save_reg_file(&temp->files,file_to_target) != 0)
			{
				printf("Error : Missing file information");
				return -1;
			}	
		}
		else
		{
			if (save_other_file(&temp->files,file_to_target) != 0)
			{
				printf("Error : Missing file information");
				return -1;
			}	
		} 
	}
	//Process all the subdirs
	while (temp->subdirs != NULL)
	{
		if (process_dir(temp->subdirs, file_to_target) != 0)
		{
			printf("Error : Couldn't reach subdirectory\n");
			return -1;
		}	
		else
		{
			temp->subdirs = temp->subdirs->next_dir;
		}
	}
	//Go to next dir
	if (temp->next_dir != NULL)
	{
		if (process_dir(temp->next_dir, file_to_target) != 0)
		{
			printf("Error : Couldn't reach next directory\n");
			return -1;
		}
	}
	else
	{
		return 0;
	}
}
