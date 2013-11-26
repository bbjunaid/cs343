#ifndef _PARENT_H__
#define _PARENT_H__

_Monitor Printer;
_Monitor Bank;

_Task Parent {
    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();

  private:
    Printer &m_prt;
    Bank &m_bank;
    unsigned int m_numStudents;
    unsigned int m_parentalDelay;
};

#endif // _PARENT_H__

