#include "watcard.h"

WATCard::WATCard() : m_balance(0) { }

// The courier call deposit after a funds transfer
void WATCard::deposit( unsigned int amount ) {
    m_balance += amount;
}

// A vending machine calls withdraw when a soda is purchased
void WATCard::withdraw( unsigned int amount ) {
    m_balance -= amount;
}

// A student and a vending machine call to determine the balance
unsigned int WATCard::getBalance() {
    return m_balance;
}

