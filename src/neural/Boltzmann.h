#ifndef BOLTZMANN_H_
#define BOLTZMANN_H_

typedef struct
{
  uint8_t grid[9][9];
} grid_t;

typedef std::vector<Square> group_t;
typedef std::vector<group_t> internal_grid_t;

const uint8_t NOT_ASSIGNED = 0;
const int MAX_TEMPERATURE = 100;
const int ITERATIONS = 100000;

/*
 *
 */
class Boltzmann
{
  public:
    Boltzmann(grid_t assigned, float maxTemperature = MAX_TEMPERATURE);
    grid_t getGrid();
    void simulate(bool step);

  private:
    std::vector<int> checkQuadrant(std::vector<int> digits,
      internal_grid_t::const_iterator row, internal_grid_t::iterator square);
    void updateNode(internal_grid_t::iterator row,
      group_t::iterator square);

    internal_grid_t grid;
    float temperature;
};

#endif
