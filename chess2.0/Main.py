import pygame

from classes.Board import Board
from classes.BoardRep import engine_move

pygame.init()

WINDOW_SIZE = (600, 600)
screen = pygame.display.set_mode(WINDOW_SIZE)

board = Board(WINDOW_SIZE[0], WINDOW_SIZE[1])
one_player = True
def draw(display):
    display.fill('white')
    board.draw(display)
    pygame.display.update()

if __name__ == '__main__':
	running = True
	player_turn = False
	while running:
		x,y = pygame.mouse.get_pos()
		for event in pygame.event.get():
			# Quit the game if the user presses the close button
			if event.type == pygame.QUIT:
				running = False
			elif player_turn and event.type == pygame.MOUSEBUTTONDOWN: 
       			# If the mouse is clicked
				if event.button == 1:
					board.handle_click(x, y)
					player_turn = False
		if not player_turn:
			new_board=  engine_move()
			player_turn = True
			print(new_board)
		if board.is_in_checkmate('black'): # If black is in checkmate
			print('White wins!')
			running = False
		elif board.is_in_checkmate('white'): # If white is in checkmate
			print('Black wins!')
			running = False
		# Draw the board
		draw(screen)