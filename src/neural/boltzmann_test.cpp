#include <cstdint>
#include <iostream>
#include <vector>

#include "Square.h"
#include "Boltzmann.h"

void dumpGrid(grid_t g)
{
  for(int i = 0; i < 9; i++) {
    for(int j = 0; j < 9; j++) {
      std::cout << g.grid[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

int main()
{
  grid_t g;

  for(int i = 0; i < 9; i++) {
    for(int j = 0; j < 9; j++) {
      g.grid[i][j] = 0;
    }
  }

  g.grid[2][0] = 8;
  g.grid[3][0] = 2;
  g.grid[8][0] = 5;
  g.grid[4][1] = 6;
  g.grid[6][1] = 1;
  g.grid[8][1] = 4;
  g.grid[0][2] = 9;
  g.grid[2][2] = 4;
  g.grid[4][2] = 1;
  g.grid[6][2] = 7;
  g.grid[8][2] = 8;
  g.grid[0][3] = 4;
  g.grid[6][3] = 5;
  g.grid[2][4] = 2;
  g.grid[3][4] = 5;
  g.grid[5][4] = 3;
  g.grid[7][4] = 8;
  g.grid[8][4] = 1;
  g.grid[0][5] = 5;
  g.grid[4][5] = 8;
  g.grid[7][5] = 7;
  g.grid[2][6] = 1;
  g.grid[3][6] = 9;
  g.grid[7][6] = 5;
  g.grid[3][7] = 1;
  g.grid[1][8] = 5;
  g.grid[2][8] = 9;
  g.grid[3][8] = 8;
  g.grid[5][8] = 2;
  g.grid[7][8] = 1;
  g.grid[8][8] = 3;

  Boltzmann solver(g);

  while(1) {
    solver.simulate(false);
    dumpGrid(solver.getGrid());
  }
}
