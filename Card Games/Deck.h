// Deck.h : holds functions that are used to create a deck, output to a stream, deal cards,
// and shuffle the deck
//
// author: Glizela Taino gtaino@wustl.edu
//

#pragma once

#include "Card.h"
#include <string>

using namespace std;

class Deck
{
public:
	Deck();
	Deck(char * file_name);
	int load_file(char * file_name);
	void shuffle_deck();
	int size() const;
	Card deal_card();
	void add_card(Card card);
	void createFullDeck();

	const int numSuits = 4;
	const int numRanks = 13;

	static const char SuitStrings1[4];
	static const string RankStrings1[13];

	friend ostream &operator<<(ostream &out, const Deck &deck);

private:
	vector<Card> aDeck;

};

ostream &operator<<(ostream &out, const Deck &deck);