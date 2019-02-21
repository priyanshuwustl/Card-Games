#pragma once
#include "stdafx.h"
#include "TexasHoldEm.h"
#include "hand.h"
#include "parser.h"
#include "PokerHandEvaluator.h"

using namespace std;

const int numSuits = 4;
const int numRanks = 13;
const int numCardsPerPlayer = 2;

const int notEnoughCardsError = -5;

TexasHoldEm::TexasHoldEm()
{
	mainDeck.createFullDeck();
}

int TexasHoldEm::before_round()
{
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

	//first betting phase
	int currentPlayer = (dealer + 1) % numPlayers;
	betting_phase(currentPlayer, gotAWinner);

	return success;
}

int TexasHoldEm::round()
{
	int numPlayers = Game::playerPtr.size();
	int currentPlayer = (dealer + 1) % numPlayers;
	int i = 0;
	//there are 5 streets
	for (int street = 0; street < 3; ++street)
	{

		if (street == 0) 
		{
			//deal three card to the communal cards
			for (int j = 0; j < 3; ++j) 
			{
				Card& c = mainDeck.deal_card();
				c.setIsFacingDown(false);
				communalCards.push_back(c);
			}
		}
		else 
		{
			//deal one card to the communal cards
			Card& c = mainDeck.deal_card();
			c.setIsFacingDown(false);
			communalCards.push_back(c);
		}

		cout << "\nCommunal Cards" << endl; 
		//print communal cards
		for (int comCard = 0; comCard < communalCards.size(); ++comCard) 
		{
			cout << communalCards[comCard].get_card_def() << " ";
		}
		cout << endl;

		//start betting phase
		betting_phase(currentPlayer, gotAWinner);

		cout << "" << endl;
	}
	return success;
}

int TexasHoldEm::after_round()
{
	//final round
	if (!gotAWinner)
	{
		showdown();
	}

	announceWinner();
	communalCards.clear();
	resetGame();

	return success;
}

void TexasHoldEm::showdown()
{
	//show all remaining players hands
	vector<shared_ptr<Player>>::iterator player;
	for (player = playerPtr.begin(); player != playerPtr.end(); ++player)
	{
		if (!((*player)->folded))
		{
			Hand playerHand = (*player)->hand;

			for (vector<Card>::iterator i = communalCards.begin(); i < communalCards.end(); i++) {
				playerHand.theHand.push_back(*i);
			}

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
			cout << (*player)->name << " 's best hand: " << (*player)->hand << endl;
		}
	}
}

int TexasHoldEm::before_turn(Player &player)
{
	return success;
}

int TexasHoldEm::turn(Player &player)
{
	return success;
}

int TexasHoldEm::after_turn(Player &player)
{
	return success;
}


