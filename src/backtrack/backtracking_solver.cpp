#include<iostream>
#include<vector>

using namespace std;

vector< vector<int> > getPuzzle(){
    vector< vector<int> > puzzle = vector< vector<int> >();
    int x;
    for(int i=0;i<9;i++){
        puzzle.push_back(vector<int>());
        for(int j=0;j<9;j++){
            cin >> x;
            puzzle[i].push_back(x);
        }
    }
    return puzzle;
}

void output(vector< vector<int> > puzzle){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++)
            cout << puzzle[i][j];
        cout << endl;
    }
}


vector< vector<int> > solve(vector< vector<int> > puzzle);

int main(){
    vector< vector<int> > puzzle = getPuzzle();
    cout << "test";
    vector< vector<int> > puzzlesol = solve(puzzle);
    output(puzzle);
    return 0;
}

vector< vector<int> > solve(vector< vector<int> > puzzle){
    bool filledIn[9][9];
    int unfilledSquares = 0;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(puzzle[9][9]==0){
                filledIn[i][j]=false;
                unfilledSquares++;
            }else
                filledIn[i][j]=true;
        }
    }
    int x=0,y=0;
    while(unfilledSquares>0){
        puzzle[x][y]++;
        if(puzzle[x][y]==10){
            puzzle[x][y]==0;
            unfilledSquares++;
            while(true){
                x--;
                if(x==-1){
                    x=8;
                    y--;
                    if(y==-1){
                        cout << "Unsolvable" << endl;
                        return puzzle;
                    }
               }
               if(!filledIn[x][y])
               break;
            }
        } 
                
        bool isOk=true;  
        for(int i=0;i<9;i++){
            if(puzzle[i][y]==puzzle[x][y] || puzzle[x][i]==puzzle[x][y]){
                isOk=false;
                break;
            }
        }
        int boxX = (x/3)*3;
        int boxY = (y/3)*3;
        for(int i=boxX;i<boxX+3;i++){
            for(int j=boxY;j<boxY+3;j++){
                if(!(x==i && y==j) && puzzle[i][j]==puzzle[x][y]){
                    isOk=false;
                    break;
                }
            }
        }
        if(!isOk)
            continue;
        //else
        unfilledSquares--;
        if(unfilledSquares==0)
            break;
        while(true){
            x++;
            if(x==9){
                x=0;
                y++;
            }
            if(y==9){
                cout << "Counted unfilled squares wrong!" << endl;
                unfilledSquares=0;
                break;
            }
            if(!filledIn[x][y])
                break;
        }
    }
}
                
            














    
