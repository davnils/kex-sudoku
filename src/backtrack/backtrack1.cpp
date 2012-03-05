#include<iostream>
#include<vector>
#include<math.h>
#include<string>
#include<algorithm>

using namespace std;

bool isValid(vector< vector<int> > puzzle){
    vector<bool> checkVector;
    for(int i=0;i<puzzle.size();i++)
        checkVector.push_back(false);

    //Check rows and columns
    for(int i=0;i<puzzle.size();i++){
        for(int x=0;x<puzzle.size();x++)
            checkVector[x]=false;
        for(int j=0;j<puzzle.size();j++){
            if(puzzle[i][j]!=0){
                if(checkVector[puzzle[i][j]-1])
                    return false; 
                checkVector[puzzle[i][j]-1]=true;  
            }
        }
        for(int x=0;x<puzzle.size();x++)
            checkVector[x]=false;
        for(int j=0;j<puzzle.size();j++){
            if(puzzle[j][i]!=0){
                if(checkVector[puzzle[j][i]-1])
                    return false; 
                checkVector[puzzle[j][i]-1]=true;  
            }
        }
    }
    //Check boxes
    int k=(int) sqrt(puzzle.size());
    for(int shifti=0;shifti<puzzle.size();shifti+=k){
        for(int shiftj=0;shiftj<puzzle.size();shiftj+=k){
            for(int x=0;x<puzzle.size();x++)
                checkVector[x]=false;
            for(int i=0;i<k;i++){
                for(int j=0;j<k;j++){
                    if(puzzle[i+shifti][j+shiftj]!=0){
                        if(checkVector[puzzle[i+shifti][j+shiftj]-1])
                            return false;
                        checkVector[puzzle[i+shifti][j+shiftj]-1]=true;
                    }
                }
            }
        }
    }
    return true;
}

void output(vector< vector<int> > puzzle){
    int k=(int) sqrt( puzzle.size());
    cout << "+---+---+---+" << endl;
    for(int i=0;i<puzzle.size();i++){
        cout << "|";
        for(int j=0;j<puzzle[i].size();j++){
            cout << puzzle[i][j];
            if(j%k==k-1 )
                cout << "|";
        }
        if(i%k==k-1)
            cout << endl << "+---+---+---+";
        cout << endl;
    }
}

vector< vector<int> > backtracking_solve(vector< vector< int > > puzzle);

int main(){
    string s;
    cin >> s;
    vector< vector<int> > puzzle;
    for(int i=0;i<9;i++){
        puzzle.push_back(vector<int>());
        for(int j=0;j<9;j++){
            puzzle[i].push_back((int) s[i*9+j]-'0');
        }
    }
    output(puzzle);
    vector< vector<int> > sol;
    sol = backtracking_solve(puzzle);
    output(sol);
    return 0;
}

vector< vector<int> > backtracking_solve(vector< vector< int > > puzzle){
    vector< vector<int> > solution; //solution that is built
    solution = puzzle; // Initialize

    vector< vector<int> > unfilledSquares;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(puzzle[i][j]==0){
                unfilledSquares.push_back(vector<int>());
                unfilledSquares.back().push_back(i);
                unfilledSquares.back().push_back(j);
            }
        }
    }
    //Shuffle squares to fill in
    random_shuffle(unfilledSquares.begin(), unfilledSquares.end());
    /*
    for(int i=0;i<unfilledSquares.size();i++){
        cout << unfilledSquares[i][0] << " " << unfilledSquares[i][1] << endl;
    }
    */
    
    int i=0;
    int x,y;
    int nr;
    while(i<unfilledSquares.size()){
        if(i<0){
            cout << "INVALID SUDOKU";
            return vector< vector<int> >();
        }
        y=unfilledSquares[i][0]; 
        x=unfilledSquares[i][1];
        /*
        output(solution);
        cout << y << " " << x << " " << solution[y][x];
        for(int a=0;a<i;a++)
            cout << " ";
        cout << i << endl;
       */ 
        while(solution[y][x]<=9){
            if(solution[y][x]==9){ //Backtrack
                solution[y][x]=0;
                i--;
                break;
            }
            solution[y][x]++;
            //Check if possible
            if(isValid(solution)){
                //cout << "correct" << endl;
                i++;
                break;
            }
        }
    }
    return solution;
}
                
            
                
       
            







                

            














    
