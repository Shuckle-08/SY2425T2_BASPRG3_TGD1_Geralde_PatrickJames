#include "Explosion.h"

Explosion::Explosion(int positionX, int positionY)
{
	this->x = positionX;
	this->y = positionY;
}

Explosion::~Explosion()
{
}

void Explosion::start()
{
	timeActive = 0;

	//Initialize Texture
	texture = loadTexture("gfx/explosion.png");
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	//Initialize Sound
	//sound = SoundManager::loadSound("sound/245372__quaker540__hq-explosion.ogg");
	sound = SoundManager::loadSound("sound/vine-boom.mp3");
	
	//0-128
	sound->volume = 20;

	SoundManager::playSound(sound);
}

void Explosion::update()
{
	timeActive++;

	if (timeActive > 5) {
		delete this;
	}
}

void Explosion::draw()
{
	blit(texture, x, y);
}

int Explosion::getX()
{
	return x;
}

int Explosion::getY()
{
	return y;
}

int Explosion::getWidth()
{
	return width;
}

int Explosion::getHeight()
{
	return height;
}

int Explosion::getTimeActive()
{
	return timeActive;
}
