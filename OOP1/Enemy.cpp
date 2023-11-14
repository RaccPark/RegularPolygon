#include "Enemy.h"
#include "Canvas.h"


void Enemy::draw() 
{
	if (n_frames > 0) {
		n_frames--;
		GameObject::draw();
		static char buffer[10];
		sprintf(buffer, "%2.1f", hp);
		auto pos = getPosition();
		canvas.draw(buffer, { pos.x + 1, pos.y }, { (int)strlen(buffer), 1 }, true);
	}
	else {
		GameObject::draw();
	}
}