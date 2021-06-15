#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <inttypes.h>

#include "save.h"

#define NAME_LENGTH 25

int save_to_file(s_directory *root, char *output_path)
{	
	// If no output path is specified, fill out output_path with default value
	if (output_path == NULL)
	{
		// echo path in shell to expand ~ ($HOME)
		char default_output_dir[PATH_MAX+1];
		{
			FILE *echo_out = popen("echo ~/.filescanner", "r");
			if (echo_out == NULL) {
				fprintf(stderr, "Error: failed to run echo command. %s\n", strerror(errno));
				return -1;
			}

			fgets(default_output_dir, PATH_MAX+1, echo_out);
			default_output_dir[strcspn(default_output_dir, "\n")] = 0; // Remove trailing /n

			if (pclose(echo_out) != 0) {
				fprintf(stderr, "Error: echo failed to return or returned an error. %s\n", strerror(errno));
				return -1;
			}
		}
		
		DIR *default_dir = opendir(default_output_dir);
		if(default_dir == NULL)
		{
			if (errno == ENOENT) // Case where ".filescaner" doesn't exist
			{
				if(mkdir(default_output_dir, 0755)==-1)
				{
					fprintf(stderr,"Error: Couldn't create directory %s! %s\n", default_output_dir, strerror(errno));
					return -1;
				}
			}
			else // Unsupported error
			{
				fprintf(stderr, "Error while checking if %s exists. %s\n", default_output_dir, strerror(errno));
				return -1;
			}
		}
		else
		{
			// ~/.filescanner is confirmed to exist, close it for now
			closedir(default_dir);
		}

		// Generate the default file name to the output path
		char file_name[NAME_LENGTH];
		if(default_file_name(file_name)==-1)
		{
			fprintf(stderr, "Error: Failure during generation of default file name!");
			return -1;
		}

		// Finally, fill out output_path with fully merged path
		output_path = merge_path(default_output_dir, file_name);
	}
		
	// Create output file
	FILE *output_file = NULL;
	output_file = fopen(output_path, "w");
	if(output_file == NULL)
	{
		fprintf(stderr, "Error: Couldn't open file %s! %s\n", output_path, strerror(errno));
		return -1;
	}

	// Empty string needs to be malloc-ed because it will be freed inside process_save_dir()
	// char *relative_path = (char *)malloc(sizeof(char));
	// strcpy(relative_path, "");
	
	// Start recursive tree runthrough
	if (process_save_dir(root, output_file, "", 0) != 0)
	{
		fprintf(stderr, "Error: failed recursive tree runthrough");
		return -1;
	}
	
	fclose(output_file);

	free(output_path);
	
	return 0;
}

int save_directory(s_directory *dir, FILE *output_file, char *relative_path)
{
	// Getting time information
	struct tm * timeinfo;
	timeinfo = localtime ( &dir->mod_time );

	char date[80];
	strftime(date, sizeof(date), "%F-%T", timeinfo);

	if (fprintf(output_file, "0\t%s\t%s/\n", date, relative_path) == 0)
	{
		return -1;
	}
	return 0;
}

int save_reg_file(s_file *file, FILE *output_file)
{	
	// Getting time informations
	struct tm * timeinfo;
	timeinfo = localtime ( &file->mod_time );

	char date[80];
	strftime(date, sizeof(date), "%F-%T", timeinfo);

	// if (fprintf(output_file,"\t%d\t%s\t" PRIu64 "\t%s\t%s\n",1,buffer,file->file_size,file->md5sum,file->name) == 0)
	if (fprintf(output_file, "1\t%s\t%ld\t%s\n", date, file->file_size, file->name) == 0)
	{
		return -1;
	} 
	return 0;
}

int save_other_file(s_file *file, FILE *output_file)
{
	struct tm * timeinfo;
	timeinfo = localtime ( &file->mod_time );

	char date[80];
	strftime(date, sizeof(date), "%F-%T", timeinfo);

	if (fprintf(output_file, "2\t%s\t%s\n", date, file->name) == 0)
	{
		return -1;
	} 
	return 0;
} 

int default_file_name(char *file_name)
{
	time_t rawtime;
	struct tm *timeinfo;

	// Getting time informations
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	// Converting time informations to a string into file_name
	if(strftime(file_name, NAME_LENGTH,"%F-%T", timeinfo)==0)
	{
		fprintf(stderr, "Error: not enough room in file_name to store date data.\n");
		return -1;
	}

	// Append .scan filetype
	strcat(file_name, ".scan");
	
	return 0;
}

void indent(FILE *output_file, int depth)
{
	for (int i=0; i<depth; i++)
	{
		fprintf(output_file, "\t");
	}
	fprintf(output_file, "|");
}

int process_save_dir(s_directory *parent, FILE *output_file, char* relative_path, int depth)
{
	// Append parent name to relative path
	char *new_relative_path = merge_path(relative_path, parent->name);

	// Save parent information
	if (save_directory(parent, output_file, new_relative_path) != 0)
	{
		fprintf(stderr, "Error: Missing directory information\n");
		return -1;
	}

	// Process all the files
	s_file *current_file = parent->files;
	while(current_file != NULL)
	{
		indent(output_file, depth+1);

		int return_value;
		if (current_file->file_type == REGULAR_FILE)
		{
			return_value = save_reg_file(current_file, output_file);
		}
		else
		{
			return_value = save_other_file(current_file, output_file);
		}

		if (return_value != 0)
		{
			fprintf(stderr, "Error: Missing file information\n");
			return -1;
		}

		current_file = current_file->next_file;
	}

	

	// Process all the subdirs recursively
	s_directory *current_subdir = parent->subdirs;
	while (current_subdir != NULL)
	{
		indent(output_file, depth+1);

		if (process_save_dir(current_subdir, output_file, new_relative_path, depth+1) != 0) { return -1; }

		current_subdir = current_subdir->next_dir;
	}

	// Add separator for clarity
	indent(output_file, depth+1);
	fprintf(output_file, "---------------------------------\n");

	free(new_relative_path);

	return 0;
}
