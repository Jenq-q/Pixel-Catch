# Pixel Catch

A simple arcade-style ball-catching game built in C++ using the BGI graphics library. This project was created to practice C++ programming and learn basic game development concepts.

## About This Project

Pixel Catch is a classic arcade game where players move a paddle across three lanes to catch falling balls. I built this game to explore graphics programming in C++ and implement fundamental game development patterns like game loops, collision detection, and state management.

## What I Learned

Through this project, I gained experience with:
- **C++ Programming**: Working with structs, functions, and basic object-oriented concepts
- **Graphics Programming**: Using BGI library to draw shapes, handle colors, and create animations
- **Game Development**: Implementing game loops, state machines, and user input handling
- **File I/O**: Reading and writing high scores to persistent storage
- **Problem Solving**: Debugging collision detection and optimizing rendering performance

## Features

### Gameplay
- 3-lane movement system with arrow key controls
- Progressive difficulty - speed increases every 5 points
- High score tracking that saves between sessions
- Game over detection when balls are missed

### Technical Implementation
- State-based game structure (Menu → Playing → Game Over)
- Real-time collision detection between ball and player
- Efficient sprite rendering with proper clearing/redrawing
- Keyboard input handling for responsive controls

## Code Structure

I organized the code into several key functions:

```cpp
// Main game functions
void initializeGame(Game& game);    // Sets up initial game state
void updateGame(Game& game);        // Main game logic loop
void handleInput(Game& game);       // Processes keyboard input

// Drawing functions
void drawPlayer(const Game& game);  // Renders the player paddle
void drawBall(const Game& game);    // Renders the falling ball
void drawGameArea();                // Sets up the playing field

// Utility functions
int readHighScoreFromFile();        // Loads saved high score
void writeHighScoreToFile(int);     // Saves new high score
```

The game uses a simple `Game` struct to hold all the important data:
```cpp
struct Game {
    int playerLane, ballLane;  // Which lane each object is in
    int score, highScore;      // Scoring system
    int speed;                 // Current ball speed
    bool ballActive;           // Whether ball is falling
    GameState state;           // Current game state
};
```

## Learning Resources Used

While building this project, I found these resources helpful:
- C++ graphics programming tutorials
- BGI library documentation
- Game development pattern guides
- Stack Overflow for debugging specific issues

I'm still learning, so constructive criticism is welcome!

---

**Thanks for checking out my game! This was a fun project to work on and helped me understand game development basics.**
