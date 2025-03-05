#include "Enemy.h"

Enemy::Enemy(bool isBoss) : isBoss(isBoss)
{
	if (isBoss) {
		x = 20;
		y = 100;

		directionX = 2;
		directionY = 0;

		health = 200;
		shootingPattern = 0;

		reloadTime = 80;
		currentReloadTime = reloadTime;
		bossShotCounter = 0;

		// Load Texture
		texture = loadTexture("gfx/benilde.png");
	}
	else {
		x = rand() % 400 + 200;
		y = 0;

		directionX = -1;
		directionY = 1;

		health = 1;

		reloadTime = 60;
		currentReloadTime = reloadTime;

		// Load Texture
		texture = loadTexture("gfx/enemy.png");
	}
}

Enemy::~Enemy()
{
}

void Enemy::start()
{
	

	width = 0;
	height = 0;
	speed = 2;

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

#pragma region Shooting Logic
	if (!isBoss) {
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
	}
	else if (isBoss) { //Boss Shooting Pattern
		if (currentReloadTime > 0) {
			currentReloadTime--;
		}

		if (currentReloadTime <= 0) {
			switch (shootingPattern) {
			case 0: // Bullet Line
				for (int i = 0; i < 5; i++) {
					float dx, dy;
					int spacing = 30;

					if (!playerTarget->IsAlive()) return;
					calcSlope(playerTarget->GetPositionX(), playerTarget->GetPositionY(), x, y, &dx, &dy);

					Bullet* bullet = new Bullet(x + (width / 2) - 50 + (i * spacing), y + height - 10, dx, dy, 7, Side::ENEMY_SIDE);
					getScene()->addGameObject(bullet);
					bullets.push_back(bullet);

					Bullet* bullet1 = new Bullet(x + (width / 2) - 50 - (i * spacing), y + height - 10, dx, dy, 7, Side::ENEMY_SIDE);
					getScene()->addGameObject(bullet1);
					bullets.push_back(bullet1);

				}
				break;

			case 1: // Double Line Bullet Pattern with Wider Gaps
				if (currentReloadTime <= 0) {
					for (int i = 0; i < 5; i++) {
						float dx, dy;
						if (!playerTarget->IsAlive()) return;

						calcSlope(playerTarget->GetPositionX(), playerTarget->GetPositionY(), x, y, &dx, &dy);

						int spacing = 125; // Increase this value to widen the gaps

						// First row of bullets (Left side)
						Bullet* bullet1 = new Bullet(x + (width / 2) - 100 + (i * spacing), y + height - 10, dx, dy, 5, Side::ENEMY_SIDE);
						getScene()->addGameObject(bullet1);
						bullets.push_back(bullet1);

						// Second row of bullets (Right side)
						Bullet* bullet2 = new Bullet(x + (width / 2) - 85 + (i * spacing), y + height - 150, dx, dy, 5, Side::ENEMY_SIDE);
						getScene()->addGameObject(bullet2);
						bullets.push_back(bullet2);
					}
				}
				break;

			case 2: // Constant Spread Shot
				for (int i = -4; i <= 4; i++) {
					float angleOffset = i * (M_PI / 18);
					float dx = sin(angleOffset);
					float dy = cos(angleOffset);

					Bullet* bullet = new Bullet(x + (width / 2), y + height - 10, dx, dy, 5, Side::ENEMY_SIDE);
					getScene()->addGameObject(bullet);
					bullets.push_back(bullet);
				}
				break;
			}

			// Play sound after firing
			SoundManager::playSound(sound);

			bossShotCounter++;
			currentReloadTime = reloadTime;

			// Change attack pattern after 2 shots
			if (bossShotCounter >= 2) {
				shootingPattern = rand() % 3; // Randomize between 0, 1, 2
				bossShotCounter = 0; // Reset counter
			}
		}
	}
#pragma endregion

#pragma region Memory Manage Bullets
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
#pragma endregion	
}

void Enemy::draw()
{
	blitRotate(texture, x, y, -90);
}

void Enemy::SetPlayerTarget(Player* player)
{
	playerTarget = player;
}

void Enemy::takeDamage()
{
	health--;
}

#pragma region Getters
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

int Enemy::getHealth()
{
	return health;
}

bool Enemy::getIsBoss()
{
	return isBoss;
}
#pragma endregion