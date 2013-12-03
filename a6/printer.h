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
        char state;         // state character
        Kind kind;          // kind of task
        int value1;         // value1 of print information, will be printed if needed
        int value2;         // value2 of print information, will be printed if needed
    };

    void flush();      
    void flushFinished( unsigned int id );
    void clear();
    unsigned int getGlobalId( Kind kind );  // return task id (out of all tasks) based on kind only
    unsigned int getGlobalId( Kind kind, unsigned int lid );    // return task id (out of all tasks) based on kind and lid
    unsigned int m_numStudents;
    unsigned int m_numVendingMachines;
    unsigned int m_numCouriers;
    const unsigned int NUM_TASK_TOTAL;
    bool m_isBufferEmpty;                   // Indicates if buffer is holding any meaningful states
    unsigned int m_finishedCnt;             // Counter for number of tasks finished
    std::vector<struct buffer> m_buffer;    // buffer state
};

#endif // _PRINTER_H__

