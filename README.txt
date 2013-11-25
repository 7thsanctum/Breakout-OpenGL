Breakout Game - OpenGL
======================

The Breakout game .exe file requires the following DLLs. Some of these are redundant and will be removed in future.

DevIL.dll
glew32.dll
GLFW.dll
ILU.dll
ILUT.dll

Controls
========

Space - Launch Ball

A or Left Arrow Key - Move Paddle Left
D or Right Arrow Key - Move Paddle Right

Up/Down Key - Speed ball up/down


Known Problems and Future Improvments
=====================================
1. Collision detection needs improving, really bad edge and corner cases. If ball moves too fast will clip through objects etc.

2. Log and display score.

3. Speed up ball as time goes by.

4. Add mouse control for paddle.

5. Menu screen with options for paddel sensitivity when using keyboard.

6. Integrate Box2D support.

7. The ball is invisible after it dies, but appears when it is alive. This is because it is marked as "inactive" when it dies and 
objects marked as "inactive" are not rendered. Need to change it so it's position is reset and velocity set to 0. 

Probably much much more.