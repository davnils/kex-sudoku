#include<vector>
#include<string>
#include<iostream>
#include<ctime>
#include "Rulebased.h"

using namespace std;

/**
 * Constructor which initialises the puzzle with the
 * given grid.
 * @param grid is the 9 by 9 puzzle.
 */
Rulebased::Rulebased(int grid[9][9]){
    board = *(new Board());
    board.setBoard(grid);
}

/**
 * Changes the puzzle to the new puzzle given.
 * @param puzzle describes the new puzzle and is a 9 by 9 int arra.
 */ 
void Rulebased::addPuzzle(grid_t puzzle){
    int newgrid[9][9];
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            newgrid[i][j] = puzzle.grid[i][j];
        }
    }
    board = *(new Board());
    board.setBoard(newgrid);
}  

/**
 * Returns the board in an u_int8 9 by 9 array (grid_t).
 * @return the board used.
 */
grid_t Rulebased::getGrid(){
    grid_t returngrid;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            returngrid.grid[i][j] = (uint8_t) board.board[i][j][0];
        }
    }
    return returngrid;
} 

/**
 * Constructor of the class Rulebased
 * Creates a solver with the specified puzzle.
 * Note that it copies the board as to avoid multiple
 * solvers using the same board.
 * @param b is the board the solver will use.
 */
Rulebased::Rulebased(Board b){
    board = b;
}

/**
 * Solves the puzzle and returns true if succesfull.
 * There is also a timelimit which must be hold.
 * @param the endtime which the solver must not exceed.
 * @return true if solved within the timelimit and false otherwise.
 */
bool Rulebased::runStep(clock_t stoppTime){
    endTime = stoppTime;
    return solve();
}

/**
 * Solves the puzzle stored in the solver
 * within the endtime that is also stored within the solver.
 * @returns true if the puzzle was solved within the specified time.
 */
bool Rulebased::solve(){
    int solutions = applyRules();
    if(solutions == 0){
        return false;
    }else{
        //board.printBoard("SIMPLE");
        return true;
    }
    /*
    if(solutions >= 1 && board.valid()){
    	cout<<"solutions: "<<solutions<<endl;
        board.printBoard();
        cout<<endl;
    }else{
	    cout<<"UNSOLVED"<<endl;
        board.printBoard();
    }
    */
}

/**
 * Applies the rules that solves the puzzles.
 * Consideres the endingtime for solutions and returns if this time is exceeded.
 * @return the number of solutions
 */ 
int Rulebased::applyRules(){
    if(clock()>endTime){
        return 0;
    }
    /*
    while(true){
        //The easy rules first.
        if(single())
            continue;
        if(naked())
            continue;
        break;
    }
*/
    return guess();
} 

/*
    Returns 1 if unique solution was found
    Returns 0 if none solution exists
    Returns >1 if more than one solution exists
*/
int Rulebased::guess(){
    //Find square with least possibilities
    int min[3] = {100,0,0};//[min,i,j]
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(     board.board[i][j][0]==0 &&
                    min[0]>board.board[i][j].size()){
                min[0]=board.board[i][j].size();
                min[1]=i;min[2]=j;
            }
        }
    }
    /*
    cout<<"Minsta:  size: "<<min[0]<<" i: "<<min[1]<<" j: "<<min[2]<<endl;
    cout<<"Possibilities: ";
    for(int i=0;i<board.board[min[1]][min[2]].size();i++){
        cout<<board.board[min[1]][min[2]][i]<<" ";
    }
    cout<<endl;
    board.printPossibilities();
    */
    if(min[0]==100){
        return 1;
    }
    if(board.board[min[1]][min[2]].size()==1){
        return 0;
    }
    vector<Board> correctGuesses;
    for(int g_index=1;g_index<
                board.board[min[1]][min[2]].size();g_index++){

        int g = board.board[min[1]][min[2]][g_index];
        //cout<<endl<<"Guess"<<g<<endl;
        Board tmp;
        tmp.operator=(board);
        vector<int> * tmpvector = &tmp.board[min[1]][min[2]];
        /*
        cout<<"board"<<&board<<endl;
        cout<<"tmp"<<&tmp<<endl;
        cout<<"board.board: "<<&board.board[min[1]][min[2]]<<endl;
        cout<<"board.regions: "<<board.regions[min[1]][min[2]]<<endl;
        cout<<"&tmp.board: "<<&tmp.board[min[1]][min[2]]<<endl;
        cout<<"tmp.region: "<<tmp.regions[min[1]][min[2]]<<endl;
        cout<<"tmpvector: "<<tmpvector<<endl;
        */
        //cout<<"guessändringscheck";
        //tmp.printPossibilities();
        (*tmpvector)[0] = g;
        //tmp.printPossibilities();
        (*tmpvector).erase((*tmpvector).begin()+1,(*tmpvector).end());
        //cout<<"before and after remove"<<endl;
        //tmp.printPossibilities();
        //cout<<endl;
        tmp.remove(min[1],min[2]);
        //tmp.printPossibilities();
        Rulebased solver(tmp);
        solver.setTime(endTime);
        int ok = solver.applyRules();
        if(ok>0){
           correctGuesses.push_back(solver.getBoard());
        }
    }
    if(correctGuesses.size()==0){
        return 0;
    }else{
        board.operator=(correctGuesses[0]);
        /*
        board.board[min[1]][min[2]].clear();
        board.board[min[1]][min[2]].push_back(correctGuesses[0]);
        board.remove(min[1],min[2]);
        */
        //cout<<"Returning"<<endl;
        //board.printPossibilities();
        //cout<<"returning:"<<endl;
        return correctGuesses.size();
    }
        
}

