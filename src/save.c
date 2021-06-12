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
	FILE *file_targetted = NULL;
	file_targetted = fopen(file_name,"w");
	if(file_targetted == NULL)
	{
		fprintf(stderr,"Error : Couldn't open file !");
		return -1;
	}
	
	
	fclose(file_targetted);
	closedir(target);
	return 0;
}

int save_file(s_file *file,FILE *file_to_target)
{
	fprintf(file_to_target,"%d\t",(int)s_file->e_type;);
	
	if(s_file->e_type == REGULAR_FILE ) 
	{
		//Getting time informations
		struct tm * timeinfo;
		timeinfo = localtime ( &file->mod_time );
		
	}
}

int puts_type_value(void *file,FILE *file_to_target)
{
	puts()
}

int default_file_name(char *file_name)
{
	time_t rawtime;
	struct tm * timeinfo;

	//Getting time informations
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	//Converting time informations to a string
	if(strftime(file_name,NAME_LENGTH,"%F:%T",timeinfo)==0)
	{
		return -1;
	}
	strcat(file_name,".scan");
	
	return 0;
}
