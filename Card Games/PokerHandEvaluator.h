// PokerHandEvaluator.h : holds functions that are used to evaluate and sort poker hands
//
// author: Glizela Taino gtaino@wustl.edu
//

#pragma once
#include "Hand.h"
#include <map>

enum Poker_Rankings { No_Rank = 0, One_Pair, Two_Pairs, Three_of_a_Kind, Straight, Flush, Full_House, Four_of_a_Kind, Straight_Flush};

Poker_Rankings evaluate_hand(Hand& hand);
void print_poker_hand_rank(Hand& hand);
bool is_four_of_a_kind(const Hand& hand);
bool is_full_house(const Hand& hand);
bool is_flush(const Hand& hand);
bool is_straight(const Hand& hand);
bool is_three_of_a_kind(const Hand& hand);
bool is_two_pairs(const Hand& hand);
bool is_one_pair(const Hand& hand);
bool is_straight_flush(const Hand& hand);
bool poker_rank(Hand& hand1, Hand& hand2);
map<Card::Rank, int> get_num_occurences(const Hand& hand);
Card::Rank get_k_occuring_rank(map<Card::Rank, int> num_occurences_map, int target_num_occur, int target_priority);
