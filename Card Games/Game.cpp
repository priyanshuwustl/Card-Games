//	Game.cpp holds functions that will start a poker game, stop the game, 
//	add a player to the game, and find a particular player.
//	It also includes virtual functions that will print out a players hand
//  before a turn, after a turn, before a round and after a round. 
//	We used shared_ptrs instead of the native C++ pointer
//
//	author: Jackie Wong jackie.wong@wustl.edu

#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include <algorithm>
#include <iostream>
#include "Player.h"
#include "Deck.h"
#include "hand.h"
#include "Card.h"
#include <memory>
#include <fstream>
#include "TexasHoldEm.h"
#include <sstream>
#include "PokerHandEvaluator.h"
#include "Parser.h"

using namespace std;

const int lessCallingChipsError = 500;

shared_ptr<Game> Game::gamePtr;

int Game::get_num_player()
{
	return playerPtr.size();
}

void Game::ante(Player &player)
{
	cout << player.name << endl;
	player.poker_chips--;
	player.moneyInRound = 1;
	pot++;
	cout << "One chip removed to play. New number of chips: " << player.poker_chips << "\n" << endl;
}


shared_ptr<Game> Game::instance() {
	shared_ptr<Game> ptr = nullptr;
	try {
		if (Game::gamePtr != nullptr) {
			ptr = Game::gamePtr;
		}
	}
	catch (...) {
		throw (errorCodes::instance_not_available);
	}
	return ptr;
}

void Game::start_game(const string& gameInput) {
	if (gameInput == "FiveCardDraw") {
		Game::gamePtr = make_shared<FiveCardDraw>();
		cout << "Welcome to Five Card Draw!" << endl;
		cout << "Let the game begin!\n" << endl;
	}
	//will create game for SevenCardStud
	else if (gameInput == "SevenCardStud") {
		Game::gamePtr = make_shared<SevenCardStud>();
		cout << "Welcome to Seven Card Stud!" << endl;
		cout << "Let the game begin!\n" << endl;
	}
	//will create game for texas hold em 
	else if (gameInput == "TexasHoldEm") {
		Game::gamePtr = make_shared<TexasHoldEm>();
		cout << "Welcome to Texas Hold Em!" << endl;
		cout << "Let the game begin!\n" << endl;
	}
	else if (Game::gamePtr != nullptr) {
		throw (errorCodes::game_already_started);
	}
	else {
		throw (errorCodes::unknown_game);
	}

}

void Game::stop_game() {
	if (Game::gamePtr == nullptr) {
		throw (errorCodes::no_game_in_progress);
	}
	else {
		Game::gamePtr = nullptr;
	}
}

//add player to game unless they already exist
void Game::add_player(const string& player) {
	//loop through each pointer of players
	for (unsigned int i = 0; i < playerPtr.size(); ++i)
	{
		//dereference pointers
		Player p = *playerPtr[i];
		string name = p.name;

		//if the player name matches throw error
		if (name == player)
		{
			throw(errorCodes::already_playing);
		}
	}

	playerPtr.push_back(make_shared<Player>(player));
	cout << player << " is added to the game!" << endl;
}

shared_ptr<Player> Game::find_player(const string& queryName) {
	//loop through each pointer of players
	for (unsigned int i = 0; i < playerPtr.size(); ++i)
	{
		//dereference pointers
		Player p = *playerPtr[i];
		string name = p.name;

		//if the player name matches throw error
		if (name == queryName)
		{
			return playerPtr[i];
		}
	}
	return nullptr;
}

//check to see who the winner is and update poker chips.
//Also print out all players stats at the end of the round
bool Game::checkForWinner() {
	if (numFoldedPlayers == (playerPtr.size() - 1)) {
		for (vector<shared_ptr<Player>>::iterator i = playerPtr.begin(); i<playerPtr.end(); ++i) {
			shared_ptr<Player> player = *i;
			if (player->folded) {
				player->hands_lost += 1;
			}
			else {
				cout << "Winner of this round is " << player->name << "\n" << endl;
				player->hands_won += 1;
				player->poker_chips += pot;
				pot = 0;
			}
		}

		for (vector<shared_ptr<Player>>::iterator i = playerPtr.begin(); i < playerPtr.end(); ++i) {
			shared_ptr<Player> player = *i;
			cout << player->name << " won " << player->hands_won << " hands and lost " << player->hands_lost << " hands. " << player->poker_chips << " poker chips." << endl;

			for (Card card : player->hand.theHand) {
				cout << card.get_card_def() << " ";
			}
			cout << "\n" << endl;
		}

		return true;
	}
	else {
		return false;
	}
}

