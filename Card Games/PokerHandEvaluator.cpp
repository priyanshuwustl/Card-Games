// PokerHandEvaluator.h : holds functions that are used to evaluate and sort poker hands
//
// author: Glizela Taino gtaino@wustl.edu
//

#include "stdafx.h"
#include "PokerHandEvaluator.h"
#include "Card.h"
#include "Hand.h"
#include <iostream>
#include <map>

using namespace std;

Poker_Rankings evaluate_hand(Hand& hand) 
{
	hand.sort_poker_hand();

	if (is_straight_flush(hand))
	{
		return Straight_Flush;
	}
	else if (is_four_of_a_kind(hand))
	{
		return Four_of_a_Kind;
	}
	else if (is_full_house(hand))
	{
		return Full_House;
	}
	else if (is_flush(hand))
	{
		return Flush;
	}
	else if (is_straight(hand))
	{
		return Straight;
	}
	else if (is_three_of_a_kind(hand))
	{
		return Three_of_a_Kind;
	}
	else if (is_two_pairs(hand))
	{
		return Two_Pairs;
	}
	else if (is_one_pair(hand))
	{
		return One_Pair;
	}
	
	return No_Rank;
}

void print_poker_hand_rank(Hand& hand)
{

	//check what rank it is
	if (is_straight_flush(hand))
	{
		cout << "Straight flush" << endl;
	}
	else if (is_four_of_a_kind(hand))
	{
		cout << "Four of a kind" << endl;
	}
	else if (is_full_house(hand))
	{
		cout << "Full house" << endl;
	}
	else if (is_flush(hand))
	{
		cout << "Flush" << endl;
	}
	else if (is_straight(hand))
	{
		cout << "Straight" << endl;
	}
	else if (is_three_of_a_kind(hand))
	{
		cout << "Three of a kind" << endl;
	}
	else if (is_two_pairs(hand))
	{
		cout << "Two pairs" << endl;
	}
	else if (is_one_pair(hand))
	{
		cout << "One pair" << endl;
	}
	else
	{
		cout << "No ranks apply" << endl;
	}
}

bool is_straight_flush(const Hand& hand)
{
	Card::Suit first_suit = hand[0].get_suit();
	int a_rank = int(hand[0].get_rank());
	for (int i = 1; i < hand.size(); ++i)
	{
		//check if all cards are same suit
		if (hand[i].get_suit() != first_suit)
		{
			return false;
		}
	}

	return is_straight(hand);
}

bool is_four_of_a_kind(const Hand& hand)
{
	//record number of each rank
	map<Card::Rank, int> num_occur = get_num_occurences(hand);

	//find if a rank has four occurences
	for (auto it = num_occur.begin(); it != num_occur.end(); ++it)
	{
		Card::Rank a_rank = it->first;

		if (num_occur[a_rank] == 4)
		{
			return true;
		}
	}

	return false;
}

bool is_full_house(const Hand& hand)
{
	// count number of occurences
	map<Card::Rank, int> num_occur = get_num_occurences(hand);

	//find if a rank has right number of occurences
	int counter = 0;
	for (auto it = num_occur.begin(); it != num_occur.end(); ++it)
	{
		Card::Rank a_rank = it->first;
		if (num_occur[a_rank] == 3)
		{
			counter++;
		}
		else if (num_occur[a_rank] == 2)
		{
			counter++;
		}
	}

	if (counter == 2 && num_occur.size() == 2)
	{
		return true;
	}

	return false;
}

bool is_flush(const Hand& hand)
{
	//check if all cards are same suit
	Card::Suit first_suit = hand[0].get_suit();
	for (int i = 1; i < hand.size(); ++i)
	{
		if (hand[i].get_suit() != first_suit)
		{
			return false;
		}
	}

	return true;
}

bool is_straight(const Hand& hand)
{
	int a_rank = int(hand[hand.size() - 1].get_rank());
	if (a_rank == 12)
	{
		a_rank = -1;
		//check for low ace
		for (int i = 0; i < hand.size() - 1; ++i)
		{

			//check if cards are consecutive in rank
			if (hand[i].get_rank() != (a_rank + 1))
			{
				break;
			}

			a_rank++;
		}

		if (a_rank == 3) {
			return true;
		}

		//check for high ace
		a_rank = 12;
		for (int i = hand.size() - 2; i >= 0; --i)
		{
			//check if cards are consecutive in rank
			if (hand[i].get_rank() != (a_rank - 1))
			{
				return false;
			}

			a_rank--;
		}
		return true;
	}
	else
	{
		a_rank = int(hand[0].get_rank());
		for (int i = 1; i < hand.size(); ++i)
		{
			//check if cards are consecutive in rank
			if (hand[i].get_rank() != (a_rank + 1))
			{
				return false;
			}

			a_rank++;
		}
		return true;
	}
}

