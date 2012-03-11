#ifndef BOLTZMANN_H_
#define BOLTZMANN_H_

typedef uint8_t[9][9] grid_t;
typedef std::vector<std::vector<Square>> internal_grid_t;
const uint8_t NOT_ASSIGNED = 0;

class Boltzmann
{
  public:
    Boltzmann(grid_t assigned);
    grid_t getGrid();
    void simulate(bool step);

  private:
    internal_grid_t grid;
};

#endif
