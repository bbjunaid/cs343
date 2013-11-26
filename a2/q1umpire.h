/* 
    Interface for the Umpire coroutine
*/

#ifndef _UMPIRE
#define _UMPIRE

#include "q1potato.h" // Potato object
#include "q1player.h" // Player coroutine defition
#include <vector>

_Coroutine Umpire {
    void main(); // cortouine main
  public:
    // Initialization of member variables and allocation of playerIndices. 
    Umpire( Player::PlayerList &players )
        : m_players( players )
        , m_currPlayer( 0 )
        , m_totalSize( m_players.size() )
        , m_playerIndices( new int[m_totalSize] )
    { 
        for (unsigned int i = 0; i < m_players.size(); i++)
            m_playerIndices[i] = i;    
    }	
    ~Umpire() { delete m_playerIndices; } // delete playerIndices
    void set( unsigned int player ); // deletes the player specified by input
  private:
	Player::PlayerList &m_players;  // list of players	
	unsigned int m_currPlayer;      // player to be eliminated
    unsigned int m_totalSize;       // number of players when the game starts
	Potato m_potato;                // Potato object which is reset and tossed by Umpire
    int* m_playerIndices;           // maps the player id to its index position in
                                    // the m_players list
};

#endif // _UMPIRE
