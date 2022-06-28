#include "Game.h"

void RunGame()
{
	GAMEDATA data;                      // Parameters struct
	STAGES Stage = STAGES::Reset;       // Stage navigator
	data.maxDim = { pokQty, sides };      // World limits on struct
	data.txtPos = { 0, 4 };             // Before board text position
	data.cursor = { 0, 0 };               // Game cursor
	data.pmptPos = { 0, 15 };             // After board text position
	char keys[inputKeyQty] = { 'w', 'a', 's', 'd', '1', '0' }; // Temporary input 
	ArraySet(data.input, keys);         // Game controls
	bool keepPlaying = true;            // Should match stop

	do
	{
		switch (Stage)
		{
		case STAGES::Reset:
			system("cls");
			ResetMap(data);

			Stage = STAGES::Selection;

			break;

		case STAGES::Selection:
			PrintTitle("POKEMON");

			data.player = 0;
			keepPlaying = SelectPokemons(data);

			if (data.multiplayer)
			{
				data.player = 1;
				keepPlaying = SelectPokemons(data);
			}
			else
			{
				// CPU select pokemon
			}

			Stage = STAGES::SetPokemons;

			break;

		case STAGES::SetPokemons:
			data.player = 0;
			keepPlaying = PrepPokemon(data);

			if (data.multiplayer)
			{
				data.player = 1;
				keepPlaying = PrepPokemon(data);
			}

			Stage = STAGES::Fight;
			keepPlaying = Fight(data);
			break;

		case STAGES::Fight:
			keepPlaying = Fight(data);
			break;

		case STAGES::End:
			keepPlaying = false;
			break;

		default:
			break;
		}
	} while (keepPlaying);
}

bool SelectPokemons(GAMEDATA& data)
{
	const int pokQty = 6;
	const int charmanderQty = 3;
	const int charmeleonQty = 2;
	const int charizardQty = 1;
	const COORD txtPos = { 1, 4 };
	INTER actions;

	// Choose 3 positions for Charmander
	for (int i = 0; i < charmanderQty; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), txtPos);
		std::cout << "Choose where to place Charmander " << i + 1;

		do
		{
			actions = MapNavigation(data);

			if (actions == INTER::Quit)
				return false;
		} while (data.cursor.Y != data.player);
		data.pkmn[data.cursor.Y][data.cursor.X].lvl = 1;
		std::cout << "Choose a name for your Charmander: " << std::endl;
		std::cin >> data.pkmn[data.cursor.Y][data.cursor.X].name;
		CleanAfterBoard(data.pmptPos);
	}

	// Choose 2 Charmeleon
	for (int i = 0; i < charmeleonQty; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), txtPos);
		std::cout << "Choose where to place Charmeleon " << i + 1;
		do
		{
			actions = MapNavigation(data);

			if (actions == INTER::Quit)
				return false;
		} while (data.cursor.Y != 1);
		data.pkmn[data.cursor.Y][data.cursor.X].lvl = 2;
		std::cout << "Choose a name for your Charmeleon: " << std::endl;
		std::cin >> data.pkmn[data.cursor.Y][data.cursor.X].name;
		CleanAfterBoard(data.pmptPos);
	}

	// Choose 1 Charizard
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), txtPos);
	std::cout << "Choose where to place your Charizard";
	do
	{
		actions = MapNavigation(data);
	} while (data.cursor.Y != 1);
	data.pkmn[data.cursor.Y][data.cursor.X].lvl = 3;
	std::cout << "Choose a name for your Charizard: " << std::endl;
	std::cin >> data.pkmn[data.cursor.Y][data.cursor.X].name;
	CleanAfterBoard(data.pmptPos);

	return true;
}

INTER MapNavigation(GAMEDATA& data)
{
	const COORD mapInitPos = { 0, 6 };
	const COORD pokSize = { 20, 5 };
	INTER selected = INTER::None;

	std::cout << std::endl;

	do
	{
		for (int side = 0; side < data.maxDim.Y; side++)
		{
			for (int pok = 0; pok < data.maxDim.X; pok++)
			{
				COORD newPos = { pokSize.X * pok + mapInitPos.X, pokSize.Y * side + mapInitPos.Y };
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);

				if (side == data.cursor.Y && pok == data.cursor.X)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WhiteOnBlack);
				else if (side == 0)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RedOnBlack);
				else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GreenOnBlack);

				LevelToDrawing(data.pkmn[side][pok].lvl, newPos);

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WhiteOnBlack);
			}
		}

		selected = GameInput(data);
	} while (selected == INTER::None);

	return selected;
}

