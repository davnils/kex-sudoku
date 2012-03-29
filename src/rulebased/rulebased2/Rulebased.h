#ifndef RULEBASED_H_
#define RULEBASED_H_
#include "Board.cpp"

class Rulebased{
    private:
        Board board;
        vector< vector<int> > findCombinations(vector<int> n,int r,int i);
    public:
        Rulebased(int [][9]);
        Rulebased(Board);
        string getName(){ return "RuleBasedSolver"; }
        Board getBoard(){ return board; }
        void printBoard();
        void printRegions();
        void solve(); 
        bool naked();
        bool naked(vector<int> * [] );
        bool single();
        int guess();
        int applyRules();
};




#endif
