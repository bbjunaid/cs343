#include "bank.h"

Bank::Bank( unsigned int numStudents ) : m_numStudents(numStudents) {
    // TODO create a vector unsigned ints for students
    m_accounts.resize(numStudents);
}

// The parent calls deposit to endow gifts to a specific student
void Bank::deposit( unsigned int id, unsigned int amount ) {
    m_accounts[id] += amount;
}

// The courier waits until enough money has been deposited which may require multiple deposits
void Bank::withdraw( unsigned int id, unsigned int amount ) {
    for ( ; amount > m_accounts[id] ; ) {   // in a loop because one deposit might not satisfy amount
        _Accept( deposit );
    }
    m_accounts[id] -= amount;
}

