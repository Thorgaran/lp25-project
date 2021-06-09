#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

int append_subdir(s_directory *child, s_directory *parent)
{
	//Setting initial child's pointers
	child->next_dir = NULL;
	child->subdirs = NULL;
	
	//Adding child to parent subdirs if it's the first one
	if(parent->subdirs == NULL)
	{
		parent->subdirs = (s_directory*)malloc(sizeof(s_directory));
		parent->subdirs = child;
		return 0;
	}
	
	//Find penultimate dir
	s_directory *temp = parent->subdirs;
	while(temp->next_dir != NULL)
	{
		temp = temp->next_dir;
	}
	
	temp->next_dir = (s_directory*)malloc(sizeof(s_directory));
	temp->next_dir = child;
	
	return 0;
}

int append_file(s_file *child, s_directory *parent)
{
	//Setting initial child's pointer
	child->next_file = NULL;
	
	//Adding child to parent files if it's the first one
	if(parent->files == NULL)
	{
		parent->files = (s_file*)malloc(sizeof(s_file));
		parent->files = child;
	}
	
	//Finding penultimate file
	s_file *temp = parent->files;
	while(temp->next_file != NULL)
	{
		temp = temp->next_file;
	}
	
	temp->next_file = (s_file*)malloc(sizeof(s_file));
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
		
		//Switch to the next subdir after free
		parent->subdirs = parent->subdirs->next_dir;
		free(current_dir);
		current_dir = parent->subdirs;
	}
}
