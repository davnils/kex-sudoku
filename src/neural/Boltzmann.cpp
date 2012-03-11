#include <vector>
#include <cstdint>

#include "Boltzmann.h"

Boltzmann::Boltzmann(grid_t assigned)
{
}

grid_t Boltzmann::getGrid()
{
}

void Boltzmann::simulate(bool step)
{
  internal_grid_t::iterator rowIt;
  for(rowIt = grid.begin(); rowIt != grid.end(); rowIt++) {
    group_t::iterator squareIt;
    for(squareIt = rowIt->begin(); squareIt != rowIt->end(); squareIt++) {
      updateNode(*rowIt, *squareIt);
    }
  }
}

void Boltzmann::updateNode(group_t & row, Square & square)
{
  //Accumulate values for all 4 checks
  //Check row
  //Check column
  //Check for multiple digits
  //Check quadrant
  //
  //Deliver values to method in square, which calculates probability and new state
  //Update probability and flip state
}
