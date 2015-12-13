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

#include "main.h"

int main (int argc, char **argv) {

    tError error;
    error = ERR_OK;

    // kontrola argumentu
    if (argc != 2){
        error = ERR_INTER;
        //printf("%d\n", error);
        return error;
    }

	FILE *f;

    // otevreni souboru
    if ((f = fopen(argv[1], "r")) == NULL){
        error = ERR_INTER;
        //printf("%d\n", error);
        return error;
 	} 

    setFile (f);

    // syntakticka analyza
    error = s_syntax();

    if (error == ERR_OK) {
        
        // interpretace
        error = preloz();
    }

    //printf("\nerror : %d\n", error);

    fclose(f); //zavreni souboru
    killThemAll(); //uvolneni naalokovane pameti
   	
    return error;
}