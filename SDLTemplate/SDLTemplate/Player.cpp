#include "Player.h"

Player::~Player()
{
	// Memory manage our bullets. Delete all bullets on player deletion/death
	for (int i = 0; i < bullets.size(); i++)
	{
		delete bullets[i];
	}

	bullets.clear();
}

void Player::start()
{
	// Load Texture
	texture = loadTexture("gfx/player.png");

	// Initialize to avoid garbage values
	x = (SCREEN_WIDTH / 2) - 30;
	y = (SCREEN_HEIGHT) - 150;
	width = 0;
	height = 0;
	speed = 5;
	level = 1;

	reloadTime = 8;
	currentReloadTime = reloadTime;

	wingGunsReloadTime = 15;
	currentWingGunsReloadTime = wingGunsReloadTime;

	isAlive = true;

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	// Initialize sound
	sound = SoundManager::loadSound("sound/334227__jradcoolness__laser.ogg");
	//sound = SoundManager::loadSound("sound/vine-boom.mp3");
	//0-128
	sound->volume = 64;
}

void Player::update()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionX() > SCREEN_WIDTH)
		{
			// Cache the variable so we can delete it later
			// We can't delete it after erasing from the vector (leaked pointer)
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);

			delete bulletToErase;

			// We can't mutate (change) our vector while looping inside it
			// this might crash on the next loop iteration
			// To counter that, we only delete one bullet per frame
			break;
		}
	}

	if (!isAlive) return;

	//Timers
	if (currentReloadTime > 0) {
		currentReloadTime--;
	}
	if (currentWingGunsReloadTime > 0) {
		currentWingGunsReloadTime--;
	}

	if (app.keyboard[SDL_SCANCODE_SPACE] &&
		currentReloadTime <= 0)
	{
		ShootMainGun();
	}

	/*if (app.keyboard[SDL_SCANCODE_G] && currentWingGunsReloadTime <= 0) {
		Bullet* wingBulletTop = new Bullet(x + (width / 4), y + height - 5, 1, 0, 5, Side::PLAYER_SIDE);
		Bullet* wingBulletBot = new Bullet(x + (width / 4), y - 5, 1, 0, 5, Side::PLAYER_SIDE);
		getScene()->addGameObject(wingBulletTop);
		getScene()->addGameObject(wingBulletBot);
		wingBulletTop->start();
		wingBulletBot->start();

		bullets.push_back(wingBulletTop);
		bullets.push_back(wingBulletBot);

		SoundManager::playSound(sound);

		currentWingGunsReloadTime = wingGunsReloadTime;
	}*/

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionX() > SCREEN_WIDTH)
		{
			// Cache the variable so we can delete it later
			// We can't delete it after erasing from the vector (leaked pointer)
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);

			delete bulletToErase;

			// We can't mutate (change) our vector while looping inside it
			// this might crash on the next loop iteration
			// To counter that, we only delete one bullet per frame
			break;
		}
	}

	if (app.keyboard[SDL_SCANCODE_W])
	{
		y -= speed;
		if (y < 0) {
			y = 0;
		}
	}

	if (app.keyboard[SDL_SCANCODE_S])
	{
		y += speed;
		if (y > SCREEN_HEIGHT - 47) {
			y = SCREEN_HEIGHT - 47;
		}
	}

	if (app.keyboard[SDL_SCANCODE_A])
	{
		x -= speed;
		if (x < 0) {
			x = 0;
		}
	}

	if (app.keyboard[SDL_SCANCODE_D])
	{
		x += speed;
		if (x > SCREEN_WIDTH - 47) {
			x = SCREEN_WIDTH - 47;
		}
	}
}

void Player::ShootMainGun()
{
	switch (level) {
	case 1: {
		Bullet* bullet = new Bullet(x + (width / 2) - 12, y, 0, -1, 5, Side::PLAYER_SIDE);
		getScene()->addGameObject(bullet);
		bullets.push_back(bullet);
		break;
	}
	case 2: {
		Bullet* bullet1 = new Bullet(x + (width / 2) - 24, y, 0, -1, 5, Side::PLAYER_SIDE);
		Bullet* bullet2 = new Bullet(x + (width / 2) , y, 0, -1, 5, Side::PLAYER_SIDE);

		getScene()->addGameObject(bullet1);
		getScene()->addGameObject(bullet2);

		bullets.push_back(bullet1);
		bullets.push_back(bullet2);
		break;
	}
	case 3: {
		Bullet* bullet1 = new Bullet(x + (width / 2) - 32, y, -0.2, -1, 5, Side::PLAYER_SIDE); // Left spread
		Bullet* bullet2 = new Bullet(x + (width / 2) - 12, y, 0, -1, 5, Side::PLAYER_SIDE);        // Centered
		Bullet* bullet3 = new Bullet(x + (width / 2) + 8, y, 0.2, -1, 5, Side::PLAYER_SIDE); // Right spread

		getScene()->addGameObject(bullet1);
		getScene()->addGameObject(bullet2);
		getScene()->addGameObject(bullet3);

		bullets.push_back(bullet1);
		bullets.push_back(bullet2);
		bullets.push_back(bullet3);
		break;
	}
	default:
		break;
	}

	SoundManager::playSound(sound);

	currentReloadTime = reloadTime;
}

void Player::draw()
{
	if (!isAlive) return;
	blitRotate(texture, x, y, -90);
}

int Player::GetPositionX()
{
	return x;
}

int Player::GetPositionY()
{
	return y;
}

int Player::GetWidth()
{
	return width;
}

int Player::GetHeight()
{
	return height;
}

int Player::GetLevel()
{
	return level;
}

bool Player::IsAlive()
{
	return isAlive;
}

void Player::DoDeath()
{
	isAlive = false;
}

void Player::LevelUp()
{
	level++;
	if (level > 3) {
		level = 3;
	}
}
