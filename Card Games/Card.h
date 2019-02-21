// Card.h
//
// author: Glizela Taino gtaino@wustl.edu
//
// purpose: Declarations for a Card struct object used to hold
//			information about valid card deinitions

#pragma once

#include <vector>

using namespace std;

struct Card
{
	enum Suit { Clubs = 0, Diamonds = 1, Hearts = 2, Spades = 3 };
	enum Rank { Two = 0, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
	static const char * SuitStrings[];
	static const char * RankStrings[];

	Suit suit;
	Rank rank;

	void set_card(string card_def);
	Rank create_rank(string rank_str);
	Suit create_suit(char suit_str);
	string get_card_def() const;
	Suit get_suit() const;
	Rank get_rank() const;
	bool operator< (const Card &) const;
	bool operator== (const Card&) const;
	bool is_facing_down;
	void setIsFacingDown(bool isFacingDown);
};