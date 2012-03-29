#include "Board.h"

void Board::printPossibilities(){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cout<<"[";
            for(int k=0;k<board[i][j].size();k++){
                cout<<board[i][j][k];
            }
            cout<<"], ";
        }
        cout<<endl;
    }
}

void Board::printPossibilities1(){
    cout<<"print from regions"<<endl;
    for(int s=0;s<3;s++){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cout<<"[";
            for(int k=0;k<(*regions[i+s*9][j]).size();k++){
                cout<<(*regions[i+s*9][j])[k];
            }
            cout<<"], ";
        }
        cout<<endl;
    }
    cout<<endl;
    }
    cout<<"end print from regions"<<endl;
}
void Board::printBoard(){
    printBoard("NORMAL");
}

void Board::printBoard(string str){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cout<<board[i][j][0];
        }
        if(str == "NORMAL")
            cout<<endl;
    }
    if(str == "SIMPLE")
        cout<<endl;
}

void Board::printRegions(){
    cout<<"--BOXES-- --ROWS--- -COLUMNS-"<<endl;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cout<< (*boxes[i][j])[0];
        }
        cout<<" ";
        for(int j=0;j<9;j++){
            cout<< (*rows[i][j])[0];
        }
        cout<<" ";
        for(int j=0;j<9;j++){
            cout<< (*columns[i][j])[0];
        }
        cout << endl;
    }
    cout << endl << "-REGIONS-"<<endl;
    for(int i=0;i<27;i++){
        for(int j=0;j<9;j++){
            cout << (*regions[i][j])[0];
        }
        cout << endl;
        if((i+1)%9==0 && i>0)
            cout << endl;
    }
}

bool Board::valid(){
    analysePossibilities(); 
    for(int i=0;i<27;i++){
        bool numbers[9];
        for(int t=0;t<9;t++){
            numbers[t]= false;
        }
        for(int j=0;j<9;j++){
            if((*regions[i][j])[0]==0){
                return false; 
            }else if(numbers[(*regions[i][j])[0]-1]){
                return false;
            }else{
                numbers[(*regions[i][j])[0]-1]=true;
            }
        }
    }
    return true;
}

void Board::analysePossibilities(){
    // Erase old data
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            int temp = board[i][j][0];
            board[i][j].clear();
            board[i][j].push_back(temp);
            for(int x=1;x<=9;x++){
                board[i][j].push_back(x);
            }
        }
    }
    //Remove from possibility vectors
    for(int i=0;i<27;i++){
        for(int j=0;j<9;j++){
            int nr = (*regions[i][j])[0];
            if(nr==0){
                continue;
            }
            (*regions[i][j])= vector<int>();
            (*regions[i][j]).push_back(nr);
            for(int k=0;k<9;k++){
                if(k==j){
                    continue;
                }
                for(int l=1;l<(*regions[i][k]).size();l++){
                    vector<int> * square = regions[i][k];
                    if((*square)[l]==nr){
                        (*square).erase((*square).begin()+l);
                        break;
                    }
                }
            }
        }
    }
}

/*
    Overloads = operator. This has to be done
    due to the use of references in regions, rows, columns and boxes.
    The difference is that the references will not be copied but rather
    reassigned to the new board created.
*/
Board Board::operator= (Board b){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            (*this).board[i][j] = b.board[i][j];
        }
    }
    (*this).createReferences();
    /*
    cout<<"this: "<<this<<endl;
    cout<<"b: "<<&b<<endl;
    */
    return *this;
}

void Board::createReferences(){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            rows[i][j] = &board[i][j];
            columns[j][i] = &board[i][j];
            regions[i][j] = &board[i][j];
            regions[j+9][i] = &board[i][j];
            //cout<<"i: "<<i<<" j: "<<j<<" adress: "<<&board[i][j]<<endl;
        }
    }
    for(int b=0;b<9;b++){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                int ishift = 3*(b/3);
                int jshift = 3*(b%3); 
                boxes[b][i*3+j] = &board[i+ishift][j+jshift];
                regions[b+18][i*3+j] = boxes[b][i*3+j];
            }
        }
    }
}

void Board::setBoard(int grid[9][9]){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            board[i][j] = vector<int>();
            board[i][j].push_back(grid[i][j]);
        }
    }
    createReferences();
    analysePossibilities();
/*
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(&board[i][j]!=regions[i][j]){
                cout<<i<<" "<<j<<" KAOS"<<endl;
            }
            if(&board[i][j]!=regions[j+9][i]){
                cout<<i<<" "<<j<<" KAOS2"<<endl;
            }
            if(&board[i][j]!=regions[18+3*(i/3)+j/3][3*(i%3)+j%3]){
                cout<<i<<" "<<j<<" KAOS3"<<endl;
            }
        }
    }
    */
    /*
    printRegions(); 
    cout << endl;
    printBoard();
    cout<<endl;
    printPossibilities();
    */
}

void Board::remove(int i,int j){
    //cout<<"Remove: "<<i<<" "<<j<<endl;
    //printPossibilities();cout<<endl;
    //printPossibilities1();cout<<endl;
    remove(regions[i],board[i][j][0]);
    remove(regions[j+9],board[i][j][0]);
    remove(regions[18+3*(i/3)+(j/3)],board[i][j][0]);
}

void Board::remove(vector<int> * region [],int nr){
    for(int i=0;i<9;i++){
        //cout<<"    "<<(*region[i])[0]<<"[";
        for(int j=1;j<(*region[i]).size();j++){
            //cout<<(*region[i])[j];
            if((*region[i])[j]==nr){
                //cout<<"*";
                (*region[i]).erase((*region[i]).begin()+j);
                /*cout<<"(";
                for(int t=0;t<(*region[i]).size();t++){
                    cout<<(*region[i])[t];
                }
                */
                //cout<<")";
                break; 
            }
        }
        //cout<<"] ";
    }
    //cout<<endl;
}

