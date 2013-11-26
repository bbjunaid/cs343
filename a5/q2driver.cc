#include <uC++.h>
#include <iostream>
#include <cstdlib> // for EXIT

#include "MPRNG.h"          // random number generator
#include "q2printer.h"
#include "q2table.h"
#include "q2phil.h"

using namespace std;

MPRNG mprng; // global random number generator used in other files

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0]
         << " [ P (> 1) [ N (> 0) [ S (> 0) ] ] ]" 
         << endl;
    exit( EXIT_FAILURE );	    // TERMINATE
} // usage

void uMain::main() {
    int numPhil, numNoodles, seed;

    // default values
    numPhil = 5;        
    numNoodles = 30;
    seed = getpid();

    // Error for extra arguments
    if ( argc > 4 ) {
        usage( argv );
    }

    switch ( argc ) {
    case 4:                     // verify seed > 0
        seed = atoi(argv[3]);
        if ( seed < 1 ) {
        usage( argv );
        }
        // FALL THROUGH
    case 3:                     // verify numNoodles > 0
        numNoodles = atoi( argv[2] ); 
        if ( numNoodles < 1 ) {
            usage( argv ); 
        } 
        // FALL THROUGH
    case 2:                     // verify numPhil > 1
        numPhil = atoi( argv[1] );
        if ( numPhil < 2 )  {
            usage( argv );
        }
        break;
    case 1:                     // no arguments, that's ok
        break;
    default:                    // wrong number of options
        usage( argv );
    } // switch 

    mprng.seed( seed );
    Printer printer( numPhil );
    Table table( numPhil, printer );
    Philosopher *phils[numPhil];

    // Create philosophers
    for ( int i = 0; i < numPhil; i++ )
        phils[i] = new Philosopher( i, numNoodles, table, printer );

    // Wait for philosophers to finish
    for ( int i = 0; i < numPhil; i++ )
        delete phils[i];
    
    // Finish message
    cout  << "***********************" << endl
          << "Philosophers terminated" << endl;
}
