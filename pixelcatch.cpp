#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Game Constants - all the important numbers for the game
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GAME_AREA_WIDTH = 300;
const int GAME_AREA_HEIGHT = 400;
const int LANE_WIDTH = 38;
const int BOX_WIDTH = 12;  // Keep small to avoid overlapping lane borders
const int BOX_HEIGHT = 20;
const int BALL_RADIUS = 10;
const int NUM_LANES = 3;    // Easy to change for different difficulty levels
const int SPEED_INCREMENT = 3;
const int LANE_BORDER_WIDTH = 4;

// Game States - menu system states
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

// Game Structure - holds all the game data
struct Game {
    int playerX, playerY;    // Player position (Y is fixed, X calculated from lane)
    int ballX, ballY;        // Ball position
    int playerLane, ballLane; // Lane indices (0-2)
    int score, highScore;
    int speed;               // Ball fall speed - increases with score
    bool ballActive;         // Whether ball is currently falling
    GameState state;
};

// Color Definitions - way easier to read than numbers
const int PLAYER_COLOR = LIGHTBLUE;
const int BALL_COLOR = LIGHTRED;
const int LANE_COLOR = YELLOW;
const int BACKGROUND_COLOR = BLACK;
const int GAME_AREA_COLOR = DARKGRAY;
const int TEXT_COLOR = WHITE;
const int SCORE_COLOR = LIGHTGREEN;

// Function Prototypes - need these declared at the top
void initializeGame(Game& game);
void drawGameArea();
void drawPlayer(const Game& game);
void drawBall(const Game& game);
void clearPlayer(const Game& game);
void clearBall(const Game& game);
void showMenu();
void showGameOver(const Game& game);
void updateGame(Game& game);
void handleInput(Game& game);
void displayScores(const Game& game);
void drawGradientBackground();
void drawEnhancedBorder();
int getLaneX(int lane);
void drawAnimatedTitle();
void drawMenuElements();
void drawLaneLines();
int readHighScoreFromFile();
void writeHighScoreToFile(int highScore);

int main() {
    int gd = DETECT, gm;
    Game game;
    char key;
    
    // Initialize graphics window
    initgraph(&gd, &gm, (char*)"");
    
    setbkcolor(BACKGROUND_COLOR);
    cleardevice();
    
    initializeGame(game);
    srand(time(NULL)); // Seed random number generator
    
    // Main game loop
    while (true) {
        switch (game.state) {
            case MENU:
                showMenu();
                key = getch();
                if (key == ' ') { // Space bar starts game
                    game.state = PLAYING;
                    cleardevice();
                    drawGameArea();
                } else if (key == 27) { // ESC key
                    closegraph();
                    return 0;
                }
                break;
                
            case PLAYING:
                handleInput(game);
                updateGame(game);
                delay(8); // Controls overall game speed/smoothness
                break;
                
            case GAME_OVER:
                showGameOver(game);
                key = getch();
                if (key == ' ') {
                    // Save high score if player beat it
                    if (game.score > game.highScore) {
                        game.highScore = game.score;
                        writeHighScoreToFile(game.highScore);
                    }
                    initializeGame(game);
                    game.state = PLAYING;
                    cleardevice();
                    drawGameArea();
                } else if (key == 27) { // ESC
                    game.state = MENU;
                    cleardevice();
                }
                break;
        }
    }
    
    closegraph();
    return 0;
}

void initializeGame(Game& game) {
    // Reset all game values to starting state
    game.playerX = SCREEN_WIDTH / 2;
    game.playerY = SCREEN_HEIGHT - 80;
    game.ballX = 0;
    game.ballY = 50;
    game.playerLane = 1; // Start in middle lane
    game.ballLane = 0;
    game.score = 0;
    game.speed = 2; // Start with reasonable speed
    game.ballActive = false;
    game.state = MENU;
    // Load high score from file
    game.highScore = readHighScoreFromFile();
}

void drawGameArea() {
    // Draw all the background elements
    drawGradientBackground();
    drawEnhancedBorder();
    
    // Main playing area rectangle
    setcolor(GAME_AREA_COLOR);
    setfillstyle(SOLID_FILL, GAME_AREA_COLOR);
    bar(SCREEN_WIDTH / 2 - 80, 30, SCREEN_WIDTH / 2 + 80, SCREEN_HEIGHT - 30);
    
    drawLaneLines();
}

