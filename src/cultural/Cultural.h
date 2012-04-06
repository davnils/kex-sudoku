#ifndef CULTURAL_H_
#define CULTURAL_H_

#include <cstdint>
#include <string>
#include <vector>

#include "../test/SudokuSolver.h"
#include "Individual.h"

const int POPULATION_SIZE = 50;
const int PASSED_ON = POPULATION_SIZE/10;

/*
 *
 */
class Cultural : public SudokuSolver
{
  public:
    Cultural();
    void addPuzzle(grid_t puzzle);
    grid_t getGrid();
    bool runStep(clock_t endTime);
    std::string getName() { return("Cultural genetic solver"); }

  private:
    void tournament();
    std::vector<Individual> population;
    grid_t grid;
};

#endif
