#ifndef SQUARE_H_
#define SQUARE_H_

#include <cstdint>
#include <vector>

const int COLLISION_OFFSET = -2;
const int COLLISION_GIVEN_OFFSET = -20;
const float BIAS = 3.0f;

struct Node
{
  bool used;
  int offset;
};

class Square
{
  public:
    Square();
    Square(int digit);
    bool update(std::vector<int> values, float temperature);
    bool isResolved();
    void sum(std::vector<int> & values);
    uint8_t bestMatch();

  private:
    std::vector<Node> digits;
    uint8_t resolved;
};

#endif
