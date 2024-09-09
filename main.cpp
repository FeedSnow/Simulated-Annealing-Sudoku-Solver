#include <iostream>
#include "sudoku.h"
#include "reader.h"

using namespace std;

int main()
{
	int temp[81];
	Read(temp);
	
	Sudoku s(temp);
	s.Solve();

	cout << endl;
	cout << "Enter any character to exit. ";
	char ch;
	cin >> ch;
	return 0;
}

