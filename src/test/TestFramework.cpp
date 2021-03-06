#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

#include "TestFramework.h"
#include "Randomizer.h"

/**
 * Setups a test framework with file paths.
 * @param puzzlePath Path to puzzles.
 * @param matlabPath Path for matlab output.
 */
TestFramework::TestFramework(std::string puzzlePath, std::string matlabPath) : of(matlabPath)
{
  this->puzzlePath = puzzlePath;
}

/**
 * Destructor that closes the associated output file.
 */
TestFramework::~TestFramework()
{
    of.close();
}

/**
 * Reads all puzzles associated with a given solver.
 * @param solver Solver to be used.
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
 * Adds a solver for future testing.
 * @param solver Solver to be added.
 */
void TestFramework::addSolver(SudokuSolver * solver)
{
  solvers.push_back(solver);
}

/**
 * Runs all tests.
 * @return All time measurements.
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
      res.timeStamps.push_back(result);
      of << res.timeStamps.back() << " ";
      of.flush();
      if(result < 0) {
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
 * Solves a given puzzle repeatedly and returns an average.
 * @param solver Solver to be used.
 * @param puzzle Puzzle to be solved.
 * @return Average running time.
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
 * See: www.public.iastate.edu/~vardeman/stat511/BootstrapPercentile.pdf
 * @param data Values to be used in calculation.
 * @param confidence Confidence level used in estimation.
 * @return Boolean indicating boostrap success.
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
 * Calculates sampled standard deviation.
 * @param data Samples to be used.
 * @param avg Average value of samples.
 * @return standard deviation.
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
 * Calculates average of all positive items.
 * @param data Samples to be used in calculation.
 * @return Average of samples.
 */
float TestFramework::sampledAverage(const std::vector<float> & data)
{
    std::vector<float>::const_iterator it;
    std::vector<float> filtered;
    float avg = 0.0f;

    if(data.size() == 0) {
        return(0);
    }

    for(it = data.begin(); it != data.end(); it++) {
      if(*it >= 0) {
        filtered.push_back(*it);
      }
    }

    for(it = filtered.begin(); it != filtered.end(); it++) {
      avg += *it / filtered.size();
    }

    return(avg);
}