// How playersChecked works:
// * Whenever a player checks it is incremented by 1
// * Anytime a player folds, we increment it by 1
// * Anytime we pass a folded player, it is incremented by 1
// * This is because folded players need to be counted when determing when to stop the loop
// * Whenever a player raises the bets, it is set to 0
// * Calling does not increment playersChecked by 1
// * Players can check only when they logically can
// * Everytime it is incremented, it checks whether playersChecked == numPlayers.
//   This is because if it's true, then it's time to stop the loop that calls betting_turn in betting_phase.
//   We stop the loop by setting everyoneChecked to true.

// How playersFolded works:
// * Whenever a player folds, it is incremented by 1
// * When incrementing we check if playersFolded == numPlayers -1
// * If true, the loop stops by everyoneChecked = true

int Game::betting_phase(int startPlayer, bool & winnerDecided) {
	cout << "\nBegin betting phase" << endl;
	int numPlayers = playerPtr.size();
	bool everyoneChecked = false;
	int playersChecked = 0;
	int playersFolded = numFoldedPlayers;
	bool betPlaced = false;

	while (!everyoneChecked) {
		shared_ptr<Player> p = playerPtr.at(startPlayer);
		int check = betting_turn(*p, everyoneChecked, playersChecked, playersFolded, numPlayers, betPlaced);
		// catching and returning error code here
		// if everything is logically correct, this condition will never be executed
		if (check != 0) {
			return check;
		}
		startPlayer = (startPlayer + 1) % numPlayers;
	}
	// betting phase ends here.
	numFoldedPlayers = playersFolded;

	//  IF ALL BUT ONE FOLDED, DECLARE WINNER HERE
	winnerDecided = checkForWinner();
	// CREATE A FUNCTION TO DEAL WITH ALL THE WINNING THINGS
	// MOST THINGS WILL BE COPIED FROM SORT PLAYERS FUNCTION

	// setting player and global variables for next possible call to this function.
	int currentPlayer = (dealer + 1) % numPlayers;
	int i = 0;

	while (i != numPlayers) {
		shared_ptr<Player> p = playerPtr.at(currentPlayer);
		p->moneyInRound = 0; // because round has ended
		currentPlayer = (currentPlayer + 1) % numPlayers;
		i++;
	}

	callChips = 0;
	betPlaced = false;
	cout << "Betting phase ended..." << endl;
	return success;
}

