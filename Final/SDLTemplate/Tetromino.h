#pragma once
#include "defs.h"

const int TETROMINO_SIZE = 4;

enum TetrominoType { 
    I, O, T, L, J, S, Z };

class Tetromino {
public:
    Tetromino(TetrominoType type);

    void rotate();
    int getCell(int x, int y); // Get value of a block in the shape
    void setPosition(int x, int y);
    int getX();
    int getY();

private:
    int shape[TETROMINO_SIZE][TETROMINO_SIZE]; // 4x4 shape matrix
    int posX, posY; // Position on grid
    TetrominoType type;

    void setShape();
};
