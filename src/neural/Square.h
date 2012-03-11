#ifndef SQUARE_H_
#define SQUARE_H_

class Square
{
  public:
    Square();
    Square(int digit);
    update(std::vector<int> values, float temperature);
    bool isResolved();

  private:
    std::vector<int> digits;
};

#endif
