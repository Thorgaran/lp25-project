#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <errno.h>

#include "tree.h"
#include "scan.h"
#include "save.h"

int main (int argc, char* argv[]) {
    char *input_path = NULL;
    char *output_path = NULL;

    int opt = 0;
    while((opt = getopt(argc, argv, "o:si:")) != -1) {
        switch(opt) {
            case 'o':
                // File to which everything is saved is specified in optarg
                output_path = (char *)malloc(sizeof(char)*(strlen(optarg) + 1));
                if(output_path==NULL)
                {
                    fprintf(stderr, "Error: Memory not available!");
                    return EXIT_FAILURE;
                }
                
                strcpy(output_path, optarg);
                break;

            case 's':
                //enable md5
                break;

            case 'i':
                // Directory to analyze is specified in optarg
                input_path = (char *)malloc(sizeof(char)*(strlen(optarg) + 1));
                if(input_path==NULL)
                {
                    fprintf(stderr, "Error: Memory not available!");
                    return EXIT_FAILURE;
                }

                strcpy(input_path, optarg);
                break;

            default:
                break;
        } 
    }

    // If the user didn't specify an input path, use default "."
    if (input_path == NULL) {
        input_path = (char *)malloc(sizeof(char)*(strlen(".") + 1));
        if(input_path==NULL)
        {
            fprintf(stderr, "Error: Memory not available!");
            return EXIT_FAILURE;
        }

        strcpy(input_path, ".");
    }

    printf("Input: %s\n", input_path);
    printf("Output: %s\n", output_path);

    // Read directory structure and save it in tree structure
    s_directory *dir = process_dir(input_path);
    printf("%s %ld\n", dir->name, dir->mod_time);

    free(input_path);

    // Save tree structure to file
    if (save_to_file(dir, output_path) != 0) {
        fprintf(stderr, "Error: failed to save to file %s! %s\n", output_path, strerror(errno));
		return EXIT_FAILURE;
    }

    // Output_path is freed inside save_to_file
    
    // Clean up after execution
    clear_subdirs(dir);
    free(dir); // Needed because clear_subdirs doesn't free the parent

    return EXIT_SUCCESS;
}
