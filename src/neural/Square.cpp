#include <cmath>
#include <cstdlib>
#include <vector>

#include "Square.h"

Square::Square()
{
  digits.reserve(9);
  //TODO: Set all to false
  resolved = false;
}

Square::Square(int digit)
{
  digits.reserve(9);
  digits[digit-1] = True;
  //TODO: Set rest to false
  resolved = true;
}

void Square::update(std::vector<int> values, float temperature)
{
  std::vector<int>::const_iterator itValues = values.begin();
  std::vector<Node>::iterator itNodes = digits.begin();

  for(; itValues != values.end(); itValues++, itNodes++) {
    itNode->offset = *itValues;
    float probability = 1.0 / (1.0 + exp(-itNode->Offset/temperature));
    itNode->used = (rand() % 1000) < (probability * 1000);
  }
}

std::vector<int> Square::sum(std::vector<int> values)
{
  std::vector<int>::iterator itAcc = values.begin();
  std::vector<Node>::const_iterator itStored = digits.begin();

  for(; itAcc != values.end(); itAcc++, itStored++) {
    *itAcc += itStored->used * COLLISION_OFFSET;
  }

  return(values);
}

bool Square::isResolved()
{
  return(resolved);
}
