/*template<typename T> _Task Mergesort {
  public:
    Mergesort( T values[], unsigned int low, unsigned int high, unsigned int depth );
};

unsigned int uDefaultStackSize() {
    return 512 * 1000;        // set task stack-size to 512K
}

uProcessor p[ processors - 1 ] __attribute__(( unused )); // use "processors" kernel threads*/

#include <uC++.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;                    // direct access to std
#include <cstdlib>                  // exit

void uMain::main() {
    istream *infile;

    if ( argc > 1 ) {
        try {
            infile = new ifstream( argv[1] );
        } catch ( uFile::Failure ) {
            cerr << "Error: cannot open " << argv[1] << " for reading" << endl;
            exit ( EXIT_FAILURE );
        }
    }
 
} // uMain::main
