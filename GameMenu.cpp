#include "GameMenu.h"

void MainMenu(SCENES& Nav)
{
	const int opsQty = 3;									// Amount of displayed options
	char menuInput[20] = { 'w', 'a', 's', 'd', '1', '0' };	// Menu controls
	int selector;											// Save selected option
	char ops[10][10] =										// Box texts
	{
		{' ', ' ', ' ', 'P', 'L', 'A', 'Y', ' ', ' ', ' '},
		{' ', ' ', 'R', 'U', 'L', 'E', 'S', ' ', ' ', ' '},
		{' ', ' ', 'A', 'U', 'T', 'H', 'O', 'R', ' ', ' '}
	};

	system("cls");
	PrintTitle("MENU");
	MenuNavigator(opsQty, ops, menuInput, selector);
	Nav = (SCENES)selector;
}