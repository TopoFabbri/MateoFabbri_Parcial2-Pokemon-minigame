#include "GameMenu.h"
#include "Game.h"

using namespace std;

void RunProgram();
void Defaults();
void Game();

void main()
{
    RunProgram();
}

void RunProgram()
{
    Defaults();
    Game();
}

void Defaults()
{
    srand(time(nullptr));
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    SetConsoleTitle(L"MateoFabbri_Parcial2 - Pokemon");
    system("color 07");
}

void Game()
{
	SCENES Nav = SCENES::Menu;				   // Main scene Navigator

    do
    {
        switch (Nav)
        {
        case SCENES::Menu:
			MainMenu(Nav);
			break;

		case SCENES::Quit:
			return;

		case SCENES::Play:
			// Start game
			RunGame();
			Nav = SCENES::Menu;
			break;

		case SCENES::Rules:
			// View rules
			std::cout << "View rules";
			system("pause");
			Nav = SCENES::Menu;
			break;

		case SCENES::Author:
			// by: Mateo Fabbri
			std::cout << "Topacio";
			system("pause");
			Nav = SCENES::Menu;
			break;

		default:
			break;
		}
	} while (Nav != SCENES::Quit);
}