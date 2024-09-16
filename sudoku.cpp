#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numbers>
#include "sudoku.h"

using namespace std;

Sudoku::Sudoku(int sudoku[81])
{
	for (int i = 0; i < 81; i++)
	{
		if (sudoku[i] < 0 || sudoku[i] > 9)
		{
			cout << "Invalid value!" << endl;
			exit(1);
		}

		cells[i].value = sudoku[i];
		cells[i].fixed = sudoku[i] != 0;
	}
}

void Sudoku::Solve()
{
	if (cost.GetTotal() == 0)
	{
		cout << "Sudoku already solved." << endl;
		return;
	}

	FillRandomly();

	const double t0 = CalcT0();
	const double alpha = 0.5;
	double t = t0;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	int it = 0;
	int no_improvement_it = 0;

	while (GetCost().GetTotal() > 0)
	{
		Sudoku newSudoku = SwapTwoRandom(*this);
		double delta = newSudoku.GetCost().GetTotal() - GetCost().GetTotal();
		if (delta < 0)
		{
			*this = newSudoku;
			no_improvement_it = -1;
		}
		else if (dis(gen) <= exp(-delta / t))
			*this = newSudoku;

		t *= alpha;
		it++;
		no_improvement_it++;
		if (no_improvement_it > 20)
		{
			t = t0;
			no_improvement_it = 0;
		}
	}

	cout << endl;
	cout << "Solution found in " << it << " iterations:" << endl;
	Display();
}

void Sudoku::Print()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cout << cells[9 * i + j].value << " ";
			if ((j + 1) % 3 == 0)
				cout << "  ";
		}
		cout << endl;
		if ((i + 1) % 3 == 0)
			cout << endl;
	}
}

void Sudoku::PrintWithCost()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cout << cells[9 * i + j].value << " ";
			if ((j + 1) % 3 == 0)
				cout << "  ";
		}
		cout << "   " << cost.row[i];
		cout << endl;
		if ((i + 1) % 3 == 0)
			cout << endl;
	}
	for (int j = 0; j < 9; j++)
	{
		cout << cost.column[j] << " ";
		if ((j + 1) % 3 == 0)
			cout << "  ";
	}
	cout << "   " << cost.GetTotal() << endl;
}

#include <SFML/Graphics.hpp>

const float CELLSIZE = 50;
const string FONTNAME = "Lato-Regular.ttf";

void Sudoku::Display()
{
	float off1 = CELLSIZE * 0.04;
	float off2 = CELLSIZE * 0.1 - off1;
	int windowsize = 9 * CELLSIZE + 10 * off1 + 4 * off2;
	sf::RenderWindow window(sf::VideoMode(windowsize, windowsize), "Solved Sudoku");

	vector<sf::RectangleShape> cell_shapes;
	float X = 0;
	float Y = 0;
	for (int i = 0; i < 9; i++)
	{
		X = 0;
		Y += off1;
		if (i % 3 == 0)
			Y += off2;
		for (int j = 0; j < 9; j++)
		{
			X += off1;
			if (j % 3 == 0)
				X += off2;

			sf::RectangleShape cell(sf::Vector2f(CELLSIZE, CELLSIZE));
			cell.setFillColor(sf::Color::White);
			cell.setPosition(sf::Vector2f(X, Y));
			cell_shapes.push_back(cell);

			X += CELLSIZE;
		}
		Y += CELLSIZE;
	}

	sf::Font font;
	if (!font.loadFromFile(FONTNAME))
	{
		cerr << "Couldn't load font from file." << endl;
		return;
	}

	vector<sf::Text> numbers;
	X = Y = CELLSIZE / 2;
	for (int i = 0; i < 9; i++)
	{
		X = CELLSIZE / 2;
		Y += off1;
		if (i % 3 == 0)
			Y += off2;
		for (int j = 0; j < 9; j++)
		{
			X += off1;
			if (j % 3 == 0)
				X += off2;
			
			sf::Text text(to_string(Get(i, j).value), font, CELLSIZE * 0.9);
			text.setFillColor(Get(i, j).fixed ? sf::Color(0, 128U, 0, 255U) : sf::Color::Black);
			auto center = text.getGlobalBounds().getSize() / 2.f;
			auto localBounds = center + text.getLocalBounds().getPosition();
			auto rounded = sf::Vector2f(round(localBounds.x), round(localBounds.y));
			text.setOrigin(rounded);
			text.setPosition(sf::Vector2f(X, Y));
			numbers.push_back(text);

			X += CELLSIZE;
		}
		Y += CELLSIZE;
	}


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (int i = 0; i < cell_shapes.size(); i++)
		{
			window.draw(cell_shapes[i]);
			window.draw(numbers[i]);
		}
		window.display();
	}
}


int Sudoku::Cost::GetTotal()
{
	int sum = 0;
	for (int i = 0; i < 9; i++)
		sum += row[i] + column[i];
	return sum;
}



