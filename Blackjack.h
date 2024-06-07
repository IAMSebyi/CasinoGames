/*

Blackjack is a popular card game played in casinos around the world. The game's objective is to beat the dealer by having a hand value as close
to 21 as possible without exceeding it. Here are the basic rules of blackjack:

1. Card Values:

- Number cards (2-10) are worth their face value.
- Face cards (King, Queen, Jack) are worth 10 points each.
- Aces can be worth either 11 points if the player's hand value doesn't exceed 10 or 1 point if it does


2. Objective:

- The goal is to have a hand value that is closer to 21 than the dealer's hand, without going over 21. Going over 21 results in a "bust," and the
player loses.


3. Dealing Cards:

- Each player, including the dealer, is dealt two cards.
- Players' cards are usually dealt face up, while one of the dealer's cards is face up (the "upcard"), and the other is face down (the "hole card").

4. Player's Turn:

- Players can choose to "hit" (take another card) or "stand" (keep their current hand).
- Players can continue to hit as many times as they wish, but they must be cautious not to exceed 21 points.

5. Dealer's Turn:

- After all players have completed their turn, the dealer reveals their hole card.y
- The dealer must hit until their hand is worth at least 17 points, then they must stand.

6. Winning and Payouts:

- If a player's hand value exceeds 21, they lose (bust).
- If the dealer busts, all remaining players win.
- If a player's hand is closer to 21 than the dealer's without busting, they win.
- If a player is dealt a natural blackjack (an Ace and a 10-value card) on their initial two cards, they win.
- If a player wins, they get double their bet, otherwise they either lose their bet, if they lose, or gain nothing if they draw against the dealer

*/
#pragma once

#include "PlayersProfile.h"
#include <iostream>
#include <map>
#include <random>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>

// MACROS (used for the sole purpose of making formatting text in the console output easier)
#define NEWLINE std::cout<<std::endl;
#define TWOENDL std::endl << std::endl

class Blackjack {
private:
    // VARIABLES AND DATA STRUCTURES
    // Map holds collection of key/value pairs, where the key represents the card itself (1 = Ace, 11 = Jack, 12 = Queen,
    // 13 = King, 2-10 = number cards) and the value represents the amount of cards in deck (4 per deck)
    std::map<int, int> cardDeck;

    // Variables used for game settings (number of decks used and the number of players playing at the table)
    int numOfDecks = 0;

    // Variables used for dealer's logic (the score and the 'hole card' - the face down card)
    int dealerScore = 0;
    int holeCard = 0;

    // Vectors hold info regarding a player's score and their name - the i-th index of both arrays hold the score value and name for the i+1-th player
    PlayersProfile* gameProfile = nullptr;
    std::vector<int> playersScore;
    std::vector<float> bets;
    std::vector<bool> results;

    // METHOD DECLARATION (Names are mostly self-explanatory; check definitions in .cpp file)
    void GenerateRandomNumber(int& cardDrawn);
    int PlayerDrawCard(int& id);
    int DealerDrawCard();
    std::string GetCardName(int& card);
    bool CheckBlackjack(int& id);
    void Intro();
    void SetGameSettings();
    void FirstDraw();
    void PlayersTurn();
    void DealersTurn();
    void Endgame();
    void PlayGame();

public:
    Blackjack(PlayersProfile* profile);
    void Begin();
};