int Game::betting_turn(Player &player, bool & everyoneChecked, int & playersChecked, int & playersFolded, int numPlayers, bool & betPlaced) {
	// folded players are also counted as checking
	// this is done so that the checking method works
	// even after players have folded
	cout << endl;
	cout << player.name << "'s turn" << endl;
	vector<Card> playerHand = player.hand.theHand;

	for (vector<Card>::iterator i = playerHand.begin(); i < playerHand.end(); ++i) {
		cout << i->get_card_def() << endl;
	}

	if (player.folded) {
		cout << player.name << "is already folded." << endl;
		playersChecked++;
		if (playersChecked == numPlayers) {
			everyoneChecked = true;
		}
		return success;
	}
	// if the player has 0 chips
	else if (player.poker_chips == 0) {
		cout << player.name << " has 0 chips. Nothing to do." << endl;
		playersChecked++;
		if (playersChecked == numPlayers) {
			everyoneChecked = true;
		}

		return success;
	}
	else if (callChips == player.moneyInRound && !betPlaced) {
		cout << "What would you like to do? (Choose 1-2)" << endl;
		cout << "1. Check" << endl;
		cout << "2. Bet(1-2)" << endl;
		char option = ' ';

		// loop exits only if valid option is chosen
		do {
			cin >> option;
			if (option != '1' && option != '2') {
				cout << "Invalid option. Try again" << endl;
			}
		} while (option != '1' && option != '2');

		// if player chooses to check
		if (option == '1') {
			playersChecked++;
			if (playersChecked == numPlayers) {
				everyoneChecked = true;
			}
			return success;
		}

		//if player chooses to raise
		else if (option == '2') {
			int raiseChips = 0;
			bool correctVal = false;

			do {
				cout << "How much would you like to bet?(1-2)" << endl;
				// loop keeps on going until player enters a correct raise value
				cin >> raiseChips;
				// if player bets something other than 1 or 2 chips or does not have enough chips
				if ((raiseChips != 1 && raiseChips != 2) || (raiseChips > player.poker_chips)) {
					// if player bets something other than 1 or 2
					if (raiseChips != 1 && raiseChips != 2) {
						cout << "Can only bet 1 or 2 chips" << endl;
					}
					// if player does not have enough chips
					else if (raiseChips > player.poker_chips) {
						cout << "Insufficient number of chips. Try raising a lower amount" << endl;
					}
				}
				// if player enters a correct value
				else {
					correctVal = true;
				}
			} while (!correctVal);

			//once we have a correct raise value
			player.poker_chips -= raiseChips;
			player.moneyInRound += raiseChips;
			pot += raiseChips;
			callChips += raiseChips;

			// round starts again after a raise
			playersChecked = 0;

			return success;
		}
	}
	else if (callChips == player.moneyInRound) {
		cout << player.name << "'s turn" << endl;
		cout << "What would you like to do? (Choose 1-3)" << endl;
		cout << "1. Fold" << endl;
		cout << "2. Check" << endl;
		cout << "3. Raise (1-2)" << endl;
		char option = ' ';

		// loop exits only if valid option is chosen
		do {
			cin >> option;
			if (option != '1' && option != '2' && option != '3') {
				cout << "Invalid option. Try again" << endl;
			}
		} while (option != '1' && option != '2' && option != '3');

		// if player chooses to fold
		if (option == '1') {
			playersChecked++;
			if (playersChecked == numPlayers) {
				everyoneChecked = true;
			}
			playersFolded++;
			if (playersFolded >= (numPlayers - 1)) {
				everyoneChecked = true;
			}
			player.folded = true;
			return success;
		}
		// if player chooses to check
		else if (option == '2') {
			playersChecked++;
			if (playersChecked == numPlayers) {
				everyoneChecked = true;
			}
			return success;
		}
		//if player chooses to raise
		else if (option == '3') {
			cout << "How much would you like to raise?(1-2)" << endl;
			int raiseChips = 0;
			bool correctVal = false;

			// loop keeps on going until player enters a correct raise value
			do {
				cout << "How much would you like to raise?(1-2)" << endl;
				// loop keeps on going until player enters a correct raise value
				cin >> raiseChips;

				// if player bets something other than 1 or 2 chips or does not have enough chips
				if ((raiseChips != 1 && raiseChips != 2) || (raiseChips > player.poker_chips)) {
					// if player bets something other than 1 or 2
					if (raiseChips != 1 && raiseChips != 2) {
						cout << "Can only bet 1 or 2 chips" << endl;
					}
					// if player does not have enough chips
					else if (raiseChips > player.poker_chips) {
						cout << "Insufficient number of chips. Try raising a lower amount" << endl;
					}
				}

				// if player enters a correct value
				else {
					correctVal = true;
				}
			} while (!correctVal);

			//once we have a correct raise value
			player.poker_chips -= raiseChips;
			player.moneyInRound += raiseChips;
			pot += raiseChips;
			callChips += raiseChips;

			// round starts again after a raise
			playersChecked = 0;

			return success;
		}
	}
	else if (callChips > player.moneyInRound && (player.poker_chips <= (callChips - player.moneyInRound))) {
		cout << "What would you like to do? (Choose 1-2)" << endl;
		cout << "1. Fold" << endl;
		cout << "2. All In" << endl;
		cout << "Note: All in rules are in accordance with the (1-2) betting only policy." << endl;
		char option = ' ';

		// loop exits only if valid option is chosen
		do {
			cin >> option;
			if (option != '1' && option != '2') {
				cout << "Invalid option. Try again" << endl;
			}
		} while (option != '1' && option != '2');

		// if player chooses to fold
		if (option == '1') {
			playersChecked++;
			if (playersChecked == numPlayers) {
				everyoneChecked = true;
			}
			playersFolded++;
			if (playersFolded == (numPlayers - 1)) {
				everyoneChecked = true;
			}
			player.folded = true;
			return success;
		}

		// if player chooses to go all in
		else if (option == '2') {
			pot += player.poker_chips;
			player.moneyInRound += player.poker_chips;
			player.poker_chips = 0;
			return success;
		}

		// no need to check for else statement as that is already being checked.
		// will not reach the statement below but for safety
		return success;
	}
	else if (callChips > player.moneyInRound) {
		cout << "What would you like to do? (Choose 1-3)" << endl;
		cout << "1. Fold" << endl;
		cout << "2. Call" << endl;
		cout << "3. Raise (1-2)" << endl;
		char option = ' ';

		// loop exits only if valid option is chosen
		do {
			cin >> option;
			if (option != '1' && option != '2' && option != '3') {
				cout << "Invalid option. Try again" << endl;
			}
		} while (option != '1' && option != '2' && option != '3');

		// if player chooses to fold
		if (option == '1') {
			playersChecked++;
			if (playersChecked == numPlayers) {
				everyoneChecked = true;
			}
			playersFolded++;
			if (playersFolded == (numPlayers - 1)) {
				everyoneChecked = true;
			}
			player.folded = true;
			return success;
		}
		// if player chooses to call
		else if (option == '2') {
			int chipsTransferred = callChips - player.moneyInRound;

			pot += chipsTransferred;
			player.poker_chips -= chipsTransferred;
			player.moneyInRound += chipsTransferred;
			playersChecked = 0;
			return success;
		}
		//if player chooses to raise
		else if (option == '3') {
			cout << "How much would you like to raise?" << endl;
			int raiseChips = 0;
			int chipsToMatchPreviousBet = callChips - player.moneyInRound;
			int totalChipsMoved = raiseChips + chipsToMatchPreviousBet;
			bool correctVal = false;

			// loop keeps on going until player enters a correct raise value
			do {
				cin >> raiseChips;
				// if player does not have enough chips to raise entered value
				if (totalChipsMoved > player.poker_chips) {
					cout << "Insufficient number of chips. Try raising a lower amount (0 works too)." << endl;
				}
				// if player enters negative value for raiseChips
				else if (raiseChips < 0) {
					cout << "Trying to be smart, huh? Can't raise negative value. Try again." << endl;
				}
				// if player enters a correct value
				else {
					correctVal = true;
				}
			} while (!correctVal);

			//once we have a correct raise value
			player.poker_chips -= totalChipsMoved;
			player.moneyInRound += totalChipsMoved;
			pot += totalChipsMoved;
			callChips += raiseChips;

			// round starts again after a raise
			playersChecked = 0;

			return success;
		}
	}
	else {
		cout << "Something went wrong. callChips cannot be less than player's moneyInRound." << endl;
		return lessCallingChipsError;
	}

	// will not reach the statement below but for safety
	return success;
}

