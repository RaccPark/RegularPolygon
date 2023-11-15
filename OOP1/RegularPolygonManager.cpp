#include "RegularPolygonManager.h"
#include "RegularPolygon.h"
#include "InputManager.h"
#include "Canvas.h"

RegularPolygon* RegularPolygonManager::cast2RegularPolygon(GameObject* source) {
	RegularPolygon* polygon = nullptr;
	polygon = dynamic_cast<RegularPolygon*>(source);
	return polygon;
}

void RegularPolygonManager::getPolygons(Position& start, Position& end) {
	auto minX = start.x < end.x ? start.x : end.x;
	auto maxX = start.x >= end.x ? start.x : end.x;
	auto minY = start.y < end.y ? start.y : end.y;
	auto maxY = start.y >= end.y ? start.y : end.y;

	//Vector2 topLeft{ minX, minY };
	//Vector2 bottomRight{ maxX, maxY };

	for_each(objs.begin(), objs.end(), [&](GameObject* obj) {
		auto objPos = obj->getPosition();
		if (objPos.x >= minX && objPos.x <= maxX
			&& objPos.y >= minY && objPos.y <= maxY) {
			selectedObjs.push_back(obj);

			//RegularPolygon* polygon = nullptr;
			//polygon = dynamic_cast<RegularPolygon*>(obj);
			//if (polygon != nullptr) {
			//	polygon->setBlinkingPeriod(30);
			//}
		}
	});
}

bool RegularPolygonManager::isPolyInRect(Position& start, Position& end, GameObject* other){
	auto minX = start.x < end.x ? start.x : end.x;
	auto maxX = start.x >= end.x ? start.x : end.x;
	auto minY = start.y < end.y ? start.y : end.y;
	auto maxY = start.y >= end.y ? start.y : end.y;

	auto otherPos = other->getPosition();
	if (otherPos.x >= minX && otherPos.x <= maxX
		&& otherPos.y >= minY && otherPos.y <= maxY) return true;
	// ELSE
	return false;
}

// 좀 더 샤프하게 드래그 안에 있는 애들은 깜빡깜빡 벗어나면 안깜빡으로 하고 마지막에 마우스를 놓았을 때 선택된 애들만 잠깐동안 깜빡깜빡
void RegularPolygonManager::setBlinking() {
	Debug::Log("%d", selectedObjs.size());
	for_each(selectedObjs.begin(), selectedObjs.end(), [&](GameObject* obj) {
		obj->setVisible(!(obj->isVisible()));
		});
}

void RegularPolygonManager::addPoint() {
	for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
		RegularPolygon* polygon = this->cast2RegularPolygon(obj);
		if (polygon != nullptr)	++(*polygon);
		});
}
void RegularPolygonManager::subPoint() {
	for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
		RegularPolygon* polygon = this->cast2RegularPolygon(obj);
		if (polygon != nullptr)	(*polygon)--;
		});
}

void RegularPolygonManager::rotate() {
	for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
		RegularPolygon* polygon = this->cast2RegularPolygon(obj);
		if (polygon != nullptr) {
			polygon->setRotationSpeed(PI / 180.0f); // reset rotation speed
			polygon->rotate();
		}
		});
}
void RegularPolygonManager::rotatePress() {
	for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
		RegularPolygon* polygon = this->cast2RegularPolygon(obj);
		if (polygon != nullptr) {
			auto rotationSpeed = polygon->getRotationSpeed();
			polygon->setRotationSpeed(rotationSpeed + (PI / 180.0f)); // reset rotation speed
			polygon->rotate();
		}
		});
}

void RegularPolygonManager::incRadius() {
	for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
		RegularPolygon* polygon = this->cast2RegularPolygon(obj);
		if (polygon != nullptr) {
			auto radius = polygon->getRadius();
			if (radius < 30.0f) polygon->setRadius(++radius);
		}
		});
}
void RegularPolygonManager::decRadius() {
	for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
		RegularPolygon* polygon = this->cast2RegularPolygon(obj);
		if (polygon != nullptr) {
			auto radius = polygon->getRadius();
			if (radius > 1.0f) polygon->setRadius(--radius);
		}
		});
}

