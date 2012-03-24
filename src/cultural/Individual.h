#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <cstdint>

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
    Individual crossover();
    void mutate();

  private:
    void initialize();

    uint8_t ** assigned;
};

#endif

