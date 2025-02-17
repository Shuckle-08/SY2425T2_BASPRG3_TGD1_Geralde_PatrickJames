#include "PowerUp.h"

PowerUp::PowerUp()
{
	x = rand() % 400 + 200;
	y = 0;
}

PowerUp::~PowerUp()
{
}

void PowerUp::start()
{
	directionX = -1;
	directionY = 1;
	speed = 3;

	//Initialize Texture
	texture = loadTexture("gfx/points.png");
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void PowerUp::update()
{
	y += directionY * speed;
}

void PowerUp::draw()
{
	blit(texture, x, y);
}

#pragma region Getters
int PowerUp::getPositionX()
{
	return x;
}

int PowerUp::getPositionY()
{
	return y;
}

int PowerUp::getDirectionY()
{
	return directionY;
}

int PowerUp::getDirectionX()
{
	return directionX;
}

int PowerUp::getWidth()
{
	return width;
}

int PowerUp::getHeight()
{
	return height;
}

int PowerUp::getSpeed()
{
	return speed;
}
#pragma endregion