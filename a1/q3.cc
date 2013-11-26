#include <uC++.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;                    // direct access to std
#include <cstdlib>                  // exit

_Coroutine Utf8 {
  public:
    struct Match {
        unsigned int unicode;
        Match( unsigned int unicode ) : unicode( unicode ) {}
    };
    struct Error {};
  private:
    union UTF8 {
        unsigned char ch;                                // character passed by cocaller
#if defined( _BIG_ENDIAN ) || BYTE_ORDER == BIG_ENDIAN   // BIG ENDIAN architecture
        struct {                                         // types for 1st utf-8 byte
            unsigned char ck : 1;                        // check
            unsigned char dt : 7;                        // data
        } t1;
        struct {
            unsigned char ck : 3;                        // check
            unsigned char dt : 5;                        // data
        } t2;
        struct {
            unsigned char ck : 4;                        // check
            unsigned char dt : 4;                        // data
        } t3;
        struct {
            unsigned char ck : 5;                        // check
            unsigned char dt : 3;                        // data
        } t4;
        struct {                                         // type for extra utf-8 bytes
            // you figure it out
            unsigned char ck : 2;
            unsigned char dt : 6;
        } dt;
#else                                                    // LITTLE ENDIAN architecture
        struct {                                         // types for 1st utf-8 byte
            unsigned char dt : 7;                        // data
            unsigned char ck : 1;                        // check
        } t1;
        struct {
            unsigned char dt : 5;                        // data
            unsigned char ck : 3;                        // check
        } t2;
        struct {
            unsigned char dt : 4;                        // check
            unsigned char ck : 4;                        // data
        } t3;
        struct {
            // you figure it out
            unsigned char dt : 3;                        // check
            unsigned char ck : 5;                        // data
        } t4;
        struct {                                         // type for extra utf-8 bytes
            // you figure it out
            unsigned char dt : 6;                        // check
            unsigned char ck : 2;                        // data
        } dt;
#endif
    } utf8;
    bool match; // flag for match 
    bool error; // flag for error
    unsigned int unicode; // communication variable for storing unicode
    // YOU MAY ADD PRIVATE MEMBERS
  public:
    // YOU MAY ADD CONSTRUCTOR/DESTRUCTOR IF NEEDED
    Utf8() : match( false ), error( false ), unicode ( 0 ) {}
    void next( unsigned char c ) {
        utf8.ch = c;                    // insert character into union for analysis
        resume();
        // if necessary throw Match or Error exception
        if ( error ) {
            throw Error();
        }
        if ( match ) {
            throw Match( unicode );
        }
    }

    void main() {
        int numBytes = 0;
        // Check if the first byte matches any of the types
        // When the type is determined, store the number of bytes in numBytes and grab the
        // unicode value from first byte. If none of the type matches, flag error.
        if ( utf8.t1.ck == 0 ) {
            numBytes = 1;
            unicode = utf8.t1.dt;
            if ( unicode <= 0x7F ) { 
                match = true;
            }
            else {
                error = true;
            }
        } 
        else if ( utf8.t2.ck == 0x6 ) {
            numBytes = 2;   
            unicode = utf8.t2.dt;
        }
        else if ( utf8.t3.ck == 0xE ) {
            numBytes = 3;    
            unicode = utf8.t3.dt;
        }
        else if ( utf8.t4.ck == 0x1E ) {
            numBytes = 4;
            unicode = utf8.t4.dt;
        }
        else {
            error = true;
        }
        
        suspend();
        // first byte was not a utf-8 match. Read all remaining bytes extracting their unicode
        // value. If the checkbits don't match, then flag error.
        for ( int i = 1; i < numBytes; ++i ) {
             if ( utf8.dt.ck != 0x2 ) error = true;
             unicode <= 6;
             unicode |= utf8.dt.dt;
             suspend();
        }
        // All the values for the UTF-8 value have been read. Now determine if the unicode
        // value falls in the appropriate range.
        switch ( numBytes ) {
            case 2:
                if ( unicode < 0x80 || unicode > 0x7FF ) error = true;
                break;
            case 3:
                if ( unicode < 0x800 || unicode > 0xFFFF ) error = true;
                break;
            case 4:
                if ( unicode < 0x10000 || unicode > 0x10FFFF ) error = true;
                break;
            default:
                break;
        }
        // If there is no error by now, there must be a match.
        if ( !error ) match = true;
    }
};

void uMain::main() {
    istream *infile = &cin;

    if ( argc > 1 ) {
        try {
            infile = new ifstream( argv[1] );
        } catch ( uFile::Failure ) {
            cerr << "Error: cannot open " << argv[1] << " for reading" << endl;
            exit ( EXIT_FAILURE );
        }
    }
    cout << hex << showbase;

    unsigned char ch;
    string line;
    string::iterator it;
    for ( ;; ) {
        getline( *infile, line );
        if ( !infile->good() ) break;            // failed ?
        if ( line.empty() ) {
            cout << " : Warning! Blank line." << endl;
            continue;
        }
        Utf8 utfChar;
        int count = 0;
        for ( it = line.begin(); it != line.end(); it++ ) {
            cout << hex << (unsigned int)(unsigned char)*it << noshowbase;
            
            try {
                utfChar.next( *it );
            }  
            catch ( Utf8::Error ) {
                cout << " : invalid" << showbase;
                break;
            }
            catch ( Utf8::Match m ) {
                cout << " : valid " << showbase << hex << m.unicode;
                break;
            }
        } // for it = line.begin()
        ++it; // breaks out of loop before iterator is incremented
        if ( it != line.end() ) cout << ". Extra characters ";
        for ( it; it != line.end(); ++it ) cout << hex << (unsigned int)(unsigned char)*it << noshowbase;
        cout << showbase << endl;
    } // for ( ;; )

    if ( infile != &cin ) delete infile;        // close file, do not delete cin!
} // uMain::main
