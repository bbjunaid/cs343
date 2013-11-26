#include "watcard.h"

WATCard::WATCard( const WATCard & ) {            // prevent copying
}

/*WATCard& WATCard::operator=( const WATCard & ) {
}*/


WATCard::WATCard() : m_balance(0) { }

void WATCard::deposit( unsigned int amount ) {
    m_balance += amount;
}

void WATCard::withdraw( unsigned int amount ) {
    m_balance -= amount;
}

unsigned int WATCard::getBalance() {
    return m_balance;
}

