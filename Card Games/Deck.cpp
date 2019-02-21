// Deck.cpp : holds functions that are used to create a deck, output to a stream, deal cards,
// and shuffle the deck
//
// author: Glizela Taino gtaino@wustl.edu
//

#include "stdafx.h"
#include "Deck.h"
#include "Parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <random>

const string Deck::RankStrings1[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
const char Deck::SuitStrings1[4] = { 'C', 'D', 'H', 'S' };

using namespace std;

Deck::Deck() {};

Deck::Deck(char * file_name)
{
	int result = load_file(file_name);
	if (result != 0) 
	{
		throw std::runtime_error("Error: Could not open file.");
	}
}

int Deck::load_file(char * file_name)
{
	// Read in file
	ifstream file(file_name);

	// Returns a value of 2 when file is not found
	if (!file.is_open())
	{
		return file_not_found;
	}

	// Loops through each card def
	string card_def;
	while (file >> card_def)
	{

		// checks if valid def 
		if (is_valid(card_def))
		{
			//create card
			Card card;
			card.set_card(card_def);

			//push to list of cards
			aDeck.push_back(card);
		}
	}

	file.close();
	return success; //Returns 0 with successful execution
}

void Deck::shuffle_deck() 
{
	random_device rd;
	mt19937 g(rd());
	shuffle(aDeck.begin(), aDeck.end(), g);

}

int Deck::size() const 
{
	return aDeck.size();
}

Card Deck::deal_card()
{
	Card c = aDeck.back();
	aDeck.pop_back();
	return c;
}

void Deck::add_card(Card card) 
{
	aDeck.push_back(card);
}

void Deck::createFullDeck()
{
	// going through all the 52 iterations
	for (int i = 0; i<numSuits; i++) {
		for (int j = 0; j<numRanks; j++) {
			Card newCard;
			char suitChar = SuitStrings1[i];
			string rankChar = RankStrings1[j];
			newCard.suit = newCard.create_suit(suitChar);
			newCard.rank = newCard.create_rank(rankChar);
			add_card(newCard);
		}
	}
}

ostream &operator<<(ostream &out, const Deck &deck)
{
	for (int i = 0; i < deck.size(); ++i)
	{
		out << deck.aDeck.at(i).get_card_def() << "\n";
	}
	return out;
}