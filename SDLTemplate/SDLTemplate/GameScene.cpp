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
	
	spawnTime = 300;
	currentSpawnTime = spawnTime;

	for (int i = 0; i < 3; i++)
	{
		Spawn();
	}
}

void GameScene::draw()
{
	Scene::draw();
}

void GameScene::update()
{
	Scene::update();

	if (currentSpawnTime > 0)
	{
		currentSpawnTime--;
	}

	if (currentSpawnTime <= 0)
	{
		Spawn();

		currentSpawnTime = spawnTime;
	}

	for (int i = 0; i < spawnedEnemies.size(); i++)
	{
		if (spawnedEnemies[i]->GetPositionX() < -100)
		{
			// Cache the variable so we can delete it later
			// We can't delete it after erasing from the vector (leaked pointer)
			Enemy* enemyToErase = spawnedEnemies[i];
 			spawnedEnemies.erase(spawnedEnemies.begin() + i);

			delete enemyToErase;

			// We can't mutate (change) our vector while looping inside it
			// this might crash on the next loop iteration
			// To counter that, we only delete one bullet per frame
			break;
		}
	}
}

void GameScene::Spawn()
{
	enemy = new Enemy();
	this->addGameObject(enemy);
	enemy->SetPlayerTarget(player);

	spawnedEnemies.push_back(enemy);
}