int Game::addPlayer()
{
	char option = '\0';

	do
	{
		cout << "Do you want to add a player? (Y/N)" << endl;
		cin >> option;
		option = toupper(option);

		if (option == 'Y')
		{
			try
			{
				cout << "Enter player name: ";
				string playerName = "";
				cin >> playerName;
				Game::add_player(playerName);
			}
			catch (errorCodes c)
			{
				cout << "Player already exists! Try again" << endl;
			}
		}
		else if (option == 'N')
		{
			cout << "Okay! No player added" << endl;
		}
		else
		{
			cout << "Invalid option! Try again" << endl;
		}

	} while (option != 'N');

	return success;
}

int Game::removePlayer()
{
	char option = '\0';

	do
	{
		cout << "Do you want to remove a player? (Y/N)" << endl;
		cout << "> ";
		cin >> option;
		cout << endl;
		option = toupper(option);

		if (option == 'Y')
		{
			cout << "Enter player name > ";
			string playerName = "";
			cin >> playerName;
			cout << endl;

			int numPlayers = playerPtr.size();

			int i = 0;
			int checkPlayer = 0;

			while (i != numPlayers) {
				if (playerPtr.at(i)->name == playerName) {
					ofstream ofile(playerName);
					ofile << **(playerPtr.begin() + i);
					playerPtr.erase(playerPtr.begin() + i);
					ofile.close();
					checkPlayer = 1;
					break;
				}
				i++;
			}

			if (checkPlayer == 0) {
				cout << "Player not found! Try again" << endl;
			}
			else {
				cout << "Player found and removed!" << endl;
			}
		}
		else if (option == 'N')
		{
			cout << "Okay! No player removed" << endl;
		}
		else
		{
			cout << "Invalid option! Try again" << endl;
		}

	} while (option != 'N');

	return success;
}


int Game::moveToMainDeck()
{
	// move all playerHands to the mainDeck
	for (shared_ptr<Player> p : playerPtr) {
		while (p->hand.size() != 0) {
			mainDeck.add_card(p->hand.theHand.back());
			p->hand.theHand.pop_back();
		}
	}

	// move cards from discardDeck to mainDeck
	while (discardDeck.size() != 0)
	{
		mainDeck.add_card(discardDeck.deal_card());
	}

	return success;
}