/**
 * Applies the rule for single Candidate.
 * This means that there is a single candidate in a square
 * and this candidate is therefore assigned to that square.
 * @return true if the rule was applyable.
 */
bool Rulebased::single(){
    bool match = false;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            vector<int> * square = &(board.board[i][j]);
            if((*square).size()==2){
                //cout<<"Single match at: "<<i<<" "<<j<<endl;
                match = true;
                int tmp = (*square)[1];
                (*square).clear();
                (*square).push_back(tmp);
                //remove from squares with common line,column,box
                board.remove(i,j); 
            }
        }
    }
    return match;
}

/**
 * Applies the rule of hidden and naked pairs, triples and up to octuples.
 * Note that naked and hidden tuples are the same rule but in reverse.
 * This means that if there is a set of squares that together form
 * a hidden tuple than the other squares in that region is a naked tuple.
 * Therefore, one only needs to check for naked tuples.
 * @return true if the rule was applyable.
 */
bool Rulebased::naked(){
    bool match = false;
    for(int i=0;i<27;i++){
        //cout<<"i: "<<i<<endl;
        if(naked(board.regions[i])){
            match = true;
        }
    }
    return match;
}

/**
 * Applies the rule of naked/hidden tuples to a specific region.
 * @param region is an 9 array of pointers to vector<int>
 * @return true if the rule was applyable for the specific region.
 */
bool Rulebased::naked(vector<int> * region[]){
    bool match = false;
    vector<int> n;
    for(int i=0;i<9;i++){ 
        if((*region[i])[0] == 0){
            n.push_back(i);
        }
    }
    //Loop through naked pair, triple, quadruple
    //This also includes hidden single, pair, triple, quad ...
    for(int r=2;r<=8;r++){
        vector< vector<int> > comb = findCombinations(n,r,0);
        for(int c=0;c<comb.size();c++){
            bool numbers[9];
            for(int t=0;t<9;t++){
                numbers[t]=false;
            }
            for(int i=0;i<comb[c].size();i++){
                //cout<<comb[c][i];
                int squarei = comb[c][i]; 
                for(int j=1;j<(*region[squarei]).size();j++){
                    //cout<<"("<<(*region[squarei])[j]<<")";
                    numbers[(*region[squarei])[j]-1]=true;
                }
            }
            //cout<<" ";
            for(int t=0;t<9;t++){
                //cout<<numbers[t]<<" ";
            }
            //cout<<endl;
            int count=0;
            for(int t=0;t<9;t++){
                if(numbers[t]){count++;}
            }
            if(count<=r){
                //Found naked pair,triple...
                //But it may have already been found previously
                //so match is not set to true

                //cout<<"Found: "<<comb[c][0]<<" "<<comb[c][1]<<endl;
                for(int i=0;i<9;i++){
                    //Search if i is contained in found pair,triple...
                    bool skip = false;
                    for(int t=0;t<comb[c].size();t++){
                        if(comb[c][t]==i){
                            skip = true;
                            break;
                        }
                    }
                    if(skip){
                        continue;
                    }
                    for(int j=1;j<(*region[i]).size();j++){
                        if(numbers[(*region[i])[j]-1]){
                            (*region[i]).erase(
                                (*region[i]).begin()+j);
                            j--; //compensate for removal
                            //Something changed so match is true
                            match = true;
                        }
                    }
                }
            }
        }
    }
    return match;
}

/**
 * Finds all combinations from an int vector containing r numbers and
 * beginning with a number with least index i.
 * The method finds the combinations by recursively calling itself
 * and changing i and r. The combinations are then concatenated into an
 * vector consisting of the combinations which are of the type vector<int>.
 * @param n is the vector from which the numbers in the combination will
 * come from.
 * @param r is the number of numbers that shall be picked fron n.
 * @param i is the least index a number can have. i=0 means that any
 * number could be picked.
 * @return vector< vector<int> > which contains r-sized vectors in a vector
 * containing all possible combinations found.
 */
vector< vector<int> > Rulebased::findCombinations(
                    vector<int> n,int r,int i){
    if(r==0){
        vector< vector<int> > x;
        x.push_back(vector<int>());
        return x;
    }
    else if(i>=n.size()){
        return vector< vector<int> >() ;
    }
    vector< vector<int> > combinations;
    vector< vector<int> > a;
    vector< vector<int> > b;
    a = findCombinations(n,r-1,i+1);
    for(int t=0;t<a.size();t++){
        a[t].push_back(n[i]);
        combinations.push_back(a[t]);
    }

    b = findCombinations(n,r,i+1);
    for(int t=0;t<b.size();t++){
        combinations.push_back(b[t]);
    }
    return combinations;
}

/*
int main(){
    //cout<<"START:"<<endl;
    int grid[9][9];
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            char x;
            cin >> x;
            if(x=='\n'){
                j--;
            }
            grid[i][j] = (int) (x-'0');
        }
    }
    Rulebased solver(grid);
    solver.runStep(clock()+CLOCKS_PER_SEC*20);
    solver.printBoard();
}
*/
