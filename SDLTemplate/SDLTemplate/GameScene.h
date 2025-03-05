#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Explosion.h"
#include "PowerUp.h"
#include <vector>
#include "text.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	void start();
	void draw();
	void update();
private:
	void SpawnEnemy(bool isBoss);
	void DespawnEnemy(Enemy* enemy);

	void DoCollisionLogic();
	void DoSpawningLogic();

	void spawnExplosion(int positionX, int positionY);

	int points;

	Player* player;
	Enemy* enemy;
	Explosion* explosion;
	PowerUp* powerUp;

	float spawnTime;
	float currentSpawnTime;

	float powerUpSpawnTime;
	float currentpowerUpSpawnTime;

	//Background
	int bgX;
	int bgY;
	int bgWidth;
	int bgHeight;
	SDL_Texture* bgTexture;

	std::vector<Enemy*> spawnedEnemies;
};