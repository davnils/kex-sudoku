#include<vector>
#include<string>
#include<iostream>

#include "Rulebased.h"

using namespace std;

Rulebased::Rulebased(int grid[9][9]){
    board = *(new Board());
    board.setBoard(grid);
}

Rulebased::Rulebased(Board b){
    board = *(new Board(b));
}

void Rulebased::solve(){
    while(true){
        //The easy rules first.
        if(single())
            continue;
        if(naked())
            continue;
        break;
    }
    bool unique = guess();
    bool solved = true;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board.board[i][j][0]==0)
                solved = false;
        }
    }
    if(!board.valid()){
        cout<<"UNVALID"<<endl;
    }else if(!solved){
        cout<<"UNSOLVED"<<endl;
        board.printBoard();
    }else if(!unique){
        cout<<"AMBIGOUS"<<endl;
    }
    else{
        board.printBoard();//"SIMPLE");
    }
} 

/*
    Returns true if unique solution was found
    Returns false if the solution doesnt exist or if the solution
    was amibous.
*/
bool Rulebased::guess(){
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
    cout<<"Minsta: "<<min[0]<<" "<<min[1]<<" "<<min[2]<<endl;
    if(min[0]==100){
        return true;
    }
    if(board.board[min[1]][min[2]].size()==1){
        return false;
    }
    vector<int> correctGuesses;
    for(int g_index=1;g_index<board.board[min[1]][min[2]].size();g_index++){
        int g = board.board[min[1]][min[2]][g_index];
        Board tmp = board;
        vector<int> * tmpvector = &tmp.board[min[1]][min[2]];
        (*tmpvector)[0] = g;
        (*tmpvector).erase((*tmpvector).begin()+1,(*tmpvector).end());
        tmp.remove(min[1],min[2]);
        Rulebased solver(tmp);
        solver.solve();
        tmp = solver.getBoard();
        if(tmp.valid()){
           correctGuesses.push_back(g);
        }
    }
    if(correctGuesses.size()==0){
        return false;
    }else{
        board.board[min[1]][min[2]].clear();
        board.board[min[1]][min[2]].push_back(correctGuesses[0]);
        board.remove(min[1],min[2]);
        if(correctGuesses.size()>2){
            return false;
        }
    }
        
}

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

bool Rulebased::naked(vector<int> * region[]){
    bool match = false;
    vector<int> n;
    for(int i=0;i<9;i++){ if((*region[i])[0] == 0){
            n.push_back(i);
        }
    }
    //Loop through naked pair, triple, quadruple
    //TODO change to r=4 when r=2 functions properly
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


int main(){
    //cout<<"START:"<<endl;
    int grid[9][9];
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            char x;
            cin >> x;
            grid[i][j] = (int) (x-'0');
        }
    }
    Rulebased solver(grid);
    solver.solve();
}


