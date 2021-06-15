#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "md5sum.h"

#define BLOCK_SIZE 512

int compute_md5(char *path, unsigned char buffer[])
{   
    // Open file
    FILE *file_ptr = NULL;
    fpos_t saved_pos;
	file_ptr = fopen(path, "rb");
    if(file_ptr == NULL)
	{
		fprintf(stderr, "Error: Couldn't open file %s to compute md5sum! %s\n", path, strerror(errno));
		return -1;
	}

    MD5_CTX ctx;
    char tmp_file_read_buf[BLOCK_SIZE];
    ssize_t nb_block_read;
    int md5_ret_val; // These functions return 1 on sucess, 0 on error

    // Initialize ctx structure
    md5_ret_val = MD5_Init(&ctx);
    if(md5_ret_val != 1)
	{
		fprintf(stderr, "Error: Something went wrong with MD5_Init() for %s! %s\n", path, strerror(errno));
		return -1;
	}
    
    // Read blocks of 512
    if (fgetpos(file_ptr, &saved_pos) != 0) { return -1; } // Save file_ptr position
    nb_block_read = fread(tmp_file_read_buf, BLOCK_SIZE, 1, file_ptr);
    if (ferror(file_ptr) != 0)
    {
        fprintf(stderr, "Error: couldn't read file %s! %s\n", path, strerror(errno));
        return -1;
    }

    while(nb_block_read > 0)
    {
        // Update ctx with a new chunk of the file
        md5_ret_val = MD5_Update(&ctx, tmp_file_read_buf, BLOCK_SIZE);
        if(md5_ret_val != 1)
        {
            fprintf(stderr, "Error: Something went wrong with MD5_Update() for %s! %s\n", path, strerror(errno));
            return -1;
        }

        if (fgetpos(file_ptr, &saved_pos) != 0) { return -1; } // Save file_ptr position
        nb_block_read = fread(tmp_file_read_buf, BLOCK_SIZE, 1, file_ptr);
        if (ferror(file_ptr) != 0)
        {
            fprintf(stderr, "Error: couldn't read file %s! %s\n", path, strerror(errno));
            return -1;
        }
    }

    // Read leftovers chars one by one
    if (fsetpos(file_ptr, &saved_pos) != 0) { return -1; } // Put file pointer back before previous read that reached EOF
    nb_block_read = fread(tmp_file_read_buf, 1, 1, file_ptr);
    if (ferror(file_ptr) != 0)
    {
        fprintf(stderr, "Error: couldn't read file %s! %s\n", path, strerror(errno));
        return -1;
    }

    while(nb_block_read > 0)
    {
        // Update ctx with a new chunk of the file
        md5_ret_val = MD5_Update(&ctx, tmp_file_read_buf, 1);
        if(md5_ret_val != 1)
        {
            fprintf(stderr, "Error: Something went wrong with MD5_Update() for %s! %s\n", path, strerror(errno));
            return -1;
        }

        nb_block_read = fread(tmp_file_read_buf, 1, 1, file_ptr);
        if (ferror(file_ptr) != 0)
        {
            fprintf(stderr, "Error: couldn't read file %s! %s\n", path, strerror(errno));
            return -1;
        }
    }
    

    // Place message digest in buffer then erase ctx
    md5_ret_val = MD5_Final(buffer, &ctx);
    if(md5_ret_val != 1)
    {
        fprintf(stderr, "Error: Something went wrong with MD5_Final() for %s! %s\n", path, strerror(errno));
        return -1;
    }

    fclose(file_ptr);

    return 0;
}
