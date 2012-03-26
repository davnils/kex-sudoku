#include "TestFramework.h"

/*
 * 
 */
TestFramework::TestFramework(std::string puzzlePath)
{
  this->puzzlePath = puzzlePath;
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
  //Read puzzles
  //Solve puzzles with every solver
  //Gather results
}

/*
 * 
 */
result_t TestFramework::runSampledSolver(SudokuSolver * solver, grid_t puzzle)
{
  //Solve the given puzzle several times
  std::vector<float> measurements;
  float avg, variance;
}
