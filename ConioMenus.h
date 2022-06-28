#pragma once

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include "Globals.h"

enum WALLS
{
	upLeftC,							// ╔
	hor,								// ═
	upT,								// ╦
	upRightC,							// ╗
	ver,								// ║
	leftT,								// ╠
	cross,								// ╬
	rightT,								// ╣
	lowLeftC,							// ╚
	lowT,								// ╩
	lowRightC,							// ╝
};

void MenuNavigator(const int opsQty, char[10][10], char input[], int& options);
void PrintOptions(char displayText[10][10], int ops, int cursor);
int GetMenuInput(int& cursor, char input[], int opsQty);
void PrintTitle(std::string title);