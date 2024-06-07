#include "Minesweeper.h"

void Minesweeper::GenerateRandomGrid(int bombs)
{
	bool validBomb = false;
	int x, y;

	// If the number of bombs chosen by the player is smaller than half of elements in the grid (6x6 = 36 elements),
	// fill the array with empty spaces (0) and randomize x and y in order to place bombs in the (x, y) spots.
	// Otherwise, fill the array with bombs (1) and randomize x and y in order to place empty spaces in the (x, y) spots.
	// This is better than simply randomly placing bombs, as for example, if bombs = 35, we would need to iterate again
	// and again until we find 35 different places in the array to place bombs in, instead of simply placing one empty space
	// in a grid full of bombs.

	if (bombs <= 18) {
		gameGrid = std::vector<std::vector<int>>(6, std::vector<int>(6, 0));
		for (int i = 0; i < 36 - bombs; i++) {
			validBomb = false;
			while (!validBomb) {
				x = rand() % 6;
				y = rand() % 6;
				// if there's no bomb in (x, y) position in the grid
				if (gameGrid[x][y] == 0) {
					validBomb = true;
					gameGrid[x][y] = 1;
				}
			}
		}
	}
	else {
		gameGrid = std::vector<std::vector<int>>(6, std::vector<int>(6, 1));
		for (int i = 0; i < bombs; i++) {
			validBomb = false;
			while (!validBomb) {
				x = rand() % 6;
				y = rand() % 6;
				// if there's a bomb in (x, y) position in the grid
				if (gameGrid[x][y] == 1) {
					validBomb = true;
					gameGrid[x][y] = 0;
				}
			}
		}
	}
}

void Minesweeper::ShowGrid(std::vector<std::vector<char>> grid)
{
	std::cout << "  1 2 3 4 5 6" << std::endl;
	for (int i = 0; i < 6; i++) {
		NEWLINE
			std::cout << i + 1 << " ";
			for (int j = 0; j < 6; j++) {
				std::cout << grid[i][j] << " ";
			}
		NEWLINE
	}
}

void Minesweeper::ShowGrid(std::vector<std::vector<int>> grid)
{
	std::cout << "  1 2 3 4 5 6" << std::endl;
	for (int i = 0; i < 6; i++) {
		NEWLINE
			std::cout << i + 1 << " ";
		for (int j = 0; j < 6; j++) {
			std::cout << grid[i][j] << " ";
		}
		NEWLINE
	}
}

void Minesweeper::Intro()
{
	// This method is used for introduction and also to guide players that are not familiar with the gambling version of Minesweeper
	// If you yourself are not yet familiar with the logic behind Minesweeper, I advise reading the upmost comment in Minesweeper.h

	std::cout << "Welcome to Minesweeper!" << TWOENDL;
	char answer;
	std::cout << "Would you like to read the rules of the game? (Please answer using the letters Y/N) ";
	std::cin >> answer;
	NEWLINE
	if (std::toupper(answer) == 'Y') {
		std::cout << "The gambling version of Minesweeper is a thrilling twist on the classic game, designed to combine the excitement of gambling with" << std::endl
		<< "strategic decision - making. In this game, players take on the role of prospectors, strategically placing bombs on a 6x6 grid to unearth" << std::endl 
		<< "multipliers and strike it rich." << TWOENDL << "Key features:" << TWOENDL 
		<< "1. Customizable Bomb Placement: Players start the game by choosing the number of bombs they want to place on the 6x6 grid." << std::endl
		<< "The minimum number of bombs the player can choose is 4, with the maximum being 35 The more bombs they place, the greater the potential reward." << TWOENDL
		<< "2. No Revealing Tiles: Unlike traditional Minesweeper, the gambling version does not involve revealing tiles. Players do not uncover numbers" << std::endl 
		<< "to indicate the proximity of bombs." << TWOENDL;
	}
}

