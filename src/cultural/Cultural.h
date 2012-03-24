#ifndef CULTURAL_H_
#define CULTURAL_H_

#include <cstdint>
#include <string>
#include <vector>

#include "../test/SudokuSolver.h"
#include "Individual.h"

/*
 *
 */
class Cultural : public SudokuSolver
{
  public:
    Cultural();
    void addPuzzle(grid_t puzzle);
    grid_t getGrid();
    void runStep(bool forever);
    std::string getName() { return("Cultural genetic solver"); }

  private:
    void tournament();
    std::vector<Individual> population;
};

#endif
