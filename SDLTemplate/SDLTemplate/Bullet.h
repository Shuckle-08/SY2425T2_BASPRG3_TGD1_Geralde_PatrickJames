#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"

class Bullet :
    public GameObject
{
public:
    Bullet(int positionX, int positionY, float directionX, float directionY, int speed);
    void start();
    void update();
    void draw();

    int getPositionX();
    int getPositionY();
    int getWidth();
    int getHeight();

private:
    int x;
    int y;
    int width;
    int height;

    int speed;

    SDL_Texture* texture;

    float directionX;
    float directionY;
};

