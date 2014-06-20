from pygame.math import Vector2
import pygame
import math

class Robot(pygame.sprite.Sprite):

    def __init__(self, x, y, orientation):
        pygame.sprite.Sprite.__init__(self)
        self.x = x
        self.y = y
        self.imageMaster = pygame.image.load('robot.png').convert()
        self.image = self.imageMaster
        self.image.set_colorkey((255,255,255))
        self.rect = self.image.get_rect()
        self.dir = orientation
        #self.rect.center = (400,300)

    def getX(self):
        return self.rect.center[0] + 0.0
    def getY(self):
        return self.rect.center[1] + 0.0
    def getAng(self):
        return self.dir + 0.0
    
    def rotate(self):
        origCenter = self.rect.center
        self.image = pygame.transform.rotate(self.imageMaster, -self.dir)
        self.rect = self.image.get_rect()
        self.rect.center = origCenter

    def turnLeft(self):
        self.dir -= 2
        if self.dir < 0:
            self.dir = 358
        
    def turnRight(self):
        self.dir += 2
        if self.dir > 360:
            self.dir = 2
        
    def move(self):
        if self.dir == 360:
            self.dir = 0
        radians = self.dir * (math.pi/180)
        vX = -math.sin(radians)
        vY = math.cos(radians)
        vec = Vector2(vX, vY)
        #print(vec)
        #print(vec.length())
        #print(self.dir)
        #print('-----------------------------------------------------------')
        #self.rect.center = (self.rect.center[0] + vX, self.rect.center[1] + vY)
        if((self.rect.center[0] + round(vX*3))>=800):
            self.rect.center = (800,self.rect.center[1])
        elif((self.rect.center[0] + round(vX*3))<0):
            self.rect.center = (0,self.rect.center[1]) 
        else:
            self.rect.center = (self.rect.center[0] + round(vX*3),self.rect.center[1])
            
        if((self.rect.center[1] + round(vY*3))>=600):
            self.rect.center = (self.rect.center[0],600)
        elif((self.rect.center[1] + round(vY*3))<0):
            self.rect.center = (self.rect.center[0],0)
        else:
            self.rect.center = (self.rect.center[0],self.rect.center[1] + round(vY*3))
