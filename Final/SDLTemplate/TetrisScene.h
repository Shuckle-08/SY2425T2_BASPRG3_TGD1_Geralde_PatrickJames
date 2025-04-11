#pragma once
#include "Scene.h"
#include "Tetromino.h"
#include "input.h"
#include "draw.h"
#include "text.h"
#include <cstdlib> 
#include <vector>
#include <algorithm>
#include <random>

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;

class TetrisScene : public Scene {
public:
    TetrisScene();
    ~TetrisScene();

    void start();
    void update();
    void draw();

    void handleInput();

private:
    int grid[GRID_HEIGHT][GRID_WIDTH];
    Tetromino* currentTetromino;
    Tetromino* nextTetromino;

    Tetromino* holdTetromino;
    bool holdUsed;

    int dropTimer;
    int dropDelay;

    int score;
    int level;
    int linesCleared;

    //Initializing
    int gridOffsetX;
    int gridOffsetY;
    void clearGrid();
    void drawGrid();

    std::vector<TetrominoType> tetrominoBag;
    void refillTetrominoBag();
    
    //Gameplay
    void drawTetromino();
    void drawNextPiece();
    void spawnNewTetromino();
    bool isValidMove(int newX, int newY);
    void drawHoldPiece();
    void swapHoldPiece();

    //Controls
    void moveTetromino(int dx, int dy);
    void rotateTetromino();

    void lockTetromino();
    void clearLines();

    void updateScore(int lines); 
    void updateLevel(); 

    bool gameOver;
    void resetGame();
};
