#pragma once
#include "GameObject.h"
#include "ICollidable.h"

// enemy

class Enemy : public GameObject, public ICollidable {

	float hp;
	int n_frames;

public:

	Enemy(const char* shape, const Vector2& pos, const Dimension& dim, bool visible)
		: GameObject(shape, pos, dim, visible), hp(5.0f), n_frames(0)
	{}

	void setTimeout(int n_frames)
	{
		this->n_frames = n_frames;
	}

	void onHit(float damage)
	{
		hp = hp - damage;
		if (hp <= 0.0f) {
			setAlive(false);
		}
		else {
			setTimeout(100);
		}
	}

	void draw() override;

	void onCollision(GameObject* collided) override {};

	void update(InputManager& input) {
		if (isAlive() == false || isVisible() == false) return;
	}

	~Enemy() {}

};