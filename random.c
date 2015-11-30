#include "random_name.h"

static unsigned long long int random_num = 999999999;
tError error;

char* generateVar(){

	if(random_num < 100000000){
		error = ERR_INTER;
	}

	char* newName = newMalloc(13*sizeof(char));
	if (newName == NULL){
		error = ERR_INTER;
	}
	
	if(sprintf(newName, "var%lld", random_num) < 0){
		error = ERR_INTER;
	}

	random_num--;
	return newName;
}