INTER GameInput(GAMEDATA& data)
{
	char key = _getch();

	if (key == data.input[(char)KEYS::Up])
	{
		if (data.cursor.Y <= 0)
		{
			data.cursor.Y = data.maxDim.Y - 1;
		}
		else
		{
			data.cursor.Y--;
		}
	}
	else if (key == data.input[(char)KEYS::Left])
	{
		if (data.cursor.X <= 0)
		{
			data.cursor.X = data.maxDim.X - 1;
		}
		else
		{
			data.cursor.X--;
		}
	}
	else if (key == data.input[(char)KEYS::Down])
	{
		if (data.cursor.Y >= data.maxDim.Y - 1)
		{
			data.cursor.Y = 0;
		}
		else
		{
			data.cursor.Y++;
		}
	}
	else if (key == data.input[(char)KEYS::Right])
	{
		if (data.cursor.X >= data.maxDim.X - 1)
		{
			data.cursor.X = 0;
		}
		else
		{
			data.cursor.X++;
		}
	}
	else if (key == data.input[(char)KEYS::Enter])
	{
		return INTER::Select;
	}
	else if (key == data.input[(char)KEYS::Back])
	{
		return INTER::Quit;
	}

	return INTER::None;
}

bool Fight(GAMEDATA& data)
{
	data.player = 0;
	ATTK attkChoice;
	INTER actions = INTER::None;
	int attkId;

	do
	{
		ChooseAttackerPkmn(data, actions);
		if (actions == INTER::Quit)
			return false;

		attkChoice = ChooseAttack(data, attkId);

		ChooseAttackedPkmn(data, actions);
		if (actions == INTER::Quit)
			return false;

		Defense(data.pkmn[data.cursor.Y][data.cursor.X], attkChoice, attkId);
	} while (CheckPokemonLeft(data));

	return false;
}

ATTK ChooseAttack(GAMEDATA data, int& attkId)
{
	char key;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
	PrintTitle("FIGHT");
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.txtPos);
	std::cout << "Choose an attack";

	do
	{
		system("cls");
		for (int i = 0; i < attkQty; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BlueOnBlack);
			std::cout << i + 1 << ": ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WhiteOnBlack);
			std::cout << data.pkmn[data.cursor.Y][data.cursor.X].att[i].name;
		}

		key = _getch() - 48;
	} while (key < 1 || key >= attkQty);

	attkId = (int)key;
	return data.pkmn[data.cursor.Y][data.cursor.X].att[(int)key - 1];
}

void ChooseAttackedPkmn(GAMEDATA& data, INTER& actions)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
	PrintTitle("FIGHT");
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.txtPos);
	std::cout << "Choose which pokemon to attack to";

	do
	{
		actions = MapNavigation(data);

		if (actions == INTER::Quit)
			return;

	} while (data.cursor.Y == data.player);
}

void ChooseAttackerPkmn(GAMEDATA& data, INTER& actions)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
	PrintTitle("FIGHT");
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.txtPos);
	std::cout << "Choose which pokemon will attack";

	do
	{
		actions = MapNavigation(data);

		if (actions == INTER::Quit)
			return;

	} while (data.cursor.Y != data.player);
}

void Defense(POKEMON& defender, ATTK attk, int attkId)
{
	system("cls");

	if ((rand() % 3 + 1) == attkId)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RedOnBlack);
		std::cout << attk.name << " dealt " << attk.damage << "damage!";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WhiteOnBlack);

		defender.DamagePokemon(attk.damage);

		if (defender.hp <= 0)
			defender.hp = defender.initHp;
		defender.lives--;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GreenOnBlack);
		std::cout << attk.name << "No damage was dealt!";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WhiteOnBlack);
	}

	if (defender.lives < 0)
	{
		system("cls");
		std::cout << defender.name << " has fainted.";
		defender.DestroyPokemon();
	}
}

