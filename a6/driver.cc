#include <string>
#include <iostream>
#include <uC++.h>
#include "MPRNG.h"          // random number generator
#include "config.h"         // ConfigParam and processConfigFile
#include "printer.h"        
#include "bank.h"
#include "parent.h"
#include "watcardoffice.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "student.h"

using namespace std;

MPRNG mprng;               // global random number generator used in other files

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0] 
         << " [ config-file [ random-seed (> 0) ] ]" 
         << endl;
    exit( EXIT_FAILURE );  // TERMINATE
} // usage

void uMain::main() {
    ConfigParms cparams;
    string configFile;
    int seed;

    // Error for extra arguments
	if ( argc > 3 ) {
		usage( argv );
	}
	
    switch ( argc ) {
	  case 3:                       // verify seed > 0
		seed = atoi(argv[2]);
		if ( seed < 1 ) {
			usage( argv );
		}
        // FALL THROUGH
      case 2:                       // verify numConsultations >= 0
        configFile = argv[1]; 
        if ( configFile.empty() ) 
            configFile = "soda.config";
        break;
      case 1:                       // no arguments, that's ok
        configFile = "soda.config";
        break;
      default:						// wrong number of options
		usage( argv );
    } // switch 

    processConfigFile( configFile.c_str(), cparams );

    Printer prt( cparams.numStudents, cparams.numVendingMachines, cparams.numCouriers );

    Bank bank( cparams.numStudents );

    Parent parent( prt, bank, cparams.numStudents, cparams.parentalDelay );

    WATCardOffice cardOffice( prt, bank, cparams.numCouriers );

    NameServer nameServer( prt, cparams.numVendingMachines, cparams.numStudents );

    VendingMachine* vendingMachines[cparams.numVendingMachines];

    // allocate BottlingPlant because it needs to be manually deleted before the
    // VendingMachines to properly close down the system
    BottlingPlant* bottPlant = new BottlingPlant( prt, nameServer, cparams.numVendingMachines, \
                             cparams.maxShippedPerFlavour, cparams.maxStockPerFlavour, \
                             cparams.timeBetweenShipments );

    Student* students[cparams.numStudents];
 
    unsigned int id;
    // create VendingMachine tasks
    for ( id = 0; id < cparams.numVendingMachines; id++ ) {
        vendingMachines[id] = new VendingMachine( prt, nameServer, id, cparams.sodaCost, \
                                                 cparams.maxStockPerFlavour );
    }

    // create Student tasks
    for ( id = 0; id < cparams.numStudents; id++ ) {
        students[id] = new Student( prt, nameServer, cardOffice, id, cparams.maxPurchases );
    }

    // delete bottling plant before vending machines to avoid deadlock
    delete bottPlant;
    
    // finish VendingMachines tasks
    for ( id = 0; id < cparams.numVendingMachines; id++ ) {
        delete vendingMachines[id];
    }

    // finish Student tasks
    for ( id = 0; id < cparams.numStudents; id++ ) {
        delete students[id];
    }

    cout << "***********************" << endl;
}

