#ifndef BOLTZMANN_H_
#define BOLTZMANN_H_

#include <vector>
#include <cstdint>

#include "Square.h"
#include "../test/SudokuSolver.h"

const uint8_t NOT_ASSIGNED = 0;
const int MAX_TEMPERATURE = 100;
const int ITERATIONS = 100000;

typedef std::vector<Square> group_t;
typedef std::vector<group_t> internal_grid_t;

/*
 *
 */
class Boltzmann : public SudokuSolver
{
  public:
    Boltzmann(float maxTemperature = MAX_TEMPERATURE);
    void addPuzzle(grid_t puzzle);
    grid_t getGrid();
    std::string getName() { return("Boltzmann machine"); }
    void runStep(bool forever);

  private:
    std::vector<int> checkQuadrant(std::vector<int> digits,
        internal_grid_t::iterator row, group_t::iterator square);
    void updateNode(internal_grid_t::iterator row,
        group_t::iterator square);

    internal_grid_t grid;
    float temperature;
};

#endif
