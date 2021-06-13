#include <stdlib.h>
#include <stdio.h>

#include "tree.h"
#include "scan.h"

int main (int argc, char* argv[]) {
	
    s_directory *dir = process_dir("testDirectory");
    printf("%s %ld",dir->name,dir->mod_time);
    
    // Clean up after execution
    clear_subdirs(dir);
    free(dir); // Needed because clear_subdirs doesn't free the parent

    return EXIT_SUCCESS;
}
