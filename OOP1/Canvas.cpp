#include "Canvas.h"
#include "Utils.h"

void Canvas::drawLine(const char shape, const Vector2& start, const Vector2& end)
{
	Vector2 diff = end - start;
	Vector2 dir = diff.unit();
	Vector2 loc = start;

	do {
		draw(&shape, loc, { 1,1 }, true);
		loc += dir * 0.5f;
		if ( (loc-start).sqrMagnitude() >= diff.sqrMagnitude())
			loc = end;
	} while (loc.equalApproximately(end) == false);
	draw(&shape, end, { 1,1 }, true);
}