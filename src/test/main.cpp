#include <iostream>
#include <string>
#include <vector>

#include "../cultural/Cultural.h"
#include "TestFramework.h"

int main()
{
  TestFramework framework("../../testdata/minlex");
  
  framework.addSolver(new Cultural);
  //...

  std::vector<result_t> res = framework.runTests();

  //TODO: Transform results and export to matlab compatible format
  return(0);
}
