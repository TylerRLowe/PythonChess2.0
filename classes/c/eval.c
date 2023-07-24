#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "defs.h"




void debugging();
u64 pawnMask();
u64 knightMask();
u64 kingMask();
u64 bishopMask();
u64 bishopMaskCurrent();
u64 rookMask();
u64 rookMaskCurrent();
void initLeaps();




//setting up the masks:
u64 pawnAttacks[2][64];

//both sides have same attacks
u64 kingtAttacks[64];

u64 knightAttacks[64];



int main(){
    u64 block = 0;
    setBit(block,c3);
    bishopMaskCurrent(d4,block);

    
    return 0;
}


u64 kingMask(int sq){
    u64 bitBoard = 0;

    u64 attacks = 0;
    setBit(bitBoard,sq);

    //  shifting does not loop so no need to check
    attacks |= (bitBoard << 7 & notHFile);
    attacks |= (bitBoard << 9 & notAFile);
    attacks |= bitBoard << 8;
    attacks |= (bitBoard >> 7 & notAFile);
    attacks |= (bitBoard >> 9 & notHFile);
    attacks |= bitBoard >> 8;
    attacks |= (bitBoard << 1 & notAFile);
    attacks |= (bitBoard >> 1 & notHFile);

    return attacks;

}

u64 knightMask(int sq){
    u64 bitBoard = 0;

    u64 attacks = 0;
    setBit(bitBoard,sq);

    attacks |= (bitBoard >> 17 & notHFile) | (bitBoard << 17 & notAFile); 
    attacks |= (bitBoard >> 15 & notAFile) | (bitBoard << 15 & notHFile); 
    attacks |= (bitBoard >> 10 & notHGFile) | (bitBoard << 10 & notABFile);
    attacks |= (bitBoard >> 6 & notABFile) | (bitBoard >> 6 & notABFile);  
    
    return attacks;

}


u64 pawnMask(int sq, int color){
    u64 bitBoard = 0;

    u64 attacks = 0;
    setBit(bitBoard,sq);

    //  black
 
    attacks |= ((bitBoard << 7) & notHFile & color);
    attacks |= ((bitBoard << 9) & notAFile & color);
    
    //white
    attacks |= ((bitBoard >> 7) & notAFile & ~ color);
    attacks |= ((bitBoard >> 9) & notHFile & ~ color);

    return attacks;
}



void initLeaps(){
    //loop board
    for(int sq = 0; sq < 64; sq++){
        
        pawnAttacks[white][sq] = pawnMask(white, sq);
        pawnAttacks[black][sq] = pawnMask(black, sq);
        knightAttacks[sq] = knightMask(sq);
        knightAttacks[sq] = kingMask(sq);
    }
}


u64 bishopMask(u64 sq){
    u64 attacks = 0;
    //rank , file
    int r, f;

    int targetRank = sq / 8;
    int targetFile = sq % 8;



    //is it possibe to combine these?
    for(r = targetRank + 1, f = targetFile + 1; r < 7 && f < 7; r++, f++) setBit(attacks, (r * 8) + f);
    for(r = targetRank + 1, f = targetFile - 1; r < 7 && f > 0; r++, f--) setBit(attacks, (r * 8) + f);
    for(r = targetRank - 1, f = targetFile + 1; r > 0 && f < 7; r--, f++) setBit(attacks, (r * 8) + f);
    for(r = targetRank - 1, f = targetFile - 1; r > 0 && f > 0; r--, f--) setBit(attacks, (r * 8) + f);
    return attacks;
}
u64 bishopMaskCurrent(u64 sq, u64 block){
    u64 attacks = 0;
    //rank , file
    int r, f;

    int targetRank = sq / 8;
    int targetFile = sq % 8;

    

    //is it possibe to combine these?
    //doing the extra bit set to set attacker as true
    for(r = targetRank + 1, f = targetFile + 1; r < 8 && f < 8 && !((bit << (((r -1) * 8) + f - 1)) & block); r++, f++) setBit(attacks, (r * 8) + f);
    for(r = targetRank + 1, f = targetFile - 1; r < 8 && f > -1 && !((bit << (((r - 1) * 8) + f + 1)) & block); r++, f--) setBit(attacks, (r * 8) + f);
    for(r = targetRank - 1, f = targetFile + 1; r > -1 && f < 8 && !(bit << ((((r + 1) * 8) + f - 1)) & block); r--, f++) setBit(attacks, (r * 8) + f);
    for(r = targetRank - 1, f = targetFile - 1; r > -1 && f > -1 && !(bit << ((((r + 1) * 8) + f + 1)) & block); r--, f--) setBit(attacks, (r * 8) + f);
    debugging(attacks);

    return attacks;
}

u64 rookMask(u64 sq){
    u64 attacks = 0;
    //rank , file
    int r, f;

    int targetRank = sq / 8;
    int targetFile = sq % 8;

    //combine?
    for(r = targetRank + 1; r < 7; r++) setBit(attacks,(r * 8) + targetFile);
    for(r = targetRank - 1; r > 0; r--) setBit(attacks,(r * 8) + targetFile);
    for(f = targetFile + 1; f < 7; f++) setBit(attacks,(targetRank * 8) + f);
    for(f = targetFile - 1;f > 0;f--) setBit(attacks,(targetRank * 8) + f);
   

    return attacks;
}

u64 rookMaskCurrent(u64 sq, u64 block){
    u64 attacks = 0;
    //rank , file
    int r, f;

    int targetRank = sq / 8;
    int targetFile = sq % 8;

    

    //is it possibe to combine these?
    for(r = targetRank + 1; r < 8 && !((bit << (((r - 1)* 8) + targetFile)) & block); r++) setBit(attacks, (r * 8) + targetFile);
    for(r = targetRank - 1; r > -1  && !(bit << ((((r + 1)* 8) + targetFile)) & block); r--) setBit(attacks, (r * 8) + targetFile);
    for(f = targetFile + 1; f < 8 && !(bit << (((targetRank * 8) + f-1)) & block);f++) setBit(attacks, (targetRank * 8) + f);
    for(f = targetFile - 1; f > -1 && !((bit << ((targetRank * 8) + f +1)) & block); f--) setBit(attacks, (targetRank * 8) + f);
    return attacks;
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



