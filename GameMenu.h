#pragma once

#include "ConioMenus.h"

enum class SCENES
{
	Quit,
	Play,
	Rules,
	Author,
	Menu
};

void MainMenu(SCENES& Nav);