/*
 * TODO: 
 		pridat chybejici vyrazy do precedencni tabulky
*/

#define RADKY   17
#define SLOUPCE 17

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
	LMNOZZAV   = 12,	// {
	PMNOZZAV   = 13,	// }
	STREDNIK   = 14,	// ;
	DEKREMENT  = 15,	// --
	INKREMENT  = 16,	// ++
	PRIRAD	   = 17,	// =
	NEGACE	   = 18,	// !
	CARKA	   = 19,	// ,
	KOMENT	   = 20,	// //
	LKOM	   = 21,	// /*
	PKOM	   = 22,	// */
	LPRIR	   = 23,	// <<
	PPRIR	   = 24,	// >>
	ID         = 25,	// konstanta nebo nazev promenne
	FUNKCE	   = 26,	// zavolani funkce
	DOLAR      = 27		// terminal
} tTabOperace;

//chyby pri vyhodnocovani tokenu
typedef enum tErrVyraz
{
	ERR_OK  = 0,	//no problem
	ERR_LEX = 1,	//lexikalni chyba
	ERR_SYN = 2,	//syntakticka chyba
	ERR_SEM = 3	//semanticka chyba
} tErrVyr;

//precedencni tabulka
const tTab precedencni_tabulka[RADKY][SLOUPCE]
{
	{V, V, M, M, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{V, V, M, M, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{V, V, V, V, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{V, V, V, V, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{M, M, M, M, V, V, V, V, V, V, M, V, V, V, M, M, V},
	{M, M, M, M, M, M, M, M, M, M, M, R, V, V, R, M, C},
	{V, V, V, V, V, V, V, V, V, V, C, V, V, V, R, R, V},
	{M, M, M, M, M, M, M, M, M, M, M, M, V, R, R, M, C},
	{V, V, V, V, V, V, V, V, V, V, C, M, C, V, R, R, V},
	{V, V, V, V, V, V, V, V, V, V, R, R, R, R, R, R, V},
	{R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
	{M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, R},
};

//polozka pro vyrazy
typedef struct
{
	tTabOperace operace;
	tHodnota hodnota;
	tData data;
} TokenResolved;
