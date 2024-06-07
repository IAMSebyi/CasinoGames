/*

Dices is a popular gambling game played with two standard dices(1 - 6 values). Each player gets their turn to roll the dices. 
Whoever rolled the most wins the game. If two or more players have rolled the same exact highest value out of all players,
they will continue rolling until one of them wins. Since everyone plays against each other, all bets are the same amount
and whoever wins the game, takes everything.

*/

#pragma once

#include "PlayersProfile.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <string>

// MACROS (used for the sole purpose of making formatting text in the console output easier)
#define NEWLINE std::cout<<std::endl;
#define TWOENDL std::endl << std::endl

class Dices {
private:
	// VARIABLES AND DATA STRUCTURES
	int numOfPlayers;
	std::vector<std::pair<int, int>> playersScore;
	int bet;
	PlayersProfile* gameProfile = nullptr;

	// METHOD DECLARATION (Names are mostly self-explanatory; check definitions in .cpp file)
	int GenerateRandomNumber();
	std::string GetPlayerName(int index);
	void Roll(int& index);
	void Intro();
	void PlayGame();

	
public:
	Dices(PlayersProfile* profile);
	void Begin();
};