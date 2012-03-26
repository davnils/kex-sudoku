#include "Cultural.h"

/*
 *
 */
Cultural::Cultural()
{
}

/*
 *
 */
void Cultural::addPuzzle(grid_t puzzle)
{
  grid = puzzle;

  for(int i = 0; i < POPULATION_SIZE; i++) {
    Individual i(puzzle);
    population.push_back(i);
  }
}

/*
 *
 */
grid_t Cultural::getGrid()
{
  return(grid);
}

/*
 *
 */
void Cultural::runStep(bool forever)
{
  do {
    //sort by fitness
    //select PASSED_ON individuals
    //crossover after tournament
    //mutate the resulting individuals
    //return if candidate has fitness zero
  } while(forever);
}

/*
 *
 */
void Cultural::tournament()
{
}
