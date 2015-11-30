#include <stdlib.h>
#include <stdio.h>

#include "lexikalni_analyzator.h"
#include "alokator.h"
#include "syntax.h"

int main (int argc, char **argv) {

    tError error;
    error = ERR_OK;

    if (argc != 2){
        error = ERR_INTER;
        printf("%d\n", error);
        return error;
    }

	FILE *f;

    if ((f = fopen(argv[1], "r")) == NULL){
        error = ERR_INTER;
        printf("%d\n", error);
        return error;
 	} 

    setFile (f);

    error = s_syntax();

    printf("%d\n", error);

    fclose(f);
    killThemAll();
   	
    return error;
}