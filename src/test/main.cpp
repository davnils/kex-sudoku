#include <iostream>
#include <string>
#include <vector>

//#include "../cultural/Cultural.h"
//#include "../neural/Boltzmann.h"
#include "TestFramework.h"
#include "../rulebased/rulebased2/Rulebased.h"

/**
 *
 */
int main(int argc, char ** argv)
{
  std::string inPath = "../../analysis/backtrack/newreference0";
  std::string outPath = "../../analysis/backtrack/newout";

  if(argc == 2) {
    std::string suffix = argv[1];
    inPath += suffix;
    outPath += suffix;
    std::cout << "Using instance " << suffix << std::endl;
  }

  TestFramework framework(inPath, outPath);//../../testdata/minlex");
  
  //framework.addSolver(new Cultural);
  //framework.addSolver(new Boltzmann);
  framework.addSolver(new Rulebased);
  //...

  std::vector<result_t> res = framework.runTests();

  //TODO: Transform results and export to matlab compatible format
  return(0);
}
