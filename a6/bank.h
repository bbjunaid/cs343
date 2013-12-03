//-----------------------------------------------------------------------------
// Header for the definition of Bank monitor
//-----------------------------------------------------------------------------

#ifndef _BANK_H__
#define _BANK_H__

#include <vector>

_Monitor Bank {
  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );

  private:
    unsigned int m_numStudents;             // number of students
    uCondition* courier;                    // couriers block on student accounts if not enough
                                            // funds
    std::vector<unsigned int> m_accounts;   // balances of the students
};

#endif // _BANK_H__

