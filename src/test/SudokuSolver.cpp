#include <string.h>
#include "SudokuSolver.h"

/**
 *
 */
unsigned int SudokuSolver::countRowColumnConflicts(const grid_t & grid)
{
  unsigned int conflicts = 0;

  for(int i = 0; i < 9; i++) {
    int used[2][9];
    memset(&used[0], 0, 9);
    memset(&used[1], 0, 9);

    for(int j = 0; j < 9; j++) {
      if(used[0][grid.grid[j][i]]) {
        conflicts++;
      }
      else {
        used[0][grid.grid[j][i]] = 1;
      }
      if(used[1][grid.grid[i][j]]) {
        conflicts++;
      }
      else {
        used[1][grid.grid[i][j]] = 1;
      }
    }
  }

  return(conflicts);
}

/**
 *
 */
unsigned int SudokuSolver::countSubSquareConflicts(const grid_t & grid)
{
  unsigned int conflicts = 0;

  for(int square = 0; square < 9; square++) {
    uint8_t used[9];
    memset(used, 0, 9);

    for(int i = 0; i < 9; i++) {
      int x = (i % 3) + ((square * 3) % 9);
      int y = (i / 3) + ((square / 3) * 3);
      
      if(used[grid.grid[x][y]] - 1) {
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
 *
 */
bool SudokuSolver::isValidSolution(const grid_t & grid)
{
  return(countRowColumnConflicts(grid) == 0
      && countSubSquareConflicts(grid) == 0);
}
