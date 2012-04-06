#ifndef BOLTZMANN_H_
#define BOLTZMANN_H_

#include <vector>
#include <cstdint>

#include "Square.h"
#include "../test/SudokuSolver.h"

const uint8_t NOT_ASSIGNED = 0;
const int MAX_TEMPERATURE = 100;
const float dTEMPERATURE = 0.0004;
const float MIN_TEMPERATURE = 0.01;

typedef std::vector<Square> group_t;
typedef std::vector<group_t> internal_grid_t;

/*
 *
 */
class Boltzmann : public SudokuSolver
{
  public:
    Boltzmann();
    void addPuzzle(grid_t puzzle);
    grid_t getGrid();
    std::string getName() { return("Boltzmann machine"); }
    bool runStep(clock_t endTime);

  private:
    void printGrid(grid_t g);
    void printDigits(std::vector<int> digits);
    void reset();
    std::vector<int> checkQuadrant(std::vector<int> digits,
        internal_grid_t::iterator row, group_t::iterator square);
    bool updateNode(internal_grid_t::iterator row,
        group_t::iterator square);

    internal_grid_t grid;
    float temperature;
};

#endif
