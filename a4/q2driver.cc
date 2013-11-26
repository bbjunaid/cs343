#include <uC++.h>
#include <iostream>
#include <cstdlib> // for EXIT

#include "MPRNG.h"          // random number generator
#include "q2workshop.h"     
#include "q2reindeer.h"
#include "q2elf.h"
#include "q2printer.h"
#include "q2santa.h"

using namespace std;

MPRNG mprng; // global random number generator used in other files

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0]
         << " [ N (> 0) [ E (> 0) [ Seed (> 0) [ C (>= 0) [ D (>= 0) ] ] ] ] ]" 
         << endl;
    exit( EXIT_FAILURE );				// TERMINATE
} // usage

void uMain::main() {
    int rBound, numElves, seed, numConsultations, numDeliveries;
    rBound = numElves = numConsultations = numDeliveries = 3; // default value
    seed = getpid();

    // Error for extra arguments
	if ( argc > 6 ) {
		usage( argv );
	}
	
    switch ( argc ) {
	  case 6:                       // verify numDeliveries >= 0
		numDeliveries = atoi(argv[5]);
		if ( numDeliveries < 0 ) {
			usage( argv );
		}
        // FALL THROUGH
      case 5:                       // verify numConsultations >= 0
        numConsultations = atoi( argv[4] ); 
        if ( numConsultations < 0 ) {
            usage( argv ); 
        } 
        // FALL THROUGH
      case 4:                       // verify seed > 0
        seed = atoi( argv[3] );
		if ( seed < 1 )  {
			usage( argv );
		}
        // FALL THROUGH
      case 3:                       // verify numElves > 0
        numElves = atoi( argv[2] );
        if ( numElves < 1 ) {
            usage( argv );
        }
        // FALL THROUGH
      case 2:                       // verify rBound > 0
        rBound = atoi( argv[1] );
        if ( rBound < 1 ) {
            usage( argv );
        }
        break;
      case 1:                       // no arguments, that's ok
        break;
      default:						// wrong number of options
		usage( argv );
    } // switch 

    mprng.seed( seed );
    Printer printer( numElves ); 
    Workshop workshop( printer, rBound, numElves, numDeliveries );
    Elf* elves[ numElves ];
    Reindeer* reindeers[ Reindeer::MAX_NUM_REINDEER ];

    Santa santa( workshop, printer ); // allocate Santa on the stack

    // Create Elf tasks, starting with id = 1
    int id = 1;
    for ( int i = 0; i < numElves; i++ ) {
        elves[i] = new Elf( id++,  workshop, printer, numConsultations );
    }
    
    // Create Reindeer tasks, starting with id 1 + numElves
    for ( int i = 0; i < Reindeer::MAX_NUM_REINDEER; i++ ) {
        reindeers[i] = new Reindeer( id++,  workshop, printer, numDeliveries );
    }

    // Wait for elves to finish
    for ( int i = 0; i < numElves; i++ ) {
        delete elves[i];
    }

    // wait for reindeer to finish
    for ( int i = 0; i < Reindeer::MAX_NUM_REINDEER; i++ ) {
        delete reindeers[i];
    }

    cout << "Workshop closed" << endl;
}
