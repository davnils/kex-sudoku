#ifndef BOARD_H_
#define BOARD_H_
#include<string>
#include<vector>
using namespace std;
class Board{
    public:
        vector<int> board[9][9];
        vector<int> * rows [9][9];
        vector<int> * columns [9][9];
        vector<int> * boxes [9][9];
        vector<int> * regions [27][9];
        void analysePossibilities();
        void setBoard(int [][9]);
        void printBoard();
        void printBoard(string);
        void printRegions();
        void printPossibilities();
        void remove(int,int);
        void remove(vector<int> * [],int);
        bool valid();
};

#endif
