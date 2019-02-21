// Parser.h
//
// author: Glizela Taino gtaino@wustl.edu
//
// purpose: Declarations for a simple parse, print, checks validity, and usage message for a card
//			creator program

#pragma once

#include "Card.h"
#include <fstream>
#include <iostream>
#include <string>
#include <set>

enum Result_Code { success=0, too_few_args, too_many_args, no_file_name, no_shuffle, file_not_found };

bool is_valid(string card_def);
int read_card_def(vector<Card> & cards, char * file_name);
int print_cards(const vector<Card> & cards);
int usage(char * program_name, int code, char * info);
int parse_line(vector<Card> & cards, string line);
static inline void ltrim(std::string &s);
static inline void rtrim(std::string &s);