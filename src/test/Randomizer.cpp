#include "Randomizer.h"

/**
 * Sets the reference puzzle solution and a reduced puzzle.
 * @param solved Solution to puzzle.
 * @param reduced Actual puzzle.
 */
void Randomizer::reference(std::string solved, std::string reduced)
{
  this->solved = solved;
  this->reduced = reduced;
}

/**
 * Sets the number of clues to be withdrawn from the complete solution.
 * @param rate Number of clues, inverted.
 */
void Randomizer::setMutationRate(int rate)
{
  this->rate = rate;
}

/**
 * Generates a single puzzle.
 * @return New puzzle.
 */
std::string Randomizer::generateCandidate()
{
  std::string candidate = solved;

  for(int i = 0; i < rate; i++) {
    int pos;
    do {
      pos = rand() % candidate.size();
    } while(candidate[pos] == '0' || reduced[pos] != '0');

    candidate[pos] = '0';
  }

  return(candidate);
}
