#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "defs.h"
#include <string.h>




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
void printBoard();
void parseFEN();




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


//the needed boards to rep every subset of pieces
u64 boards[12];
u64 occ[3];

int turn = -1;

int enpassent = noSq;
int castle;

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
    for(int r = 0; r < 8; r++){
        printf("%d  ", 8 - r);
        for(int f = 0; f < 8; f++){
            int sq = r*8 + f;
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

void printBoard(){
    printf("   A B C D E F G H\n");
    for( int r = 0; r < 8; r ++){
        printf("%d  ", 8 - r);
        for(int f = 0; f < 8; f++){
            int sq = r * 8 + f;
            int piece = -1;
            for(int i = P; i <= k; i++){
                if(getBit(boards[i],sq))
                piece = i;
            }
            printf(" %c", (piece == -1) ? '-' : ascii[piece]);
        }
        printf("\n");
    }
}

void parseFEN(char* fen){
    memset(boards, 0ULL, sizeof(boards));
    memset(occ, 0ULL, sizeof(occ));
    turn = -1;
    enpassent = noSq;
    castle = 0;
    for(int r = 0; r < 8; r++){
        for(int f = 0; f < 8; f++){
            int sq = r * 8 + f;

            //takes the first c from fen, and inc that spot on the board
            //this is for when there is not an empty sq
            if((*fen <= 'z' && *fen >= 'a') || (*fen <= 'Z' && *fen >= 'A')){
                int p = pieceChar[(int)(*fen)];
                setBit(boards[p],sq);
                fen++;
            }
            //repositioning 
            if(*fen >= '0' && *fen <= '9'){
                f +=  (int)(*fen - '0');
                //parsing empty sq
                int piece = -1;
                for(int i = P; i <= k; i++){
                    if(getBit(boards[i],sq))
                    piece = i;
                }
                
                if(piece == -1) f--;
                fen++;
            }
        }
        if(*fen == '/') fen++;

    }

    //board state
    (*fen == 'w') ? (turn = white) : (turn = black);

    fen += 2;
    
    while(*fen != ' '){
        switch (*fen)
        {
        case  'K':
            castle |= wk;
            break;
        case  'Q':
            castle |= wq;
            break;
        case  'k':
            castle |= bk;
            break;
        case  'q':
            castle |= bq;
            break;
        case  '-':
            break;
        }
        fen++;
    }
    fen++;
    if(*fen != '-'){
        int f = (int)(fen[0] - '0');
        int r = 8 - (int)(fen[1] - '0');

        enpassent = 8 * r + f;
    }
    else enpassent = noSq;

    for(int i = P; i <= k; i ++){
        // i / 6 gives side
            occ[i / p] |= boards[i];
        }
        debugging(occ[white]);
    occ[both] = occ[white] | occ[black];



}

int main(){
    initLeaps();
    initSliders(1);
    initSliders(0);
    parseFEN(trickyPosition);
    printBoard();

    
    return 0;
}