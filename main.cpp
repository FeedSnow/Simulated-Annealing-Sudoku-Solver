#include <iostream>
#include "sudoku.h"
#include "reader.h"

using namespace std;

#include <SFML/Graphics.hpp>

const float CELLSIZE = 50;

void Display(Sudoku sudoku)
{
    sf::RenderWindow window(sf::VideoMode(9 * CELLSIZE + 32, 9 * CELLSIZE + 32), "Solved Sudoku");

    vector<sf::RectangleShape> cells;
    float X = 0;
    float Y = 0;
    for (int i = 0; i < 9; i++)
    {
        X = 0;
        Y += 2;
        if (i % 3 == 0)
            Y += 3;
        for (int j = 0; j < 9; j++)
        {
            X += 2;
            if (j % 3 == 0)
                X += 3;

            sf::RectangleShape cell(sf::Vector2f(CELLSIZE, CELLSIZE));
            cell.setFillColor(sf::Color::White);
            cell.setPosition(sf::Vector2f(X, Y));
            cells.push_back(cell);

            X += CELLSIZE;
        }
        Y += CELLSIZE;
    }

    sf::Font font;
    if (!font.loadFromFile("Lato-Bold.ttf"))
    {
        cerr << "Couldn't load font from file." << endl;
        return;
    }

    vector<sf::Text> numbers;
    X = Y = CELLSIZE/2;
    for (int i = 0; i < 9; i++)
    {
        X = CELLSIZE/2;
        Y += 2;
        if (i % 3 == 0)
            Y += 3;
        for (int j = 0; j < 9; j++)
        {
            X += 2;
            if (j % 3 == 0)
                X += 3;

            Sudoku::Cell c = sudoku.Get(i, j);
            sf::Text text(to_string(c.value), font, CELLSIZE*0.9);
            text.setFillColor(c.fixed ? sf::Color(0,128U,0,255U) : sf::Color::Black);
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
        for (int i = 0; i < cells.size(); i++)
        {
            window.draw(cells[i]);
            window.draw(numbers[i]);
        }
        window.display();
    }
}

int main()
{
	int temp[81];
	Read(temp);
	
	Sudoku s(temp);
	s.Solve();
    Display(s);

    /*
	cout << endl;
	cout << "Enter any character to exit. ";
	char ch;
	cin >> ch;*/
	return 0;
}/**/