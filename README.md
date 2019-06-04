# Graphics - Assignment 1
# Submitted by: Aniruddha Deshpande - 20161058

# Directory Structure

.
├── CMakeLists.txt
├── README.md
└── src
    ├── ball.cpp
    ├── ball.h
    ├── color.cpp
    ├── input.cpp
    ├── leg.cpp
    ├── leg.h
    ├── magnet.cpp
    ├── magnet.h
    ├── main.cpp
    ├── main.h
    ├── nonedit.cpp
    ├── other_handlers.cpp
    ├── rectangle.cpp
    ├── rectangle.h
    ├── Sample_GL.frag
    ├── Sample_GL.vert
    ├── semicircle.cpp
    ├── semicircle.h
    ├── slope.cpp
    ├── slope.h
    ├── spikes.cpp
    ├── spikes.h
    ├── timer.cpp
    └── timer.h

# Make

## Instructions

1. Create a directory named 'build' using : `mkdir build`
2. Change the directory to build by following : `cd build`
3. Run : `cmake ..`
4. Run : `make`
5. Run the following to execute the game : `./graphics_asgn1`

# Entities

1. The game contains a player ball initially spawned on the ground.
2. Balls flying with some having wooden slopes on them.
3. Moving Spikes.
4. Pond.
5. Bar Magnet.
6. Trampoline.

# Controls

1. Up Arrow Key : Jump.
2. Left Arrow Key : Move Left.
3. Right Arrow Key : Move Right.
4. Q : Quit.


# Features and Rules

1. Player gets 10 points for bouncing off of a normal ball.
2. Player gets 20 points for bouncing off of a ball with a slope.
3. Player can boost the jump using the trampoline.
4. Player is penalised by 1 point when hit by the moving spikes in any direction and is moved away from the spikes.
5. Player can zoom in and out using the scroll wheel or trackpad.
6. Player when touches the magnet gets stuck, only can move left or right when stuck.
7. Player bounces off the slopes based on the random angle of the slope and its current velocity.
8. Player sinks within the pond with its ability to move along side the curved arc of the pond's floor.
9. Jumping capacity is reduced when in the pond.
