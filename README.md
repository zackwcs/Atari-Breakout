# Breakout Project
## Introduction
Based on the classic game, Breakout, the player controls a rectangle that is used to bounce a ball. The rectangle that the player controls only moves on the horizontal axis, and it’s used to redirect the ball and prevent it from going off the screen. The goal of this game is to break all of the obstacles by bouncing the ball against them. Once the ball hits an obstacle, a sound will play to indicate that a block has been destroyed. The player will either break all the blocks and win the game, or the ball goes off the screen and the player loses. The player will be given 1 extra ball in case the original ball falls off the screen.



## Complexities
All Successfully Implemented
- Nokia 5110 LCD - An 84 X 48 display used to provide the visuals for the Breakout game. All the animations and objects are displayed here and it is the main interface for the player.
- Joystick Module - The player uses this to control the rectangle at the bottom of the screen. This rectangle is responsible for bouncing the ball and can only move left and right. This is the player's main input for controlling the game.
- Passive Sound Buzzer - Every time the ball collides with a block, this sound buzzer plays a noise to indicate that the block was destroyed. Gives the player feedback while playing the game.


## User Guide
When starting up the project, the player will be greeted with an intro screen prompting the player to press start in order to play. This start button can be identified by its distinct green color. The player is also given a red reset button, which they can press anytime to reset the game and bring them back to this intro screen. When the player presses start, the game will immediately begin, and the user will have to use the Joystick module to move the rectangle horizontally. The player will see a series of blocks and a moving ball on the screen. The player’s goal is to bounce the ball and break all of the blocks with their rectangle. The player will know if they have broken a block because a sound will play and the block will disappear. The player will also be given one extra life which will be indicated by a glowing LED. The LED will be on if they have an extra life and it will be off if they have already used it. 

## Hardware Components Used
- Computing
  - Elegoo UNO R3 microcontroller
- Inputs
  - Joystick Module
  - Reset Button
  - Start Button
- Outputs
  - Nokia 5110 LCD screen
  - Passive Sound Buzzer
  - LED

## Software Libraries Used
- SPI.h and LCD_Functions.h from SparkFun - These libraries were used to help implement and display the visuals for the Nokia 5110.
- Vector.h from Arduino’s Library Manager - This was used to help store all of the obstacle entities. Allowed the obstacles to be stored dynamically so that when they are destroyed they can be removed from the vector.

## Wiring Diagram

## Task Diagram

## SynchSM Diagrams
