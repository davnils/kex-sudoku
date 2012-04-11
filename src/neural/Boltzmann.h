#ifndef BOLTZMANN_H_
#define BOLTZMANN_H_

#include <vector>
#include <cstdint>

#include "Square.h"
#include "../test/SudokuSolver.h"

const int MAX_TEMPERATURE = 100; /* Maximum temperature */
const float dTEMPERATURE = -0.000035; /* Simulated annealing constant */
const float MIN_TEMPERATURE = 0.001; /* Minimum temperature ever reached */
const int REDUCED_PUZZLE_RATE = 35; /* Number of clues to draw from a completely solved puzzle */
const int REDUCED_PUZZLE_FACTOR = 4; /* Number of puzzles to generate from every input puzzle pair */

typedef std::vector<Square> group_t;
typedef std::vector<group_t> internal_grid_t;

/*
 * Boltzmann implements the main structure of a Boltzmann machine.
 */
class Boltzmann : public SudokuSolver
{
  public:
    Boltzmann();
    void addPuzzle(grid_t puzzle);
    grid_t getGrid();
    std::string getName() { return("Boltzmann machine"); }
    bool runStep(clock_t endTime);
    bool reducedComplexity() { return(true); }
    int puzzleComplexity() { return(REDUCED_PUZZLE_RATE); }
    int puzzleFactor() { return(REDUCED_PUZZLE_FACTOR); }

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
