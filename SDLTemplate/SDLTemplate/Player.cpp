#include "Player.h"

void Player::start()
{
	//Load Texture
	texture = loadTexture("gfx/player.png");

	//Initalize Variable
	x = 100;
	y = 100;
	width = 0;
	height = 0;

	speed = defaultSpeed;
	defaultSpeed = 3;
	shiftSpeed = 5;

	//Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Player::update()
{
	if (app.keyboard[SDL_SCANCODE_W]) {
		y -= speed;
	}
	if (app.keyboard[SDL_SCANCODE_A]) {
		x -= speed;
	}
	if (app.keyboard[SDL_SCANCODE_S]) {
		y += speed;
	}
	if (app.keyboard[SDL_SCANCODE_D]) {
		x += speed;
	}

	if (app.keyboard[SDL_SCANCODE_BACKSPACE]) {
		speed = defaultSpeed;
	}

	if (app.keyboard[SDL_SCANCODE_LSHIFT]) {
		speed = shiftSpeed;
	}
}

void Player::draw()
{
	blit(texture, x, y);
}
