#include "Randomizer.h"

/**
 *
 */
void Randomizer::reference(std::string solved, std::string reduced)
{
  this->solved = solved;
  this->reduced = reduced;
}

/**
 *
 */
void Randomizer::setMutationRate(int rate)
{
  this->rate = rate;
}

/**
 *
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
