/* 
    Driver for the hot potato game. The game starts by creating a Player
    list and passing it to an Umpire object. Then Umpire.set() is called.
    This starts the game by resetting a counter on a Potato object,
    and tossing the Potato to different Players in the list and decrementing
    the counter. Once the counter goes to 0, the Player holding the Potato
    is removed from the game/Player list, the potato counter is reset, and 
    the game starts again. 

    Command syntax:
    ./hotpotato players (>= 2 && <= 20) [ seed (>=0) ] 

    Examples:
    ./hotpotato 10 6
    ./hotpotato 8
*/


#include <uC++.h>
#include <iostream>
#include <cstdlib> // for EXIT
#include "q1player.h" // Player coroutine
#include "q1umpire.h" // Umpire coroutine

using namespace std;

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0]
	 << " players " 
	 << " [ seed ]" << endl
	 << " players [2-20], seed >= 0" << endl;
    exit( EXIT_FAILURE );				// TERMINATE
} // usage

void uMain::main() {
	int seed = getpid();
	int numPlayers;
	
    // Error for extra arguments
	if ( argc > 3 ) {
		cerr << "Too many arguments" << endl;
		usage( argv );
	}
	
    switch ( argc ) {
	  case 3:                       // verify seed >= 0
		seed = atoi(argv[2]);
		if ( seed < 0 ) {
			cerr << "Incorrect input for seed." << endl;
			usage( argv );
		}
        // FALL THROUGH
      case 2:                       // verify players >= 2 && <= 20
        numPlayers = atoi( argv[1] );
		if ( numPlayers < 2 || numPlayers > 20 )  {
			cerr << "Incorrect input for number of players." << endl;
			usage( argv );
		}
        break;
      default:						// wrong number of options
		usage( argv );
    } // switch

	srand( seed );	
	Player::PlayerList players( numPlayers );	// vector of pointers to Player objects
	Umpire umpire( players );
	
	for ( int id = 0; id < numPlayers; ++id)
		players[id] = new Player( umpire, id, players );

    Player::PlayerList unmodifiedPlayers( players ) ; // vector of pointers to players 
                                                      // used for deletion after game ends

	cout << numPlayers << " players in the match" << endl;
	umpire.set( 0 ); // start the game
   
    // delete all players 
	for ( int id = 0; id < numPlayers; ++id)
		delete unmodifiedPlayers[id];

}// uMain::main

// Local Variables:
// compile-command: "u++ -g -o telephone player.C driver.C"
// End:
