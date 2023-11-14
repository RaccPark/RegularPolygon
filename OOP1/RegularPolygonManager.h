#pragma once
#include <vector>
#include <algorithm>

#include "GameObject.h"
#include "RegularPolygon.h"
//#include "Utils.h"

using namespace std;

class RegularPolygonManager: public GameObject
{
private:
	GameObject* selectRect;
	vector<GameObject*> selectedObjs;

	Position mouseStart;
	Position mouseCurr;
	Position mouseEnd;

public:
	RegularPolygonManager() : GameObject(" ", { 0, 0 }, true), selectRect(nullptr), mouseStart({0, 0}), mouseCurr({ 0, 0 }), mouseEnd({ 0, 0 })
	{}

	RegularPolygon* cast2RegularPolygon(GameObject* source);

	void getPolygons(Position& start, Position& end);

	void addPoint();
	void subPoint();

	void rotate();
	void rotatePress();

	void incRadius();
	void decRadius();

	void move(InputManager& input);

	void update(InputManager& input) override;

	~RegularPolygonManager()
	{
	}
};

