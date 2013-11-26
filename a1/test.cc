#include <iostream>
#include <cstdlib>          // access: rand, srand
#include <unistd.h>         // access: getpid
using namespace std;

int foo() { 
	int j;
	bool flag = false;
	for ( j = 0; !flag && j < 10; j += 1 ) { 
		if ( j == 5 ) flag = true; 
	}
	return j;
}

int main( int argc, char *argv[] ) {
	cout << "test:" << foo() << endl;
}