void drawLaneLines() {
    // Draw lane separator lines
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, LANE_BORDER_WIDTH);
    
    int leftLane = SCREEN_WIDTH / 2 - LANE_WIDTH;
    int rightLane = SCREEN_WIDTH / 2 + LANE_WIDTH;
    
    // Lane dividers - thick lines that won't get erased by moving objects
    line(leftLane-20, 60, leftLane-20, SCREEN_HEIGHT - 60); 
    line(leftLane+20, 60, leftLane+20, SCREEN_HEIGHT - 60);
    line(rightLane-20, 60, rightLane-20, SCREEN_HEIGHT - 60);
    line(rightLane+20, 60, rightLane+20, SCREEN_HEIGHT - 60);
    
    // Center guides - dotted lines for visual reference
    setlinestyle(DOTTED_LINE, 0, 1);
    setcolor(LIGHTGRAY);
    for (int i = 0; i < NUM_LANES; i++) {
        int x = getLaneX(i);
        for (int y = 80; y < SCREEN_HEIGHT - 80; y += 40) {
            line(x, y, x, y + 20);
        }
    }
    
    setlinestyle(SOLID_LINE, 0, 1);
}

void drawGradientBackground() {
    // Fill screen with black background
    setcolor(BACKGROUND_COLOR);
    setfillstyle(SOLID_FILL, BACKGROUND_COLOR);
    bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Add grid pattern for visual effect
    setcolor(DARKGRAY);
    setlinestyle(DOTTED_LINE, 0, 1);
    
    // Vertical grid lines
    for (int x = 50; x < SCREEN_WIDTH; x += 50) {
        line(x, 0, x, SCREEN_HEIGHT);
    }
    
    // Horizontal grid lines
    for (int y = 50; y < SCREEN_HEIGHT; y += 50) {
        line(0, y, SCREEN_WIDTH, y);
    }
    
    setlinestyle(SOLID_LINE, 0, 1);
}

void drawEnhancedBorder() {
    // Triple border effect
    setcolor(CYAN);
    setlinestyle(SOLID_LINE, 0, 3);
    rectangle(10, 10, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
    
    setcolor(BLUE);
    setlinestyle(SOLID_LINE, 0, 2);
    rectangle(15, 15, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 15);
    
    setcolor(LIGHTBLUE);
    setlinestyle(SOLID_LINE, 0, 1);
    rectangle(20, 20, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20);
}

int getLaneX(int lane) {
    // Convert lane number to X position
    return SCREEN_WIDTH / 2 - LANE_WIDTH + (lane * LANE_WIDTH);
}

void drawPlayer(const Game& game) {
    // Draw player rectangle
    setcolor(PLAYER_COLOR);
    setfillstyle(SOLID_FILL, PLAYER_COLOR);
    
    int x = getLaneX(game.playerLane);
    bar(x - BOX_WIDTH, game.playerY - BOX_HEIGHT, 
        x + BOX_WIDTH, game.playerY + BOX_HEIGHT);
    
    // Add white border highlight
    setcolor(WHITE);
    rectangle(x - BOX_WIDTH + 2, game.playerY - BOX_HEIGHT + 2,
              x + BOX_WIDTH - 2, game.playerY + BOX_HEIGHT - 2);
}

void drawBall(const Game& game) {
    if (!game.ballActive) return;
    
    // Draw ball circle
    setcolor(BALL_COLOR);
    setfillstyle(SOLID_FILL, BALL_COLOR);
    
    int x = getLaneX(game.ballLane);
    fillellipse(x, game.ballY, BALL_RADIUS, BALL_RADIUS);
    
    // Add shine effect
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x - 4, game.ballY - 4, 3, 3);
}

void clearPlayer(const Game& game) {
    // Erase player using game area color
    setcolor(GAME_AREA_COLOR);
    setfillstyle(SOLID_FILL, GAME_AREA_COLOR);
    
    int x = getLaneX(game.playerLane);
    bar(x - BOX_WIDTH - 1, game.playerY - BOX_HEIGHT - 1, 
        x + BOX_WIDTH + 1, game.playerY + BOX_HEIGHT + 1);
}

void clearBall(const Game& game) {
    if (!game.ballActive) return;
    
    // Erase ball using game area color
    setcolor(GAME_AREA_COLOR);
    setfillstyle(SOLID_FILL, GAME_AREA_COLOR);
    
    int x = getLaneX(game.ballLane);
    fillellipse(x, game.ballY, BALL_RADIUS + 1, BALL_RADIUS + 1);
}

