#include <iostream>
#include <string>
#include <vector>

#include "Boltzmann.h"
//...

#include "TestFramework.h"


int main()
{
  TestFramework framework;
  
  framework.addSolver(new Boltzmann);
  //...

  std::vector<result_t> res = framework.runTests();

  //TODO: Transform results and export to matlab compatible format
  return(0);
}
