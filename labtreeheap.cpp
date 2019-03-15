//============================================================================
// Name        : labtreeheap.cpp
// Author      : S. Miller
//============================================================================

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

#define MINRAND 10
#define MAXRAND 99
#define INITIALHEAP 15
#include "treeheap.h"

using namespace std;
using namespace LABTREEHEAP;

int main() {

/*	treeheap<int> th;
	srand(time(NULL));

	for ( int i=0 ; i < INITIALHEAP ; ++i )
		th.push( rand() % (MAXRAND-MINRAND+1) + MINRAND );

	cout << "BEFORE" << endl;
	th.prettyprint();

	cout << "After pop #1" << endl;
	th.pop();
	th.prettyprint();

	cout << "After pop #2" << endl;
	th.pop();
	th.prettyprint();

	cout << "After pop #3" << endl;
	th.pop();
	th.prettyprint();*/

	cout << "done" << endl;
	return 0;
}
