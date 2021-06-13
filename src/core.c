#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "core.h"

char *merge_path(char *path, char* file_name)
{
	//+2 because we will add "/" between and "\0" at the end
	char *newPath = (char *)malloc(sizeof(char)*(strlen(path)+strlen(file_name)+2));
	
	if(newPath==NULL)
	{
		fprintf(stderr, "Error: Memory not available!");
		exit(EXIT_FAILURE);
	}
	
	strcpy(newPath, path);
	strcat(newPath, "/");
	strcat(newPath, file_name);
	//strcat adds automatically the null terminated byte 
	
	return newPath;
}
