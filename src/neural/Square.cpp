#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Square.h"

/**
 *
 */
Square::Square()
{
  Node n = {false, 0};
  for(int i = 0; i < 9; i++) {
    digits.push_back(n);
  }

  resolved = false;
}

/**
 *
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

  resolved = true;
}

/**
 *
 */
void Square::update(std::vector<int> values, float temperature)
{
  std::vector<int>::const_iterator itValues = values.begin();
  std::vector<Node>::iterator itNode = digits.begin();

  std::cout << "use vector: ";
  for(; itValues != values.end(); itValues++, itNode++) {
    itNode->offset = *itValues + BIAS;
    float probability = 1.0 / (1.0 + exp(-itNode->offset/temperature));
    itNode->used = (rand() % 1000) < (probability * 1000);
    std::cout << itNode->used << " (" << probability << ") ";
  }
  std::cout << std::endl;
}

/**
 *
 */
std::vector<int> Square::sum(std::vector<int> values)
{
  std::vector<int>::iterator itAcc = values.begin();
  std::vector<Node>::const_iterator itStored = digits.begin();

  for(; itAcc != values.end(); itAcc++, itStored++) {
    if(isResolved()) {
      *itAcc += itStored->used * COLLISION_GIVEN_OFFSET;
    } 
    else {
      *itAcc += itStored->used * COLLISION_OFFSET;
    } 
  }

  return(values);
}

/*
 *
 */
bool Square::isResolved()
{
  return(resolved);
}

/*
 *
 */
uint8_t Square::bestMatch()
{
  for(int i = 0; i < 9; i++) {
    if(digits[i].used) {
      return(i);
    }
  }
  return(0);
}
