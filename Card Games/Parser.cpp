// Parser.cpp : Holds funtions that parse the text file, prints out cards, checks for valid input, 
// and has the usage funciton.
//
// author: Glizela Taino gtaino@wustl.edu
//
// purpose: definitions for a simple parse, print, checks validity, and usage message for a card
//			creator program. Function are called from the program main in Lab0.cpp
//

#include "stdafx.h"
#include "Card.h"
#include "Parser.h"
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

int read_card_def(vector<Card> & cards, char * file_name)
{
	// Read in file
	ifstream file(file_name);

	// Returns a value of 2 when file is not found
	if (!file.is_open())
	{
		cout << "filenotfound" << endl;
		return file_not_found;
	}

	// Loops through each card def
	string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);

		ltrim(line);
		rtrim(line);

		if (line.compare("") != 0)
		{
			parse_line(cards, line);
		}
	}

	file.close();
	return success; //Returns 0 with successful execution
}


int parse_line(vector<Card> & cards, string line)
{
	//read in line
	istringstream buf(line);
	vector<string> valid_card_defs;

	// Loops through each card def
	for (string card_def; buf >> card_def;)
	{
		if (card_def.compare("//") == 0)
		{
			break;
		}

		//save if valid
		if (is_valid(card_def)) {
			valid_card_defs.push_back(card_def);
		}
	}

	//if makes a hand
	if (valid_card_defs.size() == 7)
	{
		//create cards
		for (unsigned i = 0; i < valid_card_defs.size(); ++i)
		{
			Card card;
			card.set_card(valid_card_defs[i]);
			cards.push_back(card);
		}

		return success;
	}
	else if (valid_card_defs.size() > 7)
	{
		cout << "Too many valid cards to make a hand." << endl;
	}
	else
	{
		cout << "Not enough valid cards to make a hand." << endl;
	}

	return success;
}


bool is_valid(string card_def)
{
	//Sets used to find occurence in O(1)
	set<char> suits = { 'C', 'D', 'S', 'H' };
	set<string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

	//Used for feedback statement
	string original_card_def = card_def;

	//Checks if suit is valid
	if (suits.count(toupper(card_def.back())))
	{
		//Capitalizes ranks if a letter
		card_def.pop_back();
		if (card_def.size() == 1)
		{
			if (!isdigit(card_def[0]))
			{
				card_def = toupper(card_def[0]);
			}
		}

		//checks if rank is valid
		if (ranks.count(card_def))
		{
			return true;
		}
		else
		{
			cout << "Encountered invalid rank: " + original_card_def << endl;
		}
	}
	else
	{
		cout << "Encountered invalid suit: " + original_card_def << endl;
	}

	return false;
}

int print_cards(const vector<Card> & cards)
{
	for (unsigned i = 0; i < cards.size(); ++i)
	{
		Card card = cards.at(i);
		string def = card.get_card_def();

		cout << def << endl;
	}

	return success; //Returns 0 with successful execution
}


// Helper function to print out the program's usage message. 
int usage(char * program_name, int code, char * info)
{
	cout << "Usage: " << program_name << " [GameName] [player1] [player2]" << endl
		<< "Purpose: Plays a game of Five Card Draw, Seven Card Stud, or Texas Hold Em with two or more players." << endl
		<< "Error: " << info
		<< endl;
	

	return code; //Returns 1 with improper input
}

// trim from start (in place)
static inline void ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}