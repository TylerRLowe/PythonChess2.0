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
void initSliders();
u64 setOccupency();




//setting up the masks:
u64 pawnAttacks[2][64];

//both sides have same attacks
u64 kingtAttacks[64];

u64 knightAttacks[64];

//stolen from chess wiki magic bitboard https://www.chessprogramming.org/Main_Page
u64 bishopMasks[64];
u64 rookMasks[64];

u64 bishopAttacks[64][512];
u64 rookAttacks[64][4096];




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
    for(r = targetRank + 1, f = targetFile + 1; r < 7 && f < 7; r++, f++) setBit(attacks, ((r * 8) + f));
    for(r = targetRank + 1, f = targetFile - 1; r < 7 && f > 0; r++, f--) setBit(attacks, ((r * 8) + f));
    for(r = targetRank - 1, f = targetFile + 1; r > 0 && f < 7; r--, f++) setBit(attacks, ((r * 8) + f));
    for(r = targetRank - 1, f = targetFile - 1; r > 0 && f > 0; r--, f--) setBit(attacks, ((r * 8) + f));
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
    for(r = targetRank + 1, f = targetFile + 1; r < 8 && f < 8 && !((bit << (((r -1) * 8) + f - 1)) & block); r++, f++) setBit(attacks, ((r * 8) + f));
    for(r = targetRank + 1, f = targetFile - 1; r < 8 && f > -1 && !((bit << (((r - 1) * 8) + f + 1)) & block); r++, f--) setBit(attacks, ((r * 8) + f));
    for(r = targetRank - 1, f = targetFile + 1; r > -1 && f < 8 && !(bit << ((((r + 1) * 8) + f - 1)) & block); r--, f++) setBit(attacks, ((r * 8) + f));
    for(r = targetRank - 1, f = targetFile - 1; r > -1 && f > -1 && !(bit << ((((r + 1) * 8) + f + 1)) & block); r--, f--) setBit(attacks, ((r * 8) + f));

    return attacks;
}

u64 rookMask(u64 sq){
    u64 attacks = 0;
    //rank , file
    int r, f;

    int targetRank = sq / 8;
    int targetFile = sq % 8;

    //combine?
    for(r = targetRank + 1; r < 7; r++) setBit(attacks,((r * 8) + targetFile));
    for(r = targetRank - 1; r > 0; r--) setBit(attacks,((r * 8) + targetFile));
    for(f = targetFile + 1; f < 7; f++) setBit(attacks,((targetRank * 8) + f));
    for(f = targetFile - 1;f > 0;f--) setBit(attacks,((targetRank * 8) + f));
   

    return attacks;
}

u64 rookMaskCurrent(u64 sq, u64 block){
    u64 attacks = 0;
    //rank , file
    int r, f;

    int targetRank = sq / 8;
    int targetFile = sq % 8;

    

    //is it possibe to combine these?
    for(r = targetRank + 1; r < 8 && !((bit << (((r - 1)* 8) + targetFile)) & block); r++) setBit(attacks, ((r * 8) + targetFile));
    for(r = targetRank - 1; r > -1  && !(bit << ((((r + 1)* 8) + targetFile)) & block); r--) setBit(attacks, ((r * 8) + targetFile));
    for(f = targetFile + 1; f < 8 && !(bit << (((targetRank * 8) + f-1)) & block);f++) setBit(attacks, ((targetRank * 8) + f));
    for(f = targetFile - 1; f > -1 && !((bit << ((targetRank * 8) + f +1)) & block); f--) setBit(attacks, ((targetRank * 8) + f));
    return attacks;
}

void initSliders(int bishop){
    for(int sq = 0; sq < 64; sq ++){
        bishopMasks[sq] = bishopMask(sq);
        rookMasks[sq] = rookMask(sq);

        u64 attacks = bishop ? bishopMasks[sq] : rookMasks[sq];

        int releventBits = bitCount(attacks);

        int occupencyI = 1 << releventBits;
        for(int i = 0; i < occupencyI; i++){
            if(bishop){
                u64 occupency = setOccupency(i, releventBits, attacks);
                int magic = (occupency * bishopMagicNumbers[sq]) >> (64 - bRB[sq]);
                bishopAttacks[sq][magic] = bishopMaskCurrent(sq, occupency);
            }
            else{ //rook
                u64 occupency = setOccupency(i, releventBits, attacks);
                int magic = (occupency * rookMagicNumbers[sq]) >> (64 - rRB[sq]);
                rookAttacks[sq][magic] = rookMaskCurrent(sq, occupency);
            }
        }

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

//used for magic indicies
//is already optimal
//https://www.chessprogramming.org/Magic_Bitboards
u64 setOccupency(int i, int bitC, u64 attacks){
    u64 occupency = 0ULL;
    
    for(int j = 0; j < bitC; j++){
        int sq = getLSB(attacks);

        popBit(attacks,sq);

        if(i & (1 << j)){
            occupency |= (bit << sq);
        }
    }
     return occupency;
}


static inline u64 getBishopAttacks(int sq, u64 board){
    board &= bishopMasks[sq];
    board *= bishopMagicNumbers[sq];
    board >>= 64 - bRB[sq];
    return bishopAttacks[sq][board];
}

static inline u64 getRookAttacks(int sq, u64 board){
    board &= rookMasks[sq];
    board *= rookMagicNumbers[sq];
    board >>= 64 - rRB[sq];
    return rookAttacks[sq][board];
}


int main(){
    initLeaps();
    initSliders(1);
    initSliders(0);
    u64 board = 0ULL;
    setBit(board,c4);
    debugging(board);
    debugging(getRookAttacks(d4,board));

    
    return 0;
}