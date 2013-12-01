#ifndef _PRINTER_H__
#define _PRINTER_H__

#include <vector>

_Monitor Printer {
  public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

  private:
    struct buffer {
        char state;
        Kind kind;
        int value1;
        int value2;
    };

    void flush();
    void flushFinished( unsigned int id );
    void clear();
    unsigned int getGlobalId( Kind kind );
    unsigned int getGlobalId( Kind kind, unsigned int lid );
    unsigned int m_numStudents;
    unsigned int m_numVendingMachines;
    unsigned int m_numCouriers;
    const unsigned int NUM_TASK_TOTAL;
    bool m_isBufferEmpty;
    unsigned int m_finishedCnt;
    std::vector<struct buffer> m_buffer;
};

#endif // _PRINTER_H__

