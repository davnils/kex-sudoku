#ifndef BOLTZMANN_H_
#define BOLTZMANN_H_

typedef uint8_t[9][9] grid_t;
typedef std::vector<Square> group_t;
typedef std::vector<group_t> internal_grid_t;

const uint8_t NOT_ASSIGNED = 0;
const int MAX_TEMPERATURE = 100;
const int ITERATIONS = 100000;

class Boltzmann
{
  public:
    Boltzmann(grid_t assigned);
    grid_t getGrid();
    void simulate(bool step);
    void updateNode(group_t & row, Square & square);

  private:
    internal_grid_t grid;
    float temperature;
};

#endif
