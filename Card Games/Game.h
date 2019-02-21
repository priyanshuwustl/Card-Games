//	Game.h holds functions that will start a poker game, stop the game, 
//	add a player to the game, and find a particular player.
//	It also includes virtual functions that will print out a players hand
// before a turn, after a turn, before a round and after a round. 
//	We used shared_ptrs instead of the native C++ pointer
//
//	author: Priyanshu Jain priyanshu@wustl.edu

#pragma once
#include <memory>
#include "Deck.h"
#include "Player.h"
#include "Hand.h"

using namespace std;

//these error codes are for throwing exceptions in the Game.cpp file
enum errorCodes { instance_not_available = 6, game_already_started, unknown_game, no_game_in_progress, already_playing };
class Game {
public:
	static shared_ptr<Game> instance();
	static void start_game(const string& input);
	static void stop_game();
	void add_player(const string& name);
	shared_ptr<Player> find_player(const string& queryName);
	int get_num_player();
	void ante(Player &player);
	void resetChips(Player & player);

	virtual int before_turn(Player &player) = 0;
	virtual int turn(Player &player) = 0;
	virtual int after_turn(Player &player) = 0;
	virtual int before_round() = 0;
	virtual int round() = 0;
	virtual int after_round() = 0;

	bool checkForWinner();
	int betting_phase(int startPlayer, bool & winnerDecided);
	int betting_turn(Player &player, bool & everyoneChecked, int & playersChecked, int & playersFolded, int numPlayers, bool & betsPlaced);


protected:
	static shared_ptr<Game> gamePtr;
	Deck mainDeck;
	vector<shared_ptr<Player>> playerPtr;
	int addPlayer();
	int removePlayer();
	size_t dealer = 0;
	Deck discardDeck;
	unsigned int pot = 0;
	unsigned int callChips = 0;
	int numFoldedPlayers = 0;
	int sortPlayers(vector<Player> players);
	static bool player_rank(const Player  & p1, const Player  & p2);
	int moveToMainDeck();
	bool gotAWinner = false;
	void resetGame();
	void announceWinner();
	Hand getBestHandOfFive(Hand& h);
};


