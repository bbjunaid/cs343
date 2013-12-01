#include "bank.h"

Bank::Bank( unsigned int numStudents ) 
: m_numStudents(numStudents) {
    m_accounts.resize(numStudents);
    courier = new uCondition[m_numStudents];
}

Bank::~Bank() {
   delete[] courier; 
}

// The parent calls deposit to endow gifts to a specific student
void Bank::deposit( unsigned int id, unsigned int amount ) {
    m_accounts[id] += amount;

    if ( !courier[id].empty() ) {
        courier[id].signal();
    }
}

// The courier waits until enough money has been deposited which may require multiple deposits
void Bank::withdraw( unsigned int id, unsigned int amount ) {
    for ( ; amount > m_accounts[id] ; ) {   // in a loop because one deposit might not satisfy amount
        courier[id].wait();
        // In FIFO order wake up the next courier to check if withdrawal is possible
        if ( !courier[id].empty() ) {
            courier[id].signal();
        }
    }
    m_accounts[id] -= amount;
}

