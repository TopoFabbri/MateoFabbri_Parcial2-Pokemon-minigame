#include "Game.h"

void RunGame()
{
	GAMEDATA data;                      // Parameters struct
	STAGES Stage = STAGES::Reset;       // Stage navigator
	data.maxDim = { pokQty, sides };    // World limits on struct
	data.txtPos = { 0, 4 };				// Before board text position
	data.cursor = { 0, 0 };             // Game cursor
	data.pmptPos = { 0, 15 };           // After board text position
	char keys[inputKeyQty] = { 'w', 'a', 's', 'd', '1', '0' }; // Temporary input 
	ArraySet(data.input, keys);         // Game controls
	bool keepPlaying = true;            // Should match stop
	int selection;
	char ops[10][10]
	{
		{' ', ' ', 'S', 'I', 'N', 'G', 'L', 'E', ' ', ' '},
		{' ', ' ', 'M', 'U', 'L', 'T', 'I', ' ', ' ', ' '},
	};

	system("cls");
	MenuNavigator(2, ops, data.input, selection);
	data.multiplayer = (selection != 1);

	do
	{
		switch (Stage)
		{
		case STAGES::Reset:
			system("cls");
			ResetMap(data);

			Stage = STAGES::Fight;

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
				CpuPokemons(data);
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

void CpuPokemons(GAMEDATA& data)
{
	ATTK lvl1Attk[pkmnAttkQty] =
	{
		{"Scratch", 50},
		{"Ember", 75},
		{"Flamethrower", 100}
	};
	ATTK lvl2Attk[pkmnAttkQty] =
	{
		{"Scratch", 75},
		{"Ember", 100},
		{"Flamethrower", 150}
	};
	ATTK lvl3Attk[pkmnAttkQty] =
	{
		{"Scratch", 100},
		{"Ember", 150},
		{"Flamethrower", 300}
	};

	for (int pokN = 0; pokN < pokQty; pokN++)
	{
		if (pokN < 3)
		{
			data.pkmn[1][pokN].lvl = 1;
			data.pkmn[1][pokN].initHp = 500;
			data.pkmn[1][pokN].hp = 500;
			data.pkmn[1][pokN].lives = 1;
			data.pkmn[1][pokN].name = "Charmander";

			for (int attkN = 0; attkN < pkmnAttkQty; attkN++)
			{
				data.pkmn[1][pokN].att[attkN] = lvl1Attk[attkN];
			}
		}
		else if (pokN < 5)
		{
			data.pkmn[1][pokN].lvl = 2;
			data.pkmn[1][pokN].initHp = 1000;
			data.pkmn[1][pokN].hp = 1000;
			data.pkmn[1][pokN].lives = 2;
			data.pkmn[1][pokN].name = "Charmeleon";

			for (int attkN = 0; attkN < pkmnAttkQty; attkN++)
			{
				data.pkmn[1][pokN].att[attkN] = lvl2Attk[attkN];
			}
		}
		else
		{
			data.pkmn[1][pokN].lvl = 3;
			data.pkmn[1][pokN].initHp = 1500;
			data.pkmn[1][pokN].hp = 1500;
			data.pkmn[1][pokN].lives = 7;
			data.pkmn[1][pokN].name = "Charizard";

			for (int attkN = 0; attkN < pkmnAttkQty; attkN++)
			{
				data.pkmn[1][pokN].att[attkN] = lvl1Attk[attkN];
			}
		}
	}
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
		} while (data.cursor.Y != data.player);
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
	} while (data.cursor.Y != data.player);
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
				PrintPokemonStats(data);
			}
		}

		selected = GameInput(data);
	} while (selected == INTER::None);

	return selected;
}

