#include "Cultural.h"
#include <pair>

/*
 *
 */
Cultural::Cultural()
{
}

/*
 *
 */
void Cultural::addPuzzle(grid_t puzzle)
{
  grid = puzzle;

  for(int i = 0; i < POPULATION_SIZE; i++) {
    Individual i(puzzle);
    population.push_back(i);
  }
}

/*
 *
 */
grid_t Cultural::getGrid()
{
  return(grid);
}

bool compareFitness(const std::pair<const unsigned int, const Individual &> a, const std::pair<const unsigned int, const Individual &> b)
{
  return(a.first < b.first);
}

/*
 *
 */
bool Cultural::runStep(clock_t endTime)
{
  do {
    //sort by fitness
    std::vector<std::pair<unsigned int, Individual &>> fitness;
    std::vector<Individual>::iterator it;
    for(it = population.begin(); it != population.end(); it++) {
      if(it->fitness() == 0) {
        return(true);
      }
      fitness.push_back(std::make_pair(it->fitness, *it));
    }
    std::sort(fitness.begin(), fitness.end(), compareFitness);

    //TODO: crossover during tournament
    std::vector<Individual &> offspring;
    //mutate the resulting individuals
    std::vector<Individual & >::iterator itOffspring;
    for(itOffspring = offspring.begin(); itOffspring != offspring.end();
      itOffspring++) {
      itOffspring->mutate();
    }
  } while(clock() < endTime);

  return(false);
}

/*
 *
 */
void Cultural::tournament()
{
}
