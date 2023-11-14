#pragma once

#include <iostream>
#include <vector>
#include "Utils.h"

class Canvas;
class InputManager;
class GameObject;

class GameObject
{
private:

	char* shape;
	Dimension dimension; // x: width, y: height;
	Vector2 position;
	Vector2 direction;
	bool	visible;
	bool	alive;

	static void RemoveDeadObjects();

protected:

	static Canvas canvas;
	static std::vector<GameObject*> objs;

public:

	GameObject(const char* shape, const Vector2& pos = { 0.0f, 0.0f }, const Dimension& dim = Dimension{ 1, 1 }, bool visible = true, const Vector2& dir = { 0.0f, 0.0f })
		: shape(new char[strlen(shape) + 1]), position(pos), dimension(dim), visible(visible), direction(dir), alive(true)
	{
		if (this->shape != nullptr)
			strcpy(this->shape, shape);
	}

	GameObject(const char* shape, int pos, bool visible )
		: GameObject(shape, Vector2((float)pos, 0.0f), Dimension((int)strlen(shape), 1))
	{}

	Vector2 getPosition() const { return position;  }
	void	setPosition(const Vector2& pos) { position = pos;  }

	Vector2 getDirection() const { return direction; }
	void	setDirection(const Vector2& dir) { direction = dir.unit(); }

	Dimension getDimension() const { return dimension;  }
	void	changeDimension(const Dimension& dim) { dimension = dim; }

	const char* getShape() const { return this->shape; } // getter
	void	setShape(const char* shape) { 
		delete[] this->shape;
		this->shape = new char[strlen(shape) + 1];
		strcpy(this->shape, shape); 
	} // setter
	int		getShapeSize() const { return (int)strlen(this->shape); }

	bool	isVisible() const { return visible; } // gettter
	void	setVisible(bool visible = true) { this->visible = visible; } // setter

	bool	isAlive() const { return alive; }
	void	setAlive(bool alive = true) {
		this->alive = alive;
		if (alive == false) setVisible(alive);
	}

	virtual void draw();

	void move(float speed) { position = { direction.x * speed, direction.y * speed }; }

	void move(const Vector2& direction) { position.x += direction.x; position.y += direction.y; }

	virtual void update(InputManager& input) {}

	bool isColliding(GameObject* other) const {
		if (other == nullptr) return false;
		return isOverlapping(other->getPosition(), other->getDimension());
	}

	virtual bool isOverlapping(const Vector2& pos, const Dimension& dim) const {
		auto myPos = getPosition();
		auto myDim = getDimension();
		if (myPos.x + myDim.x < pos.x || myPos.y + myDim.y < pos.y) return false;
		if (pos.x + dim.x < myPos.x || pos.y + dim.y < myPos.y) return false;
		return true;
	}

	bool isInside(const Vector2& pos) const {
		return position.x <= pos.x && position.x + dimension.x > pos.x && position.y <= pos.y && position.y + dimension.y > pos.y;
	}

	virtual ~GameObject() {
		delete[] this->shape;
		this->shape = nullptr;
		this->visible = false;
		this->alive = false;
	}

	static void Initialize();
	static void Add(GameObject* obj);
	static void Remove(GameObject* obj);
	static void ClearFrame();
	static void ProcessInput(bool& exit_flag, InputManager& input);
	static void UpdateAll(InputManager& input);
	static void DrawAll();
	static void RenderFrame();
	static void Deinitialize();
	static void ExamineCollision();
};


