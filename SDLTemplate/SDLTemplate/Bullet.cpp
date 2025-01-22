#include "Bullet.h"
#include "common.h"
#include "draw.h"

Bullet::Bullet(int positionX, int positionY, float directionX, float directionY, int speed)
{
	this->x = positionX;
	this->y = positionY;
	this->directionX = directionX;
	this->directionY = directionY;
	this->speed = speed;
}

void Bullet::start()
{
	//Load Texture
	texture = loadTexture("gfx/playerBullet.png");

	//Initialize Values
	width = 0;
	height = 0;

	//Query
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Bullet::update()
{
	x += directionX * speed;
	y += directionY * speed;

}

void Bullet::draw()
{
	blit(texture, x, y);
}

int Bullet::getPositionX()
{
	return x;
}

int Bullet::getPositionY()
{
	return y;
}

int Bullet::getWidth()
{
	return width;
}

int Bullet::getHeight()
{
	return height;
}