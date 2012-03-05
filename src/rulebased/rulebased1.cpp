/*
    rulebased solver. 
    The rules commes from 
    http://www.sudokuoftheday.com/pages/techniques-7.php 
    Considers the following

    - TODO: Single-Candidate
        Squares with only one candidate
    - TODO: Single-Position
        Squares which have a candidate unique to its row/column/box
    - TODO: Candidate lines
        If a box have a candidate that must be on a certain row/column
        one may remove that same candidate from the other boxes in that
        row/column.
    - TODO: Double pairs
    - TODO: Multiple Lines:
        More generalized form of double pairs. Finds two blocks that
        share a row or column. If there is a candidate among the common
        row/column that only appears in two of the three common row/columns
        , then the third box must have the candidate in the third row/column.
    - TODO: Naked Pairs/Triples
        Two-Three squares limited to two/three candidates. All other 
        of those candidates in the squares common row/column/box and 
        all other candidates within those squares can be deleted.
    - TODO: 
    - TODO: whatIf
        Attempts proof by contradiction. Assumes one number for
        a specific square and shows that it leads to a contradiction.
        This could be in multiple forms including the following:
            - BLARGH
            - BLARGH
            - BLARGH
*/
#include<iostream>
#include<vector>
#include<math.h>
#include<string>
#include<algorithm>

using namespace std;

void output(vector< vector<int> > puzzle);
bool singleCandidate(vector< vector<int> >& puzzle, vector< vector< vector<int> > >& poss);
void adjustPoss(int i,int j,vector< vector< vector<int> > >& poss,vector< vector<int> >& puzzle);
vector< vector<int> > applyRules(vector< vector<int> > puzzle, vector< vector< vector<int> > > poss);
vector< vector< vector<int> > > createPoss(vector< vector<int> > puzzle);
vector< vector<int> > rulebased_solve(vector< vector< int > > puzzle);

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
    sol = rulebased_solve(puzzle);
    output(sol);
    return 0;
}

vector< vector<int> > rulebased_solve(vector< vector< int > > puzzle){
    vector< vector< vector<int> > > poss; //Possibilities for each square
    poss = createPoss(puzzle);
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            for(int k=0;k<poss[i][j].size();k++)
                cout << poss[i][j][k];
            cout << ",";
        }
        cout <<  endl;
    }

    vector< vector<int> > solution;
    solution = applyRules(puzzle,poss); 
    return solution;
}

vector< vector<int> > applyRules(vector< vector<int> > puzzle, vector< vector< vector<int> > > poss){
    bool change=true;
    while(change == true){
        change=false;
        change = change || singleCandidate(puzzle,poss);
        //change = change || singlePosition(puzzle,poss);
    }
    return puzzle;
}

bool singleCandidate(vector< vector<int> >& puzzle, vector< vector< vector<int> > >& poss){
    bool match = false;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(poss[i][j].size()==1){
                puzzle[i][j]=poss[i][j][0];
                adjustPoss(i,j,poss,puzzle);
                match = true;
            }
        }
    }
    return match;
}
/*
bool singlePosition(vector< vector<int> >& puzzle, vector< vector< vector<int> > >& poss){
    bool match = false;
    vector<int> counter;
    for(int i=0;i<9;i++)
        counter.push_back(0);

    for(int i=0;i<9;i++){
        for(int a=0;a<9;a++)
            counter[a]=0; 
        for(int j=0;j<9;j++){
            counter[puzzle[i][j]-1]++;    
        }
        for(int a=0;a<9;a++){
            if(counter[a]==1){
                
    }
    return match;
}
   */ 

void adjustPoss(int i,int j,vector< vector< vector<int> > >& poss,vector< vector<int> >& puzzle){
        //Remove row candidates
        for(int col=0;col<9;col++){
            for(int a=0;a<poss[i][col].size();a++){
                if(poss[i][col][a]==puzzle[i][j]){
                    poss[i][col].erase(poss[i][col].begin()+a);
                    break;
                }
            }
        }
        //Remove column candidates
        for(int row=0;row<9;row++){
            for(int a=0;a<poss[row][j].size();a++){
                if(poss[row][j][a]==puzzle[i][j]){
                    poss[row][j].erase(poss[row][j].begin()+a);
                    break;
                }
            }
        }
        
        //Remove box candidates
        int ishift = ((int) i/3)*3;
        int jshift = ((int) j/3)*3;
        for(int i2=ishift;i2<ishift+3;i2++){
            for(int j2=jshift;j2<jshift+3;j2++){
                for(int a=0;a<poss[i2][j2].size();a++){
                    if(poss[i2][j2][a]==puzzle[i][j]){
                        poss[i2][j2].erase(poss[i2][j2].begin()+a);
                        break;
                    }
                }
            }
        }
}


vector< vector< vector<int> > > createPoss(vector< vector<int> > puzzle){
    vector< vector< vector<int> > > poss; //Possibilities for each square
    for(int i=0;i<9;i++){
        poss.push_back(vector< vector<int> >());
        for(int j=0;j<9;j++){
            poss[i].push_back(vector<int>());
            if(puzzle[i][j]==0){
                for(int k=1;k<=9;k++)
                    poss[i][j].push_back(k);
            }
        }
    }

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(puzzle[i][j]!=0){
                adjustPoss(i,j,poss,puzzle); 
                
            }
        }
    }
    return poss;
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












