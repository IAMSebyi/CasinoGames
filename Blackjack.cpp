#include "Blackjack.h"

void Blackjack::GenerateRandomNumber(int& cardDrawn) {
    // The rand() function generates random integer, the 1 + random number modulo 13 formula is used for range purposes (1-13)
    cardDrawn = 1 + (rand() % 13);
}

int Blackjack::PlayerDrawCard(int& id) {
    int card;
    GenerateRandomNumber(card);
    // Generate random number (cards) until valid, where valid means that the type of card we generated is still in the deck
    // e.g. if we are playing a game with a single card deck, and there are 4 players that have drawn a Jack each, there are
    // no more Jacks in said deck, therefore a 5th player cannot draw another Jack
    while (cardDeck[card] == 0) {
        Blackjack::GenerateRandomNumber(card);
    }

    // Reduce the number of cards of the type generated in the cardDeck map
    cardDeck[card]--;

    // Ace card logic (add either 1 or 11, depending on which benefits the player)
    if (card == 1) {
        if (playersScore[id] <= 10) playersScore[id] += 11;
        else playersScore[id]++;
    }

    // In case of Jack, Queen and King (which have the card values 11, 12 and 13 respectively), the number added to the player's
    // score is 10; the number cards are smaller or equal to 10
    else playersScore[id] += std::min(10, card);

    return card;
}

int Blackjack::DealerDrawCard() {
    // Same logic used for PlayerDrawCard method, only change is that it's the dealerScore value that updates (check that method for more info)
    int card;
    GenerateRandomNumber(card);
    while (cardDeck[card] == 0) {
        GenerateRandomNumber(card);
    }
    cardDeck[card]--;

    if (card == 1) {
        if (dealerScore <= 10) dealerScore += 11;
        else dealerScore++;
    }
    else dealerScore += std::min(10, card);
    return card;
}

std::string Blackjack::GetCardName(int& card) {
    // This method is used to help output the card's real name using the card 'ID' integer value
    std::string nameResult;
    switch (card) {
    case 1:
        nameResult = "Ace";
        break;
    case 11:
        nameResult = "Jack";
        break;
    case 12:
        nameResult = "Queen";
        break;
    case 13:
        nameResult = "King";
        break;
    default:
        // The default case is for number cards, where their ID correlates with their real name, therefore we only have to convert from integer to string.
        nameResult = std::to_string(card);
        break;
    }
    return nameResult;
}

bool Blackjack::CheckBlackjack(int& id) {
    return playersScore[id] == 21;
}

void Blackjack::Intro() {
    // This method is used for introduction and also to guide players that are not familiar with the game of Blackjack
    // If you yourself are not yet familiar with the logic behind Blackjack, I advise reading the upmost comment in Blackjack.h

    std::cout << "Welcome to The Blackjack Game!" << TWOENDL;
    char answer;
    std::cout << "Would you like to read the rules of the game? (Please answer using the letters Y/N) ";
    std::cin >> answer;
    NEWLINE
    if (toupper(answer) == 'Y') {
        std::cout << "Blackjack is a popular card game played in casinos around the world. "
        << "The game's objective is to beat the dealer by having a hand value as close to 21 as possible without exceeding it. "
        << "Here are the basic concepts of blackjack:" << TWOENDL << "1. Card Values:" << std::endl << "- Number cards (2-10) "
        << "are worth their face value." << std::endl << "- Face cards (King, Queen, Jack) are worth 10 points each." << std::endl
        << "- Aces can be worth either 11 points if the player's hand value doesn't exceed 10 or 1 point if it does" << TWOENDL
        << "2. Objective:" << std::endl << "- The goal is to have a hand value that is closer to 21 than the dealer's hand, "
        << "without going over 21. Going over 21 results in a bust, and the player loses." << TWOENDL << "3. Dealing cards:"
        << std::endl << "- Each player, including the dealer, is dealt two cards." << std::endl << "Players' cards are "
        << "usually dealt face up, while one of the dealer's cards is face up (the upcard), and the other is face down "
        << "(the hole card)." << TWOENDL << "4. Player's turn: " << std::endl << "- Players can choose to hit (take another card) "
        << "or stand (keep their current hand)." << std::endl << "- Players can continue to hit as many times as they wish, but "
        << "they must be cautious not to exceed 21 points." << TWOENDL << "5. Dealer's turn: " << std::endl << "- After all "
        << "players have completed their turn, the dealer reveals their hole card." << std::endl << "- The dealer must hit until "
        << "their hand is worth at least 17 points, then they must stand." << TWOENDL << "6. Winning:" << std::endl
        << "- If a player's hand value exceeds 21, they lose (bust)." << std::endl << "- If the dealer busts, all remaining players win."
        << std::endl << "- If a player's hand is closer to 21 than the dealer's without busting, they win." << std::endl
        << "- If a player is dealt a natural blackjack (an Ace and a 10-value card) on their initial two cards, they win." << std::endl
        << "- If a player wins, they get double their bet, otherwise they either lose their bet, if they lose, or gain nothing if they draw against the dealer" << TWOENDL;
        std::cout << "Before we start things off, let's choose the settings." << TWOENDL;
    }
}

