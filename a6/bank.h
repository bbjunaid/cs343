//-----------------------------------------------------------------------------
// Header for the definition of Bank monitor
//-----------------------------------------------------------------------------

#ifndef _BANK_H__
#define _BANK_H__

#include <vector>

_Monitor Bank {
    unsigned int m_numStudents;
    std::vector<unsigned int> m_accounts;
  public:
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif // _BANK_H__

