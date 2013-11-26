/* 
    Interface for the Player coroutine
*/

#ifndef _PLAYER
#define _PLAYER

#include <vector>
#include "q1potato.h" // Potato object

_Coroutine Umpire; // forward declare Umpire

_Coroutine Player {
    void main(); // coroutine main
  public:
    typedef std::vector<Player*> PlayerList; // container type of your choice
    Player( Umpire &umpire, unsigned int Id, PlayerList &players );
    unsigned int getId() { return m_id; } // returns id of player
    void toss( Potato &potato ); // toss potato to randomly selected player
  private:
	Umpire &m_umpire;           // used for calling Umpire::set() when player loses
	unsigned int m_id;          // unqiue player id
	PlayerList &m_players;      // list of players to potentially toss Potato to
    Potato* m_potato;           // Potato object which is tossed around
};

#endif // _PLAYER
