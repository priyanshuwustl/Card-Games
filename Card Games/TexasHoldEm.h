#pragma once
#include "Deck.h"
#include "Card.h"
#include "Player.h"
#include "Game.h"
#include "hand.h"
#include "PokerHandEvaluator.h"

class TexasHoldEm : public Game
{
public:
	//done
	TexasHoldEm();
	virtual int before_round();
	virtual int round();
	virtual int after_round();
	virtual int before_turn(Player &player);
	virtual int turn(Player &player);
	virtual int after_turn(Player &player);

private:
	vector<Card> communalCards;
	void showdown();
};
