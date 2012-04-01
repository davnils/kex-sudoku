#include "TestFramework.h"

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

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
      puzzle.grid[i/9][i%9] = line[i] - '0';
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

    res.avg = sampledAverage(res.timeStamps);
  }

  return(results);
}

/*
 * 
 */
float TestFramework::runSampledSolver(SudokuSolver * solver, grid_t puzzle)
{
  std::cout << "runSampledSolver with new puzzle\n";
  std::vector<float> samples;
  long measurement;

  for(measurement = 0; measurement < MAX_TRIES; measurement++) {
    std::cout << "Running measurement #" << measurement << std::endl;
    float runtime;
    solver->addPuzzle(puzzle);

    clock_t reference = clock();
    solver->runStep(true);
    runtime = (clock() - reference)/(float)CLOCKS_PER_SEC;
    samples.push_back(runtime);

    float avg = sampledAverage(samples);
    //std::cout << "(avg, stddev) = (" << avg << ", "
      //<< sampledStdDeviation(samples, avg) << ")" << std::endl;
    if(sampledStdDeviation(samples, avg) / avg <= STD_DEVIATION_LIMIT
        && measurement >= MIN_MEASUREMENT) {
      std::cout << "Valid measurement performed.\n";
      return(avg);
    }
  }

  return(UNSTABLE_MEASUREMENT);
}

/*
 * 
 */
float TestFramework::sampledStdDeviation(const std::vector<float> & data, float avg)
{
  std::vector<float>::const_iterator it;
  float variance = 0;
  for(it = data.begin(); it != data.end(); it++) {
    variance += pow(*it - avg, 2);
  }

  if(data.size() > 1) {
    variance /= data.size() - 1;
  }
  else {
    variance = 0;
  }

  
  return(sqrt(variance));
}

/*
 * 
 */
float TestFramework::sampledAverage(const std::vector<float> & data)
{
    std::vector<float>::const_iterator it;
    float avg = 0.0f;

    if(data.size() == 0) {
        return(0);
    }

    for(it = data.begin(); it != data.end(); it++) {
      avg += *it / data.size();
    }
}