void Blackjack::SetGameSettings() {
    // TODO add restriction on number of players based on number of decks (based on the number of cards in the deck, practically speaking)

    std::cout << "How many decks would you like to play the game with? ";
    std::cin >> numOfDecks;

    // Initializes the card deck, inserting for each card ID as key, the value 4 * number of decks used, as in a single deck of cards 
    // (without jokers) there are 4 cards (clubs, diamonds, hearts and spades) of each type of card 
    for (int cardNum = 1; cardNum <= 13; cardNum++) {
        cardDeck.insert(std::pair<int, int>(cardNum, 4 * numOfDecks));
    }

    for (int index = 0; index < gameProfile->numOfPlayers; index++) {
        NEWLINE
        int bet;
        std::cout << gameProfile->players[index].first << ", your current balance is " << gameProfile->players[index].second << ". How much would you like to bet? ";
        std::cin >> bet;
        while (bet > gameProfile->players[index].second || bet <= 0) {
            std::cout << "That's not a valid bet. Try again with a bet smaller or equal to your balance or larger than 0. ";
            std::cin >> bet;
        }
        bets[index] = bet;
    }
    std::cout << "Alright then. We shall let the games begin!" << TWOENDL;
}

void Blackjack::FirstDraw() {
    // This method is used for the first draw of 2 cards for each player and dealer at the beginning of the game

    // Players draw 2 cards first
    for (int index = 0; index < gameProfile->numOfPlayers; index++) {
        int card1 = PlayerDrawCard(index);
        int card2 = PlayerDrawCard(index);
        std::cout << gameProfile->players[index].first << " has drawn a " << GetCardName(card1) << " and a " << GetCardName(card2) << ". His current score is " << playersScore[index] << ".";
        NEWLINE
    }

    // We are drawing 2 cards for the dealer, but only outputting one of them, as the other, called 'hole card' (or the face down card), is supposed to remain unknown until the
    // dealer's turn later in the game; we're holding said value in the holeCard variable, as we want to output the card itself eventually rather than simply revealing the dealerScore
    int card = DealerDrawCard();
    holeCard = DealerDrawCard();
    std::cout << "The dealer has drawn a " << GetCardName(card) << " and an unknown card.";
    std::cout << TWOENDL;
}

