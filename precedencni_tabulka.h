/*
 * TODO: 
 		pridat chybejici vyrazy do precedencni tabulky
*/

#define RADKY   14
#define SLOUPCE 14

//data do precedencni tabulky
typedef enum
{
	V = 0,	//>
	M = 1,	//<
	R = 2,	//=
	C = 3	//syntakticka chyba
} tTab;

typedef enum
{
	PLUS       = 0,		// +
	MINUS      = 1,		// -
	KRAT       = 2,		// *
	DELENO     = 3,		// /
	ROVNASE	   = 4,		// ==
	NEROVNASE  = 5,		// !=
	MENSIROVNO = 6,		// <=
	VETSIROVNO = 7,		// >=
	MENSI      = 8,		// <
	VETSI      = 9,		// >
	LKULZAV    = 10,	// (
	PKULZAV    = 11,	// )
	STREDNIK   = 12,	// ;
	PRIRAD	   = 13,	// =
	NEGACE	   = 14,	// !
	CARKA	   = 15,	// ,
	ID         = 16,	// konstanta nebo nazev promenne
	DOLAR      = 17		// terminal
} tTabOperace;

//chyby pri vyhodnocovani tokenu
typedef enum tErrVyraz
{
	ERR_OK  = 0,	//no problem
	ERR_LEX = 1,	//lexikalni chyba
	ERR_SYN = 2,	//syntakticka chyba
	ERR_SEM = 3		//semanticka chyba
} tErrVyr;

//precedencni tabulka
const tTab precedencni_tabulka[RADKY][SLOUPCE]
{
	{V, V, M, M, V, V, V, V, V, V, M, V, M, V},
	{V, V, M, M, V, V, V, V, V, V, M, V, M, V},
	{V, V, V, V, V, V, V, V, V, V, M, V, M, V},
	{V, V, V, V, V, V, V, V, V, V, M, V, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	{M, M, M, M, M, M, M, M, M, M, M, R, R, C},
	{V, V, V, V, V, V, V, V, V, V, C, V, R, V},
	{V, V, V, V, V, V, V, V, V, V, R, R, R, V},
	{M, M, M, M, M, M, M, M, M, M, M, M, M, R},
};

//polozka pro vyrazy
typedef struct
{
	tTabOperace operace;
	tData data;
} tTokenResolved;
