#ifndef SUDOKUSOLVER_H_
#define SUDOKUSOLVER_H_

typedef struct
{
  uint8_t grid[9][9];
} grid_t;

/*
 * Abstract parent class for sudoku solvers.
 */
class SudokuSolver
{
  public:
    virtual ~SudokuSolver() {}
    virtual grid_t getGrid() = 0;
    virtual std::string getName() = 0;
};

#endif
