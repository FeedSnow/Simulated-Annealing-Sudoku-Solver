#include <iostream>
#include <filesystem>
#include "sudoku.h"
#include "reader.h"

using namespace std;
namespace fs = filesystem;

int main()
{
	if (fs::exists("data.txt"))
		fs::remove("data.txt");

	int temp[81];
	Read(temp);
	
	Sudoku s(temp);
	s.Solve();

	cout << "Enter any character to exit." << endl;
	char ch;
	cin >> ch;

	return 0;
}