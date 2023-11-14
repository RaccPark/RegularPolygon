#include "ParticleBullet.h"
#include "Enemy.h"


void ParticleBullet::onCollision(GameObject* collided)
{
	Enemy* enemy = dynamic_cast<Enemy*>(collided);
	if (enemy == nullptr) return;
	//enemy->onHit(getDamagePower());
	//setAlive(false);
}