void PrintPokemonStats(GAMEDATA data)
{
	CleanAfterBoard(data.pmptPos);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.pmptPos);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BlueOnBlack);
	std::cout << "    Pokemon's Stats:" << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WhiteOnBlack);
	std::cout << "    " << data.pkmn[data.cursor.Y][data.cursor.X].name << std::endl
		<< "    HP: " << data.pkmn[data.cursor.Y][data.cursor.X].hp << std::endl
		<< "    Lives: " << data.pkmn[data.cursor.Y][data.cursor.X].lives << std::endl
		<< "    Level: " << data.pkmn[data.cursor.Y][data.cursor.X].lvl << std::endl;

	for (int attkN = 0; attkN < pkmnAttkQty; attkN++)
	{
		std::cout << "    " << data.pkmn[data.cursor.Y][data.cursor.X].att[attkN].name
			<< " damage: " << data.pkmn[data.cursor.Y][data.cursor.X].att[attkN].damage << std::endl;
	}
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
	int winner;

	do
	{
		if (data.multiplayer || data.player == 0)
		{

			ChooseAttackerPkmn(data, actions);
			if (actions == INTER::Quit)
				return false;

			attkChoice = ChooseAttack(data, attkId);

			ChooseAttackedPkmn(data, actions);
			if (actions == INTER::Quit)
				return false;
		}
		else
		{
			do
			{
				attkChoice = data.pkmn[1][(rand() % pokQty)].att[(rand() % pkmnAttkQty)];
			} while (data.pkmn[1][(rand() % pokQty)].att[(rand() % pkmnAttkQty)].damage <= 0);

			do
			{
				data.cursor = { (short)(rand() % pokQty), 0 };
			} while (data.pkmn[data.cursor.Y][data.cursor.X].lives <= 0);
		}

		Defense(data.pkmn[data.cursor.Y][data.cursor.X], attkChoice, attkId);

		if (data.player == 0)
			data.player = 1;
		else
			data.player = 0;
	} while (CheckPokemonLeft(data, winner));

	system("cls");
	std::cout << "Player " << winner << " wins!";

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
		for (int i = 0; i < pkmnAttkQty; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BlueOnBlack);
			std::cout << i + 1 << ": ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WhiteOnBlack);
			std::cout << data.pkmn[data.cursor.Y][data.cursor.X].att[i].name << std::endl;
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
	std::cout << attk.name << "(" << attk.damage << ")" << "-->"
		<< defender.name << std::endl;
	if ((rand() % 3 + 1) == attkId)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RedOnBlack);
		std::cout << attk.name << " dealt " << attk.damage << " damage!" << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WhiteOnBlack);
		system("pause");

		defender.DamagePokemon(attk.damage);

		if (defender.hp <= 0)
		{
			defender.hp = defender.initHp;
			defender.lives--;
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GreenOnBlack);
		std::cout << "No damage was dealt!" << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WhiteOnBlack);
		system("pause");

	}

	if (defender.lives <= 0)
	{
		system("cls");
		std::cout << defender.name << " has fainted." << std::endl;
		system("pause");
		defender.Destroy();
	}
}

