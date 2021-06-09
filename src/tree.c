#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

int append_subdir(s_directory *child, s_directory *parent)
{
	//Setting initial child's pointers
	child->next_dir = NULL;
	child->subdirs = NULL;
	
	//Case where this is the first subdir of the parent
	if(parent->subdirs == NULL)
	{
		parent->subdirs = (s_directory*)malloc(sizeof(s_directory));
		if(parent->subdirs == NULL)
		{
			fprintf(stderr,"Error : Memory not available !");
			return 1;
		}
		parent->subdirs = child;
		return 0;
	}
	
	//Find last dir
	s_directory *temp = parent->subdirs;
	while(temp->next_dir != NULL)
	{
		temp = temp->next_dir;
	}
	
	//Linking last dir to the child
	temp->next_dir = (s_directory*)malloc(sizeof(s_directory));
	if(temp->next_dir == NULL)
	{
			fprintf(stderr,"Error : Memory not available !");
			return 1;
	}
	temp->next_dir = child;
	
	return 0;
}

int append_file(s_file *child, s_directory *parent)
{
	//Setting initial child's pointer
	child->next_file = NULL;
	
	//Case where this is the first file of parent
	if(parent->files == NULL)
	{
		parent->files = (s_file*)malloc(sizeof(s_file));
		if(parent->files == NULL)
		{
			fprintf(stderr,"Error : Memory not available !");
			return 1;
		}
		parent->files = child;
		return 0;
	}
	
	//Finding last file
	s_file *temp = parent->files;
	while(temp->next_file != NULL)
	{
		temp = temp->next_file;
	}
	
	//Linking last file to the child
	temp->next_file = (s_file*)malloc(sizeof(s_file));
	if(temp->next_file == NULL)
	{
		fprintf(stderr,"Error : Memory not available !");
		return 1;
	}
	temp->next_file=child;
	
	return 0;
}


void  clear_files(s_directory *parent)
{
	s_file *current_file = parent->files;
	
	while(current_file != NULL)
	{
		parent->files = parent->files->next_file;
		free(current_file);
		current_file = parent->files;
	}
}

void clear_subdirs(s_directory *parent)
{
	s_directory *current_dir = parent->subdirs;
	
	while(current_dir != NULL)
	{
		//Freeing recursively what current_dir contains
		clear_files(current_dir);
		clear_subdirs(current_dir);
		
		//Switch to the next subdir after freeing memory
		parent->subdirs = parent->subdirs->next_dir;
		free(current_dir);
		current_dir = parent->subdirs;
	}
}
