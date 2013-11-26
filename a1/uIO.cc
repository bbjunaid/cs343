// Purpose: copy file
//
// Command syntax:
//   $ ./a.out [ size (>= 0) [ code (>= 0) [ input-file [ output-file ] ] ] ]
// all parameters are optional. Defaults: size=>20, code=>5, input=>cin, output=>cout
//
// Examples:
//   $ ./a.out 34
//   $ ./a.out 34 0
//   $ ./a.out 34 0 in
//   $ ./a.out 34 0 in out

#include <uC++.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;                    // direct access to std
#include <cstdlib>                  // exit

bool convert( int &val, char *buffer ) {        // convert C string to integer
    std::stringstream ss( buffer );         // connect stream and buffer
    ss >> dec >> val;                   // convert integer from buffer
    return ! ss.fail() &&               // conversion successful ?
    // characters after conversion all blank ?
    string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
} // convert

enum { sizeDeflt = 20, codeDeflt = 5 };         // global defaults

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0]
     << " [ size (>= 0 : " << sizeDeflt
     << ") [ code (>= 0 : " << codeDeflt
     << ") [ input-file [ output-file ] ] ] ]" << endl;
    exit( EXIT_FAILURE );               // TERMINATE
} // usage

void uMain::main() {
    int size = sizeDeflt, code = codeDeflt;     // default value
    istream *infile = &cin;             // default value
    ostream *outfile = &cout;               // default value

    switch ( argc ) {
      case 5:
    try {
        outfile = new ofstream( argv[4] );
    } catch( uFile::Failure ) {         // open failed ?
        cerr << "Error! Could not open output file \"" << argv[4] << "\"" << endl;
        usage( argv );
    } // try
        // FALL THROUGH
      case 4:
    try {
        infile = new ifstream( argv[3] );
    } catch( uFile::Failure ) {
        cerr << "Error! Could not open input file \"" << argv[3] << "\"" << endl;
        usage( argv );
    } // try
        // FALL THROUGH
      case 3:
        if ( ! convert( code, argv[2] ) || code < 0 ) { // invalid integer ?
        usage( argv );
    } // if
        // FALL THROUGH
      case 2:
        if ( ! convert( size, argv[1] ) || size < 0 ) { // invalid integer ?
        usage( argv );
    } // if
        // FALL THROUGH
      case 1:                       // all defaults
        break;
      default:                      // wrong number of options
    usage( argv );
    } // switch

    *infile >> noskipws;                // turn off white space skipping during input

    char ch;
    for ( ;; ) {                    // copy input file to output file
    *infile >> ch;                  // read a character
      if ( infile->fail() ) break;          // failed ?
    *outfile << ch;                 // write a character
    } // for

    if ( infile != &cin ) delete infile;        // close file, do not delete cin!
    if ( outfile != &cout ) delete outfile;     // close file, do not delete cout!
} // uMain::main

// Local Variables: //
// compile-command: "u++ uIO.cc" //
// End: //
