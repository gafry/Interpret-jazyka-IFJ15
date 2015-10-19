/*
* STAVY AUTOMATU
*/

typedef enum
{
    //RIDICI STAVY
    S_START,        //  0 START
    S_KONEC,        //  1 KONEC
    S_IDENT,        //  2 IDENTIFIKÁTOR
    S_KLIC,         //  3 KLICOVA SLOVA

    //CHYBOVE STAVY
    S_CHYBAVS,      //  4 CHYBA NA VSTUPU
    S_CHYBABE,      //  5 CHYBA ZA BEHU

    // DATOVE TYPY
    S_TYPDOUBLE,    //  6
    S_TYPINT,       //  7
    S_TYPSTRING,    //  8

    //KLICOVA SLOVA
    S_MAIN,			//  9 MAIN
    S_AUTO = 10,    // 10 AUTO
    S_CIN,          // 11 CIN
    S_COUT,         // 12 COUT
    S_DOUBLE,       // 13 DOUBLE
    S_ELSE,         // 14 ELSE
    S_FOR,          // 15 FOR
    S_IF,           // 16 IF
    S_INT,          // 17 INT
    S_RETURN,       // 18 RETURN
    S_STRING,       // 19 STRING

    //ZNAKY
    S_CARKA,        // 20 ,
    S_KOMENT,       // 21 // + KOMENTAR DO KONCE RADKU
    S_LKOM,         // 22 /* + ZACATEK BLOKU KOMENTARU
    S_PKOM,         // 23 */ KONEC BLOKU KOMENTARU
    S_LPRIR,        // 24 <<
    S_PPRIR,        // 25 >>
    S_TECKA,        // 26 .     PRO DESETINNA CISLA
    S_STREDNIK,     // 27 ;
    S_LZ,           // 28 (
    S_PZ,           // 29 )
    S_SLZ,          // 30 {
    S_SPZ,          // 31 }

    //POMOCNE STAVY
    S_DPOM,         // 32 DOUBLE JAKO DESETINNE CISLO
    S_DEXP,         // 33 DOUBLE JAKO EXPONENT
    S_DEXP1,        // 34
    S_DEXPP,        // 35 DOUBLE JAKO EXPONENT POMOCNY
    S_DPOM2,  		// 36 PRO HEXADECIMALNI CISLA
    S_KOMENTP,      // 37 ZJISTI OBSAH RDKOVEHO KOMENTARE

    //MATEMATICKE OPERACE
    S_ADD = 40,     // 40 +
    S_DEK,          // 41 --
    S_DIV,          // 42 /
    S_INK,          // 43 ++
    S_MEN,          // 44 <
    S_MUL,          // 45 *
    S_PRIR,         // 46 =
    S_ROV,          // 47 ==
    S_NEROV,        // 48 !=
    S_MENROV,       // 49 <=
    S_VECROV,       // 50 >=
    S_SUB,          // 51 -
    S_VEC,          // 52 >
    S_VYKR,         // 53 !  NEBO NEGACE

    S_EOF = 60      // 60 EOF
} tStav;

/*
*   STRUKTURA PRO TOKEN
*/

typedef struct
{
    tStav stav;
    char *data;
} tToken;

/*
* ZAJISTI ABY TOKEN BYL GLOBALNI PROMENNOU
*/

extern tToken token;

/*
* PROTOTYPY FUNKCI
*/

tToken getToken();  // VRATI TOKEN
void setFile (FILE *f);