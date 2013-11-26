#include "q1potato.h"
#include "q1player.h" // Player couroutine definition
#include "q1umpire.h" // Umpiure coroutine definiton
#include <iostream>

using namespace std;

/* 
    This function checks if the Player has been eliminated by calling
    Potato::countdown() which returns true if the counter reaches 0.    
    If not, it prints out the appropriate 'tossing' string and then 
    generates a player id other than itself. When such an id is generated, the 
    Potato is tossed to this next player.

    When the Player is eliminated, it calls Umpire::set, passing its 
    player id as an input which tells the Umpire to remove this player from
    the list and continue the game if possible.
*/
void Player::main() {
    unsigned int nextPlayer;
    
    // while not eliminated
	while ( !m_potato->countdown() ) {
	    cout << m_id << " -> ";
        // toss to next unique player
        do { 
        	nextPlayer = rand() % m_players.size();
        } while ( m_players[nextPlayer]->getId() == m_id );
        m_players[nextPlayer]->toss( *m_potato );
	}

    // eliminate the Player by passing m_id to Umpire
	cout << m_id << " is eliminated" << endl;
	m_umpire.set( m_id );
}

/* 
    Constructor taking in an Umpire, Id, and player list from driver
    program.
*/
Player::Player( Umpire &umpire, unsigned int Id, PlayerList &players )
    : m_umpire( umpire )
    , m_id( Id )
    , m_players( players )
{ }

/* 
    Stores the Potato object as a member variable and then starts the coroutine.
    If the coroutine is already started, then it continues whereever it left off
    in Player::main().
*/
void Player::toss( Potato &potato ) {
    m_potato = &potato;
    resume(); 
}