bool CheckPokemonLeft(GAMEDATA& data, int& winner)
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

	if (!hasPokemonsP1)
		winner = 2;
	else if (!hasPokemonsP2)
		winner = 1;

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
	struct TOKENS
	{
		int res = initHPTokens;
		int lives = initLifeTokens;
		int attk[pkmnAttkQty]
		{
			initAttkTokens[0],
			initAttkTokens[1],
			initAttkTokens[2]
		};
	};

	int resTmp = 0;
	int livesTmp = 0;
	int attkTmp[3]{ 0, 0, 0 };

	TOKENS toSpend;
	INTER actions = INTER::None;

	system("cls");
	do
	{
		do
		{
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
			PrintTitle("POKEMON PREP");

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.txtPos);
			toSpend.res = CalculateHPTokens(toSpend.res, data.pkmn, data.player);
			std::cout << "HP tokens to spend: " << toSpend.res << "           ";

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.pmptPos);
			actions = MapNavigation(data);
			if (actions == INTER::Quit)
				return false;

		} while (data.cursor.Y != data.player);

		do
		{
			std::cin >> resTmp;
			CleanAfterBoard(data.pmptPos);

			if (toSpend.res - resTmp < 0)
			{
				std::cout << "You don't have that much HP tokens, try again: ";
			}
		} while (toSpend.res - resTmp < 0 || resTmp < 0);
		data.pkmn[data.cursor.Y][data.cursor.X].hp = resTmp;
		data.pkmn[data.cursor.Y][data.cursor.X].initHp = resTmp;
		toSpend.res = CalculateHPTokens(toSpend.res, data.pkmn, data.player);
	} while (toSpend.res != 0 || !IsHPTokenSpreadValid(data));;

	do
	{
		do
		{
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
			PrintTitle("POKEMON PREP");

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.txtPos);
			toSpend.lives = CalculateLifeTokens(toSpend.lives, data.pkmn, data.player);
			std::cout << "Lives tokens to spend: " << toSpend.lives << "           ";

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.pmptPos);
			actions = MapNavigation(data);
			if (actions == INTER::Quit)
				return false;

		} while (data.cursor.Y != data.player);

		do
		{
			std::cin >> livesTmp;
			CleanAfterBoard(data.pmptPos);

			if (toSpend.res - resTmp < 0)
			{
				std::cout << "You don't have that much Lives tokens, try again: ";
			}
		} while (toSpend.attk[0] - livesTmp < 0 || livesTmp < 0);
		data.pkmn[data.cursor.Y][data.cursor.X].lives = livesTmp;
		toSpend.lives = CalculateLifeTokens(toSpend.res, data.pkmn, data.player);
	} while (toSpend.lives != 0 || !IsLivesTokenSpreadValid(data));

	for (int attkN = 0; attkN < pkmnAttkQty; attkN++)
	{
		do
		{
			do
			{
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
				PrintTitle("POKEMON PREP");

				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.txtPos);
				toSpend.attk[attkN] = CalculateAttackTokens(toSpend.attk[attkN], data.pkmn, data.player, attkN);
				std::cout << "Attack 1 tokens to spend: " << toSpend.attk[attkN] << "           ";

				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), data.pmptPos);
				actions = MapNavigation(data);
				if (actions == INTER::Quit)
					return false;

			} while (data.cursor.Y != data.player);

			do
			{
				std::cin >> attkTmp[attkN];
				CleanAfterBoard(data.pmptPos);

				if (toSpend.attk[attkN] - attkTmp[attkN] < 0)
				{
					std::cout << "You don't have that much Attack 1 tokens, try again: ";
				}
			} while (toSpend.attk[attkN] - attkTmp[attkN] < 0 || attkTmp[attkN] < 0);
			data.pkmn[data.cursor.Y][data.cursor.X].att[attkN].damage = attkTmp[attkN];
			toSpend.attk[attkN] = CalculateAttackTokens(toSpend.attk[attkN], data.pkmn, data.player, attkN);
		} while (toSpend.attk[attkN] != 0 || !IsAttackTokenSpreadValid(data, attkN));
	}

	return true;
}

bool IsHPTokenSpreadValid(GAMEDATA data)
{
	const int charmanderQty = 3;
	const int charmeleonQty = 2;

	int lvl2HPCap = initHPTokens;
	int lvl1HPCap = initHPTokens;

	for (int i = 0; i < pokQty; i++)
	{
		if (data.pkmn[data.player][i].lvl >= 3)
		{
			lvl2HPCap = data.pkmn[data.player][i].hp;
		}
		else if (data.pkmn[data.player][i].lvl == 2 && data.pkmn[data.player][i].hp < lvl1HPCap)
		{
			lvl1HPCap = data.pkmn[data.player][i].hp;
		}
	}

	for (int i = 0; i < pokQty; i++)
	{
		if (data.pkmn[data.player][i].hp <= 0)
			return false;

		if (data.pkmn[data.player][i].lvl <= 1 && data.pkmn[data.player][i].hp > lvl1HPCap)
			return false;
		else if (data.pkmn[data.player][i].lvl <= 2 && data.pkmn[data.player][i].hp > lvl2HPCap)
			return false;
	}

	return true;
}

