#pragma once

#include <Windows.h>
#include <iostream>
#include <conio.h>

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

enum COLORS
{
	BlueOnBlack = 9,
	GreenOnBlack = 2,
	RedOnBlack = 4,
	PurpleOnBlack = 5,
	YellowOnBlack = 6,
	GrayOnBlack = 8,
	CyanOnBlack = 11,
	OrangeOnBlack = 12,
	WhiteOnBlack = 15,
	BlackOnBlue = 16,
	BlackOnGreen = 32,
	BlackOnRed = 64,
	BlackOnPurple = 80,
	WhiteOnPurple = 87,
	BlackOnYellow = 96,
	BlackOnGray = 128,
	BlackOnWhite = 240,
	BlueOnWhite = 241,
	GreenOnWhite = 242,
	CyanOnWhite = 243,
	PurpleOnWhite = 245,
	YellowOnWhite = 246,
	OrangeOnWhite = 252,
};

enum class KEYS
{
	Up,
	Left,
	Down,
	Right,
	Enter,
	Back,
};

void MenuNavigator(const int opsQty, char[10][10], char input[], int& options);
void PrintOptions(char displayText[10][10], int ops, int cursor);
int GetMenuInput(int& cursor, char input[], int opsQty);
void PrintTitle(std::string title);