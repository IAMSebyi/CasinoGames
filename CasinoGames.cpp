/*

Casino Games Project by Iorga Sebastian George

*/

#include <iostream>
#include "PlayersProfile.h"
#include "Blackjack.h"
#include "Dices.h"
#include "Minesweeper.h"

int main() {

	bool loopGames = true;
	int code = 0;
	char saveAnswer;
	PlayersProfile* profile = new PlayersProfile;

	std::cout << "Welcome to our Casino!" << TWOENDL;

	std::cout << "Would you like to create a profile or select an existing one? (1 for create, 0 for select) ";
	std::cin >> code;
	switch(code){
	case 0:
		profile->SelectProfile();
		break;
	default:
		profile->CreateProfile();
		break;
	}

	NEWLINE
	while (loopGames) {
		NEWLINE
		for (int index = 0; index < profile->numOfPlayers; index++) {
			std::cout << profile->players[index].first << "'s current balance is " << profile->players[index].second << "." << std::endl;
		}
		NEWLINE

		std::cout << "1 = Blackjack" << std::endl << "2 = Dices" << std::endl << "3 = Minesweeper" << std::endl << "0 = Quit Game" << TWOENDL
		<< "Please insert a code: ";

		std::cin >> code;

		switch (code) {
		case 0:
			loopGames = false;
			break;
		case 1:
		{
			Blackjack blackjack (profile);
			blackjack.Begin();
			break;
		}
		case 2:
		{
			Dices dices (profile);
			dices.Begin();
			break;
		}
		case 3:
		{
			Minesweeper minesweeper(profile);
			minesweeper.Begin();
			break;
		}
		default:
			break;
		}

		std::cout << "Would you like to save your profile? (Y/N) ";
		std::cin >> saveAnswer;

		if (toupper(saveAnswer) == 'Y') profile->SaveProfile();
	}

	profile->~PlayersProfile();

	NEWLINE
	std::cout << "Thanks for playing!" << std::endl;
	return 0;
}