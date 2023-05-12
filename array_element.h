#pragma once
#include "global.h"
#include "utils.h"
class ArrayElement {
public:
	int x;
	int y;
	int w;
	int h;
	int textSize;
	string index;
	SDL_Color color;
	SDL_Color bgColor;
	SDL_Color textColor;
	string val;
	SDL_Rect rect;
	ArrayElement();
	ArrayElement(int x, int y, int w, int h, SDL_Color color, SDL_Color bgColor, SDL_Color textColor, int textSize, string val, string index);
	void display();
	void changeColor(SDL_Color color);
	void changeColor(SDL_Color elemColor, SDL_Color textColor);
	void changeCoordinate(int x, int y, int w, int h);
};
