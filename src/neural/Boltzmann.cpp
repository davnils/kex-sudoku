#include "Boltzmann.h"
#include <iostream>

/**
 *
 */
Boltzmann::Boltzmann(float maxTemperature)
{
  temperature = maxTemperature;
}

/**
 *
 */
void Boltzmann::addPuzzle(grid_t puzzle)
{
  std::cout << "addPuzzle()\n";
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

  printGrid(getGrid());
}

/**
 *
 */
grid_t Boltzmann::getGrid()
{
  std::cout << "getGrid()\n";
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
 *
 */
bool Boltzmann::runStep(clock_t endTime)
{
  std::cout << "runStep()" << std::endl;
  do {
    std::cout << "New iteration with temperature: " << temperature << std::endl;
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
      std::cout << "Found valid puzzle" << std::endl;
      return(true);
    }

    temperature = std::max(temperature - dTEMPERATURE, MIN_TEMPERATURE);
    printGrid(getGrid());
  } while(clock() < endTime);

  return(false);
}

void Boltzmann::printDigits(std::vector<int> digits)
{
  for(int i = 0; i < 9; i++) {
    std::cout << digits[i] << " ";
  }
  std::cout << std::endl;
}

/**
 *
 */
void Boltzmann::updateNode(internal_grid_t::iterator row,
  group_t::iterator square)
{
  //std::cout << "updateNode()\n";
  std::vector<int> digits(9, 0);

  //Check row
  group_t::iterator rowIt = row->begin();
  for(; rowIt != row->end(); rowIt++) {
    if(rowIt != square) {
      digits = rowIt->sum(digits); 
    }
  }

  printDigits(digits);
  
  //Check column, doesn't count the reference square.
  internal_grid_t::iterator colIt = grid.begin();
  int pos = square - row->begin();
  for(; colIt != grid.end(); colIt++) {
    if(colIt->begin() + pos != square) {
      digits = colIt->at(pos).sum(digits); 
    }
  }

  printDigits(digits);

  //Check quadrant
  digits = checkQuadrant(digits, row, square);

  printDigits(digits);

  //Update current failure offset and state
  square->update(digits, temperature);
}

/**
 *
 */
std::vector<int> Boltzmann::checkQuadrant(std::vector<int> digits,
  internal_grid_t::iterator row, group_t::iterator square)
{
  //std::cout << "checkQuadrant()\n";
  internal_grid_t::difference_type firstX, firstY;

  firstX = (std::distance(row->begin(), square) / 3) * 3;
  firstY = (std::distance(grid.begin(), row) / 3) * 3;

  row = grid.begin() + firstY;

  for(int i = 0; i < 3; i++, row++) {
    square = row->begin() + firstX;
    for(int j = 0; j < 3; j++, square++) {
      digits = square->sum(digits);
    }
  }

  return(digits);
}