bool IsAttackTokenSpreadValid(GAMEDATA data, int attkN)
{
	const int charmanderQty = 3;
	const int charmeleonQty = 2;

	int lvl2AttkCap = initAttkTokens[attkN];
	int lvl1AttkCap = initAttkTokens[attkN];

	for (int i = 0; i < pokQty; i++)
	{
		if (data.pkmn[data.player][i].lvl >= 3)
		{
			lvl2AttkCap = data.pkmn[data.player][i].att[attkN].damage;
		}
		else if (data.pkmn[data.player][i].lvl == 2 && data.pkmn[data.player][i].att[attkN].damage < lvl1AttkCap)
		{
			lvl1AttkCap = data.pkmn[data.player][i].att[attkN].damage;
		}
	}

	for (int i = 0; i < pokQty; i++)
	{
		if (data.pkmn[data.player][i].att[attkN].damage <= 0)
			return false;

		if (data.pkmn[data.player][i].lvl <= 1 && data.pkmn[data.player][i].att[attkN].damage > lvl1AttkCap)
			return false;
		else if (data.pkmn[data.player][i].lvl <= 2 && data.pkmn[data.player][i].att[attkN].damage > lvl2AttkCap)
			return false;
	}

	return true;
}

bool IsLivesTokenSpreadValid(GAMEDATA data)
{
	const int charmanderQty = 3;
	const int charmeleonQty = 2;

	int lvl2LivesCap = initLifeTokens;
	int lvl1LivesCap = initLifeTokens;

	for (int i = 0; i < pokQty; i++)
	{
		if (data.pkmn[data.player][i].lvl >= 3)
		{
			lvl2LivesCap = data.pkmn[data.player][i].lives;
		}
		else if (data.pkmn[data.player][i].lvl == 2 && data.pkmn[data.player][i].lives < lvl1LivesCap)
		{
			lvl1LivesCap = data.pkmn[data.player][i].lives;
		}
	}

	for (int i = 0; i < pokQty; i++)
	{
		if (data.pkmn[data.player][i].lives <= 0)
			return false;

		if (data.pkmn[data.player][i].lvl <= 1 && data.pkmn[data.player][i].lives > lvl1LivesCap)
			return false;
		else if (data.pkmn[data.player][i].lvl <= 2 && data.pkmn[data.player][i].lives > lvl2LivesCap)
			return false;
	}

	return true;
}

int CalculateHPTokens(int tokens, POKEMON pkmn[sides][pokQty], int player)
{
	tokens = initHPTokens;
	for (int pokN = 0; pokN < pokQty; pokN++)
	{
		tokens -= pkmn[player][pokN].hp;
	}
	return tokens;
}

int CalculateLifeTokens(int tokens, POKEMON pkmn[sides][pokQty], int player)
{
	tokens = initLifeTokens;
	for (int pokN = 0; pokN < pokQty; pokN++)
	{
		tokens -= pkmn[player][pokN].lives;
	}
	return tokens;
}

int CalculateAttackTokens(int tokens, POKEMON pkmn[sides][pokQty], int player, int attkN)
{
	tokens = initAttkTokens[attkN];
	for (int pokN = 0; pokN < pokQty; pokN++)
	{
		tokens -= pkmn[player][pokN].att[attkN].damage;
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
		<< "                                                                                " << std::endl
		<< "                                                                                " << std::endl
		<< "                                                                                " << std::endl
		<< "                                                                                " << std::endl
		<< "                                                                                " << std::endl
		<< "                                                                                " << std::endl
		<< "                                                                                " << std::endl
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