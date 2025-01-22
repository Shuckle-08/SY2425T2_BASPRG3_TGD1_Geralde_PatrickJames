#include "Player.h"

Player::~Player()
{
	for (int i = 0; i < bullets.size() + i; i++) {
		delete bullets[i];
	}

	bullets.clear();
}

void Player::start()
{
	//Load Texture
	texture = loadTexture("gfx/player.png");

	//Initalize Variable
	x = 100;
	y = 100;
	width = 0;
	height = 0;

	defaultSpeed = 3;
	speed = defaultSpeed;
	shiftSpeed = 5;

	reloadTime = 8;
	currentReloadTime = reloadTime;

	wingGunsReloadTime = 15;
	currentWingGunsReloadTime = wingGunsReloadTime;


	//Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	//Initialize Sound
	//sound = SoundManager::loadSound("sound/334227__jradcoolness__laser.ogg");
	sound = SoundManager::loadSound("sound/vine-boom.mp3");

}

void Player::update()
{
	//Weapons
	if (currentReloadTime > 0) {
		currentReloadTime--;
	}
	if (currentWingGunsReloadTime > 0) {
		currentWingGunsReloadTime--;
	}

	if (app.keyboard[SDL_SCANCODE_SPACE] && currentReloadTime <= 0) {
		Bullet* bullet = new Bullet(x + width - 2, y + (height/2) - 5, 1,0,5);
		getScene()->addGameObject(bullet);
		bullet->start();

		bullets.push_back(bullet);

		SoundManager::playSound(sound);

		currentReloadTime = reloadTime;
	}

	if (app.keyboard[SDL_SCANCODE_G] && currentWingGunsReloadTime <= 0) {
		Bullet* wingBulletTop = new Bullet(x + (width / 4), y + height - 5, 1, 0, 5);
		Bullet* wingBulletBot = new Bullet(x + (width / 4), y - 5, 1, 0, 5);
		getScene()->addGameObject(wingBulletTop);
		getScene()->addGameObject(wingBulletBot);
		wingBulletTop->start();
		wingBulletBot->start();

		bullets.push_back(wingBulletTop);
		bullets.push_back(wingBulletBot);


		SoundManager::playSound(sound);

		currentWingGunsReloadTime = wingGunsReloadTime;
	}



	//Memory Management
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->getPositionX() > SCREEN_WIDTH) {
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);

			delete bulletToErase;

			break;
		}
	}

	//Movement
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
