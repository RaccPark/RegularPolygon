#pragma once

class GameObject;

class ICollidable {

public:
	virtual ~ICollidable() {}
	virtual void onCollision(GameObject* collided) = 0; // pure virtual function

};
