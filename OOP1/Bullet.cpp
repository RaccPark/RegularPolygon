#include "Bullet.h"
#include "Canvas.h"
#include "Enemy.h"
#include "ParticleBullet.h"

int Bullet::nCreatedBullets = 0;

void Bullet::move()
{
	auto pos = getPosition();
	auto dir = getDirection();
	auto canDim = canvas.getDimension();
	setPosition(pos + dir);
	if (!isOverlapping({ 0, 0 }, { canDim.x, canDim.y })) {
		setAlive(false);
	}
}

void Bullet::onCollision(GameObject* collided)
{
	Enemy* enemy = dynamic_cast<Enemy*>(collided);
	if (enemy == nullptr) return;
	enemy->onHit(getDamagePower());

	auto pos = getPosition();
	for (int i = 0; i < 10; i++)
	{
		auto randPos = (Vector2{ (float)(rand() % 20), (float)(rand() % 20) } - Vector2{10.0f, 10.0f}).unit();
		GameObject::Add(new ParticleBullet(pos, randPos));
	}
	setAlive(false);
}