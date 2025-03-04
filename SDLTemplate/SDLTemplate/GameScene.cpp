#include "GameScene.h"

GameScene::GameScene()
{
	// Register and add game objects on constructor
	player = new Player();
	this->addGameObject(player);
}

GameScene::~GameScene()
{
	delete player;
}

void GameScene::start()
{
	Scene::start();
	
	//Initialize
	bgX = 0;
	bgY = 0;
	bgWidth = 0;
	bgHeight = 0;
	spawnTime = 100;
	currentSpawnTime = spawnTime;
	powerUpSpawnTime = 500;
	currentpowerUpSpawnTime = powerUpSpawnTime;

	for (int i = 0; i < 3; i++)
	{
		SpawnEnemy(false);
	}

	initFonts();
	points = 0;

	//Background
	bgTexture = loadTexture("gfx/background.png");
	SDL_QueryTexture(bgTexture, NULL, NULL, &bgWidth, &bgHeight);
}

void GameScene::draw()
{
	blitScale(bgTexture, bgX, bgY, &bgWidth, &bgHeight, 3);

	Scene::draw();

	drawText(110, 20, 255, 255, 255, TEXT_CENTER, "POINTS: %03d", points);
	drawText(82, 50, 255, 255, 255, TEXT_CENTER, "LEVEL: %01d", player->GetLevel());


	if (!player->IsAlive())
	{
		drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 255, 255, 255, TEXT_CENTER, "GAME OVER!");
	}
}

void GameScene::update()
{
	Scene::update();

	DoSpawningLogic();
	DoCollisionLogic();

	if (currentpowerUpSpawnTime > 0) {
		currentpowerUpSpawnTime--;
	}

	if (currentpowerUpSpawnTime <= 0) {
		currentpowerUpSpawnTime = powerUpSpawnTime;
		powerUp = new PowerUp();
		this->addGameObject(powerUp);
	}
}

void GameScene::SpawnEnemy(bool isBoss)
{
	enemy = new Enemy(isBoss);
	this->addGameObject(enemy);
	enemy->SetPlayerTarget(player);
	spawnedEnemies.push_back(enemy);
}


void GameScene::DespawnEnemy(Enemy* enemy)
{
	spawnExplosion(enemy->GetPositionX(), enemy->GetPositionY());

	int index = -1;
	for (int i = 0; i < spawnedEnemies.size(); i++)
	{
		if (enemy == spawnedEnemies[i])
		{
			index = i;
			break;
		}
	}

	if (index != -1)
	{
		Enemy* enemy = spawnedEnemies[index];
		spawnedEnemies.erase(spawnedEnemies.begin() + index);
		delete enemy;
	}
}

void GameScene::DoCollisionLogic()
{
#pragma region BulletCollision
	// Check for collision
	for (int i = 0; i < objects.size(); i++)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(objects[i]);

		if (bullet != NULL)
		{
			// If the bullet is from the enemy side, check againts player
			if (bullet->getSide() == Side::ENEMY_SIDE)
			{
				int collision = checkCollision(
					player->GetPositionX(), player->GetPositionY(), player->GetWidth(), player->GetHeight(),
					bullet->getPositionX(), bullet->getPositionY(), bullet->getWidth(), bullet->getHeight());

				if (collision == 1)
				{
					//std::cout << "Player Has Been Hit!" << std::endl;
					spawnExplosion(player->GetPositionX(), player->GetPositionY());
					player->DoDeath();
					break;
				}
			}

			// If the bullet is from the player side, check againts ememy
			else if (bullet->getSide() == Side::PLAYER_SIDE)
			{
				for (int i = 0; i < spawnedEnemies.size(); i++)
				{
					Enemy* currentEnemy = spawnedEnemies[i];

					int collision = checkCollision(
						currentEnemy->GetPositionX(), currentEnemy->GetPositionY(), currentEnemy->GetWidth(), currentEnemy->GetHeight(),
						bullet->getPositionX(), bullet->getPositionY(), bullet->getWidth(), bullet->getHeight());

					if (collision == 1)
					{
						currentEnemy->takeDamage();
					}

					if (currentEnemy->getHealth() == 0) {
						DespawnEnemy(currentEnemy);
						points++;
						break;
					}
				}
			}
		}
	}
#pragma endregion
#pragma region PowerUp Pickup
	for (int i = 0; i < objects.size(); i++) {
		PowerUp* powerUp = dynamic_cast<PowerUp*>(objects[i]);

		if (powerUp != NULL) {
			int collision = checkCollision(player->GetPositionX(), player->GetPositionY(), player->GetWidth(), player->GetHeight(), powerUp->getPositionX(), powerUp->getPositionY(), powerUp->getWidth(), powerUp->getHeight());

			if (collision == 1) {
				player->LevelUp();
				delete powerUp;
			}
		}
	}
#pragma endregion
}

void GameScene::DoSpawningLogic()
{
	if (currentSpawnTime > 0)
	{
		currentSpawnTime--;
	}

	bool bossExists = false;
	for (Enemy* enemy : spawnedEnemies)
	{
		if (enemy->getIsBoss()) {
			bossExists = true;
			break;
		}
	}

	if (currentSpawnTime <= 0 && !bossExists)
	{
		if (points % 20 == 0 && points != 0) 
		{
			SpawnEnemy(true);
		}
		else
		{
			SpawnEnemy(false);
		}

		currentSpawnTime = spawnTime;
	}

	for (int i = 0; i < spawnedEnemies.size(); i++)
	{
		if (spawnedEnemies[i]->GetPositionY() > SCREEN_HEIGHT)
		{
			Enemy* enemyToErase = spawnedEnemies[i];
			spawnedEnemies.erase(spawnedEnemies.begin() + i);
			delete enemyToErase;

			break;
		}
	}
}



void GameScene::spawnExplosion(int positionX, int positionY)
{
	if (!player->IsAlive()) return;

	explosion = new Explosion(positionX, positionY);
	this->addGameObject(explosion);

	spawnedExplosions.push_back(explosion);
}

