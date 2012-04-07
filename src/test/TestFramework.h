#ifndef TESTFRAMEWORK_H_
#define TESTFRAMEWORK_H_

#include <fstream>
#include <string>
#include <vector>

#include "SudokuSolver.h"

const long MAX_TRIES = 100;
const long MIN_MEASUREMENT = 4;
const float STD_DEVIATION_LIMIT = 0.1f;
const clock_t MAX_EXECUTION_TIME = 20;
const float UNSTABLE_MEASUREMENT = -1;
const float NO_SOLUTION_FOUND = -2;
const float CONFIDENCE = 0.95;
const float BOOTSTRAP_INTERVAL = 0.01f;

/**
 * 
 */
typedef struct
{
  std::string algorithm;
  float avg;
  unsigned int unstableCount, unsolvedCount;
  std::vector<float> timeStamps;
} result_t;

/**
 * 
 */
class TestFramework
{
  public:
    TestFramework(std::string puzzlePath, std::string matlabPath);
    ~TestFramework();
    void addSolver(SudokuSolver * solver);
    std::vector<result_t> runTests();

  private:
    void readPuzzles(SudokuSolver * solver);
    float runSampledSolver(SudokuSolver * solver, grid_t puzzle);
    float sampledStdDeviation(const std::vector<float> & data, float avg);
    float sampledAverage(const std::vector<float> & data);
    bool bootstrap(std::vector<float> data, float confidence);

    std::vector<SudokuSolver*> solvers;
    std::vector<grid_t> puzzles;
    std::string puzzlePath;
    std::ofstream of;
};

#endif
