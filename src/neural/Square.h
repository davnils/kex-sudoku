#ifndef SQUARE_H_
#define SQUARE_H_

#include <cstdint>
#include <vector>

/* Offset added to colliding nodes */
const int COLLISION_OFFSET = -2;
/* Offset added to colliding nodes if already resolved */
const int COLLISION_GIVEN_OFFSET = -20;
/* Bias value used in offset calculation */
const float BIAS = 3.0f;

/**
 * Node describes a possible candidate for the current grid position.
 */
struct Node
{
  bool used;
  int offset;
};

/**
 * Square class describes a single Sudoku grid value.
 */
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
