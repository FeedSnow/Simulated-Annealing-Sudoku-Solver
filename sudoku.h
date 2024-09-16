#ifndef SUDOKU
#define SUDOKU

class Sudoku
{
public:
	Sudoku(int sudoku[81]);
	void Solve();
	void Print();
	void PrintWithCost();
	void Display();
private:
	struct Cell
	{
		int value = 0;
		bool fixed = false;
	};

	struct Cost
	{
		int row[9] = { -1 };
		int column[9] = { -1 };
		int GetTotal();
	};

	Cell cells[81];
	Cost cost;

	Cell Get(int i, int j) { return cells[9 * i + j]; }
	bool Set(int i, int j, int value);

	void FillRandomly();
	Sudoku SwapTwoRandom(Sudoku sudoku);
	double CalcT0();
	int NonFixedInSquare(int r, int c);

	void EvaluateCost();
	void ReevaluateCost(int i1, int j1, int i2, int j2);
	Cost GetCost() { return cost; }
};

#endif