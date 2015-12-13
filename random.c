/*
 * Implementace interpretu imperativního jazyka IFJ15
 *
 * Zadání: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2015.pdf
 *
 * Tým 094, varianta b/3/II:

 * Jakub Menšík - vedoucí (xmensi03)
 * Vojtěch Měchura (xmechu00)
 * Matěj Moravec (xmorav32)
 * Jan Morávek (xmorav33)
 * Jan Svoboda (xsvobo0u)
 *
 */

#include "random.h"

static unsigned long long int random_num = 999999999;
tError error;

// generuje jmena promennych
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