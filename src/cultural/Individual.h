#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <cstdint>
#include <ctime>

#include "../test/SudokuSolver.h"

/*
 *
 */
class Individual
{
  public:
    Individual(grid_t given);
    ~Individual();
    unsigned int fitness();
    Individual crossover(Individual & p);
    void mutate();

  private:
    void initialize();

    grid_t given;
    grid_t assigned;
};

#endif

