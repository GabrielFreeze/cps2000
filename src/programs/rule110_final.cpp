#include <iostream>
#include <cstdio>
#include <unistd.h>

#define BOARD_CAP 31

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

int main(void) {


    // Initial seed
    int board = 1 << 30;
    

    for (int i = 0; i < BOARD_CAP; i++) {
        //Display board
        for (int j = 0; j < BOARD_CAP; j++)
            cout << " #"[getBit(board,j)];
        cout << '\n';

        //Update Board
        int pattern = (getBit(board,0)<<1) | getBit(board,1);

        for (int j = 1; j < BOARD_CAP-1; j++) {
            pattern = ((pattern << 1) & 7) | getBit(board,j+1); //Add next bit to the front
            cout << pattern;
            board = setBit(board,j,(110 >> pattern) & 1); //Get pattern'th least significant bit
        }


    }
    return 0;
}





// int main(void) {


//     //Initial seed
//     // int board = 1 << 30;
//     bool board[BOARD_CAP]{0};
//     board[BOARD_CAP-1] = 1;
    

//     for (int i = 0; i < BOARD_CAP; i++) {
//         //Display board
//         for (int j = 0; j < BOARD_CAP; j++)
//             cout << " #"[board[j]];
//         cout << '\n';

//         //Update Board
//         int pattern = (board[0]<<1) | board[1];

//         for (int j = 1; j < BOARD_CAP-1; j++) {
//             pattern = ((pattern << 1) & 7) | board[j+1]; //Add next bit to the front
//             cout << pattern;
//             board[j] = (110 >> pattern) & 1; //Get pattern'th least significant bit
//         }

//     }
//     return 0;
// }
