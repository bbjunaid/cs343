//-----------------------------------------------------------------------------
// Header for the definition of a Student task
//-----------------------------------------------------------------------------

#ifndef _STUDENT_H__
#define _STUDENT_H__

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task VendingMachine;

_Task Student {
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, \
             unsigned int maxPurchases );

  private:
    Printer &m_prt;
    NameServer &m_nameServer;
    WATCardOffice &m_cardOffice;
    unsigned int m_id;
    unsigned int m_maxPurchases;
    // The vending machine assigned by nameServer that purchases can be made from
    VendingMachine* m_vendingMachine;   

};

#endif // _STUDENT_H__

