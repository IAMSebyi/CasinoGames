#pragma once

#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstring>
#include "sqlite/sqlite3.h"

// MACROS (used for the sole purpose of making formatting text in the console output easier)
#define NEWLINE std::cout<<std::endl;
#define TWOENDL std::endl << std::endl

class PlayersProfile
{
private:
	sqlite3* db;
	bool newProfile = true;
public:
	std::vector<std::pair<std::string, float>> players; // first (string) represent name, second (int) represents balance
	int numOfPlayers = 0;
	int profileId = 0;

	PlayersProfile();
	~PlayersProfile();

	void CreateProfile();
	void SaveProfile();
	void SelectProfile();
};

