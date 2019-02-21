#pragma once

#include "Hand.h"

using namespace std;

struct Player
{
	Player(const string& name);

	string name = "";
	Hand hand;
	unsigned int hands_won;
	unsigned int hands_lost;
	unsigned int poker_chips;
	int moneyInRound;
	bool folded;
};

ostream &operator<<(ostream &out, const Player &player);

