#pragma once

#include "Game.h"

class SevenCardStud: public Game
{
protected:

public:
	SevenCardStud();
	virtual int before_round();
	virtual int round();
	virtual int after_round();
	virtual int before_turn(Player &player);
	virtual int turn(Player &player);
	virtual int after_turn(Player &player);

private:
	void showdown();
	Hand getBestHandOfFive(Hand& h);
	int get_player_with_lowest_card();
	int get_player_with_highest_card();
};