#ifndef RULEBASED_H_
#define RULEBASED_H_
#include "Board.h"
#include "../../test/SudokuSolver.h"

class Rulebased:public SudokuSolver{
    private:
        Board board;
        vector< vector<int> > findCombinations(vector<int> n,int r,int i);
    public:
        Rulebased(){};
        Rulebased(int [][9]);
        Rulebased(Board);
        void addPuzzle(grid_t);  
        grid_t getGrid();
        string getName(){ return "RuleBasedSolver"; }
        Board getBoard(){ return board; }
        void runStep(bool);
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
