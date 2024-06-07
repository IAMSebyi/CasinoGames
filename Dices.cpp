#include "Dices.h"

int Dices::GenerateRandomNumber()
{
	return 1 + (rand() % 6);
}

std::string Dices::GetPlayerName(int index)
{
	return gameProfile->players[playersScore[index].first].first;
}

void Dices::Roll(int& index)
{
	playersScore[index].second = 0;
	int dice1 = GenerateRandomNumber(), dice2 = GenerateRandomNumber();
	playersScore[index].second = dice1 + dice2;
	std::cout << GetPlayerName(index) << " has rolled " << dice1 << " & " << dice2 << TWOENDL;
}

void Dices::Intro(){
	// This method is used for introduction and also to guide players that are not familiar with the Dices game
	// If you yourself are not yet familiar with the logic behind Dices, I advise reading the upmost comment in Dices.h

	std::cout << "Welcome to The Dices Game!" << TWOENDL;
	char answer;
	std::cout << "Would you like to read the rules of the game? (Please answer using the letters Y/N) ";
	std::cin >> answer;
	NEWLINE
	if (std::toupper(answer) == 'Y') {
		std::cout << "Dices is a popular gambling game played with two standard dices(1 - 6 values). Each player gets their turn to roll the dices."
		<< std::endl << "Whoever rolled the most wins the game. If two or more players have rolled the same exact highest value out of all players,"
		<< std::endl << "they will continue rolling until one of them wins. Since everyone plays against each other, all bets are the same amount"
		<< std::endl << "and whoever wins the game, takes everything." << TWOENDL;
	}

	float minBalance = FLT_MAX;
	for (int index = 0; index < numOfPlayers; index++)
		if (minBalance > gameProfile->players[index].second) minBalance = gameProfile->players[index].second;
	
	int bet;
	std::cout << "Please insert a bet in between the range of [1, " << minBalance << "]. ";
	std::cin >> bet;
	while (bet > minBalance || bet <= 0) {
		NEWLINE
		std::cout << "That's not a valid bet. Try again with a bet smaller or equal to " << minBalance << " or larger than 0. ";
		std::cin >> bet;
	}

	for (int index = 0; index < numOfPlayers; index++) {
		gameProfile->players[index].second -= bet;
	}

	bet *= numOfPlayers;

	std::cout << "Alright then. We shall let the games begin!" << TWOENDL;

	std::cin.ignore();
}

void Dices::PlayGame()
{
	bool foundWinner = false;
	while (!foundWinner) {
		for (int index = 0; index < numOfPlayers; index++) {
			std::cout << GetPlayerName(index) << ", press 'Enter' when you would like to roll." << TWOENDL;
			std::cin.get();
			Roll(index);
		}
		std::sort(playersScore.begin(), playersScore.end(), [](const std::pair<int, int>& playerA, const std::pair<int, int>& playerB)-> bool{
			return playerA.second > playerB.second;
		});
		int maxRollValue = playersScore[0].second;
		int playersTied = 0;
		for (int index = 1; index < numOfPlayers; index++) {
			if (playersScore[index].second == maxRollValue) playersTied++;
			else break;
		}
		if (playersTied == 0) {
			foundWinner = true;
			std::cout << GetPlayerName(0) << " has won! Congratulations!" << TWOENDL;
			gameProfile->players[playersScore[0].first].second += bet;
		}
		else {
			playersTied++;
			for (int i = numOfPlayers; i > playersTied; i--) {
				playersScore.pop_back();
				numOfPlayers--;
			}
			for (int index = 0; index < numOfPlayers - 2; index++) {
				std::cout << GetPlayerName(index) << ", ";
			}
			std::cout << GetPlayerName(numOfPlayers - 2) << " & " << GetPlayerName(numOfPlayers - 1) << " have both rolled the highest (" << maxRollValue << ")."
			<< std::endl << "They will roll again" << TWOENDL;
		}
	}
}

Dices::Dices(PlayersProfile* profile)
{
	gameProfile = profile;
	numOfPlayers = gameProfile->numOfPlayers;
	// Resizes the playersScore and playersName vectors for the number of players and initializes the indexes' values with 0 and name inputs, respectively
	std::pair<int, int> pairInitValues;
	pairInitValues.first = 0;
	pairInitValues.second = 0;
	playersScore = std::vector<std::pair<int, int>>(numOfPlayers, pairInitValues);
	for (int index = 0; index < numOfPlayers; index++) {
		playersScore[index].first = index;
	}
}

void Dices::Begin() {
	srand(std::time(0));
	Intro();
	PlayGame();
}
