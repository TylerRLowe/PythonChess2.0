

#define WHITE 0x00
#define BLACK 0x10

#define EMPTY 0x00
#define PAWN 0x01
#define KNIGHT 0x02
#define BISHOP 0x03
#define ROOK 0x04
#define QUEEN 0x05
#define KING 0x06

#define WHITE_PAWN 0x01
#define WHITE_KNIGHT 0x02
#define WHITE_BISHOP 0x03
#define WHITE_ROOK 0x04
#define WHITE_QUEEN 0x05
#define WHITE_KING 0x06

#define BLACK_PAWN 0x11
#define BLACK_KNIGHT 0x12
#define BLACK_BISHOP 0x13
#define BLACK_ROOK 0x14
#define BLACK_QUEEN 0x15
#define BLACK_KING 0x16

#define PIECE(x) ((x) & 0x0f)
#define COLOR(x) ((x) & 0x10)

#define CASTLE_ALL 14
#define CASTLE_WHITE 3
#define CASTLE_BLACK 11
#define CASTLE_WHITE_KING 1
#define CASTLE_WHITE_QUEEN 2
#define CASTLE_BLACK_KING 4
#define CASTLE_BLACK_QUEEN 7

#define MATERIAL_PAWN 100
#define MATERIAL_KNIGHT 320
#define MATERIAL_BISHOP 330
#define MATERIAL_ROOK 500
#define MATERIAL_QUEEN 900
#define MATERIAL_KING 20000


#define RF(rank, file) ((rank) * 8 + (file))




#define RANK_1 0x00000000000000ffL
#define RANK_2 0x000000000000ff00L
#define RANK_3 0x0000000000ff0000L
#define RANK_4 0x00000000ff000000L
#define RANK_5 0x000000ff00000000L
#define RANK_6 0x0000ff0000000000L
#define RANK_7 0x00ff000000000000L
#define RANK_8 0xff00000000000000L

#define FILE_A 0x0101010101010101L
#define FILE_B 0x0202020202020202L
#define FILE_C 0x0404040404040404L
#define FILE_D 0x0808080808080808L
#define FILE_E 0x1010101010101010L
#define FILE_F 0x2020202020202020L
#define FILE_G 0x4040404040404040L
#define FILE_H 0x8080808080808080L

typedef unsigned long long bb;

void bb_init();
void bb_print(bb value);
bb bb_random();

extern const int POSITION_WHITE_PAWN;
extern const int POSITION_WHITE_KNIGHT;
extern const int POSITION_WHITE_BISHOP;
extern const int POSITION_WHITE_ROOK;
extern const int POSITION_WHITE_QUEEN;
extern const int POSITION_WHITE_KING;
extern const int POSITION_BLACK_PAWN;
extern const int POSITION_BLACK_KNIGHT;
extern const int POSITION_BLACK_BISHOP;
extern const int POSITION_BLACK_ROOK;
extern const int POSITION_BLACK_QUEEN;
extern const int POSITION_BLACK_KING;

typedef struct {
    int squares[64];
    int color;
    int castle;
    int white_material;
    int black_material;
    int white_position;
    int black_position;

} Board;

