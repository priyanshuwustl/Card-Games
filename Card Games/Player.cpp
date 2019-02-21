#include "stdafx.h"
#include "Player.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Player::Player(const string& player_name)
{

	// Read in file
	ifstream file(player_name);

	// Returns a value of 2 when file is not found
	if (file.is_open())
	{
		cout << "Player exists " << player_name << endl;
		// Loops through each card def
		string word;
		stringstream strValue;
		int counter = 0;
		while (file >> word)
		{
			if (counter == 0)
			{
				name = word;
			}
			else if (counter == 1)
			{
				strValue << word;
				strValue >> hands_won;
				strValue.clear();
			}
			else if (counter == 2)
			{
				strValue << word;
				strValue >> hands_lost;
			}
			//This is for storing poker chips with a player
			else if (counter == 3)
			{
				strValue << word;
				strValue >> poker_chips;
			}
			counter++;
		}

		file.close();
		if (counter == 3)
		{
			return;
		}
	}

	name = player_name;
	hands_won = 0;
	hands_lost = 0;
	poker_chips = 20;
	folded = false;
}

ostream &operator<<(ostream &out, const Player &player)
{
	out << player.name << " " << player.hands_won << " " << player.hands_lost << " " << player.poker_chips << endl;
	return out;
}
