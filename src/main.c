#include <stdio.h>
#include "include/pcp.h"

int main(int argc, char* argv[]){
    if (argc < 2) {
        printf("You need to provide file to compile.\n");
        return 1;
    }

    pcp_compile_file(argv[1]);

    return 0;
}
