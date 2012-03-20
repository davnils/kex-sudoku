#include <vector>
#include <cstdint>

#include "Square.h"
#include "Boltzmann.h"

/*
 *
 */
Boltzmann::Boltzmann(grid_t assigned, float maxTemperature)
{
  temperature = maxTemperature;

  for(int i = 0; i < 9; i++) {
    group_t row;
    for(int j = 0; j < 9; j++) {
      if(assigned.grid[i][j] == 0) {
        row.push_back(Square());
      }
      else {
        row.push_back(Square(assigned.grid[i][j]));
      }
    }
    grid.push_back(row);
  }
}

/*
 *
 */
grid_t Boltzmann::getGrid()
{
  grid_t g;
  internal_grid_t::iterator rowIt;
  for(rowIt = grid.begin(); rowIt != grid.end(); rowIt++) {
    group_t::iterator squareIt;
    for(squareIt = rowIt->begin(); squareIt != rowIt->end(); squareIt++) {
      g.grid[][] = squareIt;
    }
  }
}

/*
 *
 */
void Boltzmann::simulate(bool step)
{
  internal_grid_t::iterator rowIt;
  for(rowIt = grid.begin(); rowIt != grid.end(); rowIt++) {
    group_t::iterator squareIt;
    for(squareIt = rowIt->begin(); squareIt != rowIt->end(); squareIt++) {
      updateNode(rowIt, squareIt);
    }
  }
}

/*
 *
 */
void Boltzmann::updateNode(internal_grid_t::iterator row,
  group_t::iterator square)
{
  std::vector<int> digits(9, 0);

  //Check row
  group_t::iterator rowIt = row->begin();
  for(; rowIt != row->end(); rowIt++) {
    if(rowIt != square) {
      digits = rowIt->sum(digits); 
    }
  }
  
  //Check column, also counts the reference square (for multiple digits).
  internal_grid_t::iterator colIt = grid.begin();
  int pos = square - row->begin();
  for(; colIt != grid.end(); colIt++) {
    digits = colIt->at(pos).sum(digits); 
  }

  //Check quadrant
  digits = checkQuadrant(digits, row, square);

  //Update current failure offset and state
  square->update(digits, temperature);
}

/*
 *
 */
std::vector<int> Boltzmann::checkQuadrant(std::vector<int> digits,
  internal_grid_t::iterator row, group_t::iterator square)
{
  internal_grid_t::difference_type firstX, firstY;

  if(std::distance(row, grid.begin()) < 3) {
    firstY = 0;
  }
  else {
    firstY = 3;
  }

  if(std::distance(square, row->begin()) < 3) {
    firstX = 0;
  }
  else {
    firstX = 3;
  }

  internal_grid_t::iterator firstRow = grid.begin(), rowIt;
  std::advance(firstRow, firstY);

  for(rowIt = firstRow;
    std::distance(rowIt, grid.end()) % 3 && rowIt != firstRow;
    rowIt++) {
    group_t::iterator firstColumn = rowIt->begin(), columnIt;
    std::advance(firstColumn, firstX);

    for(columnIt = firstColumn;
      std::distance(square, rowIt->end()) % 3 && columnIt != firstColumn;
      columnIt++) {
      digits = columnIt->sum(digits);
    }
  }

  return(digits);
}