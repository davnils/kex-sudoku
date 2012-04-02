#include "TestFramework.h"

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

/*
 * 
 */
TestFramework::TestFramework(std::string puzzlePath, std::string matlabPath) : of(matlabPath)
{
  this->puzzlePath = puzzlePath;
  readPuzzles();
}

TestFramework::~TestFramework()
{
    of.close();
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
    of << res.algorithm << " = [";

    for(; itPuzzle != puzzles.end(); itPuzzle++) {
      res.timeStamps.push_back(runSampledSolver(*itSolver, *itPuzzle));
      of << res.timeStamps.back() << " ";
    }
    
    of << "];\n";

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
    //std::cout<<"clock(): "<<clock()<<" clocks-per-sc"<<CLOCKS_PER_SEC
    //    <<std::endl;
    bool ret = solver->runStep(clock()+CLOCKS_PER_SEC*MAX_EXECUTION_TIME);
    runtime = (clock() - reference)/(float)CLOCKS_PER_SEC;
    //TODO Om ret är false så ska väl pusslet markeras som unsolved.
    //Det går ju inte att veta saker som mean och std om
    //lösaren en gång misslyckas så då måste vi istället markera det som unsolvable.
    //NO_SOLUTION_FOUND är väl dessutom ett negativt tal så det kommer verkligen
    //göra average funktionen konstig som den ser ut nu. Ingen koll finns. Dock
    //så är det som sagt ändå inte så lyckat eftersom man inte bara kan ta bort
    //okänd data.
    samples.push_back(ret ? runtime : NO_SOLUTION_FOUND);

    float avg = sampledAverage(samples);
    //std::cout << "(avg, stddev) = (" << avg << ", "
      //<< sampledStdDeviation(samples, avg) << ")" << std::endl;
    //TODO Ändrade ifsatsen här. I och med att vi ändå inte kan räkna ut
    //konfidenceintervall för tillfället så känns det inte så meningsfullt
    //att sätta upp en randomvillkor för avg. Så nu körs testet 4a ggr för alla.
    if(true//bootstrap(samples,CONFIDENCE)//sampledStdDeviation(samples, avg) / avg <= STD_DEVIATION_LIMIT
        && measurement >= MIN_MEASUREMENT) {
      std::cout << "Valid measurement performed.\n";
      return(avg);
    }
  }

  return(UNSTABLE_MEASUREMENT);
}

bool TestFramework::bootstrap(const std::vector<float> & data, float confidence){
    std::vector<float> meanvalues;
    int n = data.size();
    std::vector<int> tmp;
    for(int i=0;i<n;i++){
        tmp.push_back(0);
    }
    for(int testfall=0;testfall<100;testfall++){
        for(int i=0;i<n;i++){
            int r = rand()%n;
            tmp[i] = data[r];
        }
        int sum = 0;
        for(int i=0;i<n;i++){
            sum += tmp[i];
        }
        float avg = sum/n;
        meanvalues.push_back(avg); 
    }
    //TODO calculate the confidence and compare to the required
    //confidence. Return true/false depending on the result.
    //One problem seems to be that with n only equal to 4
    //the bootstrapping will have a hard time producing high
    //confidence. This is because the unknown distribution is
    //poorly represented by the four datapoints.

    
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
    //TODO Ska det inte vara return avg här?
}
