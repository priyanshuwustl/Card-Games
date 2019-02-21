// Card.cpp: A data structure to hold the suits and ranks as strings for the purpose of 
//			 switching between the enum representation and string representation.
//
// author: Glizela Taino gtaino@wustl.edu
//
// purpose: Declarations of a simple valid card creator
//

#include "stdafx.h"
#include "Card.h"
#include <fstream>
#include <iostream>
#include <string>
#include <set>

using namespace std;
const char * Card::RankStrings[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",  "A" };
const char * Card::SuitStrings[] = { "C", "D", "H", "S" };

void Card::set_card(string card_def)
{
	//parsing valid card def and creates a card
	char suit_str = card_def[card_def.length() - 1];
	suit = create_suit(suit_str);
	card_def.pop_back();

	rank = create_rank(card_def);
}

Card::Rank Card::create_rank(string rank_str)
{

	//Upper case rank
	if (rank_str.length() == 1 && !isdigit(rank_str[0]))
	{
		rank_str = toupper(rank_str[0]);
	}

	//Returns enum representation
	if (rank_str == "A")
	{
		return Card::Ace;
	}
	else if (rank_str == "2")
	{
		return Card::Two;
	}
	else if (rank_str == "3")
	{
		return Card::Three;
	}
	else if (rank_str == "4")
	{
		return Card::Four;
	}
	else if (rank_str == "5")
	{
		return Card::Five;
	}
	else if (rank_str == "6")
	{
		return Card::Six;
	}
	else if (rank_str == "7")
	{
		return Card::Seven;
	}
	else if (rank_str == "8")
	{
		return Card::Eight;
	}
	else if (rank_str == "9")
	{
		return Card::Nine;
	}
	else if (rank_str == "10")
	{
		return Card::Ten;
	}
	else if (rank_str == "J")
	{
		return Card::Jack;
	}
	else if (rank_str == "Q")
	{
		return Card::Queen;
	}
	else
	{
		return Card::King;
	}
}

Card::Suit Card::create_suit(char suit_str)
{
	//Upper case suit
	suit_str = toupper(suit_str);

	//Return enum representation
	if (suit_str == 'C')
	{
		return Card::Clubs;
	}
	else if (suit_str == 'H')
	{
		return Card::Hearts;
	}
	else if (suit_str == 'S')
	{
		return Card::Spades;
	}
	else
	{
		return Card::Diamonds;
	}
}

string Card::get_card_def() const
{
	//get enum value representation
	int rankVal = (int)rank;
	int suitVal = (int)suit;

	//get string representation
	string suit_str = SuitStrings[suitVal];
	string def = RankStrings[rankVal] + suit_str;
	return def;
}

Card::Rank Card::get_rank() const
{
	return rank;
}

Card::Suit Card::get_suit() const
{
	return suit;
}

bool Card::operator< (const Card & card) const
{
	//order by rank unless theyre equal
	if (rank == card.rank)
	{
		return suit < card.suit;
	}

	return rank < card.rank;
}

bool Card::operator== (const Card& other_card) const 
{
	return ((rank == other_card.rank) && (suit == other_card.suit));
}

void Card::setIsFacingDown(bool isFacingDown) 
{
	is_facing_down = isFacingDown;
}