void Minesweeper::SetGameSettings()
{
	for (int index = 0; index < gameProfile->numOfPlayers; index++) {
		NEWLINE
		float bet;
		std::cout << gameProfile->players[index].first << ", your current balance is " << gameProfile->players[index].second << ". How much would you like to bet? ";
		std::cin >> bet;
		bet *= 1.;
		while (bet > gameProfile->players[index].second || bet <= 0) {
			std::cout << "That's not a valid bet. Try again with a bet smaller or equal to your balance or larger than 0. ";
			std::cin >> bet;
			bet *= 1.;
		}
		bets[index] = bet;

		int numOfBombs;
		std::cout << "And how many bombs would you like on your grid? (minimum 4, maximum 35) ";
		std::cin >> numOfBombs;
		while (numOfBombs < 4 || numOfBombs > 35) {
			std::cout << "That's not a valid number of bombs. Try again with a number larger than 4 and smaller than 35. ";
			std::cin >> numOfBombs;
		}
		bombs[index] = numOfBombs;

		rates[index] = (int)(pow(100, 1. / (36.0 - bombs[index])) * 100) / 100.0;

		std::cout << "Your multiplier rate is " << rates[index] << "." << TWOENDL;
	}

	std::cout << "Alright then. We shall let the games begin!" << TWOENDL;
}

void Minesweeper::PlayGame()
{
	for (int index = 0; index < gameProfile->numOfPlayers; index++) {
		outputGrid = std::vector<std::vector<char>> (6, std::vector<char>(6, 'X'));
		GenerateRandomGrid(bombs[index]);
		bool exploded = false;
		float multiplier = 1;
		int tilesRevealed = 0;

		std::cout << "It is " << gameProfile->players[index].first << "'s turn." << TWOENDL;

		while (!exploded) {
			NEWLINE
			if (tilesRevealed == 36 - bombs[index]) {
				std::cout << "JACKPOT! You have picked all the empty spaces. You have won " << multiplier * bets[index] << "!" << TWOENDL;
				ShowGrid(gameGrid);
				break;
			}
			std::cout << "Current grid" << TWOENDL;
			ShowGrid(outputGrid);
			NEWLINE
			std::cout << gameProfile->players[index].first << ", would you like to pick a tile (1) or to cash out (0)? (current multiplier: " << multiplier << ") ";
			int code;
			std::cin >> code;
			if (code == 0) {
				NEWLINE
				std::cout << "Congratulations! You have won " << multiplier * bets[index] << "!";
				NEWLINE
				std::cout << "Revealed grid" << TWOENDL;
				ShowGrid(gameGrid);
				NEWLINE
				gameProfile->players[index].second += bets[index] * (multiplier - 1);
				break;
			}
			else {
				int x, y;
				bool validPosition = false;
				
				while (!validPosition) {
					std::cout << "Insert a valid grid position (x, y = [1, 6], grid(x, y) not revealed) ";
					std::cin >> x >> y;
					if (x >= 1 && x <= 6 && y >= 1 && y <= 6) {
						if (outputGrid[x-1][y-1] == 'X') {
							validPosition = true;
						}
						else {
							std::cout << "Position not valid! The tile picked has already been revealed. Please pick an unrevealed tile marked by X in the grid." << std::endl;
						}
					}
					else {
						std::cout << "Position not valid! Either x, y or both are smaller than 1 or larger than 6. Please pick a valid pair of numbers (x, y in range [1, 6])." << std::endl;
					}
				}

				x--; y--;

				NEWLINE
				if (gameGrid[x][y] == 0) {
					std::cout << "No bomb there. You're safe!" << TWOENDL;
					multiplier *= rates[index];
					outputGrid[x][y] = '0';
				}
				else {
					std::cout << "BOOM! You lost!" << TWOENDL;
					ShowGrid(gameGrid);
					gameProfile->players[index].second -= bets[index];
					exploded = true;
				}
			}
		}

		if (exploded) {

		}
	}
}

Minesweeper::Minesweeper(PlayersProfile* profile)
{
	gameProfile = profile;
	rates = std::vector<float>(gameProfile->numOfPlayers, 0);
	bets = std::vector<float>(gameProfile->numOfPlayers, 0);
	bombs = std::vector<int>(gameProfile->numOfPlayers, 0);
}

void Minesweeper::Begin()
{
	srand(std::time(0));
	Intro();
	SetGameSettings();
	PlayGame();
}
