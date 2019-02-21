// Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"
#include "Game.h"

using namespace std;

int main(int argc, char *argv[])
{
	// validate user input
	const int min_arg = 4;
	const int program_name_index = 0; 
	shared_ptr<Game> gamePtr;


	int value;
	if (argc < min_arg)
	{
		value = usage(argv[program_name_index], too_few_args, "Too few arguments.");
		return value;
	}

	else {
		try
		{
			//call the Game class' static start_game method with the first argument 
			//after the program name
			Game::start_game(argv[1]);
		}
		catch (errorCodes c)
		{
			value = usage(argv[program_name_index], (int)c, "Unknown game");
			return value;
		}


		//call the Game class' static instance method to obtain a pointer(or shared_ptr) to the game object.
		gamePtr = Game::instance();

		//add players to game
		try {
			for (int i = 2; i < argc; ++i)
			{
				string playerName = argv[i];
				gamePtr->add_player(playerName);
			}
		}
		catch (errorCodes c) {
			value = usage(argv[program_name_index], already_playing, "Player already added");
		}


		//While there are still at least two players in the game, 
		while (gamePtr->get_num_player() >= 2)
		{
			try
			{

				//repeatedly call the game object's before_round, round, and after_round methods.
				gamePtr->before_round();
				gamePtr->round();
				gamePtr->after_round();
			}
			//catch any exceptions
			catch (errorCodes c)
			{
				//call the Game class' static stop_game method
				gamePtr->stop_game();

				//return an appropriate non-zero value to indicate failure
				cout << c << endl;
				return 4;
			}

		}


		//there are no more players in the game it should call the Game class' static stop_game method
		try
		{
			gamePtr->stop_game();
		}
		catch (errorCodes c) {
			return no_game_in_progress;
		}
	}

	while (true) {
		string firstChoice;
		string gameOption;
		string names;

		cout << "Thanks for playing! Would you like to play another game or quit the program? (Select option)" << endl;
		cout << "1: I want to play another game!" << endl;
		cout << "2: I want to quit the program." << endl;
		cin >> firstChoice;

		if (firstChoice != "1" && firstChoice != "2") {
			cout << "Invalid option, please try again" << endl;
		}
		else if (firstChoice == "1") {
			cout << "Would you like to play FiveCardDraw, SevenCardStud, or TexasHoldEm?" << endl;
			cin >> gameOption;
			if (gameOption == "FiveCardDraw" || gameOption == "SevenCardStud" || gameOption == "TexasHoldEm") {
				Game::start_game(gameOption);
				gamePtr = Game::instance();

			}
			else {
				cout << "Please try again and enter FiveCardDraw, SevenCardStud, or TexasHoldEm" << endl;
			}


			cout << "Please enter names of players with spaces in between followed by > when you are done." << endl;
			cout << "For example: Zela Jackie Priyanshu >" << endl;

			while (cin >> names) {
				if (names == ">") {
					break;
				}
				else {
					try {
						gamePtr->Game::add_player(names);
					}

					catch (errorCodes c) {
						value = usage(argv[program_name_index], already_playing, "Player already added");
					}
					
				}
			}

			while (gamePtr->get_num_player() >= 2)
			{
				try
				{

					//repeatedly call the game object's before_round, round, and after_round methods.
					gamePtr->before_round();
					gamePtr->round();
					gamePtr->after_round();
				}

				//catch any exceptions
				catch (errorCodes c)
				{
					//call the Game class' static stop_game method
					gamePtr->stop_game();

					//return an appropriate non-zero value to indicate failure
					cout << c << endl;
					return 4;
				}
			}


			//there are no more players in the game it should call the Game class' static stop_game method
			try
			{
				gamePtr->stop_game();
			}
			catch (errorCodes c) {
				return no_game_in_progress;
			}		
		}

		else if (firstChoice == "2") {
			cout << "Exiting game." << endl;
			cout << "Thanks for playing!" << endl;
			break;
		}
	}

	//return 0 to indicate success
	return success;
}