bool CheckPokemonLeft(GAMEDATA& data)
{
	bool hasPokemonsP1 = false;
	bool hasPokemonsP2 = false;

	for (int side = 0; side < sides; side++)
	{

		for (int pok = 0; pok < pokQty; pok++)
		{
			if (side == 0 && data.pkmn[side][pok].lvl > 0)
			{
				hasPokemonsP1 = true;
				break;
			}
			else if (side == 1 && data.pkmn[side][pok].lvl > 0)
			{
				hasPokemonsP2 = true;
				break;
			}
		}
	}

	return (hasPokemonsP1 && hasPokemonsP2);
}

POKEMON CreatePokemon(std::string name, int lvl, int lives, int res, ATTK att[3])
{
	POKEMON pok = { name, lvl, lives, res };

	for (int i = 0; i < pkmnAttkQty; i++)
	{
		pok.att[i] = att[i];
	}

	return pok;
}

bool PrepPokemon(GAMEDATA& data)
{
	struct TOSPEND
	{
		int res = initHPTokens;
		int lives = initLifeTokens;
	};

	int resTmp = 0;
	int livesTmp = 0;

	TOSPEND toSpend;
	INTER actions = INTER::None;

	system("cls");

	do
	{
		do
		{
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
			PrintTitle("POKEMON PREP");

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.txtPos);
			toSpend.res = CalculateTokens(toSpend.res, data.pkmn, data.player, initHPTokens);
			std::cout << "HP tokens to spend: " << toSpend.res;

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.pmptPos);
			actions = MapNavigation(data);
		} while (data.cursor.Y != 1);

		do
		{
			std::cout << data.pkmn[data.cursor.Y][data.cursor.X].hp << std::endl;
			std::cin >> resTmp;
			CleanAfterBoard(data.pmptPos);

			if (toSpend.res - resTmp < 0)
			{
				std::cout << "You don't have that much HP tokens, try again: ";
			}
		} while (toSpend.res - resTmp < 0 || resTmp < 0);
		data.pkmn[data.cursor.Y][data.cursor.X].hp = resTmp;
	} while (toSpend.res != 0);

	return true;
}

int CalculateTokens(int tokens, POKEMON pkmn[sides][pokQty], int player, int initTokens)
{
	tokens = initTokens;
	for (int pokN = 0; pokN < pokQty; pokN++)
	{
		tokens -= pkmn[player][pokN].hp;
	}
	return tokens;
}

void ArraySet(char array[], char value[])
{
	for (int i = 0; i < inputKeyQty; i++)
	{
		array[i] = value[i];
	}
}

void LevelToDrawing(int lvl, COORD newPos)
{
	switch (lvl)
	{
	case 1:
		std::cout << "--------------";
		newPos.Y++;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);
		std::cout << "| CHARMANDER |";
		newPos.Y++;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);
		std::cout << "--------------";
		break;

	case 2:
		std::cout << "--------------";
		newPos.Y++;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);
		std::cout << "| CHARMELEON |";
		newPos.Y++;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);
		std::cout << "--------------";
		break;

	case 3:
		std::cout << "--------------";
		newPos.Y++;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);
		std::cout << "| CHARIZARD  |";
		newPos.Y++;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);
		std::cout << "--------------";
		break;

	default:
		std::cout << "--------------";
		newPos.Y++;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);
		std::cout << "|    NONE    |";
		newPos.Y++;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);
		std::cout << "--------------";
		break;
	}
	std::cout << std::endl << std::endl;
}

void CleanAfterBoard(COORD pmptPos)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pmptPos);
	std::cout << "                                                                                " << std::endl
		<< "                                                                                " << std::endl
		<< "                                                                                " << std::endl
		<< "                                                                                " << std::endl;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pmptPos);
}

void ResetMap(GAMEDATA& data)
{
	for (int side = 0; side < data.maxDim.Y; side++)
	{
		for (int pok = 0; pok < data.maxDim.X; pok++)
		{
			data.pkmn[side][pok].name = "None";
		}
	}
}