// 0D_Genetic_NH_C.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "0D_model_NH.h"
#include "Genetic.h"
#include <string>
#include <array>

using namespace std;
int main()
{
	variable test(2.5, 1, "test_name", "m/s");
	
	test.printVariable();
	test.printMode();
	
	double temp = 3.75;
	
	test.setValue( &temp );
	test.printVariable();

	cout << "All done." << endl;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
