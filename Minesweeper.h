/*

The gambling version of Minesweeper is a thrilling twist on the classic game, designed to combine the excitement of gambling with strategic decision-making. 
In this game, players take on the role of prospectors, strategically placing bombs on a 6x6 grid to unearth multipliers and strike it rich.

Key Features:

1. Customizable Bomb Placement: Players start the game by choosing the number of bombs they want to place on the 6x6 grid. The minimum number of bombs the
player can choose is 4, with the maximum being 35 The more bombs they place, the greater the potential reward.

2. No Revealing Tiles: Unlike traditional Minesweeper, the gambling version does not involve revealing tiles. Players do not uncover numbers 
to indicate the proximity of bombs.

3. 

*/

#pragma once

#include "PlayersProfile.h"

#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <math.h>

// MACROS (used for the sole purpose of making formatting text in the console output easier)
#define NEWLINE std::cout<<std::endl;
#define TWOENDL std::endl << std::endl

class Minesweeper
{
private:
	// VARIABLES AND DATA STRUCTURES
	PlayersProfile* gameProfile = nullptr;
	std::vector<std::vector<int>> gameGrid;
	std::vector<std::vector<char>> outputGrid; // the grid visible to the player (with Xs marking unknown positions in the grid)

	std::vector<int> bombs;
	std::vector<float> rates;
	std::vector<float> bets;

	// METHOD DECLARATION (Names are mostly self-explanatory; check definitions in .cpp file)
	void GenerateRandomGrid(int bombs);
	void ShowGrid(std::vector<std::vector<char>> grid);
	void ShowGrid(std::vector<std::vector<int>> grid);
	void Intro();
	void SetGameSettings();
	void PlayGame();
public:
	Minesweeper(PlayersProfile* profile);
	void Begin();
};

