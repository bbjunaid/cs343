#include <iostream>
#include <cstdlib>          // access: rand, srand
#include <unistd.h>         // access: getpid

using namespace std;

void f( int &i );                                         // mutually recursive routines
void g( int &i );

struct Handler { 
	Handler( int &i ) : m_i( i ) {}
	int operator() (  char c ) {
		if ( c == 'g' ) {
        	cout << "g handler, i:" << m_i << endl;
        	m_i -= 5;                                         
        	if ( rand() % 5 == 0 ) f( m_i );                  
		}
		else if ( c == 'f' ) {
        	cout << "f handler, i:" << m_i << endl;
        	m_i -= 7;                                         
        	if ( rand() % 7 == 0 ) g( m_i );                 
		}	
	}
	private:
		int &m_i;
};

void f( int &i ) {
    cout << "f " << i << endl;
	static Handler h( i );
    if ( rand() % 5 == 0 ) h( 'f' );            // require correction ?
    if ( rand() % 7 == 0 ) g( i );                    // mutual recursion
    
    if ( 0 < i ) f( i );                                  // recursion
}
void g( int &i ) {
    cout << "g " << i << endl;
	static Handler h( i );
    if ( rand() % 7 == 0 ) h( 'g' );            // require correction ?
    if ( rand() % 5 == 0 ) f( i );                    // mutual recursion

	if ( 0 < i ) g( i );                                  // recursion
}
int main( int argc, char *argv[] ) {
    int times = 25, seed = getpid();
    if ( argc >= 2 ) times = atoi( argv[1] );             // control recursion depth
    if ( argc == 3 ) seed  = atoi( argv[2] );             // allow repeatable experiment
    srand( seed );                                        // fixed or random seed
    f( times );
}
