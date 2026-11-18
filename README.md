# Apartment-
This project presents a 2D interior room scene developed using OpenGL. 

Algorithms Used:
1. DDA Algorithm →
     smooth line drawing
     (window, floor)

2. Bresenham Algorithm → 
     efficient line drawing (borders)

3. Midpoint Circle Algorithm → 
     circles (clock, door knob)

2D Transformations:
1. Translation → moving cloud, object placement
2. Scaling → resizing objects
3. Rotation → clock hands
4. Reflection → sofa symmetry
5. Shear → tilted book

Scene Components:
1. Window
Shows outdoor view with tree and moving cloud
2. Sofa
Designed with symmetry using reflection
3. Bookshelf
Contains multiple books, including a tilted one
4. Table
Created using polygon shapes
5. Door & Clock
Includes circular knob using circle algorithm . Animated with rotating hands


Animations:
1. Cloud Animation
2. Clock Animation
3. Ball & Tree Animation


Implementation:
1. Lanuguage: C++
2. Library: OpenGL (FreeGLUT)
3. Modular design using functions like: drawCloud(), drawTree(), drawSofa(), drawClock()
