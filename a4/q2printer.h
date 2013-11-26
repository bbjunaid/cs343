/* 
    Interface for the Printer Monitor
*/

#ifndef _PRINTER
#define _PRINTER

#include <uC++.h>

_Monitor Printer {
  public:
    enum States { Starting = 'S', Blocked = 'B', Unblocked = 'U', Finished = 'F', // general
        Napping = 'N', Awake = 'A',                        // Santa
        Working = 'W', NeedHelp = 'H',                     // elf
        OnVacation = 'V', CheckingIn = 'I',                // reindeer
        DeliveringToys = 'D', DoneDelivering = 'd',        // Santa, reindeer
        Consulting = 'C', DoneConsulting = 'c',            // Santa, elf
        ConsultingFailed = 'X',                            // elf
    }; 
    Printer( const unsigned int MAX_NUM_ELVES );
    ~Printer() { 
        delete m_states; 
        delete m_stateNumbers;
    }
    void print( unsigned int id, States state );    // update state and print if change in buffer
    void print( unsigned int id, States state, unsigned int numBlocked ); // update state and number 
                                                                          // for B/U states
    void finish( unsigned int id );                 // finish flush message
    void flush();                                   // print the states in a line

  private:
    const unsigned int m_numElves;      // number of elves in the workshop
    const unsigned int m_numStates;     // number of columns in output
    char* m_states;                     // character array to store state
    int* m_stateNumbers;                // associated numbers with Blocked, Unblocked states
    bool m_isEmpty;                     // is empty buffer
};

#endif // _PRINTER

