#pragma once
#include "GameObject.h"
#include "ICollidable.h"
#include "Utils.h"

// bullet
class Bullet : public GameObject, public ICollidable {

	float		damagePower;

	static int nCreatedBullets;

public:

	Bullet(const char* shape, const Vector2& pos, const Dimension& dim, float damagePower, bool visible = true, const Vector2& dir = { 0.0f, 0.0f } )
		: GameObject(shape, pos, dim, visible, dir), damagePower(damagePower)
	{
		nCreatedBullets++;
	}

	void fire(GameObject* player, const Vector2& dest)
	{
		if (player == nullptr) return;

		auto player_pos = player->getPosition();

		setVisible();
		setPosition(player->getPosition());

		setDirection({ dest.x - player_pos.x, dest.y - player_pos.y } );
		setShape("O");
	}

	float getDamagePower() const { return damagePower; }

	void move();

	void onCollision(GameObject* collided) override;


	void update(InputManager& input) override
	{
		if (isAlive() == false || isVisible() == false) return;

		move();
	}

	~Bullet()
	{	
	}

	static void printNumberOfCreatedBullets()
	{
		Debug::Log("# of Created Bullets = %d", nCreatedBullets);
	}
};


