#pragma once
#include "Bullet.h"

// particle bullet
class ParticleBullet : public Bullet {
	int		n_frames_to_disappear;

public:

	ParticleBullet(const Vector2& pos, const Vector2& dir)
		: Bullet("*", pos, { 1, 1 }, 0.005f, true, dir), n_frames_to_disappear(200)
	{}

	void onCollision(GameObject* collided) override;

	void update(InputManager& input) override
	{
		Bullet::update(input);

		if (isAlive() == false || isVisible() == false) return;

		if (n_frames_to_disappear > 0) {
			--n_frames_to_disappear;
			if (n_frames_to_disappear <= 0) {
				setAlive(false);
			}
		}
	}

	~ParticleBullet()
	{
		n_frames_to_disappear = 0;
	}
};

