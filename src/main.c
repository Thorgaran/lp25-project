#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 

#include "tree.h"
#include "scan.h"
#include "save.h"


int main (int argc, char* argv[]) {
	
    int opt = 0;
    while((opt = getopt(argc, argv, "o:si:")) != -1)   {
        switch(opt) {
            case 'o':
                //file in which everything is saved is specified in optarg
                s_directory *dir = process_dir("testDirectory");
                if (save_to_file(dir,optarg) != 0) {
                    printf("Error : Failed to save in file %s\n", optarg);
                    return -1;
                } 
                break;
            case 's':
                //enable md5
                break;
            case 'i':
                //directory to analize is specified in optarg
                s_directory *dir = process_dir(optarg);
                if (save_to_file(dir,"no file specified") != 0) {
                    printf("Error : Failed to save in file\n");
                    return -1;
                } 
                break;
            default:
                break;
        } 
    } 

    s_directory *dir = process_dir("testDirectory");
    printf("%s %ld",dir->name,dir->mod_time);
    
    // Clean up after execution
    clear_subdirs(dir);
    free(dir); // Needed because clear_subdirs doesn't free the parent

    return EXIT_SUCCESS;
}
