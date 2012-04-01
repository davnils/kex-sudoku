#include <iostream>
#include <string>
#include <vector>

//#include "../cultural/Cultural.h"
//#include "../neural/Boltzmann.h"
#include "TestFramework.h"
#include "../rulebased/rulebased2/Rulebased.h"

int main()
{
  TestFramework framework("../../testdata/sample10");//../../testdata/minlex");
  
  //framework.addSolver(new Cultural);
  //framework.addSolver(new Boltzmann);
  framework.addSolver(new Rulebased);
  //...

  std::vector<result_t> res = framework.runTests();

  //TODO: Transform results and export to matlab compatible format
  return(0);
}
