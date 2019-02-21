// hand.h
//
// author: Glizela Taino gtaino@wustl.edu
//
// purpose: Definitions for a sorting hands, printing hand ranks, and identifying hand ranks
//

#pragma once
#include "Card.h"
#include "Deck.h"
#include <vector>

using namespace std;

class Hand 
{
	friend class TexasHoldEm; 
public:
	Hand() : theHand() {}
	Hand(const vector<Card>& aHand) : theHand(aHand) { sort_poker_hand();}
	Hand& operator= (const Hand& a_diff_hand);
	bool operator== (const Hand&) const;
	bool operator< (const Hand&) const;
	const Card& operator[] (size_t index) const;
	void remove_card(size_t index);
	int size() const;
	string as_string() const;
	void sort_poker_hand();
	void pop_back();
	Card& back();
	bool contains(Card c);
	vector<Hand> combination(int choose);
	string as_anonymous_string();

	friend ostream &operator<<(ostream &out, const Hand &hand);
	friend Hand &operator<<(Hand& hand, Deck& deck);
	friend class FiveCardDraw; 
	friend class SevenCardStud;
	friend class Game;
private:
	vector<Card> theHand;
	void combinationRecursive(int choose, int start, int currLen, bool check[], int len, vector<Hand>& allPossibleHands);
};

ostream &operator<<(ostream &out, const Hand &hand);
Hand &operator<<(Hand& hand, Deck& deck);
