#ifndef RANDOMIZER_H_
#define RANDOMIZER_H_

#include <string>

/**
 * Randomizer provides functionality for randomly generating Sudokus.
 */
class Randomizer
{
  public:
    Randomizer() {};
    void reference(std::string solved, std::string reduced);
    void setMutationRate(int rate);
    std::string generateCandidate();

  private:
    std::string solved, reduced;
    int rate;
};

#endif
