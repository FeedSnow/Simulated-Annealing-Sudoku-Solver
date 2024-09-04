#include <iostream>
#include "sudoku.h"

using namespace std;

int main()
{
	cout << "Pass sudoku to solve below. Separate numbers with space and use zeros for empty fields." << endl;
	cout << "E.g. 9 0 0 0 1 2 5 0 7" << endl;
	int temp[81];
	for (int i = 0; i < 81; i++)
	{
		cin >> temp[i];
	}

	Sudoku s(temp);
	s.Solve();
}
