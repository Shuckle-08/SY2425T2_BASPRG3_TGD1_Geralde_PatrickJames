#include "Enemy.h"

Enemy::Enemy()
{
	x = rand() % 400 + 200;
	y = 0;
}

Enemy::~Enemy()
{
}

void Enemy::start()
{
	// Load Texture
	texture = loadTexture("gfx/enemy.png");

	directionX = -1;
	directionY = 1;

	width = 0;
	height = 0;
	speed = 2;

	reloadTime = 60;
	currentReloadTime = reloadTime;

	directionChangeTime = rand() % 300 + 180;
	currentDirectionChangeTime = directionChangeTime;

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	// Initialize sound
	sound = SoundManager::loadSound("sound/334227__jradcoolness__laser.ogg");
	//0-128
	sound->volume = 64;
}

void Enemy::update()
{
	x += directionX * speed;
	y += directionY * speed;

	if (x + width > SCREEN_WIDTH || x < 0)
	{
		directionX = -directionX;
	}

#pragma region Direction Change Logic
	if (currentDirectionChangeTime > 0)
	{
		currentDirectionChangeTime--;
	}

	if (currentDirectionChangeTime <= 0)
	{
		directionX = -directionX;
		currentDirectionChangeTime = directionChangeTime;
	}
#pragma endregion

	if (currentReloadTime > 0)
	{
		currentReloadTime--;
	}

	if (currentReloadTime <= 0)
	{
		float dx;
		float dy;

		if (!playerTarget->IsAlive()) return;

		calcSlope(playerTarget->GetPositionX(), playerTarget->GetPositionY(),
			x, y,
			&dx, &dy);

		Bullet* bullet = new Bullet(x + (width / 2) - 10,
			y + height - 10,
			dx, dy, 5, Side::ENEMY_SIDE);

		getScene()->addGameObject(bullet);

		bullets.push_back(bullet);

		SoundManager::playSound(sound);

		currentReloadTime = reloadTime;
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionX() < 0)
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
}

void Enemy::draw()
{
	blitRotate(texture, x, y, -90);
}

void Enemy::SetPlayerTarget(Player* player)
{
	playerTarget = player;
}

int Enemy::GetPositionX()
{
	return x;
}

int Enemy::GetPositionY()
{
	return y;
}

int Enemy::GetWidth()
{
	return width;
}

int Enemy::GetHeight()
{
	return height;
}
