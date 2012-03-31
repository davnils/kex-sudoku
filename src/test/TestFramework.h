#ifndef TESTFRAMEWORK_H_
#define TESTFRAMEWORK_H_

#include <string>
#include <vector>

#include "SudokuSolver.h"

const long MAX_TRIES = 1000;
const long MIN_MEASUREMENT = 4;
const float UNSTABLE_MEASUREMENT = -1;
const float STD_DEVIATION_LIMIT = 2.0f;

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
    float sampledStdDeviation(const std::vector<float> & data, float avg);
    float sampledAverage(const std::vector<float> & data);

    std::vector<SudokuSolver*> solvers;
    std::vector<grid_t> puzzles;
    std::string puzzlePath;
};

#endif
