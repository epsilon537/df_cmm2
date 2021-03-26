Dragonfly Game Engine for CMM2
------------------------------
Author: Mark Claypool
CMM2 port by Epsilon

Changelog
---------
0.1: Initial version

Description
-----------
Dragonfly is a text-based game engine, primarily designed to teach about game engine development.
The author wrote a free book that teaches how to program a game engine from scratch.
For more info, see the Dragonfly website:

https://dragonfly.wpi.edu

This is a CMM2 MMBasic port of the Dragonfly Game Engine. Dragonfly as documented on the website
and book, is a C++ Object Oriented Design. The CMM2 version, although written in MMBasic, has
retained most of the Object Oriented Design, with compromises for the sake of performance.

Features:
--------
* Sprite Animation
* Simple Kinematics
* World/View representation
* Timing support
* Bounding Box based collision detection
* Object Management
* Event Dispatching (E.g. collision, keyboard or nunchuk input events, user defined events...)
* Nunchuk support
* Sound support
* HUD support

Directory Layout:
----------------
* INC/ contains the game engine implementation
* TEST/ contain game engine test cases. They roughly match the assignments in the book but have
evolved a bit more there to make sure that they can run against the finalized version of the
game engine. The test code is messy but all test cases work and it's a good repository to check
to see how certain parts of the API work.
* SaucerShooter/ A demo mini-game used to demonstrate most of the features of the game engine.
SaucerShooter also serves as my benchmark for performance tuning:
  *ss_game: Starts the game
  *ss_game L: Starts the game and displays the game loop timing in the upper left corner.
  *ss_game F: Starts the game with frame rate limiting disabling and xProfiling enabled
              (ref. https://github.com/epsilon537/xprof_cmm2 for more info on xProf).

Required CMM2 FW:
----------------
V5.07.00b2 or later

GitHub:
------
CMM2 code base: https://github.com/epsilon537/df_win
Windows MSVC code base: https://github.com/epsilon537/df_cmm2

               
