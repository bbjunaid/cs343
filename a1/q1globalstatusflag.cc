#include <iostream>
#include <cstdlib>          // access: rand, srand
#include <unistd.h>         // access: getpid
using namespace std;

int times = 1000;
int throwVal = 0;
bool isThrow = false;

void rtn1( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 100000000 == 42 ) {
			isThrow = true;
			throwVal = j;
			break;
		} 
    }
}
void rtn2( int i ) {
    for ( int j = 0; -j < times; j -= 1 ) {
        if ( rand() % 100000000 == 42 ) {
			isThrow = true;
			throwVal = j;
			break;
		}
    }
}
void g( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 2 == 0 ) rtn1( i );
        else rtn2( i );
		if ( isThrow ) return;
    }
    if ( i % 2 ) rtn2( i );
	if ( isThrow ) return;
    rtn1( i );
}
void f( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        g( i );
		if ( isThrow ) return;
    }
    if ( i % 2 ) g( i );
	if ( isThrow ) return;
    g( i );
}
int main( int argc, char *argv[] ) {
    int seed = getpid();
    if ( argc >= 2 ) seed = atoi( argv[1] );
    srand( seed );
    if ( argc == 3 ) times = atoi( argv[2] );
    f( 3 );
	if ( !isThrow ) 
        cout << "seed:" << seed << " times:" << times << " complete" << endl;
	else
        cout << "seed:" << seed << " times:" << times << " rc:" << throwVal << endl;
}
