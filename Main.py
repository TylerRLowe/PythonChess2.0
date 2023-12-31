import pygame

from classes.Board import Board

pygame.init()

WINDOW_SIZE = (600, 600)
screen = pygame.display.set_mode(WINDOW_SIZE)
one_player = True
board = Board(WINDOW_SIZE[0], WINDOW_SIZE[1],one_player,screen)
def draw(display):
    display.fill('white')
    board.draw()
    pygame.display.update()

if __name__ == '__main__':
	running = True
	while running:
		x,y = pygame.mouse.get_pos()
		for event in pygame.event.get():
			# Quit the game if the user presses the close button
			if event.type == pygame.QUIT:
				running = False
			elif event.type == pygame.MOUSEBUTTONDOWN: 
       			# If the mouse is clicked
				if event.button == 1:
					board.handle_click(x, y,)
		if board.is_in_checkmate('black'): # If black is in checkmate
			print('White wins!')
			running = False
		elif board.is_in_checkmate('white'): # If white is in checkmate
			print('Black wins!')
			running = False
		# Draw the board
		draw(screen)