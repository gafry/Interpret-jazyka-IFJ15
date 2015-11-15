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