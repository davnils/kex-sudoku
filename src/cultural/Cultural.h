#ifndef CULTURAL_H_
#define CULTURAL_H_

typedef struct
{
  uint8_t grid[9][9];
} grid_t;

/*
 *
 */
class Cultural
{
  public:
    Cultural(grid_t g);
    void simulate();
    grid_t getGrid();

  private:
    void tournament();
    std::vector<Individual> population;
};

#endif
