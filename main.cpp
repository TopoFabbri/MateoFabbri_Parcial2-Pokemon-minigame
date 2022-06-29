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
			system("cls");
			PrintTitle("RULES");
			std::cout << R"(
Es necesario para el comienzo del juego la creacion de 6 pokemones, teniendo este dos modos, en el MultiPlayer,
el P2 tendra que crear otros 6. El juego consta
de una sucesion de ataques y defensas, siendo el perdedor aquel que quede sin pokemons vivos.)";
			system("pause");
			Nav = SCENES::Menu;
			break;

		case SCENES::Author:
			// by: Mateo Fabbri
			system("cls");
			std::cout << "Topacio" << std::endl;
			system("pause");
			Nav = SCENES::Menu;
			break;

		default:
			break;
		}
	} while (Nav != SCENES::Quit);
}