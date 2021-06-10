#include <stdlib.h>
#include <stdio.h>

#include "tree.h"
#include "scan.h"

int main (int argc, char* argv[]) {
	
    s_directory *dir = process_dir("testDirectory");
    printf("%s %ld",dir->name,dir->mod_time);
    clear_subdirs(dir);
    return 0;
}
