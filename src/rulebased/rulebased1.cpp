/*
    rulebased solver. 
    The rules commes from 
    http://www.sudokuoftheday.com/pages/techniques-7.php 
    Considers the following

    - Single-Candidate
        Squares with only one candidate
    - Single-Position
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
    - TODO: X-wing/swordfish/jellyfish
        All these are the same thing but generalized to different
        loop size. TODO bättre förklaring
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
bool singlePosition(vector< vector<int> >& puzzle, vector< vector< vector<int> > >& poss);
void adjustPoss(int i,int j,vector< vector< vector<int> > >& poss,vector< vector<int> >& puzzle);
vector< vector<int> > applyRules(vector< vector<int> > puzzle, vector< vector< vector<int> > > poss);
vector< vector< vector<int> > > createPoss(vector< vector<int> > puzzle);
vector< vector<int> > rulebased_solve(vector< vector< int > > puzzle);
bool isSolved(vector< vector<int> > puzzle);
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
    if(!isSolved(sol))
    cout<<"UNSOLVED"<<endl;
    return 0;
}

bool isSolved(vector< vector<int> > puzzle){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(puzzle[i][j]==0){
                return false;
            }
        }
    }
    return true;
}

void printPoss(vector< vector< vector<int> > > poss){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            for(int k=0;k<poss[i][j].size();k++)
                cout << poss[i][j][k];
            cout << ",";
        }
        cout <<  endl;
    }
}

vector< vector<int> > rulebased_solve(vector< vector< int > > puzzle){
    vector< vector< vector<int> > > poss; //Possibilities for each square
    poss = createPoss(puzzle);
    //printPoss(poss);    
    vector< vector<int> > solution;
    solution = applyRules(puzzle,poss); 
    return solution;
}

vector< vector<int> > applyRules(vector< vector<int> > puzzle, vector< vector< vector<int> > > poss){
    bool change=true;
    while(change){
        change=false;
        //cout<<"varv"<<endl;
        change = change || singleCandidate(puzzle,poss);
        change = change || singlePosition(puzzle,poss);
    }
    //printPoss(poss);
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
    //if(match)
    //cout<<"Match on singleCandidate"<<endl;
    return match;
}
bool nakedPair(vector< vector<int> >& puzzle, vector< vector< vector<int> > >& poss){
    bool match;
 
    vector<int> count;
    for(int a=0;a<9;a++){
        count.push_back(0);
    }
    for(int row=0;row<9;row++){
        for(int a=0;a<8;a++){
            for(int b=a+1;b<9;b++){
                for(int i=0;i<9;i++){
                    count[i]=0;
                }
                for(int i=0;i<poss[row][b].size();i++){
                    count[poss[row][b][i]--]++;
                }
                for(int i=0;i<poss[row][a].size();i++){
                    count[poss[row][a][i]--]++;
                }
                vector<int> t;
                for(int i=0;i<9;i++){
                    if(count[i]>0){
                        t.push_back(i+1);
                    }
                }
                if(t.size()<=2){
                    match = true;
                    for(int i=0;i<t.size();i++){
                        for(int col=0;col<9;col++){
                            if(col!=a && col!=b){
                                for(int i2=0;i2<poss[row][col].size();i2++){
                                    if(poss[row][col][i2]==t[i]){
                                        poss[row][col].erase(poss[row][col].begin()+i2);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
            


    for(int s=2;s<3;s++){//This will be extended to generalize for triples..
        vector< vector<int> > koords;
        for(int a=0;a<s;a++){
            koords.push_back(vector<int>());
            koords[a].push_back(-1);
            koords[a].push_back(-1);
        }
         
        // so that it also checks for triples/quadruples/quintuples.
    }
    
    //check columns
    //check boxes


        
    return match;
}



bool singlePosition(vector< vector<int> >& puzzle, vector< vector< vector<int> > >& poss){

    bool match = false;
    vector<int> counter;
    for(int i=0;i<9;i++)
        counter.push_back(0);
    //Kolla rader
    for(int i=0;i<9;i++){
        for(int a=0;a<9;a++)
            counter[a]=0; 
        for(int j=0;j<9;j++){
            for(int a=0;a<poss[i][j].size();a++){
                counter[poss[i][j][a]-1]++;    
            }
        }
        for(int a=0;a<9;a++){
            if(counter[a]==1){
                match = true;
                for(int j2=0;j2<9;j2++){
                    for(int a2=0;a2<poss[i][j2].size();a2++){
                        if(poss[i][j2][a2]==a+1){
                            puzzle[i][j2]=a+1;
                            adjustPoss(i,j2,poss,puzzle);
                        }
                    }
                }
            }
        }
    }
    //Kolla kolumner
    for(int j=0;j<9;j++){
        for(int a=0;a<9;a++)
            counter[a]=0; 

        for(int i=0;i<9;i++){
            for(int a=0;a<poss[i][j].size();a++){
                counter[poss[i][j][a]-1]++;    
            }
        }
        for(int a=0;a<9;a++){
            if(counter[a]==1){
                match = true;
                for(int i2=0;i2<9;i2++){
                    for(int a2=0;a2<poss[i2][j].size();a2++){
                        if(poss[i2][j][a2]==a+1){
                            puzzle[i2][j]=a+1;
                            adjustPoss(i2,j,poss,puzzle);
                        }
                    }
                }
            }
        }
    }

    for(int jshift=0;jshift<9;jshift+=3){
        for(int ishift=0;ishift<9;ishift+=3){
            for(int a=0;a<9;a++)
                counter[a]=0; 
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    for(int a=0;a<poss[i+ishift][j+jshift].size();a++){
                        counter[poss[i+ishift][j+jshift][a]-1]++;    
                    }
                }
            }
            for(int a=0;a<9;a++){
                if(counter[a]==1){
                    match = true;
                    for(int i2=0;i2<3;i2++){
                        for(int j2=0;j2<3;j2++){
                            for(int a2=0;a2<poss[i2+ishift][j2+jshift].size();a2++){
                                if(poss[i2+ishift][j2+jshift][a2]==a+1){
                                    puzzle[i2+ishift][j2+jshift]=a+1;
                                    adjustPoss(i2+ishift,j2+jshift,poss,puzzle);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /*
    if(match)
    cout<<"Match on singlePosition"<<endl;
    output(puzzle);
    */
    return match;
}

void adjustPoss(int i,int j,vector< vector< vector<int> > >& poss,vector< vector<int> >& puzzle){
        poss[i][j]=vector<int>();
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












