#include "RegularPolygon.h"
#include "InputManager.h"
#include "Canvas.h"

void RegularPolygon::rotate()
{
	float speed = rotationSpeed;
	for_each(points.begin(), points.end(), [=](auto& point) {
		point = Vector2{ cos(speed) * point.x + sin(speed) * point.y, 
			-1.0f * sin(speed) * point.x + cos(speed) * point.y };
	});	
}

void RegularPolygon::update(InputManager& input)
{
	/*
	if (input.getKeyDown(VK_RIGHT)) ++(*this);
	
	if (input.getKeyDown(VK_LEFT)) (*this)--;

	if (input.getKeyDown(VK_SPACE)) {
		rotationSpeed = (PI / 180.0f); // reset rotation speed
		rotate();
	}
	
	if (input.getKey(VK_SPACE)) {
		rotationSpeed += (PI / 180.0f); // increment rotation speed by one degree.
		rotate();
	}
	if (input.getKey(VK_UP)) {
		if (radius < 30.0f) 
			radius++;
	}
	if (input.getKey(VK_DOWN)) {
		if (radius > 1.0f) 
			radius--;
	}
	if (input.getKey('A')) setPosition( getPosition() + Vector2{ -1, 0 } );
	if (input.getKey('D')) setPosition( getPosition() + Vector2{ 1, 0 } );
	if (input.getKey('W')) setPosition( getPosition() +  Vector2{0, -1} );
	if (input.getKey('S')) setPosition( getPosition() + Vector2{0, 1 } );
	*/
}


void RegularPolygon::draw()
{
	if (!isVisible()) return;

	if (n_frames > 0) {
		n_frames--;
		if (n_frames % 2 == 0) return;
	}

	for (int i = 0; i < points.size(); i++) {
		canvas.drawLine('#', 
			points.at( i      % (int)points.size()) * radius + getPosition(),
			points.at((i + 1) % (int)points.size()) * radius + getPosition());
	}
}

