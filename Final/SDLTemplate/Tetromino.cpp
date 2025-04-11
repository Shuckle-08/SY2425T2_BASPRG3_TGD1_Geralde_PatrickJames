#include "Tetromino.h"

Tetromino::Tetromino(TetrominoType type) : type(type), posX(3), posY(0) {
    setShape();
}

void Tetromino::setShape() {
    const int shapes[7][4][4] = {
        // I piece
        {{0, 0, 0, 0},
         {1, 1, 1, 1},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},

         // O piece
         {{1, 1, 0, 0},
          {1, 1, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 0, 0}},

          // T piece
          {{0, 1, 0, 0},
           {1, 1, 1, 0},
           {0, 0, 0, 0},
           {0, 0, 0, 0}},

           // L piece
           {{0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},

            // J piece
            {{1, 0, 0, 0},
             {1, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}},

             // S piece
             {{0, 1, 1, 0},
              {1, 1, 0, 0},
              {0, 0, 0, 0},
              {0, 0, 0, 0}},

              // Z piece
              {{1, 1, 0, 0},
               {0, 1, 1, 0},
               {0, 0, 0, 0},
               {0, 0, 0, 0}}
    };

    // Copy selected shape into Tetromino instance
    for (int y = 0; y < TETROMINO_SIZE; y++) {
        for (int x = 0; x < TETROMINO_SIZE; x++) {
            shape[y][x] = shapes[type][y][x];
        }
    }
}

void Tetromino::rotate() {
    int rotated[TETROMINO_SIZE][TETROMINO_SIZE];

    for (int y = 0; y < TETROMINO_SIZE; y++) {
        for (int x = 0; x < TETROMINO_SIZE; x++) {
            rotated[x][TETROMINO_SIZE - 1 - y] = shape[y][x];
        }
    }

    // Apply rotation
    for (int y = 0; y < TETROMINO_SIZE; y++) {
        for (int x = 0; x < TETROMINO_SIZE; x++) {
            shape[y][x] = rotated[y][x];
        }
    }
}

int Tetromino::getCell(int x, int y) {
    return shape[y][x];
}

void Tetromino::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

int Tetromino::getX() {
    return posX;
}

int Tetromino::getY() {
    return posY;
}
