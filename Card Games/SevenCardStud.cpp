#include "stdafx.h"
#include "SevenCardStud.h"
#include "hand.h"
#include "parser.h"
#include "PokerHandEvaluator.h"
#include <iostream>
#include <algorithm>

using namespace std;

const int numCardsPerPlayer = 7;


//bool gotAWinner = false;

SevenCardStud::SevenCardStud()
{
	mainDeck.createFullDeck();
}

int SevenCardStud::before_round() 
{
	cout << int(dealer) << endl;

	// have players ante
	for (int i = 0; i < playerPtr.size(); ++i) 
	{
		shared_ptr<Player> p = playerPtr.at(i);
		ante(*p);
	}

	callChips = 1;

	//deal two cards to each player (face down)
	mainDeck.shuffle_deck();
	int numPlayers = playerPtr.size();
	vector<shared_ptr<Player>>::iterator player;
	for (player = playerPtr.begin(); player != playerPtr.end(); ++player)
	{
		for (int j = 0; j < 2; ++j)
		{
			(*player)->hand << mainDeck;
			(*player)->hand.back().setIsFacingDown(true);
		}
	}
	return success;
}

int SevenCardStud::round() 
{
	int numPlayers = Game::playerPtr.size();
	int currentPlayer = (dealer + 1) % numPlayers;
	int i = 0; 
	//there are 5 streets
	for (int street = 3; street < 8; ++street) 
	{
		//i think i do this just in case every one folded
		if (gotAWinner) 
		{
			return success;
		}

		cout << street << " Street" << endl;
		
		// deal one card to each player face up
		vector<shared_ptr<Player>>::iterator player;
		for (player = playerPtr.begin(); player != playerPtr.end(); ++player)
		{
			if (!((*player)->folded)) 
			{
				(*player)->hand << mainDeck;
				if (street == 7)
				{
					(*player)->hand.back().setIsFacingDown(true);
				}
				else
				{
					(*player)->hand.back().setIsFacingDown(false);
				}
			}
			
		}

		//show all players all visible cards
		for (player = playerPtr.begin(); player != playerPtr.end(); ++player) 
		{
			if (!((*player)->folded)) {}
			cout << (*player)->name << ": " << (*player)->hand.as_anonymous_string() << endl;
		}

		while (i != numPlayers) {
			currentPlayer = (currentPlayer + 1) % numPlayers;
			i++;
		}
		
		//start betting phase
		betting_phase(currentPlayer, gotAWinner);

		cout << "" << endl;
	}
	return success;
}

int SevenCardStud::get_player_with_lowest_card() 
{
	Card lowestCard;
	lowestCard.set_card("AS");
	int indexOfPlayerWithLowestCard = -1;
	bool test = false; 

	//find player with lowest card
	for (int i = 0; i < playerPtr.size(); ++i)
	{
		if (!(playerPtr[i]->folded)) 
		{
			//for each visible card in players hand
			for (int j = 0; j < (playerPtr[i])->hand.size(); ++j)
			{
				if (!((playerPtr[i])->hand[j].is_facing_down))
				{
					test = (playerPtr[i])->hand[j] < lowestCard;
					if (test || ((playerPtr[i])->hand[j] == lowestCard))
					{
						lowestCard = (playerPtr[i])->hand[j];
						indexOfPlayerWithLowestCard = i;
					}
				}
			}
		}
	}
	return indexOfPlayerWithLowestCard;
}

int SevenCardStud::get_player_with_highest_card() 
{
	Card highestCard;
	highestCard.set_card("2C");
	int numPlayers = playerPtr.size();
	int hasHighestCard =-1;

	for (int i = 0; i < numPlayers; ++i) {
		if (!(playerPtr[i]->folded)) 
		{
			for (int j = 0; j < playerPtr[i]->hand.size(); ++j) {
				if (!(playerPtr[i]->hand[j].is_facing_down))
				{
					if (highestCard < playerPtr[i]->hand[j] || highestCard == playerPtr[i]->hand[j]) {
						highestCard = playerPtr[i]->hand[j];
						hasHighestCard = i;
					}
				}
			}
		}
	}
	return hasHighestCard;
}

int SevenCardStud::after_round()
{
	//final round
	if (!gotAWinner) 
	{
		showdown();
	}
	
	announceWinner();

	resetGame();

	return success;
}

void SevenCardStud::showdown() 
{
	//show all remaining players hands
	vector<shared_ptr<Player>>::iterator player;
	for (player = playerPtr.begin(); player != playerPtr.end(); ++player)
	{
		if (!((*player)->folded)) 
		{
			Hand playerHand = (*player)->hand;

			//get best possible hand of five cards
			Hand bestHand = getBestHandOfFive(playerHand);

			//replace hand with best hand and put remaining cards in discarded deck
			for (int i = 0; i < playerHand.size(); ++i)
			{
				//if this card is not in the best possible hand
				if (!bestHand.contains(playerHand[i]))
				{
					//put it into discarded deck
					discardDeck.add_card(playerHand[i]);
				}
			}

			//replace their hand
			(*player)->hand = bestHand;
		}
	}

	//print all best hands of each player
	for (player = playerPtr.begin(); player != playerPtr.end(); ++player) 
	{
		if (!((*player)->folded)) 
		{
			cout << (*player)->name << "'s best hand: " << (*player)->hand << endl;
		}	
	}
}

Hand SevenCardStud::getBestHandOfFive(Hand& h) 
{
	vector<Hand> allPossibleHands = h.combination(5);
	sort(allPossibleHands.begin(), allPossibleHands.end(), poker_rank);
	return allPossibleHands[0];
}

int SevenCardStud::before_turn(Player &player) 
{
	return success;
}

int SevenCardStud::turn(Player &player) 
{
	return success;
}

int SevenCardStud::after_turn(Player &player) 
{
	return success;
}

