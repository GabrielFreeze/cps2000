#include <iostream>
#include <cstdio>
#include <unistd.h>

using namespace std;

bool getBit(int n, int k) {
    int i = 1;

    for (int j = 0; j < k; j=j+1) {
        i = i*2;
    }

    return (n & i) == i;
}

int setBit(int n, int k, bool bit) {
    if (bit) {
        return n | 1ULL << k;
    } else {
        return n &  ~(1ULL << k);
    }
}

bool printBoard(int n) {
    //-2,147,483,648 == 1 << 31. NB: Most significant bit is 2's Complement 
    for (int i = 1; i != -2147483648; i = i*2) {
        
        if ((n & i) == i) {
            cout << '#';
        } else {
            cout << ' ';
        }
    }

    return true;
}


int main(void) {


    //Initial seed
    int board =  1 << 30;
    int BOARD_CAP = 5;


    for (int i = 0; i < BOARD_CAP; i++) {
        
        //Display board
        printBoard(board);
        cout << '\n';
        //Update Board
        int pattern = (getBit(board,0) << 1) | (getBit(board,1));


        for (int j = 1; j < BOARD_CAP-1; j++) {
            pattern = ((pattern << 1) & 7) | getBit(board,j+1); //Add next bit to the front
            board = setBit(board,j, (110 >> pattern) & 1); //Get pattern'th least significant bit
        }

        

        // for (let j:int = 1; j < BOARD_CAP-1; j=j+1) {
        //     pattern = (SHL(pattern,1) and 7) or (getBit(board,j+1));
        //     board = setBit(board,j,SHR(110,pattern) and 1);
        // }

    }

    return 0;
}


