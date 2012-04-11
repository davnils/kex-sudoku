#include "Boltzmann.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

/**
 * Resets the current state.
 */
Boltzmann::Boltzmann()
{
  reset();
}

/**
 * Randomizes the RNG and performs a reset.
 */
void Boltzmann::reset()
{
  srand(time(0));
  temperature = MAX_TEMPERATURE;
  grid.clear();
}

/**
 * Adds a puzzle to be solved.
 * @puzzle Puzzle to be solved.
 */
void Boltzmann::addPuzzle(grid_t puzzle)
{
  reset();
  for(int i = 0; i < 9; i++) {
    group_t row;
    for(int j = 0; j < 9; j++) {
      if(puzzle.grid[i][j] == 0) {
        row.push_back(Square());
      }
      else {
        row.push_back(Square(puzzle.grid[i][j]));
      }
    }
    grid.push_back(row);
  }
}

/**
 * Returns the current grid.
 * @return Current grid.
 */
grid_t Boltzmann::getGrid()
{
  grid_t g;
  internal_grid_t::iterator rowIt;
  for(rowIt = grid.begin(); rowIt != grid.end(); rowIt++) {
    group_t::iterator squareIt;
    for(squareIt = rowIt->begin(); squareIt != rowIt->end(); squareIt++) {
      int first = std::distance(grid.begin(), rowIt);
      int second = std::distance(rowIt->begin(), squareIt);
      g.grid[first][second] = squareIt->bestMatch() + 1;
    }
  }

  return(g);
}

void Boltzmann::printGrid(grid_t g)
{
  for(int i = 0; i < 9; i++) {
    for(int j = 0; j < 9; j++) {
      std::cout << (char)(g.grid[i][j] + '0') << " ";
    }
    std::cout << std::endl;
  }
}

/**
 * Runs until a given deadline.
 * @param endTime clock() deadline.
 * @return True on solving success.
 */
bool Boltzmann::runStep(clock_t endTime)
{
  unsigned long iteration = 0;
  do {
    internal_grid_t::iterator rowIt;
    for(rowIt = grid.begin(); rowIt != grid.end(); rowIt++) {
      group_t::iterator squareIt;
      for(squareIt = rowIt->begin(); squareIt != rowIt->end(); squareIt++) {
        if(!squareIt->isResolved()) {
          updateNode(rowIt, squareIt);
        }
      }
    }

    if(isValidSolution(getGrid())) {
      return(true);
    }

    iteration++;
    temperature = std::max((float)(MAX_TEMPERATURE*exp(dTEMPERATURE*iteration)), MIN_TEMPERATURE);
  } while(clock() < endTime);

  return(false);
}

/**
 * Updates a single grid node.
 * @param row Current row.
 * @param square Current grid node.
 * @return True on success.
 */
bool Boltzmann::updateNode(internal_grid_t::iterator row,
  group_t::iterator square)
{
  std::vector<int> digits(9, 0);

  //Check row
  group_t::iterator rowIt = row->begin();
  for(; rowIt != row->end(); rowIt++) {
    if(rowIt != square) {
      rowIt->sum(digits); 
    }
  }

  //Check column, doesn't count the reference square.
  internal_grid_t::iterator colIt = grid.begin();
  int pos = square - row->begin();
  for(; colIt != grid.end(); colIt++) {
    if(colIt->begin() + pos != square) {
      colIt->at(pos).sum(digits); 
    }
  }

  //Check quadrant
  digits = checkQuadrant(digits, row, square);

  //Update current failure offset and state
  return(square->update(digits, temperature));
}

/**
 * Checks a single quadrant for conflicts.
 * @param digits Current accumulator of digits offsets.
 * @param row Current row.
 * @param square Current grid node.
 * @return Updated accumulator with added offsets.
 */
std::vector<int> Boltzmann::checkQuadrant(std::vector<int> digits,
  internal_grid_t::iterator row, group_t::iterator square)
{
  internal_grid_t::difference_type firstX, firstY;

  firstX = (std::distance(row->begin(), square) / 3) * 3;
  firstY = (std::distance(grid.begin(), row) / 3) * 3;

  row = grid.begin() + firstY;

  for(int i = 0; i < 3; i++, row++) {
    square = row->begin() + firstX;
    for(int j = 0; j < 3; j++, square++) {
      square->sum(digits);
    }
  }

  return(digits);
}
