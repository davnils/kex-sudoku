#include <vector>
#include <cstdint>

#include "Square.h"
#include "Boltzmann.h"

Boltzmann::Boltzmann(grid_t assigned)
{
  temperature = MAX_TEMPERATURE;
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
  std::vector<int> digits(9, 0);

  //Check row
  group_t rowIt = row.begin();
  for(; rowIt != row.end(); rowIt++) {
    digits = square.sum(digits); 
  }
  
  //Check column
  //Check for multiple digits
  //Check quadrant

  square.update(digits, temperature);
}
