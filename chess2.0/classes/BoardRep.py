import ctypes
import os
from classes.Board import Board

lib = ctypes.CDLL(os.getcwd() +"/c/./engine.out")

lib.move.argtypes = [ctypes.c_uint64]

# passes the board to the c proram
#it will find the diference between the prev board and current to determine what pieces re moved
#therefore peice info does not need to be passed
#board starts at top left and moves down
def engine_move():
    boardvalue = 0
    shift = 63
    for y, row in enumerate(Board.config):
        for x, piece in enumerate(row):
            if piece is not "":
                boardvalue += 1 << shift
            
        shift -= 1
        
    value_from_python = ctypes.c_uint64(boardvalue)
    return lib.move(value_from_python)