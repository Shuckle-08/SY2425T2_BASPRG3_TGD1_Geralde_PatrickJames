#include "TetrisScene.h"

TetrisScene::TetrisScene() {
    clearGrid();
    refillTetrominoBag();
    nextTetromino = new Tetromino(tetrominoBag.back());
    tetrominoBag.pop_back();

    holdTetromino = nullptr;
    holdUsed = false;

    spawnNewTetromino();
    dropTimer = 0;
    score = 0;
    level = 1;
    linesCleared = 0;
    dropDelay = 50;

    initFonts();

    // Center the grid on the screen
    gridOffsetX = (SCREEN_WIDTH - (GRID_WIDTH * 32)) / 2;
    gridOffsetY = (SCREEN_HEIGHT - (GRID_HEIGHT * 32)) / 2;

    gameOver = false;
}


TetrisScene::~TetrisScene() {
    delete currentTetromino;
}

void TetrisScene::start() {
    Scene::start();
}

void TetrisScene::update() {
    if (gameOver) return;

    Scene::update();
    dropTimer++;

    if (dropTimer >= dropDelay) {
        int newX = currentTetromino->getX();
        int newY = currentTetromino->getY() + 1;

        if (isValidMove(newX, newY)) {
            currentTetromino->setPosition(newX, newY);
        }
        else {
            lockTetromino();
            spawnNewTetromino();
            clearLines();
        }

        dropTimer = 0;
    }
}

void TetrisScene::draw() {
    Scene::draw();
    drawGrid();
    drawTetromino();
    drawNextPiece();
    drawHoldPiece();

    drawText(30, 10, 255, 255, 255, TEXT_LEFT, "SCORE: %d", score);
    drawText(30, 50, 255, 255, 255, TEXT_LEFT, "LEVEL: %d", level);

    if (gameOver) {
        drawText((SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2, 255, 0, 0, TEXT_CENTER, "GAME OVER!");
        drawText((SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2 + 30, 255, 0, 0, TEXT_CENTER, "R TO RESTART!");
    }
}

void TetrisScene::clearGrid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = 0;
        }
    }
}

void TetrisScene::drawGrid() {
    SDL_SetRenderDrawColor(app.renderer, 50, 50, 50, 255);

    // Draw the grid lines
    for (int y = 0; y <= GRID_HEIGHT; y++) {
        SDL_RenderDrawLine(app.renderer, gridOffsetX, gridOffsetY + y * 32, gridOffsetX + GRID_WIDTH * 32, gridOffsetY + y * 32);
    }
    for (int x = 0; x <= GRID_WIDTH; x++) {
        SDL_RenderDrawLine(app.renderer, gridOffsetX + x * 32, gridOffsetY, gridOffsetX + x * 32, gridOffsetY + GRID_HEIGHT * 32);
    }

    // Draw locked blocks in the grid
    SDL_SetRenderDrawColor(app.renderer, 200, 200, 200, 255);
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] != 0) {
                SDL_Rect rect = { gridOffsetX + x * 32, gridOffsetY + y * 32, 32, 32 };
                SDL_RenderFillRect(app.renderer, &rect);
            }
        }
    }
}

void TetrisScene::refillTetrominoBag() {
    // Fill the bag with all 7 Tetromino types
    tetrominoBag.clear();
    for (int i = 0; i < 7; i++) {
        tetrominoBag.push_back(static_cast<TetrominoType>(i));
    }

    // Shuffle the bag to randomize order
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(tetrominoBag.begin(), tetrominoBag.end(), g);
}

void TetrisScene::drawTetromino() {
    SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);

    int x = currentTetromino->getX();
    int y = currentTetromino->getY();

    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (currentTetromino->getCell(j, i)) {
                SDL_Rect rect = { gridOffsetX + (x + j) * 32, gridOffsetY + (y + i) * 32, 32, 32 };
                SDL_RenderFillRect(app.renderer, &rect);
            }
        }
    }
}

