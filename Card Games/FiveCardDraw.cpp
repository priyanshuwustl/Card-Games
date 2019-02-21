#include "stdafx.h"
#include "FiveCardDraw.h"
#include "Player.h"
#include "Game.h"
#include "Deck.h"
#include "hand.h"
#include "Card.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "PokerHandEvaluator.h"
#include "Parser.h"

using namespace std;

const int numSuits = 4;
const int numRanks = 13;
const int numCardsPerPlayer = 5;

const int notEnoughCardsError = -5;

string RankStrings1[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
char SuitStrings1[] = { 'C', 'D', 'H', 'S' };

FiveCardDraw::FiveCardDraw()
{
	// going through all the 52 iterations
	for (int i = 0; i<numSuits; i++) {
		for (int j = 0; j<numRanks; j++) {
			Card newCard;
			char suitChar = SuitStrings1[i];
			string rankChar = RankStrings1[j];
			newCard.suit = newCard.create_suit(suitChar);
			newCard.rank = newCard.create_rank(rankChar);
			mainDeck.add_card(newCard);
		}
	}
};

int FiveCardDraw::before_turn(Player &player)
{
	cout << player.name << "'s turn" << endl;

	// display all the cards of the player
	for (Card card : player.hand.theHand) {
		cout << card.get_card_def() << " ";
	}
	cout << endl;

	int isValid = 0;
	int option = 0;
	int playerCards = player.hand.size();

	do
	{
		cout << "How many cards do you need to discard? (Select an option)" << endl;
		cout << "1. None" << endl;
		cout << "2. All cards" << endl;
		cout << "3. Discard custom number of cards" << endl;
		cout << "> ";
		cin >> option;
		cout << endl;

		if (option == 1) {
			isValid = 1;
		}
		else if (option == 2) {
			for (int i = 0; i < player.hand.size(); ++i) {
				discardDeck.add_card(player.hand[i]);
			}
			player.hand.theHand.clear();
			isValid = 1;
		}
		else if (option == 3) {
			int numDiscardCards = 0;
			cout << "Enter number of cards to discard > ";
			cin >> numDiscardCards;
			cout << endl;

			// check code inside these curly braces
			if (numDiscardCards <= player.hand.size()) {
				isValid = 1;
				vector<int> posDiscardCards;
				int i = 0;
				int pos = 0;

				//get indicies of cards in hand to remove
				while (i != numDiscardCards) {
					cout << "Enter card position to be deleted > ";
					cin >> pos;

					if (pos <= playerCards) {
						posDiscardCards.push_back(pos);
						i++;
					}
					else {
						cout << "Wrong card position! Try again.\n" << endl;
					}
				}
				cout << endl;

				for (int j = 0; j < numDiscardCards; j++) {
					int index = posDiscardCards[j];

					for (int k = j + 1; k < numDiscardCards; k++) {
						if (posDiscardCards[k] > index) {
							posDiscardCards[k] -= 1;
						}
					}

					player.hand.theHand.erase(player.hand.theHand.begin() + index - 1);
				}
			}
			else {
				isValid = 0;
				cout << "You don't have that many cards! Try again.\n" << endl;
			}
		}
		else {
			isValid = 0;
			cout << "Wrong choice. Try again.\n" << endl;
		}

	} while (isValid == 0);

	return success;
}

int FiveCardDraw::turn(Player &player)
{
	while (player.hand.theHand.size() != 5) { // dealing until player has 5 cards
		if (mainDeck.size() == 0) { // checking if main deck is empty
			if (discardDeck.size() == 0) { // checking if discard deck is empty
				return notEnoughCardsError; //both main and discard deck are empty
			}
			else { // taking from discardDeck
				discardDeck.shuffle_deck();
				player.hand << discardDeck;
			}
		}
		else { // taking from mainDeck
			player.hand << mainDeck;
		}
	}

	return success;
}

int FiveCardDraw::after_turn(Player &player)
{
	cout << player.name << endl;

	// display all the cards of the player
	for (Card card : player.hand.theHand) {
		cout << card.get_card_def() << " ";
	}
	cout << "\n" << endl;

	return success;
}

int FiveCardDraw::before_round()
{
	mainDeck.shuffle_deck();
	int numPlayers = Game::playerPtr.size();
	int currentPlayer = (dealer + 1) % numPlayers;
	int i = 0;

	while (i != (numCardsPerPlayer*numPlayers)) {
		shared_ptr<Player> p = playerPtr.at(currentPlayer);
		p->hand << mainDeck;
		currentPlayer = (currentPlayer + 1) % numPlayers;
		i++;
	}

	i = 0;

	while (i != numPlayers) {
		shared_ptr<Player> p = playerPtr.at(currentPlayer);
		ante(*p);
		currentPlayer = (currentPlayer + 1) % numPlayers;
		i++;
	}

	callChips = 1;

	// calling first betting phase
	// currentPlayer should be (dealer+1)%numPlayers
	betting_phase(currentPlayer, gotAWinner);

	if (gotAWinner) {
		return success;
	}

	i = 0;

	while (i != numPlayers) {
		shared_ptr<Player> p = playerPtr.at(currentPlayer);
		before_turn(*p);
		currentPlayer = (currentPlayer + 1) % numPlayers;
		i++;
	}

	// calling second betting phase
	// currentPlayer should be (dealer+1)%numPlayers
	betting_phase(currentPlayer, gotAWinner);

	return success;
}

int FiveCardDraw::round()
{
	if (!gotAWinner) {
		int numPlayers = playerPtr.size();
		int currentPlayer = (dealer + 1) % numPlayers;
		int i = 0;

		while (i != numPlayers) {
			shared_ptr<Player> p = playerPtr.at(currentPlayer);
			int check = turn(*p);
			if (check != 0) {
				return check;
			}
			after_turn(*p);
			currentPlayer = (currentPlayer + 1) % numPlayers;
			i++;
		}
	}

	return success;
}

int FiveCardDraw::after_round()
{
	announceWinner();
	resetGame();
	return success;
}

