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
	PLUS       = 0,
	MINUS      = 1,
	KRAT       = 2,
	DELENO     = 3,
	ROVNASE	   = 4,
	NEROVNASE  = 5,
	MENSIROVNO = 6,
	VETSIROVNO = 7,
	MENSI      = 8,
	VETSI      = 9,
	LKULZAV    = 10,	// (
	PKULZAV    = 11,	// )
	LMNOZZAV   = 12,	// {
	PMNOZZAV   = 13,	// }
	ID         = 24,
	FUNKCE	   = 25,	//zavolani funkce
	DOLAR      = 26
} tTabOperace;

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
