import pygame
from classes.Square import Square
from classes.pieces.Rook import Rook
from classes.pieces.Bishop import Bishop
from classes.pieces.Knight import Knight
from classes.pieces.Queen import Queen
from classes.pieces.King import King
from classes.pieces.Pawn import Pawn
import ctypes
import os

lib = ctypes.CDLL(os.getcwd() +"/classes/c/./engine.out")
lib.move.argtypes = [ctypes.c_uint64]
# Game state checker
class Board:
	def __init__(self, width, height, one_player, display):
		self.width = width
		self.height = height
		self.display = display
		self.tile_width = width // 8
		self.tile_height = height // 8
		self.selected_piece = None
		self.turn = 'white'
		self.one_player = one_player

		# try making it chess.board.fen()
		self.config = [
			['bR', 'bN', 'bB', 'bQ', 'bK', 'bB', 'bN', 'bR'],
			['bP', 'bP', 'bP', 'bP', 'bP', 'bP', 'bP', 'bP'],
			['','','','','','','',''],
			['','','','','','','',''],
			['','','','','','','',''],
			['','','','','','','',''],
			['wP', 'wP', 'wP', 'wP', 'wP', 'wP', 'wP', 'wP'],
			['wR', 'wN', 'wB', 'wQ', 'wK', 'wB', 'wN', 'wR'],
		]

		self.squares = self.generate_squares()
		self.setup_board()


	def generate_squares(self):
		output = []
		for y in range(8):
			for x in range(8):
				output.append(
					Square(x,  y, self.tile_width, self.tile_height)
				)
		return output


	def get_square_from_pos(self, pos):
		for square in self.squares:
			if (square.x, square.y) == (pos[0], pos[1]):
				return square


	def get_piece_from_pos(self, pos):
		return self.get_square_from_pos(pos).piece


	def setup_board(self):
		# iterating 2d list
		for y, row in enumerate(self.config):
			for x, piece in enumerate(row):
				if piece != '':
					square = self.get_square_from_pos((x, y))

					# looking inside contents, what piece does it have
					if piece[1] == 'R':
						square.piece = Rook(
							(x, y), 'white' if piece[0] == 'w' else 'black', self
						)
					# as you notice above, we put `self` as argument, or means our class Board

					elif piece[1] == 'N':
						square.piece = Knight(
							(x, y), 'white' if piece[0] == 'w' else 'black', self
						)

					elif piece[1] == 'B':
						square.piece = Bishop(
							(x, y), 'white' if piece[0] == 'w' else 'black', self
						)

					elif piece[1] == 'Q':
						square.piece = Queen(
							(x, y), 'white' if piece[0] == 'w' else 'black', self
						)

					elif piece[1] == 'K':
						square.piece = King(
							(x, y), 'white' if piece[0] == 'w' else 'black', self
						)

					elif piece[1] == 'P':
						square.piece = Pawn(
							(x, y), 'white' if piece[0] == 'w' else 'black', self
						)


	def handle_click(self, mx, my):
		x = mx // self.tile_width
		y = my // self.tile_height
		clicked_square = self.get_square_from_pos((x, y))

		if self.selected_piece is None:
			if clicked_square.piece is not None:
				if clicked_square.piece.color == self.turn:
					self.selected_piece = clicked_square.piece

		elif self.selected_piece.move(self, clicked_square):
			self.turn = 'white' if self.turn == 'black' else 'black'
			if self.one_player:
				self.display.fill('white')
				self.draw()
				pygame.display.update()
				self.engine()

		elif clicked_square.piece is not None:
			if clicked_square.piece.color == self.turn:
				self.selected_piece = clicked_square.piece


	def is_in_check(self, color, board_change=None): # board_change = [(x1, y1), (x2, y2)]
		output = False
		king_pos = None

		changing_piece = None
		old_square = None
		new_square = None
		new_square_old_piece = None

		if board_change is not None:
			for square in self.squares:
				if square.pos == board_change[0]:
					changing_piece = square.piece
					old_square = square
					old_square.piece = None
			for square in self.squares:
				if square.pos == board_change[1]:
					new_square = square
					new_square_old_piece = new_square.piece
					new_square.piece = changing_piece

		pieces = [
			i.piece for i in self.squares if i.piece is not None
		]

		if changing_piece is not None:
			if changing_piece.notation == 'K':
				king_pos = new_square.pos
		if king_pos == None:
			for piece in pieces:
				if piece.notation == 'K' and piece.color == color:
						king_pos = piece.pos
		for piece in pieces:
			if piece.color != color:
				for square in piece.attacking_squares(self):
					if square.pos == king_pos:
						output = True

		if board_change is not None:
			old_square.piece = changing_piece
			new_square.piece = new_square_old_piece
						
		return output


	def is_in_checkmate(self, color):
		output = False

		for piece in [i.piece for i in self.squares]:
			if piece != None:
				if piece.notation == 'K' and piece.color == color:
					king = piece

		if king.get_valid_moves(self) == []:
			if self.is_in_check(color):
				output = True

		return output


	def draw(self):
		if self.selected_piece is not None:
			self.get_square_from_pos(self.selected_piece.pos).highlight = True
			for square in self.selected_piece.get_valid_moves(self):
				square.highlight = True

		for square in self.squares:
			square.draw(self.display, self.turn)
   

	def engine(self) -> ctypes.c_uint: 
  
		boardvalue = 0
		shift = 63
		for y, row in enumerate(self.config):
			for x, piece in enumerate(row):
				if piece != "":
					boardvalue += 1 << shift
				shift -= 1
		

		value_from_python = ctypes.c_uint64(boardvalue)
		return lib.move(value_from_python)