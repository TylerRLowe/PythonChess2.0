#include "defs.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

uint64_t whitePawn;
uint64_t whiteRook;
uint64_t whiteKnight;
uint64_t whiteBishop;
uint64_t whiteKing;
uint64_t whiteQueen;
uint64_t blackPawn;
uint64_t blackKnight;
uint64_t blackBishop;
uint64_t blackKing;
uint64_t blackQueen;
uint64_t blackRook;
// making the starting positions
void initilize(){
    //bottom right square = bit 0 , top right = bit 63
    whitePawn = 0xFF00;
    whiteRook = 0x81;
    whiteKnight = 0x42;
    whiteBishop = 0x24;
    whiteKing = 0x8;
    whiteQueen = 0x10;
    blackPawn = whitePawn << 40;
    blackRook = whiteRook << 56;
    blackKnight = whiteKnight << 56;
    blackBishop = whiteBishop << 56;
    blackKing = whiteKing << 56;
    blackQueen = whiteQueen << 56;
}


int main(){
    initilize();
    return 0;
}
int firstPass = 0;

int color;

int move(uint64_t board){
    uint64_t curr = whitePawn | whiteRook | whiteKnight | whiteBishop | whiteKing | whiteQueen | blackPawn | blackRook | blackKnight | blackBishop | blackKing | blackQueen;
    if(firstPass){
        main();
        //if the board is the same as origanl board, we are white
        if(board == curr)
            color = BLACK;
        else{
            color = WHITE;
        }
    }
    printf("Total value: %" PRIx64 "\n%" PRIx64 " ", board , curr);
    return 0;
}