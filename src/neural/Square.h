#ifndef SQUARE_H_
#define SQUARE_H_

const int COLLISION_OFFSET = -2;
const int COLLISION_GIVEN_OFFSET = -20;

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
    void update(std::vector<int> values, float temperature);
    bool isResolved();
    std::vector<int> sum(std::vector<int> values);

  private:
    std::vector<Node> digits;
    bool resolved;
};

#endif