void TetrisScene::drawNextPiece() {
    if (!nextTetromino) return;

    int previewX = 50;
    int previewY = 100;
    int boxSize = 80;
    int blockSize = 16;

    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    SDL_Rect border = { previewX - 10, previewY - 10, boxSize, boxSize };
    SDL_RenderDrawRect(app.renderer, &border);

    int minX = TETROMINO_SIZE, minY = TETROMINO_SIZE, maxX = 0, maxY = 0;

    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (nextTetromino->getCell(j, i)) {
                if (j < minX) minX = j;
                if (j > maxX) maxX = j;
                if (i < minY) minY = i;
                if (i > maxY) maxY = i;
            }
        }
    }

    int tetrominoWidth = (maxX - minX + 1) * 20;
    int tetrominoHeight = (maxY - minY + 1) * 20;

    int centerX = previewX + (boxSize / 2) - (tetrominoWidth / 2);
    int centerY = previewY + (boxSize / 2) - (tetrominoHeight / 2);

    SDL_SetRenderDrawColor(app.renderer, 255, 165, 180, 255);
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (nextTetromino->getCell(j, i)) {
                SDL_Rect rect = { centerX + (j - minX) * blockSize, centerY + (i - minY) * blockSize, blockSize, blockSize };
                SDL_RenderFillRect(app.renderer, &rect);
            }
        }
    }
}

void TetrisScene::spawnNewTetromino() {
    if (currentTetromino) delete currentTetromino;

    currentTetromino = nextTetromino;
    currentTetromino->setPosition(GRID_WIDTH / 2 - 2, 0);

    // Check for game over
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (currentTetromino->getCell(j, i)) {
                int gridX = currentTetromino->getX() + j;
                int gridY = currentTetromino->getY() + i;
                if (gridY >= 0 && grid[gridY][gridX] != 0) {
                    gameOver = true;
                    return;
                }
            }
        }
    }

    if (tetrominoBag.empty()) {
        refillTetrominoBag();
    }

    nextTetromino = new Tetromino(tetrominoBag.back());
    tetrominoBag.pop_back(); 

    holdUsed = false;
}

bool TetrisScene::isValidMove(int newX, int newY) {
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (currentTetromino->getCell(j, i)) {
                int gridX = newX + j;
                int gridY = newY + i;

                // Out of bounds?
                if (gridX < 0 || gridX >= GRID_WIDTH || gridY >= GRID_HEIGHT) {
                    return false;
                }

                // Colliding with filled grid cell?
                if (gridY >= 0 && grid[gridY][gridX] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

void TetrisScene::drawHoldPiece()
{
    if (!holdTetromino) return;

    int holdX = 50; 
    int holdY = 200;
    int boxSize = 80;
    int blockSize = 16; 


    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    SDL_Rect border = { holdX - 10, holdY - 10, boxSize, boxSize };
    SDL_RenderDrawRect(app.renderer, &border);

    // Center the piece in the hold box
    int minX = TETROMINO_SIZE, minY = TETROMINO_SIZE, maxX = 0, maxY = 0;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (holdTetromino->getCell(j, i)) {
                if (j < minX) minX = j;
                if (j > maxX) maxX = j;
                if (i < minY) minY = i;
                if (i > maxY) maxY = i;
            }
        }
    }

    int tetrominoWidth = (maxX - minX + 1) * 20;
    int tetrominoHeight = (maxY - minY + 1) * 20;

    int centerX = holdX + (boxSize / 2) - (tetrominoWidth / 2);
    int centerY = holdY + (boxSize / 2) - (tetrominoHeight / 2);

    SDL_SetRenderDrawColor(app.renderer, 255, 165, 0, 255); // Orange for held piece
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (holdTetromino->getCell(j, i)) {
                SDL_Rect rect = { centerX + (j - minX) * blockSize, centerY + (i - minY) * blockSize, blockSize, blockSize };
                SDL_RenderFillRect(app.renderer, &rect);
            }
        }
    }
}

