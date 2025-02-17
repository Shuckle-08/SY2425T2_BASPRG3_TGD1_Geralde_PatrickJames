#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"

class PowerUp :
    public GameObject
{
public:
    PowerUp();
    ~PowerUp();
    void start();
    void update();
    void draw();

    int getPositionX();
    int getPositionY();
    int getDirectionY();
    int getDirectionX();
    int getWidth();
    int getHeight();
    int getSpeed();

private:
    int x;
    int y;
    int directionX;
    int directionY;
    int width;
    int height;
    int speed;

    SDL_Texture* texture;
    Mix_Chunk* sound;

};