void Game::resetChips(Player & player) {
	int isValid = 0;
	int option = 0;
	int i = 0;
	int numPlayers = playerPtr.size();
	if (player.poker_chips == 0) {
		do {
			cout << player.name << " ran out of chips. What do you want to do? (Select option)" << endl;
			cout << "1: Reset poker chip count back to 20" << endl;
			cout << "2: Quit game" << endl;
			cin >> option;
			cout << endl;

			if (option == 1) {
				player.poker_chips = 20;
				cout << "Poker chips restored! " << player.name << "'s poker chip count is " << player.poker_chips << endl;
				isValid = 1;
			}
			else if (option == 2) {
				while (i != numPlayers) {
					if (playerPtr.at(i)->name == player.name) {
						ofstream ofile(player.name);
						ofile << **(playerPtr.begin() + i);
						playerPtr.erase(playerPtr.begin() + i);
						ofile.close();
						break;
					}
					i++;
				}
				cout << player.name << " removed! "<< endl ;
					isValid = 1;
			}
			else {
				isValid = 0;
				cout << "Wrong choice. Try again.\n" << endl;
			}
		} while (isValid == 0);
	}
}

int Game::sortPlayers(vector<Player> players)
{
	sort(players.begin(), players.end(), player_rank);

	shared_ptr<Player> thePlayer = find_player(players.begin()->name);

	cout << "Winner of this round is " << thePlayer->name << "\n" << endl;

	thePlayer->hands_won += 1;
	thePlayer->poker_chips += pot;

	players.begin()->hands_won += 1;
	players.begin()->poker_chips += pot;
	pot = 0;

	for (vector<Player>::iterator i = (players.begin() + 1); i < players.end(); ++i) {
		shared_ptr<Player> loserPlayer = find_player(i->name);
		loserPlayer->hands_lost += 1;
		i->hands_lost += 1;
	}

	for (vector<Player>::iterator i = players.begin(); i < players.end(); ++i) {
		cout << i->name << " won " << i->hands_won << " hands and lost " << i->hands_lost << " hands. " << i->poker_chips << " poker chips." << endl;

		for (Card card : i->hand.theHand) {
			cout << card.get_card_def() << " ";
		}
		cout << "\n" << endl;
	}

	return success;
}

bool Game::player_rank(const Player & p1, const Player & p2) {
	if (&p1 == nullptr) {
		return false;
	}
	else if (&p2 == nullptr) {
		return true;
	}
	else {
		Hand hand1 = p1.hand;
		Hand hand2 = p2.hand;

		return poker_rank(hand1, hand2);
	}
}

void Game::announceWinner()
{
	vector<Player> playersCopy;
	if (!gotAWinner) {
		for (shared_ptr<Player> p : playerPtr) {
			if (p->folded) {
				p->hands_lost += 1;
			}
			else {
				playersCopy.push_back(*p);
			}
		}

		// calling the sorting and printing info function
		sortPlayers(playersCopy);
	}
}

void Game::resetGame()
{
	gotAWinner = false;

	// setting all players' folded to false as a new round is starting
	for (vector<shared_ptr<Player>>::iterator k = playerPtr.begin(); k < playerPtr.end(); ++k) {
		shared_ptr<Player> anyPlayer = *k;
		anyPlayer->folded = false;
	}
	numFoldedPlayers = 0;

	// moving all cards to main deck
	moveToMainDeck();

	int i = 0;
	int numPlayers = playerPtr.size();
	int currentPlayer = (dealer + 1) % numPlayers;
	// ask player who has 0 chips if they want to reset chips or quit the game
	while (i != numPlayers) {
		shared_ptr<Player> p = playerPtr.at(currentPlayer);
		resetChips(*p);
		currentPlayer = (currentPlayer + 1) % numPlayers;
		i++;
	}
	// ask if anyone wants to leave the game
	removePlayer();

	// ask if anyone wants to join the game
	addPlayer();

	// move dealer to next player position
	dealer = (dealer + 1) % numPlayers;

	// all functions detect and ignore the invalid inputs
	// therefore, all of them return 0 always.
	// We do not need to check.
}

Hand Game::getBestHandOfFive(Hand& h)
{
	vector<Hand> allPossibleHands = h.combination(5);
	sort(allPossibleHands.begin(), allPossibleHands.end(), poker_rank);
	return allPossibleHands[0];
}

