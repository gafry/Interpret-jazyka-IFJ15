#include <stdlib.h>
#include <stdio.h>

#include "lexikalni_analyzator.h"
#include "alokator.h"
#include "str.h"

int main (int argc, char **argv) {

	FILE *f;

	if ((f = fopen(argv[1], "r")) == NULL)
 	{
 	    printf("Soubor se nepodarilo otevrit\n");
 	   	return -1;
  	} 

  	setFile (f);

  	token = getToken();

   	while (token.stav != S_EOF) {
   		printf("%d			%s\n", token.stav, token.data);
   		token = getToken();
   	}

   	fclose(f);
   	killThemAll();
   	
   	return 0;

}