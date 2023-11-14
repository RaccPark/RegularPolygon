#include "Utils.h"
#include <typeinfo>

using namespace std;

int Debug::nDebugLine;
char Debug::whiteSpaces[120] = { '\0' };

template<>
bool  _Vector2<float>::equalApproximately(const _Vector2<float>& other) const { return distance(other) < 0.5f; }

template<>
bool  _Vector2<int>::equalApproximately(const _Vector2<int>& other) const { return this->x == other.x && this->y == other.y; }

template<>
void _Vector2<float>::print() const
{
	Debug::Log("(%f, %f)", x, y);
}

template<>
void _Vector2<int>::print() const
{
	Debug::Log("(%d, %d)", x, y);
}

void Debug::Log(const char* fmt, ...)
{
	char dest[1024];
	static clock_t startTime = clock();

	if (whiteSpaces[0] == '\0') { // if not initialized yet.
		memset(whiteSpaces, ' ', 120);
		whiteSpaces[119] = '\0';
	}

	va_list argPtr;

	va_start(argPtr, fmt);

	Borland::GotoXY(47, nDebugLine++ % 45);
	vsprintf(dest, fmt, argPtr);
	va_end(argPtr);
	printf("[%5d: %.3f] %s", nDebugLine, (clock() - startTime) / 1000.0f, dest);
	printf("\n%s", whiteSpaces);
}