void RegularPolygonManager::move(InputManager& input) {
	if (input.getKey('A')) {
		for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
			RegularPolygon* polygon = this->cast2RegularPolygon(obj);
			if (polygon != nullptr) {
				polygon->setPosition(polygon->getPosition() + Vector2{ -1, 0 });
			}
			});
	}
	if (input.getKey('D')) {
		for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
			RegularPolygon* polygon = this->cast2RegularPolygon(obj);
			if (polygon != nullptr) {
				polygon->setPosition(polygon->getPosition() + Vector2{ 1, 0 });
			}
			});
	}
	if (input.getKey('W')) {
		for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
			RegularPolygon* polygon = this->cast2RegularPolygon(obj);
			if (polygon != nullptr) {
				polygon->setPosition(polygon->getPosition() + Vector2{ 0, -1 });
			}
			});
	}
	if (input.getKey('S')) {
		for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
			RegularPolygon* polygon = this->cast2RegularPolygon(obj);
			if (polygon != nullptr) {
				polygon->setPosition(polygon->getPosition() + Vector2{ 0, 1 });
			}
			});
	}
}

void RegularPolygonManager::update(InputManager& input) {

	sort(selectedObjs.begin(), selectedObjs.end());
	selectedObjs.erase(unique(selectedObjs.begin(), selectedObjs.end()), selectedObjs.end());

	if (input.getMouseButtonDown(0)) {
		auto mousePos = input.getMousePosition();

		// 디설렉된 오브젝트 원상태로
		if (selectedObjs.size() > 0) {
			for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
				RegularPolygon* polygon = this->cast2RegularPolygon(obj);
				if (polygon != nullptr) {
					polygon->setState(SelectState::Idle);
				}
				});
			selectedObjs.clear();
		}
		selectRect = new GameObject(" ", { 0, 0 }, { 1, 1 }, true, { 0, 0 });
		this->mouseStart = mousePos;
	}

	if (input.getMouseButton(0)) {
		// 디설렉된 오브젝트 원상태로
		if (selectedObjs.size() > 0) {
			auto firstToRemove = stable_partition(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
				return this->isPolyInRect(mouseStart, mouseCurr, obj);
				});

			for (auto it = firstToRemove; it != selectedObjs.end(); it++) {
				RegularPolygon* polygon = this->cast2RegularPolygon(*it);
				if (polygon != nullptr) {
					polygon->setState(SelectState::Idle);
				}
			}
			selectedObjs.erase(firstToRemove, selectedObjs.end());
		}
		auto mousePos = input.getMousePosition();
		this->mouseCurr = mousePos;
		selectRect->canvas.drawRectangle(mouseStart, mouseCurr, true);
		getPolygons(mouseStart, mouseCurr);
		for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
			RegularPolygon* polygon = this->cast2RegularPolygon(obj);
			if (polygon != nullptr) {
				polygon->setState(SelectState::isSelecting);
			}
			});
	}

	if (input.getMouseButtonUp(0)) {
		auto mousePos = input.getMousePosition();

		this->mouseEnd = mousePos;
		getPolygons(mouseStart, mouseEnd);
		for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
			RegularPolygon* polygon = this->cast2RegularPolygon(obj);
			if (polygon != nullptr) {
				polygon->setState(SelectState::Selected);
				polygon->setBlinkingPeriod(30);
			}
			});
		this->mouseStart = { 0,0 };
		this->mouseCurr = { 0,0 };
		this->mouseEnd = { 0,0 };

		delete selectRect;
		selectRect = nullptr;
	}

	if (input.getKeyDown(VK_RIGHT)) {
		addPoint();
	}
	if (input.getKeyDown(VK_LEFT)) {
		subPoint();
	}
	if (input.getKeyDown(VK_SPACE)) {
		rotate();
	}
	if (input.getKey(VK_SPACE)) {
		rotatePress();
	}
	if (input.getKey(VK_UP)) {
		incRadius();
	}
	if (input.getKey(VK_DOWN)) {
		decRadius();
	}
	if (input.getKey('A') || input.getKey('D') || input.getKey('W') || input.getKey('S')) {
		move(input);
	}
	// DEBUG
	/*
	if (input.getKeyDown(VK_F1)) {
		for_each(selectedObjs.begin(), selectedObjs.end(), [&](auto obj) {
			RegularPolygon* polygon = nullptr;
			polygon = dynamic_cast<RegularPolygon*>(obj);
			if(polygon != nullptr)	polygon->setBlinkingPeriod(30);
			});
	}
	*/
}