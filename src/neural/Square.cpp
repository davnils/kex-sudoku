#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Square.h"

/**
 * Assigns all nodes to not be used.
 */
Square::Square()
{
  Node n = {false, 0};
  for(int i = 0; i < 9; i++) {
    digits.push_back(n);
  }

  resolved = 0;
}

/**
 * Assigns the current node be clamped at a given value.
 * @param digit Value to be used.
 */
Square::Square(int digit)
{
  for(int i = 0; i < 9; i++) {
    Node n = {false, 0};
    if(i == digit - 1) {
      n.used = true;
    }
    digits.push_back(n);
  }

  resolved = digit;
}

/**
 * Updates the current node with the given failure offsets.
 * @param values Current digit values for this collection of candidates.
 * @param temperature Current temperature.
 * @return Returns true on success.
 */
bool Square::update(std::vector<int> values, float temperature)
{
  std::vector<int>::const_iterator itValues = values.begin();
  std::vector<Node>::iterator itNode = digits.begin();
  bool conflict = false, used = false;

  for(; itValues != values.end(); itValues++, itNode++) {
    itNode->offset = *itValues + BIAS;
    float probability = 1.0 / (1.0 + exp(-itNode->offset/temperature));
    itNode->used = (rand() % 1000) < (probability * 1000);
  }

  return(true);
}

/**
 * Adds an offset for every collision with the current node.
 * @param values Accumulator used for failure offsets.
 */
void Square::sum(std::vector<int> & values)
{
  std::vector<int>::iterator itAcc = values.begin();
  std::vector<Node>::const_iterator itStored = digits.begin();

  if(resolved) {
    values[resolved - 1] += COLLISION_GIVEN_OFFSET;
  } 

  for(; itAcc != values.end(); itAcc++, itStored++) {
    if(itStored->used) {
      *itAcc += COLLISION_OFFSET;
    }
  }
}

/**
 * Checks if this square is resolved to a single digit.
 * @return True if clamped to a single value.
 */
bool Square::isResolved()
{
  return(resolved != 0);
}

/**
 * Returns the best matching digit for the current square.
 * @return Best matching digit.
 */
uint8_t Square::bestMatch()
{
  if(resolved) {
    return(resolved - 1);
  }
  for(int i = 0; i < 9; i++) {
    if(digits[i].used) {
      return(i);
    }
  }
  return(0);
}
