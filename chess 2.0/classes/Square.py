import pygame
from classes.Color import Color
color = Color()
# Tile creator
class Square:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.abs_x = x * width
        self.abs_y = y * height
        self.abs_pos = (self.abs_x, self.abs_y)
        self.pos = (x, y)
        self.color = color.cream if  (x + y) % 2 else color.lightBlue
        self.highlight_color = color.blue
        self.piece = None
        self.coord = self.get_coord()
        self.highlight = False
        self.rect = pygame.Rect(
            self.abs_x,
            self.abs_y,
            self.width,
            self.height
        )

    # get the formal notation of the tile
    def get_coord(self):
        columns = 'abcdefgh'
        return columns[self.x] + str(self.y + 1)

    def draw(self, display, move_color):
        radius = self.rect.width // 3
        # configures if tile should be light or dark or highlighted tile
        if self.highlight:
            if self.piece is None:
                # Draw solid gray circle for empty squares
                pygame.draw.rect(display, self.color, self.rect)
                pygame.draw.circle(display, color.gray, self.rect.center, radius)
            # The selected piece
            elif self.piece.color != move_color: 
                pygame.draw.rect(display, self.color, self.rect)
                radius *= 1.5
                pygame.draw.circle(display, color.gray, self.rect.center, radius, 15)    
            else:
                # Draw a hollow blue circle for other highlighted squares
                pygame.draw.rect(display, self.highlight_color, self.rect)
                  
        else:
            pygame.draw.rect(display, self.color, self.rect)
        # adds the chess piece icons
        if self.piece != None:
            centering_rect = self.piece.img.get_rect()
            centering_rect.center = self.rect.center
            display.blit(self.piece.img, centering_rect.topleft)