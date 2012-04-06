#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

#include "TestFramework.h"
#include "Randomizer.h"

/**
 * 
 */
TestFramework::TestFramework(std::string puzzlePath, std::string matlabPath) : of(matlabPath)
{
  this->puzzlePath = puzzlePath;
}

TestFramework::~TestFramework()
{
    of.close();
}

/**
 * 
 */
void TestFramework::readPuzzles(SudokuSolver * solver)
{
  puzzles.clear();
  this->puzzlePath = puzzlePath;
  std::ifstream input(puzzlePath, std::ifstream::in);
  std::vector<std::string> lines;

  std::string line;
  while(std::getline(input, line)) {
    std::string solved;
    std::getline(input, solved);

    if(solver->reducedComplexity()) {
      Randomizer r;
      r.reference(solved, line);
      r.setMutationRate(solver->puzzleComplexity());
      for(int i = 0; i < solver->puzzleFactor(); i++) {
        lines.push_back(r.generateCandidate());
      }
    }
    else {
      lines.push_back(line);
    }
  }

  std::vector<std::string>::iterator it;
  for(it = lines.begin(); it != lines.end(); it++) {
    grid_t puzzle;
    for(int i = 0; i < 81; i++) {
      puzzle.grid[i/9][i%9] = (*it)[i] - '0';
    }
    puzzles.push_back(puzzle);
  }
}

/**
 * 
 */
void TestFramework::addSolver(SudokuSolver * solver)
{
  solvers.push_back(solver);
}

/**
 * 
 */
std::vector<result_t> TestFramework::runTests()
{
  std::vector<result_t> results;
  std::vector<SudokuSolver*>::iterator itSolver = solvers.begin();

  for(; itSolver != solvers.end(); itSolver++) {
    result_t res;
    res.algorithm = (*itSolver)->getName();
    res.unstableCount = res.unsolvedCount = 0;
    of << res.algorithm << " = [";

    readPuzzles(*itSolver);
    std::vector<grid_t>::iterator itPuzzle = puzzles.begin();

    for(; itPuzzle != puzzles.end(); itPuzzle++) {
      float result = runSampledSolver(*itSolver, *itPuzzle);
      if(result > 0) {
        res.timeStamps.push_back(result);
        of << res.timeStamps.back() << " ";
        of.flush();
      }
      else {
        std::cerr << "Warning: Invalid measurement with solver: "
          << res.algorithm << ", code: " << result << std::endl;
        if(result == UNSTABLE_MEASUREMENT) {
            res.unstableCount++;
        }
        else {
            res.unsolvedCount++;
        }
      }
    }

    of << "];\n";

    res.avg = sampledAverage(res.timeStamps);
    std::cerr << "Unstable measurements: "
        << res.unstableCount
        << ", Unsolved puzzles: " << res.unsolvedCount
        << ", total: " << puzzles.size() << std::endl;
  }

  return(results);
}

/**
 * 
 */
float TestFramework::runSampledSolver(SudokuSolver * solver, grid_t puzzle)
{
  std::vector<float> samples;
  long measurement;

  for(measurement = 1; measurement <= MAX_TRIES; measurement++) {
    std::cout << "Running measurement #" << measurement << std::endl;
    float runtime;
    solver->addPuzzle(puzzle);

    clock_t reference = clock();
    bool ret = solver->runStep(clock()+CLOCKS_PER_SEC*MAX_EXECUTION_TIME);

    if(!ret) {
      return(NO_SOLUTION_FOUND);
    }

    runtime = (clock() - reference)/(float)CLOCKS_PER_SEC;
    samples.push_back(runtime);

    float avg = sampledAverage(samples);
    if(bootstrap(samples, CONFIDENCE) && measurement >= MIN_MEASUREMENT) {
      return(avg);
    }
  }

  return(UNSTABLE_MEASUREMENT);
}

bool compareFloat(const float a, const float b)
{
  return(a < b);
}

/**
 * Percentile bootstrap implementation.
 * www.public.iastate.edu/~vardeman/stat511/BootstrapPercentile.pdf
 */
bool TestFramework::bootstrap(std::vector<float> data, float confidence)
{
  if(data.size() <= 1) {
    return(false);
  }

  std::sort(data.begin(), data.end(), compareFloat);

  float inv = 1 - confidence;
  float firstPercentile = (inv / 2)*(data.size() - 1);
  float secondPercentile = ((1 - inv / 2))*(data.size() - 1);

  if(data[round(secondPercentile)] - data[round(firstPercentile)]
      <= BOOTSTRAP_INTERVAL) {
    return(true);
  }

  return(false);
}

/**
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

/**
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

    return(avg);
}
