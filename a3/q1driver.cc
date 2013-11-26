#include <uC++.h>
#include <iostream>
#include <fstream>
#include <cstdlib> // for EXIT
#include <uSemaphore.h>
#include <vector>                   
#include <dirent.h> // opendir(), readdir()
#include "q1mapper.h"
#include "q1reducer.h"

using namespace std;

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0]
	     << " input-directory [ num-reducers ( > 0 ) "
         << "[ queue-length ( > 0 ) [ sort-buffer-size ( >= 0 ) ] ] ]" << endl;
    exit( EXIT_FAILURE );				// TERMINATE
} // usage

void uMain::main() {
    int num_reducers = 4;
    int queue_length = 16;
    int sort_buffer_size = 0;
    string dir_path, file_path;

    // Error for extra arguments
	if ( argc > 5 ) {
		cerr << "Too many arguments" << endl;
		usage( argv );
	}
	
    switch ( argc ) {
	  case 5:                       // verify sort_buffer_size >= 0
		sort_buffer_size = atoi(argv[4]);
		if ( sort_buffer_size < 0 ) {
			cerr << "Incorrect input for sort-buffer-size." << endl;
			usage( argv );
		}
        // FALL THROUGH
      case 4:                       // verify queue-length > 0
        queue_length = atoi( argv[3] ); 
        if ( queue_length < 1 )  { 
            cerr << "Incorrect input for queue length." << endl; 
            usage( argv ); 
        } 
        // FALL THROUGH
      case 3:                       // verify num-reducers > 0
        num_reducers = atoi( argv[2] );
		if ( num_reducers < 1 )  {
			cerr << "Incorrect input for num reducers." << endl;
			usage( argv );
		}
        // FALL THROUGH
      case 2:
        dir_path = string( argv[1] );
        break;
      default:						// wrong number of options
		usage( argv );
    } // switch 

    DIR *pDir;
    struct dirent *pEnt;
    string word;
    ifstream fin;

    vector<Mapper*> mappers;
    vector<Reducer*> reducers;
    uSemaphore signal(0); 

    // valid directory
    if ( (pDir=opendir(dir_path.c_str())) == NULL ) {
        cerr << "Error! Cannot open directory "
             << '"' << dir_path << '"' << endl;
        usage( argv );
    }

    // iteratate through all redable files in the directory and create mappers for each file
    while ( (pEnt=readdir(pDir)) != NULL ) {
        if ( pEnt->d_type == DT_REG ) { // only read regular files
            file_path = dir_path + string("/") + string( pEnt->d_name); 
            fin.open( file_path.c_str() );
            if ( !fin.fail() ) { // if valid file, then create a mapper for it
                mappers.push_back( new Mapper( file_path, queue_length, &signal ) );
            }            
            else {
                cout << "The file " << file_path << " cannot be opened." << endl;
            }
            fin.close();
            fin.clear();
        } // if regular file
    } // while
    closedir( pDir );

    // create reducers
    for ( int i = 0; i < num_reducers; i++ ) {
        reducers.push_back( new Reducer( (unsigned int)i, num_reducers, &signal, mappers ) ); 
    }

    // delete mappers
    for ( unsigned int i = 0; i < mappers.size(); i++ ) {
        delete mappers[i];
    }

    // delete reducers
    for ( unsigned int i = 0; i < reducers.size(); i++ ) {
        delete reducers[i];
    }

    // finished
    osacquire(cout) << "Finished! Semaphore counter: " << signal.counter() << endl;
}
