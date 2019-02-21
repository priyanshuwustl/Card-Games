NAME: Priyanshu Kumar Jain, Glizela Taino, Jackie Wong
EMAIL: priyanshu@wustl.edu, gtaino@wustl.edu, jackie.wong@wustl.edu
STUDENT ID: 451744, 452163, 450740
LAB : lab4
//////////////////////////////////////////////////////////////////////////////////////////////////////

1. ReadMe.txt -> this file

2. Card.h -> Card class declaration with member declarations

3. Card.cpp -> Card class implementation

4. Deck.h -> Deck class declaration with member declarations

5. Deck.cpp -> Deck class implementation

6. hand.h -> Hand class declaration with member declarations

7. hand.cpp -> Hand class implementation

8. Game.h -> Game class declaration with member declarations

9. Game.cpp -> Game class implementation

10. FiveCardDraw.h -> FiveCardDraw class declaration with member declarations

11. FiveCardDraw.cpp -> FiveCardDraw class implementation

12. SevenCardStud.h -> SevenCardStud class declaration with member declarations

13. SevenCardStud.cpp -> SevenCardStud class implementation

14. Parser.h -> Declarations for a simple parse, print, checks validity, and usage message for a card creator program.

15. Parser.cpp -> Definitions for a simple parse, print, checks validity, and usage message for a card creator program.

16. PokerHandEvaluator.h -> holds declarations of functions that are used to evaluate and sort poker hands.

17. PokerHandEvaluator.cpp -> holds definitions of functions that are used to evaluate and sort poker hands.

18. Lab4.cpp -> main function implementation

//////////////////////////////////////////////////////////////////////////////////////////////////////

Return Codes:

enum Result_Code { success=0, too_few_args, too_many_args, no_file_name, no_shuffle, file_not_found }

1. success = 0 -> default success code

2. too_few_args = 1 -> when number of arguments are less than 3

3. too_many_args = 2 -> from previous implementation, not triggered in this lab

4. no_file_name = 3 -> from previous implementation, not triggered in this lab

5. no_shuffle = 4 -> from previous implementation, not triggered in this lab

6. file_not_found = 5 -> from previous implementation, not triggered in this lab

enum errorCodes { instance_not_available = 6, game_already_started, unknown_game, no_game_in_progress, already_playing };

1. instance_not_available -> there isn't an instance of the game that exists

2. game_already_started -> occurs if the program tries to start another game when a game is already in progress

3. unknown_game -> the game that is passed by the user isn't know by the program 

4. no_game_in_progress -> occurs when some method is called and no game has been started

5. already_playing -> occurs if a player is already playing

//////////////////////////////////////////////////////////////////////////////////////////////////////

Errors Encountered:

1. Error: Abort error out of bounds
	Solution: before we didn't have a try catch for adding a player. Now it will return the error that the player is already playing
	if a user tries to put in the same name for the user. 

2. Error: debug abort error
	Solution: Accidentally called stop game in the wrong location in main. Removed stop game and the program worked as 
	expected. 

3. warning C4101: 'c': unreferenced local variable
	Solution: Ignore. c is used as formal parameters for a catch block.

4. warning c4018: ' signed/unsigned mismatch' 
	Solution: Ignore. It is fine because we are not dealing with negative numbers so signed/unsigned doesn't make a difference

//////////////////////////////////////////////////////////////////////////////////////////////////////

Test Cases:

1. We tested a case where we would enter the name of the program, some name that is not "FiveCardDraw" and "SevenCardStud" and two names.
Result: This resulted in the return of our usage message, our purpose of the game, and an error message saying that the game is unknown. 

2. We also tested what would happen if we only had the correct program name, game name, and either only one player or no players.
Result: This resulted in the return of our usage message, our purpose of the game, and an error message saying that there were too few arguments.

3. We also tested what would happen if you tried to add two players in the game with the same name as the first input at the start of the program. 
Result: This will return the reason that there is already a player added with the same name. It will also prompt the player to 
either join another game our quit the program

4. In the before_round stage, we tested trying to remove a player whose name was not one of the existing players.
Result: This resulted in a message that said "player not found".

5. We delete players so that there is only one player left in the game.
Result: The game will automatically quit and return a game exiting message.

6. We tried to add a player with the same name as an existing player after a round. 
Result: This returned the error that there is already a player in the game and it quits out of the game. 

7. We tested an instance of the game with the correct program name, FiveCardDraw, and with two players. In this case, we didn't discard any cards.

Result: This resulted in none of the cards being discarded in the hands and the correct winner of that round based on the existing hands to print out as well as the current score and number of chips. 

8. Testing: Then we added an additional player and tested the no cards discarded. This again prompted no cards to be removed from any of the player's hands. 

Result: It also resulted in the winner to be printed out as well as the tallies of the scores that are appropriate based on who just joined. 
(i.e. we had Zela and Jackie as the first two players, and their scores are based on the two rounds, while the new player, Priyanshu, has his tallied score based on the one round that he just joined in on).

8. We tested an instance of the game with the correct program name, SevenCardStud, and with two players. In this case, we didn't discard any cards.

Result: This resulted in none of the cards being discarded in the hands and the correct winner of that round based on the existing hands to print out as well as the current score and number of chips. 

9. Testing: Then we added an additional player and tested the no cards discarded. This again prompted no cards to be removed from any of the player's hands. 

Result: It also resulted in the winner to be printed out as well as the tallies of the scores that are appropriate based on who just joined. 
(i.e. we had Zela and Jackie as the first two players, and their scores are based on the two rounds, while the new player, Priyanshu, has his tallied score based on the one round that he just joined in on).

10. We also tested the program with multiple players (up to 8) and received expected results.

//////////////////////////////////////////////////////////////////////////////////////////////////////

Design Choices:

1. There were a lot of similar functions that would be applicable to FiveCardDraw and SevenCardStud, so we added a lot of the same functions 
like ante, resetGame, announceWinner, betting_phase and betting_turn into Game. We also added variables that we would need for both games such as
checkForWinner, getNumPlayers, pot and callChips. By doing so, we were able to improve the modularity of our code. 

2. One of our design choices is in the way we ask the user to interact with the game. 
If someone raises and the other player calls it, then we still ask both of them to check again before we go onto the next round. 


//////////////////////////////////////////////////////////////////////////////////////////////////////

EXTRA CREDIT: 

We implemented texas hold em and it is played in a similar way to FiveCardDraw and SevenCardStud