bool is_three_of_a_kind(const Hand& hand)
{
	map<Card::Rank, int> num_occur = get_num_occurences(hand);

	//find if a rank has three occurences
	for (auto it = num_occur.begin(); it != num_occur.end(); ++it)
	{
		Card::Rank a_rank = it->first;

		if (num_occur[a_rank] == 3)
		{
			return true;
		}
	}

	return false;
}

bool is_two_pairs(const Hand& hand)
{
	// count number of occurences
	map<Card::Rank, int> num_occur = get_num_occurences(hand);

	//find if a rank has correct number of occurences
	int counter = 0;
	for (auto it = num_occur.begin(); it != num_occur.end(); ++it)
	{
		Card::Rank a_rank = it->first;

		if (num_occur[a_rank] == 2)
		{
			counter++;
		}
		else if (num_occur[a_rank] == 1)
		{
			counter++;
		}
	}

	if (counter == 3 && num_occur.size() == 3)
	{
		return true;
	}

	return false;
}

bool is_one_pair(const Hand& hand)
{
	// count number of occurences
	map<Card::Rank, int> num_occur = get_num_occurences(hand);

	//find if a rank has right number of occurences
	int counter = 0;
	for (auto it = num_occur.begin(); it != num_occur.end(); ++it)
	{
		Card::Rank a_rank = it->first;

		if (num_occur[a_rank] == 2)
		{
			counter++;
		}
		else if (num_occur[a_rank] == 1)
		{
			counter++;
		}
	}

	if (counter == 4 && num_occur.size() == 4)
	{
		return true;
	}

	return false;
}

Card::Rank get_k_occuring_rank(map<Card::Rank, int> num_occurences_map, int target_num_occur, int target_priority)
{
	//target_priority = 1 means highest
	//target_priority = 2 means second highest
	//target_priority = 3 means third highest

	int priority = 1;
	for (auto rit = num_occurences_map.rbegin(); rit != num_occurences_map.rend(); ++rit)
	{
		Card::Rank a_rank = rit->first;
		int num_occur = rit->second;
		if (num_occur == target_num_occur)
		{
			if(priority == target_priority)
			{
				return a_rank;
			}
			priority++;
		}
	}

	return Card::Ace;
}

map<Card::Rank, int> get_num_occurences(const Hand& hand) 
{
	map<Card::Rank, int> num_occur_hand;
	for (int i = 0; i < hand.size(); ++i)
	{
		Card::Rank key = hand[i].get_rank();
		if (num_occur_hand.count(key) != 0)
		{
			num_occur_hand[key] += 1;
		}
		else
		{
			num_occur_hand[key] = 1;
		}
	}

	return num_occur_hand;
}