bool Sudoku::Set(int i, int j, int value)
{
	int id = 9 * i + j;
	if (cells[id].fixed || value < 0 || value > 9)
		return false;

	cells[id].value = value;
	return true;
}



void Sudoku::FillRandomly()
{
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			vector<int> present;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
					int val = Get(3 * r + i, 3 * c + j).value;
					if (val > 0)
						present.push_back(val);
				}

			if (present.size() == 9)
				continue;

			sort(present.begin(), present.end());
			vector<int> nums;
			int p = 0, n = 1;
			while (p < 10 && n < 10)
			{
				if (p >= present.size() || n < present[p])
				{
					nums.push_back(n);
					n++;
					continue;
				}

				if (n == present[p])
				{
					n++;
					continue;
				}

				if (n > present[p])
					p++;
			}

			random_device rd;
			mt19937 g(rd());
			shuffle(nums.begin(), nums.end(), g);

			p = 0;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (!Get(3 * r + i, 3 * c + j).fixed)
						Set(3 * r + i, 3 * c + j, nums[p++]);
		}
	}
	EvaluateCost();
}

Sudoku Sudoku::SwapTwoRandom(Sudoku sudoku)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(0, 2);

	int r, c, i, j, k, l;

	do
	{
		r = dist(gen);
		c = dist(gen);
	} while (sudoku.NonFixedInSquare(r, c) < 2);

	do
	{
		i = dist(gen);
		j = dist(gen);
	} while (sudoku.Get(3 * r + i, 3 * c + j).fixed);

	do
	{
		k = dist(gen);
		l = dist(gen);
	} while (sudoku.Get(3 * r + k, 3 * c + l).fixed || (k == i && l == j));

	int temp = sudoku.Get(3 * r + i, 3 * c + j).value;
	sudoku.Set(3 * r + i, 3 * c + j, sudoku.Get(3 * r + k, 3 * c + l).value);
	sudoku.Set(3 * r + k, 3 * c + l, temp);

	sudoku.ReevaluateCost(3 * r + i, 3 * c + j, 3 * r + k, 3 * c + l);

	return sudoku;
}

double Sudoku::CalcT0()
{
	vector<int> costs;
	costs.push_back(GetCost().GetTotal());
	for (int i = 0; i < 10; i++)
		costs.push_back(SwapTwoRandom(*this).GetCost().GetTotal());

	int sum = 0;
	for (int i = 0; i < 11; i++)
		sum += costs[i];
	double mean = sum / 11.;

	double stddev = 0;
	for (int i = 0; i < 11; i++)
		stddev += pow((double)costs[i] - mean, 2) / 11.;
	stddev = sqrt(stddev);
	return stddev;
}

int Sudoku::NonFixedInSquare(int r, int c)
{
	int nf = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (!Get(3 * r + i, 3 * c + j).fixed)
				nf++;
	return nf;
}



void Sudoku::EvaluateCost()
{
	for (int i = 0; i < 9; i++)
	{
		vector<int> row;
		for (int j = 0; j < 9; j++)
		{
			if (find(row.begin(), row.end(), Get(i, j).value) == row.end())
				row.push_back(Get(i, j).value);
		}
		cost.row[i] = 9 - row.size();
	}

	for (int j = 0; j < 9; j++)
	{
		vector<int> column;
		for (int i = 0; i < 9; i++)
		{
			if (find(column.begin(), column.end(), Get(i, j).value) == column.end())
				column.push_back(Get(i, j).value);
		}
		cost.column[j] = 9 - column.size();
	}
}

void Sudoku::ReevaluateCost(int i1, int j1, int i2, int j2)
{
	vector<int> rc;
	for (int j = 0; j < 9; j++)
	{
		if (find(rc.begin(), rc.end(), Get(i1, j).value) == rc.end())
			rc.push_back(Get(i1, j).value);
	}
	cost.row[i1] = 9 - rc.size();
	rc.clear();

	if (i1 != i2)
	{
		for (int j = 0; j < 9; j++)
		{
			if (find(rc.begin(), rc.end(), Get(i2, j).value) == rc.end())
				rc.push_back(Get(i2, j).value);
		}
		cost.row[i2] = 9 - rc.size();
		rc.clear();
	}

	for (int i = 0; i < 9; i++)
	{
		if (find(rc.begin(), rc.end(), Get(i, j1).value) == rc.end())
			rc.push_back(Get(i, j1).value);
	}
	cost.column[j1] = 9 - rc.size();
	rc.clear();

	if (j1 != j2)
	{
		for (int i = 0; i < 9; i++)
		{
			if (find(rc.begin(), rc.end(), Get(i, j2).value) == rc.end())
				rc.push_back(Get(i, j2).value);
		}
		cost.column[j2] = 9 - rc.size();
	}
}