void drawAnimatedTitle() {
    // Draw title with shadow effect
    setcolor(DARKGRAY);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    
    // Center the title text
    int titleWidth = textwidth((char*)"PIXEL CATCH");
    int titleX = (SCREEN_WIDTH - titleWidth) / 2;
    int titleY = SCREEN_HEIGHT / 2 - 80;
    
    // Draw shadow
    outtextxy(titleX + 2, titleY + 2, (char*)"PIXEL CATCH");
    
    // Draw main title
    setcolor(LIGHTRED);
    outtextxy(titleX, titleY, (char*)"PIXEL CATCH");
    
    // Draw border around title
    setcolor(RED);
    setlinestyle(SOLID_LINE, 0, 2);
    rectangle(titleX - 10, titleY - 10, titleX + titleWidth + 10, titleY + textheight((char*)"PIXEL CATCH") + 10);
    setlinestyle(SOLID_LINE, 0, 1);
}

void drawMenuElements() {
    // Add decorative elements
    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    
    // Left side decoration
    for (int i = 0; i < 5; i++) {
        fillellipse(50 + i * 15, 100 + i * 20, 5, 5);
    }
    
    // Right side decoration
    for (int i = 0; i < 5; i++) {
        fillellipse(SCREEN_WIDTH - 50 - i * 15, 100 + i * 20, 5, 5);
    }
    
    // Bottom decoration
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    for (int i = 0; i < 10; i++) {
        fillellipse(100 + i * 44, SCREEN_HEIGHT - 50, 3, 3);
    }
}

void showMenu() {
    // Clear and setup menu screen
    cleardevice();
    setbkcolor(BACKGROUND_COLOR);
    
    setcolor(BACKGROUND_COLOR);
    setfillstyle(SOLID_FILL, BACKGROUND_COLOR);
    bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Draw background grid
    setcolor(DARKGRAY);
    setlinestyle(DOTTED_LINE, 0, 1);
    for (int i = 0; i < SCREEN_WIDTH; i += 30) {
        line(i, 0, i, SCREEN_HEIGHT);
    }
    for (int i = 0; i < SCREEN_HEIGHT; i += 30) {
        line(0, i, SCREEN_WIDTH, i);
    }
    setlinestyle(SOLID_LINE, 0, 1);
    
    drawMenuElements();
    
    // Draw title
    drawAnimatedTitle();
    
    // Draw subtitle
    setcolor(CYAN);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    int subtitleWidth = textwidth((char*)"Swift, Grab, Score!");
    int subtitleX = (SCREEN_WIDTH - subtitleWidth) / 2;
    outtextxy(subtitleX, SCREEN_HEIGHT / 2 - 30, (char*)"Swift, Grab, Score!");
    
    // Draw instructions
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    
    int instrWidth1 = textwidth((char*)"Use LEFT/RIGHT arrows to move");
    int instrX1 = (SCREEN_WIDTH - instrWidth1) / 2;
    outtextxy(instrX1, SCREEN_HEIGHT / 2 + 20, (char*)"Use LEFT/RIGHT arrows to move");
    
    int instrWidth2 = textwidth((char*)"Catch the falling balls!");
    int instrX2 = (SCREEN_WIDTH - instrWidth2) / 2;
    outtextxy(instrX2, SCREEN_HEIGHT / 2 + 40, (char*)"Catch the falling balls!");
    
    // Draw start instruction
    setcolor(LIGHTGREEN);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    int startWidth = textwidth((char*)"Press SPACE to play");
    int startX = (SCREEN_WIDTH - startWidth) / 2;
    outtextxy(startX, SCREEN_HEIGHT / 2 + 80, (char*)"Press SPACE to play");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    int exitWidth = textwidth((char*)"or ESC to exit");
    int exitX = (SCREEN_WIDTH - exitWidth) / 2;
    outtextxy(exitX, SCREEN_HEIGHT / 2 + 105, (char*)"or ESC to exit");
    
    // Draw demo player and ball
    setcolor(PLAYER_COLOR);
    setfillstyle(SOLID_FILL, PLAYER_COLOR);
    bar(SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT - 90, SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT - 60);
    
    setcolor(WHITE);
    rectangle(SCREEN_WIDTH / 2 - 18, SCREEN_HEIGHT - 88, SCREEN_WIDTH / 2 + 18, SCREEN_HEIGHT - 62);
    
    setcolor(BALL_COLOR);
    setfillstyle(SOLID_FILL, BALL_COLOR);
    fillellipse(SCREEN_WIDTH / 2 + 40, SCREEN_HEIGHT - 130, 12, 12);
    
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(SCREEN_WIDTH / 2 + 36, SCREEN_HEIGHT - 134, 3, 3);
}

