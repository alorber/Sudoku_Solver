//
//  main.cpp
//  Sudoku Solver
//
//  Created by Andrew Lorber on 6/28/19.
//  Copyright © 2019 Andrew Lorber. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>


using namespace std;

// Board
int board[9][9] = {0};

// Board sections to simplify checking
// T: Top, C: Center, B: Bottom, L: Left, M: Middle, R: Right
vector<int*> TL { &board[0][0], &board[0][1], &board[0][2], &board[1][0], &board[1][1], &board[1][2], &board[2][0], &board[2][1], &board[2][2] };
vector<int*> TM = { &board[0][3], &board[0][4], &board[0][5], &board[1][3], &board[1][4], &board[1][5], &board[2][3], &board[2][4], &board[2][5] };
vector<int*> TR = { &board[0][6], &board[0][7], &board[0][8], &board[1][6], &board[1][7], &board[1][8], &board[2][6], &board[2][7], &board[2][8] };
vector<int*> CL = { &board[3][0], &board[3][1], &board[3][2], &board[4][0], &board[4][1], &board[4][2], &board[5][0], &board[5][1], &board[5][2] };
vector<int*> CM = { &board[3][3], &board[3][4], &board[3][5], &board[4][3], &board[4][4], &board[4][5], &board[5][3], &board[5][4], &board[5][5] };
vector<int*> CR = { &board[3][6], &board[3][7], &board[3][8], &board[4][6], &board[4][7], &board[4][8], &board[5][6], &board[5][7], &board[5][8] };
vector<int*> BL = { &board[6][0], &board[6][1], &board[6][2], &board[7][0], &board[7][1], &board[7][2], &board[8][0], &board[8][1], &board[8][2] };
vector<int*> BM = { &board[6][3], &board[6][4], &board[6][5], &board[7][3], &board[7][4], &board[7][5], &board[8][3], &board[8][4], &board[8][5] };
vector<int*> BR = { &board[6][6], &board[6][7], &board[6][8], &board[7][6], &board[7][7], &board[7][8], &board[8][6], &board[8][7], &board[8][8] };

// Coordinate class to simplify things
class Coordinate {
public:
    int x;
    int y;
    Coordinate(int i, int j) : x{i}, y{j} {}
};

// List of starting board numbers that cannot be changed
vector<Coordinate> startingNumbers;

// Initializes The Board
void initializeBoard(){
    // Reads in board from file
    string file;
    cout << "Please enter the name of the file containing a sudoku board.\n";
    cin >> file;
    ifstream fin;
    fin.open(file);
    
    // Checks for incorrect input
    while(!fin.is_open()){
        cout << "The file you entered cannot be opened.\nPlease enter a new file.\n";
        cin >> file;
        fin.open(file);
    }
    
    int square;
    
    int i = 0; int j = 0;
    while(fin >> square){
        
        board[j][i] = square;
        // Keeps track of starting numbers that cannot be changed
        if(square != 0){
            startingNumbers.push_back(Coordinate(i, j));
        }
        
        i++;
        if(i == 9){
            i = 0;
            j++;
        }
        if(j == 9){
            break;
        }
    }
}

// Finds section of square
vector<int*>* sqSection(int x, int y){
    if (y <= 2) {
        if (x <= 2) {
            return &TL;
        } else if (x <= 5){
            return &TM;
        } else {
            return &TR;
        }
    } else if (y <=5) {
        if (x <= 2) {
            return &CL;
        } else if (x <= 5){
            return &CM;
        } else {
            return &CR;
        }
    } else {
        if (x <= 2) {
            return &BL;
        } else if (x <= 5){
            return &BM;
        } else {
            return &BR;
        }
    }
}

//Checks if legal move
bool legalMove(int num, int x, int y){
    //Checks row / col
    for(int i = 0; i < 9; i++){
        if (board[i][x] == num) {
            return false;
        }
        if (board[y][i] == num) {
            return false;
        }
    }
    //Checks section
    for (auto sq : *(sqSection(x, y))) {
        if (*sq == num) {
            return false;
        }
    }
    return true;
}

//Checks if square is starting number
bool isStartingNumber(int x, int y){
    for (auto coord : startingNumbers) {
        if (coord.x == x && coord.y == y) {
            return true;
        }
    }
    return false;
}

// Prints the board
void printBoard() {
    cout << '\n' << " |---|---|---||---|---|---||---|---|---|\n";
    for (int i = 0, j = 0; j < 9; i++) {
        if(i == 3 || i == 6){
            cout << " || ";
        } else {
            cout << " | ";
        }
        cout << board[j][i];
        if(i == 8){
            cout << " |\n";
            if(j ==  2 || j == 5){
                cout << " |===|===|===||===|===|===||===|===|===|\n";
            } else {
                cout << " |---|---|---||---|---|---||---|---|---|\n";
            }
            i = -1;
            j++;
        }
    }
}

//Solves the puzzle
void solvePuzzle(){
    for (int i = 0, j = 0; j < 9;){
        //If unchangeable num, skip
        while (isStartingNumber(i, j)) {
            i++;
            if (i > 8) {
                i = 0;
                j++;
                if (j > 8) {
                    return;
                }
            }
        }
        
        //Finds a valid number for sq. If none, backtrack
        while (!legalMove(board[j][i] + 1, i, j)) {
            if (board[j][i] >= 8) {
                board[j][i] = 0;
                i--;
                if (i == -1) {
                    i = 8;
                    j--;
                }
                
                while (isStartingNumber(i, j) || board[j][i] >= 9){
                    if (!isStartingNumber(i, j)) {
                        board[j][i] = 0;
                    }
                    i--;
                    if (i == -1) {
                        i = 8;
                        j--;
                    }
                }
            } else {
                board[j][i] += 1;
            }
        }
        board[j][i] += 1;
        i++;
        if(i > 8){
            i = 0;
            j++;
        }
    }
}

int main(int argc, const char * argv[]) {
    initializeBoard();
    solvePuzzle();
    printBoard();
}
