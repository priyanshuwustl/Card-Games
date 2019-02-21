// hand.h
//
// author: Glizela Taino gtaino@wustl.edu
//
// purpose: Definitions for a sorting hands, printing hand ranks, and identifying hand ranks
//

#include "stdafx.h"
#include "Card.h"
#include "hand.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <sstream>

using namespace std;

void Hand::sort_poker_hand()
{
	//sort hand
	sort(theHand.begin(), theHand.end());
}

Hand& Hand::operator= (const Hand& a_diff_hand)
{
	if (&a_diff_hand == this) return *this;  // early exit if self assignment

	theHand.clear();
	for (unsigned int i =	 0; i < a_diff_hand.theHand.size(); ++i) 
	{
		theHand.push_back(a_diff_hand.theHand.at(i));
	}
	sort_poker_hand();
	return *this;
}

int Hand::size() const
{
	return theHand.size();
}

bool Hand::operator== (const Hand& other_hand) const
{
	return theHand == other_hand.theHand;
}

bool Hand::operator< (const Hand& other_hand) const 
{

	Card card1;
	Card card2;

	if (size() == other_hand.size()) 
	{
		for (int i = 0; i < size(); ++i)
		{
			card1 = theHand[i];
			card2 = other_hand.theHand[i];
			if (!(card1 == card2))
			{
				return card1 < card2;
			}
		}
		return true;
	}
	else if (size() < other_hand.size()) 
	{
		for (int i = 0; i < size(); ++i)
		{
			card1 = theHand[i];
			card2 = other_hand.theHand[i];
			if (!(card1 == card2))
			{
				return card1 < card2;
			}
		}
		return true;
	}
	else 
	{
		for (int i = 0; i < other_hand.size(); ++i)
		{
			card1 = theHand[i];
			card2 = other_hand.theHand[i];
			if (!(card1 == card2))
			{
				return card1 < card2;
			}
		}
		return false;
	}
}

ostream &operator<<(ostream &out, const Hand &hand) 
{
	for (int i = 0; i < hand.size(); ++i)
	{
		out << hand.theHand.at(i).get_card_def() << " ";
	}
	return out;
}

string Hand::as_string() const 
{
	stringstream s;
	for (unsigned int i = 0; i < theHand.size(); ++i) 
	{
		s << theHand[i].get_card_def() << " ";
	}

	return s.str();
}

Hand &operator<<(Hand& hand, Deck& deck)
{
	//if (hand.size() == hand.numCardsInHand)
	//{
	//	return hand;
	//}

	hand.theHand.push_back(deck.deal_card());
	//hand.sort_poker_hand();
	return hand;
}

const Card& Hand::operator[] (size_t index) const
{
	if (index >= theHand.size()) 
	{
		throw out_of_range("Index out of bounds");
	} //what should catch this? Do i have to make something catch this?
		// why do we have to throw this? Wont the program throw this?
	return theHand[index];
}

void Hand::remove_card(size_t index) 
{
	if (index >= theHand.size()) 
	{
		throw out_of_range("No card at that index");
	}
	theHand.erase(theHand.begin() + index);
	//if you use the iterator to erase, you much reposition the iterator
}

void Hand::pop_back() 
{
	theHand.pop_back();
}

Card& Hand::back() 
{
	return theHand.back();
}

bool Hand::contains(Card c) 
{

	for (int i = 0; i < size(); ++i) 
	{
		if (theHand[i] == c) 
		{
			return true;
		}
	}

	return false;
}

vector<Hand> Hand::combination(int choose)
{
	const int n = 7; //hard coded
	bool check[n];

	for (int i = 0; i < n; ++i)
	{
		check[i] = false;
	}

	vector<Hand> allPossibleHands;

	combinationRecursive(choose, 0, 0, check, size(), allPossibleHands);
	return allPossibleHands;
}

void Hand::combinationRecursive(int choose, int start, int currLen, bool check[], int len, vector<Hand>& allPossibleHands)
{

	vector<Card> combinationOfCards;

	// Return if the currLen is more than the required length.
	if (currLen > choose)
		return;
	// If currLen is equal to required length then print the sequence.
	else if (currLen == choose)
	{
		//cout << "\t";combinationOfCards
		for (int i = 0; i < len; i++)
		{
			if (check[i] == true)
			{
				//cout << theHand[i].get_card_def() << " ";
				combinationOfCards.push_back(theHand[i]);
			}
		}
		//cout << "\n";
		Hand possibleHand(combinationOfCards);
		allPossibleHands.push_back(possibleHand);
		return;
	}
	// If start equals to len then return since no further element left.
	if (start == len)
	{
		return;
	}
	// For every index we have two options.
	// First is, we select it, means put true in check[] and increment currLen and start.
	check[start] = true;
	combinationRecursive(choose, start + 1, currLen + 1, check, len, allPossibleHands);
	// Second is, we don't select it, means put false in check[] and only start incremented.
	check[start] = false;
	combinationRecursive(choose, start + 1, currLen, check, len, allPossibleHands);
}

string Hand::as_anonymous_string() 
{
	stringstream s;
	for (unsigned int i = 0; i < theHand.size(); ++i)
	{
		//cout << theHand[i].is_facing_down << endl;
		if (theHand[i].is_facing_down)
		{
			s << "* ";
		}
		else 
		{
			s << theHand[i].get_card_def() << " ";
		}
	}

	return s.str();
}