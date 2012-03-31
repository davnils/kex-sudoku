#ifndef SUDOKUSOLVER_H_
#define SUDOKUSOLVER_H_

#include <cstdint>
#include <string>

typedef struct
{
  uint8_t grid[9][9];
} grid_t;

/**
 * Parent class for sudoku solvers.
 */
class SudokuSolver
{
  public:
    virtual ~SudokuSolver() {}
    virtual void addPuzzle(grid_t puzzle) = 0;
    virtual grid_t getGrid() = 0;
    virtual std::string getName() = 0;
    virtual void runStep(bool forever) = 0;
    bool isValidSolution(const grid_t & grid);

  protected:
    unsigned int countRowColumnConflicts(const grid_t & grid);
    unsigned int countSubSquareConflicts(const grid_t & grid);
};

#endif
