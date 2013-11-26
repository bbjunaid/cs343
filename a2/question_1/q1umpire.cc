#include "q1potato.h"
#include "q1umpire.h"
#include "q1player.h"
#include <iostream>

using namespace std;

/* 
    This function executes while more than 1 player remains in the game. 
    In its initial execution, it resets the Potato counter and selects a random player
    to toss the potato to. The toss function then goes onto the Player stack
    where the game is played out. When the set finishes, Umpire::set is called
    by the losing player and the losing player id is stored. This continues 
    Umpire::main past the toss call where the player is removed from the list.

    The indices are adjusted appropraitely at end of each iteration in m_playerIndices.
    When main finishes, an appropriate winner message is printed.
*/
void Umpire::main() {
    int set = 1;
    int next;
    // while there are players in the game
    while ( m_players.size() > 1 ) {
    	m_potato.reset(); 
    	next = randr( 0, m_players.size() );
    	cout << "Set " << set << ":  U -> ";
    	m_players[next]->toss( m_potato ); // start the set
       	m_players.erase( m_players.begin() + m_playerIndices[m_currPlayer] );
    	set++;
        // update indices which map player id to its index in m_players
        for (unsigned int i = m_currPlayer ; i < m_totalSize; i++)
            m_playerIndices[i]--;
    }
    cout << m_players[0]->getId() << " wins the Match!" << endl;
}

/* 
    This function is passed the player to remove. It assigns its input into a
    member variable and then starts coroutine main. 

    On the first call, it starts the main and the game is started and input player
    is not deleted. On subsequent calls, it continues in main() where the player 
    is deleted and the new set is started.
*/
void Umpire::set( unsigned int player ) {
    m_currPlayer = player;
	resume();	
}
