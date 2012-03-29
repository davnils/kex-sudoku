#include "TestFramework.h"

#include <fstream>

/*
 * 
 */
TestFramework::TestFramework(std::string puzzlePath)
{
  this->puzzlePath = puzzlePath;
  readPuzzles();
}


/*
 * 
 */
void TestFramework::readPuzzles()
{
  this->puzzlePath = puzzlePath;
  std::ifstream input(puzzlePath, std::ifstream::in);

  std::string line;
  while(std::getline(input, line)) {
    grid_t puzzle;

    for(int i = 0; i < 81; i++) {
      puzzle.grid[i/9][i%9] = line[i];
    }

    puzzles.push_back(puzzle);
  }
}

/*
 * 
 */
void TestFramework::addSolver(SudokuSolver * solver)
{
  solvers.push_back(solver);
}

/*
 * 
 */
std::vector<result_t> TestFramework::runTests()
{
  std::vector<result_t> results;
  std::vector<SudokuSolver*>::iterator itSolver = solvers.begin();
  std::vector<grid_t>::iterator itPuzzle = puzzles.begin();

  for(; itSolver != solvers.end(); itSolver++) {
    result_t res;
    res.algorithm = (*itSolver)->getName();

    for(; itPuzzle != puzzles.end(); itPuzzle++) {
      res.timeStamps.push_back(runSampledSolver(*itSolver, *itPuzzle));
    }

    std::vector<float>::iterator it = res.timeStamps.begin();
    res.avg = 0;
    for(; it != res.timeStamps.end(); it++) {
      res.avg += *it / res.timeStamps.size();
    }
  }

  return(results);
}

/*
 * 
 */
float TestFramework::runSampledSolver(SudokuSolver * solver, grid_t puzzle)
{
  //Solve the given puzzle several times
  float avg, variance;
  solver->addPuzzle(puzzle);
  float runtime;
  //TODO: Start timer
  solver->runStep(true);
  //TODO: Stop timer
  return(avg);
}
