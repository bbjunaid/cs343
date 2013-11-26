//-----------------------------------------------------------------------------
// Header for the definition of a WATCard class
//-----------------------------------------------------------------------------

#ifndef _WATCARD_H__
#define _WATCARD_H__

#include "uFuture.h"

class WATCard {
    unsigned int m_balance;                              // Watcard balance 
    WATCard( const WATCard & );            // prevent copying
    WATCard &operator=( const WATCard & );
  public:
    WATCard();
    typedef Future_ISM<WATCard *> FWATCard; // future watcard pointer
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

#endif // _WATCARD_H__