void Blackjack::PlayersTurn() {
    for (int index = 0; index < gameProfile->numOfPlayers; index++) {
        std::cout << gameProfile->players[index].first << ", your score is " << playersScore[index] << ". ";
        // Checks for natural blackjack, if not, then begin taking inputs for hitting and standing
        if (CheckBlackjack(index)) {
            std::cout << "YOU WON!";
            NEWLINE
                continue;
        }
        else {
            char decision;
            bool isStanding = false;
            while (playersScore[index] < 21 && !isStanding) {
                std::cout << "Would you like to hit or stand? (Please answer using the letters H/S) ";
                std::cin >> decision;
                NEWLINE;
                // Standing means the player stops at their current score (lower than 21) and does not draw any more cards
                if (std::toupper(decision) == 'S') isStanding = true;
                // Hitting means the player draws another card
                // The loop goes on as long as the player continues to hit and his score does not exceed 20
                else if (std::toupper(decision) == 'H') {
                    int card = PlayerDrawCard(index);
                    std::cout << "You have drawn a " << GetCardName(card) << ". Your score is " << playersScore[index] << ". ";
                    // Checks for blackjack after every hit, if blackjack is reached, inform the player that they have won
                    if (CheckBlackjack(index)) {
                        std::cout << "YOU WON!";
                        break;
                    }
                }
                else continue;
            }
            // If the score is bigger than 21, inform the player that they busted and lost; if not, then continue with next player's turn or return 
            if (playersScore[index] > 21) {
                std::cout << "YOU BUSTED!";
            }
            if (!isStanding) std::cout << TWOENDL;
        }
    }
}

void Blackjack::DealersTurn() {
    std::cout << "Dealer reveals his hole card. It's a " << GetCardName(holeCard) << ". His score is " << dealerScore << ".";
    std::cout << TWOENDL;

    // Dealer has to draw until he reached or went beyond 17
    while (dealerScore < 17) {
        int card = DealerDrawCard();
        std::cout << "Dealer has drawn a " << GetCardName(card) << ". Their score is " << dealerScore << ".";
        std::cout << TWOENDL;

    }
    std::cout << "Dealer has stopped drawing." << TWOENDL;

}

void Blackjack::Endgame() {
    // This method is used to check winning conditions and to output the list of players and if they won, tied or lost (plus the reasoning)
    // If the dealer busted, the players that have not busted (that 'remain' in the game) won
    if (dealerScore > 21) {
        std::cout << "All remaining players win.";
        for (int index = 0; index < gameProfile->numOfPlayers; index++) {
            if (playersScore[index] <= 21) gameProfile->players[index].second += bets[index];
            else gameProfile->players[index].second -= bets[index];
        }
        NEWLINE
    }
    else {
        for (int index = 0; index < gameProfile->numOfPlayers; index++) {
            std::cout << gameProfile->players[index].first << " ";
            if (playersScore[index] == 21) { std::cout << "has WON! (Blackjack) +" << bets[index]; results[index] = true; }
            else if (playersScore[index] > 21) { std::cout << "has LOST! (Bust) -" << bets[index]; }
            else if (playersScore[index] < dealerScore) { std::cout << "has LOST! (Score smaller than dealer's) -" << bets[index]; }
            // setting bets[index] value to 0 and leaving results[index] as 'false' in the case of a tie means 0 will be subtracted from (index+1)-th player's balance
            // thus, their balance will remain the same 
            else if (playersScore[index] == dealerScore) { std::cout << "has TIED! (Score equal to dealer's) -0"; bets[index] = 0; }
            else if (playersScore[index] > dealerScore) { std::cout << "has WON! (Score bigger than dealer's) +" << bets[index]; results[index] = true; }
            std::cout << TWOENDL;

            if (results[index]) gameProfile->players[index].second += bets[index];
            else gameProfile->players[index].second -= bets[index];
        }
    }
}

void Blackjack::PlayGame() {
    // The entire game logic is encapsulated in here
    FirstDraw();
    PlayersTurn();
    DealersTurn();
    Endgame();
}

Blackjack::Blackjack(PlayersProfile* profile)
{
    gameProfile = profile;
    playersScore = std::vector<int>(gameProfile->numOfPlayers, 0);
    results = std::vector<bool>(gameProfile->numOfPlayers, false);
    bets = std::vector<float>(gameProfile->numOfPlayers, 0);
}

void Blackjack::Begin() {
    srand(std::time(0)); // The srand() function sets the starting point for producing a series of pseudo-random integers.
    Intro();
    SetGameSettings();
    PlayGame();
}