bool poker_rank(Hand& hand1, Hand& hand2) 
{
	Poker_Rankings r1 = evaluate_hand(hand1);
	Poker_Rankings r2 = evaluate_hand(hand2);

	if (r1 != r2)
	{
		return r1 > r2;
	}

	map<Card::Rank, int> num_occur_hand1 = get_num_occurences(hand1);
	map<Card::Rank, int> num_occur_hand2 = get_num_occurences(hand2);

	Card::Rank rank1;
	Card::Rank rank2;

	if (r1 == Straight_Flush) 
	{
		//sorted by the highest card in each hand
		Card highest1 = hand1[hand1.size() - 1];
		Card highest2 = hand2[hand2.size() - 1];

		return highest2 < highest1;
	}
	else if (r1 == Four_of_a_Kind) 
	{
		//sorted by the rank of the 4 cards that have the same rank
		rank1 = get_k_occuring_rank(num_occur_hand1, 4, 1);
		rank2 = get_k_occuring_rank(num_occur_hand2, 4, 1);
		
		//sort based on found ranks
		return rank1 > rank2;
	}
	else if (r1 == Full_House) 
	{
		//get rank of three of a kind for hand1 and hand2
		rank1 = get_k_occuring_rank(num_occur_hand1, 3, 1);
		rank2 = get_k_occuring_rank(num_occur_hand2, 3, 1);

		if (rank1 != rank2) 
		{
			return rank1 > rank2;
		}

		rank1 = get_k_occuring_rank(num_occur_hand1, 2, 1);
		rank2 = get_k_occuring_rank(num_occur_hand2, 2, 1);

		return rank1 > rank2;

	}
	else if (r1 == Flush) 
	{
		//sorted by highest ranking card, then by the next highest ranking card, etc.
		for (int i = hand1.size(); i > 0; --i) 
		{
			rank1 = hand1[i - 1].get_rank();
			rank2 = hand2[i - 1].get_rank();
			
			if (rank1 != rank2) 
			{
				return rank1 > rank2;
			}
		}

		return rank1 > rank2;
	}
	else if (r1 == Straight) 
	{
		//sorted by the highest card in each hand
		for (int i = hand1.size(); i > 0; --i)
		{
			Card card1 = hand1[i - 1];
			Card card2 = hand2[i - 1];

			if (!(card1 == card2))
			{
				return card2 < card1;
			}
		}
		return false;
	}
	else if (r1 == Three_of_a_Kind)
	{
		//sorted by the rank of the 3 cards that have the same rank
		rank1 = get_k_occuring_rank(num_occur_hand1, 3, 1);
		rank2 = get_k_occuring_rank(num_occur_hand2, 3, 1);

		return rank1 > rank2;
	}
	else if (r1 == Two_Pairs)
	{
		// sorted by the rank of the higher pair
		rank1 = get_k_occuring_rank(num_occur_hand1, 2, 1);
		rank2 = get_k_occuring_rank(num_occur_hand2, 2, 1);

		if (rank1 == rank2) 
		{
			return rank1 > rank2;
		}

		//then by the rank of the lower pair, 
		rank1 = get_k_occuring_rank(num_occur_hand1, 2, 2);
		rank2 = get_k_occuring_rank(num_occur_hand2, 2, 2);

		if (rank1 == rank2)
		{
			return rank1 > rank2;
		}

		//and then by the 5th card
		rank1 = get_k_occuring_rank(num_occur_hand1, 1, 1);
		rank2 = get_k_occuring_rank(num_occur_hand2, 1, 1);

		return rank1 > rank2;

	}
	else if (r1 == One_Pair)
	{
		
		//sorted by the rank of the pair, 
		rank1 = get_k_occuring_rank(num_occur_hand1, 2, 1);
		rank2 = get_k_occuring_rank(num_occur_hand2, 2, 1);

		if (rank1 != rank2) 
		{
			return rank1 > rank2;
		}

		//then by the highest unpaired card,
		rank1 = get_k_occuring_rank(num_occur_hand1, 1, 1);
		rank2 = get_k_occuring_rank(num_occur_hand2, 1, 1);

		if (rank1 != rank2)
		{
			return rank1 > rank2;
		}

		//then the next highest unpaired card, 
		rank1 = get_k_occuring_rank(num_occur_hand1, 1, 2);
		rank2 = get_k_occuring_rank(num_occur_hand2, 1, 2);

		if (rank1 != rank2)
		{
			return rank1 > rank2;
		}

		//and then by the next highest unpaired card
		rank1 = get_k_occuring_rank(num_occur_hand1, 1, 3);
		rank2 = get_k_occuring_rank(num_occur_hand2, 1, 3);

		if (rank1 != rank2)
		{
			return rank1 > rank2;
		}

		return false;

	}
	else if (r1 == No_Rank) 
	{
		//sorted by highest ranking card, then by the next highest ranking card, etc	
		rank1 = get_k_occuring_rank(num_occur_hand1, 1, 1);
		rank2 = get_k_occuring_rank(num_occur_hand2, 1, 1);

		rank1 = get_k_occuring_rank(num_occur_hand1, 1, 2);
		rank2 = get_k_occuring_rank(num_occur_hand2, 1, 2);

		if (rank1 != rank2)
		{
			return rank1 > rank2;
		}

		rank1 = get_k_occuring_rank(num_occur_hand1, 1, 3);
		rank2 = get_k_occuring_rank(num_occur_hand2, 1, 3);

		if (rank1 != rank2)
		{
			return rank1 > rank2;
		}

		rank1 = get_k_occuring_rank(num_occur_hand1, 1, 4);
		rank2 = get_k_occuring_rank(num_occur_hand2, 1, 4);

		if (rank1 != rank2)
		{
			return rank1 > rank2;
		}

		rank1 = get_k_occuring_rank(num_occur_hand1, 1, 5);
		rank2 = get_k_occuring_rank(num_occur_hand2, 1, 5);

		if (rank1 != rank2)
		{
			return rank1 > rank2;
		}

		return rank1 > rank2;
	}

	return false;
}