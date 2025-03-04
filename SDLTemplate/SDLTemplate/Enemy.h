#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h"
#include "Scene.h"
#include <vector>
#include <cmath>
#include "util.h"
#include "Explosion.h"
#include "Player.h"

class Enemy : public GameObject
{
public:
	Enemy(bool isBoss);
	~Enemy();
	void start();
	void update();
	void draw();
	void SetPlayerTarget(Player* player);
	void takeDamage();

	int GetPositionX();
	int GetPositionY();
	int GetWidth();
	int GetHeight();
	int getHealth();
	bool getIsBoss();
private:
	int x;
	int y;
	int directionX;
	int directionY;
	int width;
	int height;
	int health;
	
	bool isBoss;
	int shootingPattern;
	int bossShotCounter;

	int speed;

	float reloadTime;
	float currentReloadTime;
	float directionChangeTime;
	float currentDirectionChangeTime;

	SDL_Texture* texture;
	Mix_Chunk* sound;

	std::vector<Bullet*> bullets;

	Player* playerTarget;
};

