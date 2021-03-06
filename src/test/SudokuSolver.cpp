#include <iostream>
#include <string.h>
#include "SudokuSolver.h"

/**
 * Counts the number of row and columns conflicts.
 * @param grid Grid to be used.
 * @return Number of conflicts.
 */
unsigned int SudokuSolver::countRowColumnConflicts(const grid_t & grid)
{
  //std::cout << "countRowColumnConflicts() \n";
  unsigned int conflicts = 0;

  for(int i = 0; i < 9; i++) {
    uint8_t used[2][9];
    memset(&used[0], 0, 9);
    memset(&used[1], 0, 9);

    for(int j = 0; j < 9; j++) {
      if(used[0][grid.grid[j][i] - 1]) {
        return(1);
        conflicts++;
      }
      else {
        used[0][grid.grid[j][i] - 1] = 1;
      }

      if(used[1][grid.grid[i][j] - 1]) {
        return(1);
        conflicts++;
      }
      else {
        used[1][grid.grid[i][j] - 1] = 1;
      }
    }
  }

  return(conflicts);
}

/**
 * Counts the number of sub-square conflicts.
 * @param grid Grid to be used.
 * @return Number of conflicts.
 */
unsigned int SudokuSolver::countSubSquareConflicts(const grid_t & grid)
{
  //std::cout << "countSubSquareConflicts() \n";
  unsigned int conflicts = 0;

  for(int square = 0; square < 9; square++) {
    uint8_t used[9];
    memset(used, 0, 9);

    for(int i = 0; i < 9; i++) {
      int x = (i % 3) + ((square * 3) % 9);
      int y = (i / 3) + ((square / 3) * 3);
      
      if(used[grid.grid[x][y] - 1]) {
        conflicts++;
      }
      else {
        used[grid.grid[x][y] - 1] = 1;
      }
    }
  }

  return(conflicts);
}

/**
 * Checks if grid is a valid solution.
 * @param grid Grid to be used.
 * @return True if a valid Sudoku.
 */
bool SudokuSolver::isValidSolution(const grid_t & grid)
{
  int a, b;
  return(!countRowColumnConflicts(grid) && !countSubSquareConflicts(grid));
}
