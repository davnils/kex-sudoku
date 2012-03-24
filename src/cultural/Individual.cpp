#include "Individual.h"

/*
 *
 */
Individual::Individual(grid_t given)
{
  genes = new uint8_t[9][9];
  memset(genes, 0, 81);

  for(int row = 0; row < 9; row++) {
    for(int column = 0; column < 9; column++) {
      genes[][] = given[][];
    }
  }
}

/*
 *
 */
Individual::~Individual()
{
  delete [] assigned;
}

/*
 *
 */
void Individual::initialize()
{
}

/*
 *
 */
unsigned int Individual::fitness()
{
}

/*
 *
 */
Individual Individual::crossover()
{
}

/*
 *
 */
void Individual::mutate()
{
}