void TetrisScene::swapHoldPiece() {
    if (holdUsed) return;

    if (!holdTetromino) {
        holdTetromino = new Tetromino(*currentTetromino);
        spawnNewTetromino();
    }
    else {
        std::swap(currentTetromino, holdTetromino);
        currentTetromino->setPosition(GRID_WIDTH / 2 - 2, 0);
    }

    holdUsed = true;
}

void TetrisScene::moveTetromino(int dx, int dy) {
    int newX = currentTetromino->getX() + dx;
    int newY = currentTetromino->getY() + dy;

    if (isValidMove(newX, newY)) {
        currentTetromino->setPosition(newX, newY);
    }
}

void TetrisScene::rotateTetromino() {
    Tetromino rotated = *currentTetromino; // Copy current Tetromino
    rotated.rotate(); // Rotate the copy

    int newX = rotated.getX();
    int newY = rotated.getY();

    // Check for out-of-bounds positions
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (rotated.getCell(j, i)) {
                int gridX = newX + j;
                int gridY = newY + i;

                // Shift piece if it goes out of bounds
                if (gridX < 0) {
                    newX += 1; // Shift right
                }
                else if (gridX >= GRID_WIDTH) {
                    newX -= 1; // Shift left
                }
                else if (gridY >= GRID_HEIGHT) {
                    newY -= 1; // Shift up if hitting the bottom
                }
            }
        }
    }

    // Apply rotation only if it's a valid move
    if (isValidMove(newX, newY)) {
        *currentTetromino = rotated;
        currentTetromino->setPosition(newX, newY);
    }
}

void TetrisScene::handleInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            exit(0);
            break;

        case SDL_KEYDOWN:
            if (gameOver && event.key.keysym.sym == SDLK_r) {
                resetGame(); // Restart when "R" is pressed
            }

            if (!gameOver) {
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    moveTetromino(-1, 0);
                    break;
                case SDLK_RIGHT:
                    moveTetromino(1, 0);
                    break;
                case SDLK_DOWN:
                    moveTetromino(0, 1);
                    break;
                case SDLK_UP:
                    rotateTetromino();
                    break;
                case SDLK_LSHIFT:
                    swapHoldPiece();
                    break;
                }
            }
            break;

        default:
            break;
        }
    }
}

void TetrisScene::lockTetromino() {
    int x = currentTetromino->getX();
    int y = currentTetromino->getY();

    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (currentTetromino->getCell(j, i)) {
                int gridX = x + j;
                int gridY = y + i;

                if (gridY >= 0 && gridY < GRID_HEIGHT && gridX >= 0 && gridX < GRID_WIDTH) {
                    grid[gridY][gridX] = 1; // Mark the grid cell as filled
                }
            }
        }
    }
}


void TetrisScene::clearLines() {
    int lines = 0;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        bool full = true;

        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            // Clear the row and shift everything down
            for (int row = y; row > 0; row--) {
                for (int col = 0; col < GRID_WIDTH; col++) {
                    grid[row][col] = grid[row - 1][col];
                }
            }

            // Clear the top row
            for (int col = 0; col < GRID_WIDTH; col++) {
                grid[0][col] = 0;
            }

            lines++;
            linesCleared++;
        }
    }

    if (lines > 0) {
        updateScore(lines);
        updateLevel();
    }
}

void TetrisScene::updateScore(int lines) {
    int points[] = { 0, 100, 300, 500, 800 }; // Points per lines cleared (1, 2, 3, 4)
    score += points[lines];
}

void TetrisScene::updateLevel() {
    if (linesCleared >= level * 5) { // Level up every 5 lines
        level++;
        dropDelay = std::max(10, dropDelay - 5); // Speed up but not too much
    }
}

void TetrisScene::resetGame() {
    clearGrid();
    spawnNewTetromino();
    score = 0;
    level = 1;
    linesCleared = 0;
    dropDelay = 30;
    gameOver = false;
}