#pragma once

#include <iostream>
#include "Utils.h"

// canvas
class Canvas {
	char* frameBuffer;
	int		size;
	int		width;
	int		height;

public:

	Canvas(int width, int height) : width(width), height(height), size( (width+1)*height), frameBuffer(new char[((size_t)width+1)*height])
	{
		Borland::Initialize();
		clear();
	}

	int getSize() const { return size; }

	Dimension getDimension() const { return Dimension{ width, height }; }


	void clear()
	{
		memset(frameBuffer, ' ', size);
		
		frameBuffer[size-1] = '\0';
	}

	void draw(const char* shape, int pos, bool visible)
	{
		if (visible == false) return;

		for (int i = 0; i < strlen(shape); i++)
		{
			if (pos + i < 0) continue;
			if (pos + i > size - 1) continue;

			frameBuffer[pos + i] = shape[i];
		}
	}

	void drawLine(const char shape, const Vector2& start, const Vector2& end);

	void drawRectangle(const Position& start, const Position& end, bool visible)
	{
		if (visible == false) return;

		auto minX = start.x < end.x ? start.x : end.x;
		auto maxX = start.x >= end.x ? start.x : end.x;
		auto minY = start.y < end.y ? start.y : end.y;
		auto maxY = start.y >= end.y ? start.y : end.y;

		Vector2 topLeft{ minX, minY };
		Vector2 topRight{ maxX, minY };
		Vector2 bottomLeft{ minX, maxY };
		Vector2 bottomRight{ maxX, maxY };

		drawLine('\xc4', topLeft, topRight);
		drawLine('\xb3', topLeft, bottomLeft);
		drawLine('\xb3', bottomRight, topRight);
		drawLine('\xc4', bottomLeft, bottomRight);

		draw("\xda", topLeft, { 1,1 }, visible);
		draw("\xbf", topRight, { 1,1 }, visible);
		draw("\xc0", bottomLeft, { 1,1 }, visible);
		draw("\xd9", bottomRight, { 1,1 }, visible);
	}

	void draw(const char* shape, const Vector2& pos, const Dimension& dim, bool visible)
	{
		if (visible == false) return;
		int pos_x = (int)pos.x;
		int pos_y = (int)pos.y;

		for (int h = 0; h < dim.y; h++)
		{
			if (pos_y + h < 0) continue;
			if (pos_y + h >= height) break;

			for (int w = 0; w < dim.x; w++)
			{
				if (pos_x + w < 0) continue;
				if (pos_x + w >= width) break;

				if (shape[w + h * dim.x] == ' ') continue;

				frameBuffer[pos_x + w + (pos_y + h) * (width + 1)] = shape[w + h * dim.x];
			}
		}
	}

	void render() const
	{
		Borland::GotoXY(0, 0);

		// during the class, i referred to frameBuffer[h*width], which is wrong eventually.
		for (int h = 0; h < height; h++)
			frameBuffer[h*(width+1)+width] = '\n'; 
		frameBuffer[size - 1] = '\0';
		printf("%s", frameBuffer);
	}

	~Canvas()
	{
		delete[] frameBuffer;
		frameBuffer = nullptr;
		size = 0;
	}
};

