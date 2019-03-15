//============================================================================
// Name        : Lab9.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "set.h"

using namespace std;

int main() {

	Set my_set;

	my_set.insert(9);
	my_set.insert(2);
	my_set.insert(7);
	my_set.insert(1);
	my_set.insert(6);
	my_set.insert(8);
	my_set.insert(14);
	my_set.insert(19);
	my_set.insert(11);
	my_set.insert(17);
	my_set.insert(20);

	my_set.print(100);

	return 0;
}
