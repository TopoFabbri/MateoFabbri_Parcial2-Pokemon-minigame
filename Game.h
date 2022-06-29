#pragma once

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include "Globals.h"
#include "ConioMenus.h"

const int sides = 2;									  // Amount of sides on map
const int pokQty = 6;									  // Amount of pkmn per side
const int attkQty = 12;									  // Amount of attacks that exist
const int pkmnAttkQty = 3;								  // 
const int inputKeyQty = 20;								  // 
const int initHPTokens = 5000;							  // 
const int initLifeTokens = 14;							  // 
const int initAttkTokens[3]{ 400, 600, 900 };			  // 

enum class STAGES
{
	Reset,
	Selection,
	SetPokemons,
	Fight,
	End,
};

enum class INTER
{
	Quit,
	Select,
	None,
};

struct ATTK
{
	std::string name = "'None'";
	int damage = 0;
};

struct POKEMON
{
	std::string name = "'None'";
	int lvl{};
	int lives{};
	int hp{};
	int initHp{};
	ATTK att[3]{};

	void Destroy()
	{
		name = "Fainted";
		lvl = 0;
		lives = 0;
		hp = 0;
		initHp = 0;

		for (int i = 0; i < pkmnAttkQty; i++)
		{
			att[i].name = "'None'";
			att[i].damage = 0;
		}
	}

	void DamagePokemon(int value)
	{
		hp -= value;
	}
};

struct GAMEDATA
{
	COORD maxDim{};
	COORD cursor{};
	COORD pmptPos{};
	COORD txtPos{};
	POKEMON pkmn[sides][pokQty]{};
	ATTK defAttk[attkQty]{};
	bool multiplayer = true;
	char input[inputKeyQty]{};
	int player = 1;
};

POKEMON CreatePokemon(std::string name, int lvl, int lives, int hp, ATTK att[3]);
INTER GameInput(GAMEDATA& data);
INTER MapNavigation(GAMEDATA& data);
ATTK ChooseAttack(GAMEDATA data, int& attkId);
bool Fight(GAMEDATA& data);
bool PrepPokemon(GAMEDATA& data);
bool CheckPokemonLeft(GAMEDATA& data, int& winner);
bool SelectPokemons(GAMEDATA& data);
bool IsHPTokenSpreadValid(GAMEDATA data);
bool IsLivesTokenSpreadValid(GAMEDATA data);
bool IsAttackTokenSpreadValid(GAMEDATA data, int attkN);
int CalculateHPTokens(int tokens, POKEMON pkmn[sides][pokQty], int player);
int CalculateLifeTokens(int tokens, POKEMON pkmn[sides][pokQty], int player);
int CalculateAttackTokens(int tokens, POKEMON pkmn[sides][pokQty], int player, int attkN);
void PrintPokemonStats(GAMEDATA data);
void Defense(POKEMON& defender, ATTK attk, int attkId);
void ChooseAttackedPkmn(GAMEDATA& data, INTER& actions);
void ChooseAttackerPkmn(GAMEDATA& data, INTER& actions);
void ArraySet(char array[], char value[]);
void RunGame();
void ResetMap(GAMEDATA& data);
void LevelToDrawing(int lvl, COORD newPos);
void CleanAfterBoard(COORD pmptPos);