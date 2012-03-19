#ifndef TESTFRAMEWORK_H_
#define TESTFRAMEWORK_H_

typedef struct
{
  std::string algorithm;
  float avg;
  std::vector<float> timeStamps;
} result_t;

class TestFramework
{
  public:
    TestFramework();
    void addSolver(SudokuSolver solver);
    std::vector<result_t> runTests();

  private:
    std::vector<SudokuSolver> solvers;
};

#endif
