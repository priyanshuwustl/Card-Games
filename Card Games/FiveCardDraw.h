#pragma once

#include "Deck.h"
#include "Card.h"
#include "Player.h"
#include "Game.h"
#include "hand.h"
#include "PokerHandEvaluator.h"

class FiveCardDraw : public Game
{
public:
	//done
	FiveCardDraw();

	//done
	virtual int before_turn(Player &player);

	//done
	virtual int turn(Player &player);

	//done
	virtual int after_turn(Player &player);

	//done
	virtual int before_round();

	//done
	virtual int round();

	//done
	virtual int after_round();

};
