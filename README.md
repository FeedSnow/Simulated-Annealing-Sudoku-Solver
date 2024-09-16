# Sudoku Solver with Simulated Annealing and Image Recognition

This project uses a combination of Python and C++ to solve Sudoku puzzles from images. The Python part uses a convolutional neural network model to recognize digits from a Sudoku image, and the recognized grid is then solved using a C++ program. The solving algorithm is based on the simulated annealing approach outlined in the article “[Metaheuristics can solve Sudoku puzzles](https://www.researchgate.net/publication/220403361_Metaheuristics_can_solve_Sudoku_puzzles)”.

## Table of Contents
- [Requirements](#requirements)
- [Setup and Compilation](#setup-and-compilation)
- [Usage](#usage)

## Requirements
To run this project, you need the following tools and libraries installed:

### General:
- Python 3.12
- C++ compiler (e.g., g++ for Windows or Linux)
- OpenCV
- TensorFlow

### Python Libraries:
Install the required Python packages using pip:

```sh
pip install opencv-python pillow tensorflow
```

### C++ Compiler:
For **Windows**, install [MinGW-w64](https://www.mingw-w64.org) to get a g++ compiler.

For **Linux** (Ubuntu):

```sh
sudo apt update
sudo apt install g++
```

## Setup and Compilation

1. **Compile the C++ code:** Make sure you have the Python development headers and libraries installed, as well as OpenCV. Then, use the following command to compile the C++ program:
```bash
g++ -o sudoku_solver main.cpp sudoku.cpp reader.h sudoku.h -I <python-include-folder> -L <python-libs-folder> -lpython312
```

2. **Place Files in Correct Locations:**
Ensure `reader.py`, `sudoku_model.keras`, and the `images/` folder are in the same directory as the compiled executable (`sudoku_solver.exe` on Windows or `sudoku_solver` on Linux).

## Usage
1. **Prepare the Image:** Add the image you want to solve (e.g., `sudoku.png`) to the `images/` folder.
2. **Run the Solver:** Execute the compiled C++ program. It will prompt you to enter the name of the image file (located in the `images/` folder) that contains the Sudoku puzzle.
3. **Image Processing and Recognition:** The Python script `reader.py` will preprocess the image, recognize the digits using the neural network model (`sudoku_model.keras`), and save the recognized Sudoku grid to `data.txt`.
4. **Sudoku Solving:** The C++ program will load the recognized Sudoku grid from `data.txt`, solve it using a simulated annealing algorithm, and display the solved puzzle on the screen.

---

**Disclaimer:** Error handling doesn't exist for the moment. If the app closes without reason, ensure you have all needed files and use correct image name.