void showGameOver(const Game& game) {
    // Draw game over dialog box
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(SCREEN_WIDTH / 2 - 160, SCREEN_HEIGHT / 2 - 90, 
        SCREEN_WIDTH / 2 + 160, SCREEN_HEIGHT / 2 + 90);
    
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    bar(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 80, 
        SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2 + 80);
    
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, 3);
    rectangle(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 80, 
              SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2 + 80);
    
    // Draw "GAME OVER" text with shadow
    setcolor(DARKGRAY);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    int gameOverWidth = textwidth((char*)"GAME OVER");
    int gameOverX = (SCREEN_WIDTH - gameOverWidth) / 2;
    outtextxy(gameOverX + 2, SCREEN_HEIGHT / 2 - 48, (char*)"GAME OVER");
    
    setcolor(WHITE);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(gameOverX, SCREEN_HEIGHT / 2 - 50, (char*)"GAME OVER");
    
    // Show final score
    char scoreText[50];
    sprintf(scoreText, "Final Score: %d", game.score);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    int scoreWidth = textwidth(scoreText);
    int scoreX = (SCREEN_WIDTH - scoreWidth) / 2;
    outtextxy(scoreX, SCREEN_HEIGHT / 2 - 10, scoreText);
    
    // Show high score message if achieved
    if (game.score == game.highScore && game.score > 0) {
        setcolor(YELLOW);
        settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
        int highScoreWidth = textwidth((char*)"NEW HIGH SCORE!");
        int highScoreX = (SCREEN_WIDTH - highScoreWidth) / 2;
        outtextxy(highScoreX, SCREEN_HEIGHT / 2 + 15, (char*)"NEW HIGH SCORE!");
    }
    
    // Show control instructions
    setcolor(LIGHTCYAN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    int instrWidth = textwidth((char*)"SPACE: Play Again  ESC: Menu");
    int instrX = (SCREEN_WIDTH - instrWidth) / 2;
    outtextxy(instrX, SCREEN_HEIGHT / 2 + 50, (char*)"SPACE: Play Again  ESC: Menu");
}

void handleInput(Game& game) {
    // Check for keyboard input
    if (kbhit()) {
        char key = getch();
        switch (key) {
            case 75: // Left arrow key
                if (game.playerLane > 0) {
                    clearPlayer(game);
                    game.playerLane--;
                }
                break;
            case 77: // Right arrow key
                if (game.playerLane < NUM_LANES - 1) {
                    clearPlayer(game);
                    game.playerLane++;
                }
                break;
            case 27: // ESC key
                game.state = MENU;
                break;
        }
    }
}

void updateGame(Game& game) {
    // Main game update logic
    
    // Spawn ball if needed
    if (!game.ballActive) {
        game.ballLane = rand() % NUM_LANES;
        game.ballY = 70;
        game.ballActive = true;
    }
    
    clearBall(game);
    
    // Move ball down
    game.ballY += game.speed;
    
    // Check collision with player
    if (game.ballY >= game.playerY - BOX_HEIGHT) {
        if (game.ballLane == game.playerLane) {
            // Ball caught
            game.score++;
            game.ballActive = false;
            
            // Increase speed every 5 points
            if (game.score % 5 == 0) {
                game.speed++;
            }
            
        } else {
            // Ball missed
            game.state = GAME_OVER;
            return;
        }
    }
    
    // Ball fell off screen
    if (game.ballY > SCREEN_HEIGHT) {
        game.state = GAME_OVER;
        return;
    }
    
    // Redraw game objects
    drawPlayer(game);
    drawBall(game);
    displayScores(game);
}

void displayScores(const Game& game) {
    // Only redraw if values changed (optimization)
    static int lastScore = -1;
    static int lastHighScore = -1;
    static int lastSpeed = -1;
    
    char scoreText[50];
    
    if (game.score != lastScore || game.highScore != lastHighScore || game.speed != lastSpeed) {
        // Clear score area
        setcolor(BACKGROUND_COLOR);
        setfillstyle(SOLID_FILL, BACKGROUND_COLOR);
        bar(20, 20, SCREEN_WIDTH - 20, 50);
        
        // Draw current score
        setcolor(LIGHTGREEN);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        sprintf(scoreText, "Score: %d", game.score);
        outtextxy(30, 25, scoreText);
        
        // Draw high score
        setcolor(YELLOW);
        sprintf(scoreText, "High: %d", game.highScore);
        outtextxy(180, 25, scoreText);
        
        // Draw speed indicator
        setcolor(CYAN);
        sprintf(scoreText, "Speed: %d", game.speed);
        outtextxy(320, 25, scoreText);
        
        // Update cache values
        lastScore = game.score;
        lastHighScore = game.highScore;
        lastSpeed = game.speed;
    }
}

// File I/O functions for high score persistence
int readHighScoreFromFile() {
    FILE* file = fopen("highscore.txt", "r");
    int highScore = 0;
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
    return highScore;
}

void writeHighScoreToFile(int highScore) {
    FILE* file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
    // Could add error handling here
}