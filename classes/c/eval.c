#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "defs.h"




void debugging();
u64 pawnMask();
void initLeaps();




//setting up the masks:
u64 pawnAttacks[2][64];

//both sides have same attacks
u64 knightAttacks[64];



int main(){
    pawnMask(a4,white);

    
    return 0;
}


u64 knightMask(int sq){
    u64 bitBoard = 0ULL;

    u64 attacks = 0ULL;
    setBit(bitBoard,sq);

    //  black
    attacks |= (bitBoard >> 17 & notHFile) | (bitBoard << 17 & notAFile); 
    attacks |= (bitBoard >> 15 & notAFile) | (bitBoard << 15 & notHFile); 
    attacks |= (bitBoard >> 10 & notHGFile) | (bitBoard << 10 & notABFile);
    attacks |= (bitBoard >> 6 & notABFile) | (bitBoard >> 6 & notABFile);  
    
    return attacks;

}
u64 pawnMask(int sq, int color){
    u64 bitBoard = 0ULL;

    u64 attacks = 0ULL;
    setBit(bitBoard,sq);

    //  black
 
    attacks |= ((bitBoard << 7) & notHFile & color);
    attacks |= ((bitBoard << 9) & notAFile & color);
    
    //white
    attacks |= ((bitBoard >> 7) & notAFile & ~ color);
    attacks |= ((bitBoard >> 9) & notHFile & ~ color);

    debugging(attacks);
    return attacks;
}



void initLeaps(){
    //loop board
    for(int sq = 0; sq < 64; sq++){
        
        pawnAttacks[white][sq] = pawnMask(white, sq);
        pawnAttacks[black][sq] = pawnMask(black, sq);
        knightAttacks[sq] = knightMask(sq);
    }
}




void debugging(u64 board){
    printf("   A B C D E F G H\n");
    for(int rank = 0; rank < 8; rank++){
        printf("%d  ", 8 - rank);
        for(int file = 0; file < 8; file++){
            int sq = rank*8 + file;
            //printing 1 if sq is ocupied
            printf("%d ", getBit(board,sq) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n%llu",board);
}
// #include "defs.h"
// #include <stdio.h>
// #include <stdint.h>
// #include <inttypes.h>

// uint64_t whitePawn;
// uint64_t whiteRook;
// uint64_t whiteKnight;
// uint64_t whiteBishop;
// uint64_t whiteKing;
// uint64_t whiteQueen;
// uint64_t blackPawn;
// uint64_t blackKnight;
// uint64_t blackBishop;
// uint64_t blackKing;
// uint64_t blackQueen;
// uint64_t blackRook;
// uint64_t squares[64];

// // making the starting positions
// void initilize(){
//     //bottom right square = bit 0 , top right = bit 63
//     whitePawn = 0xFF00;
//     whiteRook = 0x81;
//     whiteKnight = 0x42;
//     whiteBishop = 0x24;
//     whiteKing = 0x8;
//     whiteQueen = 0x10;
//     blackPawn = whitePawn << 40;
//     blackRook = whiteRook << 56;
//     blackKnight = whiteKnight << 56;
//     blackBishop = whiteBishop << 56;
//     blackKing = whiteKing << 56;
//     blackQueen = whiteQueen << 56;
// }
// int firstPass = 1;

// int color;
// int castleState;
// uint64_t curr;
// int move(uint64_t board){
//     if(firstPass){
//         firstPass = 0;
//         initilize();
//         curr = whitePawn | whiteRook | whiteKnight | whiteBishop | whiteKing | whiteQueen | blackPawn | blackRook | blackKnight | blackBishop | blackKing | blackQueen;

//         //if the board is the same as origanl board, we are white
//         if(board != curr)
//             color = BLACK;
//         else{
//             color = WHITE;
//         }
//         castleState = CASTLE_ALL;
//     }
//     curr = whitePawn | whiteRook | whiteKnight | whiteBishop | whiteKing | whiteQueen | blackPawn | blackRook | blackKnight | blackBishop | blackKing | blackQueen;
//     printf("Total value: %" PRIx64 "\n%" PRIx64 " ", board , curr);
//     return 0;
// }

// int main(){
//     move(4);
//     return 0;
// }



