# -*- coding: cp1252 -*-
import pygame
from pygame.locals import *
from robot import Robot
import math
import sys
import threading
import thread
from pybrain.utilities import threaded

class Simulation():

    def __init__(self):
        self.started = False
        self.run = True
        pass
    
    def setLeft(self,left):
        self.leftTrack = left

    def setRight(self,right):
        self.rightTrack = right

    def getX(self):
        if(self.started):
            return self.robot.getX()
        else:
            return 0
    def getY(self):
        if(self.started):
            return self.robot.getY()
        else:
            return 0
    def getAng(self):
        if(self.started):
            return self.robot.getAng()
        else:
            return 0
    def stop(self):
        self.run = False
        
    def start(self):
        pygame.init()
        screen = screen = pygame.display.set_mode((800,600))
        pygame.display.set_caption("Simulation")
        self.leftTrack = False
        self.rightTrack = False
        background = pygame.Surface(screen.get_size())

        white = pygame.Color(255,255,255)
        self.robot = Robot(400,300,90)
        sprites = pygame.sprite.Group(self.robot)

        clock = pygame.time.Clock()

        self.started = True
        while self.run:
            screen.fill(white)
            pressed = pygame.key.get_pressed()
            moving = False

            #if pressed[K_d]:
            #    leftTrack = True
            #if pressed[K_a]:
            #    rightTrack = True

            if self.leftTrack and self.rightTrack:
                self.robot.move()
            elif self.leftTrack and not self.rightTrack:
                self.robot.turnRight()
                self.robot.rotate()

            elif self.rightTrack and not self.leftTrack:
                self.robot.turnLeft()
                self.robot.rotate()

            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    sys.exit()

            sprites.draw(screen)
            font = pygame.font.SysFont("Calibri", 12)
            coords = font.render("(%d , %d)" % (self.robot.rect.center[0], self.robot.rect.center[1]), 1, (56,56,56))
            rota = font.render("%d°" % (self.robot.dir), 1, (56,56,56))
            screen.blit(coords,(700,30))
            screen.blit(rota,(718,45))

            if self.leftTrack:
                track = font.render("Left Track Active", 1, (56,56,56))
                screen.blit(track,(688,62))
            if self.rightTrack:
                track = font.render("Right Track Active", 1, (56,56,56))
                screen.blit(track,(688,75))
                
            pygame.display.flip()    
            clock.tick(30)
        pygame.quit()
        sys.exit()


