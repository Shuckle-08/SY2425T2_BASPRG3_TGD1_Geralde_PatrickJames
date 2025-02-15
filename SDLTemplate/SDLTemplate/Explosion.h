#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"

class Explosion :
    public GameObject
{
public:
    Explosion(int positionX, int positionY);
    ~Explosion();
    void start();
    void update();
    void draw();

    int getX();
    int getY();
    int getWidth();
    int getHeight();
    int getTimeActive();

private:
    int x;
    int y;
    int width;
    int height;

    int timeActive;

    SDL_Texture* texture;
    Mix_Chunk* sound;
};

