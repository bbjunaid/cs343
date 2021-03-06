#include <uC++.h>

_Coroutine Player;
_Coroutine Umpire;

class Potato {
  public:
    Potato( unsigned int maxTicks = 10 );
    void reset( unsigned int maxTicks = 10 );
    bool countdown();
};

_Coroutine Player {
    void main();
  public:
    typedef int PlayerList; // container type of your choice
    Player( Umpire &umpire, unsigned int Id, PlayerList &players );
    unsigned int getId();
    void toss( Potato &potato );
};

_Coroutine Umpire {
    void main();
  public:
    Umpire( Player::PlayerList &players );
    void set( unsigned int player );
};

void uMain::main() {
}
