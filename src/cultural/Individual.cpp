#include "Individual.h"
#include <cstdlib>

/*
 *
 */
Individual::Individual(grid_t given)
{
  genes = new uint8_t[9][9];
  this.given = given;

  for(int row = 0; row < 9; row++) {
    memset(genes[row], 0, 9);
    for(int column = 0; column < 9; column++) {
      if(given.g[row][column]) {
        genes[row][column] = given.g[row][column];
      }
    }
  }

  for(int row = 0; row < 9; row += 3) {
    for(int column = 0; column < 9; column += 3) {
      initializeSubSquare(row, column);
    }
  }
}

/*
 *
 */
Individual::~Individual()
{
  delete [] genes;
}

/*
 *
 */
void Individual::initializeSubSquare(int row, int column)
{
  int digits[9];
  memset(digits, 0, 9);

  for(int i = 0; i < 9; i++) {
    int val = assigned.grid[row+i/3][column+i%3];
    if(val) {
      digits[val-1] = 1;
    }
  }

  for(int i = 0; i < 9; i++) {
    int * pVal = &assigned.grid[row+i/3][column+i%3];
    if(*pVal) {
      int r;
      while(digits[(r = 1 + rand() % 10)]);
      *pVal = r;
    }
  }
}

/*
 *
 */
unsigned int Individual::fitness()
{
  return(countRowColumnConflicts(assigned));
}

/*
 *
 */
Individual Individual::crossover(Individual & p)
{
  int subSquares = rand() % 9;

  //TODO: Pick out subsquares from both individuals
  for() 
  {
  }
}

/*
 *
 */
void Individual::mutate()
{
  int mutations = rand() % 10;

  while(mutations-- > 0) {
    //TODO: Swap two elements while keeping
    //      subsquares invariant
  }
}
