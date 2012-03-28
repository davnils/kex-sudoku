#ifndef TESTFRAMEWORK_H_
#define TESTFRAMEWORK_H_

#include <string>
#include <vector>

#include "SudokuSolver.h"

/*
 * 
 */
typedef struct
{
  std::string algorithm;
  float avg;
  std::vector<float> timeStamps;
} result_t;

/*
 * 
 */
class TestFramework
{
  public:
    TestFramework(std::string puzzlePath);
    void addSolver(SudokuSolver * solver);
    std::vector<result_t> runTests();

  private:
    void readPuzzles();
    float runSampledSolver(SudokuSolver * solver, grid_t puzzle);

    std::vector<SudokuSolver*> solvers;
    std::vector<grid_t> puzzles;
    std::string puzzlePath;
};

#endif
