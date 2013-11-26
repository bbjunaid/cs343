#include <iostream>
#include <cstdlib>          // access: rand, srand
#include <unistd.h>         // access: getpid
using namespace std;

int times = 1000;

typedef struct IntWithReturnCode{
	int value;
	bool valid;
	IntWithReturnCode() {
		value = 0;
		valid = false;		
	}
}IntWithReturnCode;

IntWithReturnCode rtn1( int i ) {
	IntWithReturnCode var;
    for ( var.value; var.value < times; var.value += 1 ) {
        if ( rand() % 100000000 == 42 ) {
			var.valid = true;
			return var;
		}
    }
}
IntWithReturnCode rtn2( int i ) {
	IntWithReturnCode var;
    for ( var.value; -var.value < times; var.value -= 1 ) {
        if ( rand() % 100000000 == 42 ) {
			var.valid = true;
			return var;
		}
    }
}

IntWithReturnCode g( int i ) {
	IntWithReturnCode var;
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 2 == 0 ) var = rtn1( i );
        else var = rtn2( i );
		if ( var.valid ) return var; 
    }
    if ( i % 2 ) {
		var = rtn2( i );
		if ( var.valid ) return var;
	}
    return rtn1( i );
}

IntWithReturnCode f( int i ) {
	IntWithReturnCode var;
    for ( int j = 0; j < times; j += 1 ) {
        var = g( i );
		if ( var.valid ) return var;
    }
    if ( i % 2 ) {
		var = g( i );
		if ( var.valid ) return var;
	}
    return g( i );
}

int main( int argc, char *argv[] ) {
    int seed = getpid();
    if ( argc >= 2 ) seed = atoi( argv[1] );
    srand( seed );
    if ( argc == 3 ) times = atoi( argv[2] );
	IntWithReturnCode var = f( 3 );
	if ( var.valid )
		cout << "seed:" << seed << " times:" << times << " rc:" << var.value << endl;
	else
		cout << "seed:" << seed << " times:" << times << " complete" << endl;
}
