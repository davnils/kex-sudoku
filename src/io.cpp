#include<iostream>
#include<vector>
#include"io.h"

using namespace std;

vector< vector<unsigned char> > getPuzzle(){
    vector< vector<unsigned char> > puzzle;
    puzzle.resize(9);
    for(int i=0;i<9;i++)
        puzzle[i].resize(9);

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cin >> puzzle[i][j];
        }
    }
    return puzzle;
}

void output(vector< vector<unsigned char> > puzzle){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cout << puzzle[i][j];
        }
        cout << endl;
    }
}
