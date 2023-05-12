#include "array_element.h"
ArrayElement::ArrayElement() { this->val = "N"; }
ArrayElement::ArrayElement(int x, int y, int w, int h, SDL_Color color, SDL_Color bgColor, SDL_Color textColor, int textSize, string val, string index) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->color = color;
	this->bgColor = bgColor;
	this->textColor = textColor;
	this->textSize = textSize;
	this->val = val;
	this->index = index;
}
void ArrayElement::display() {
	SDL_Color old;
	SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(gRenderer, createRect(&rect, x, y, w, h));
	SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderFillRect(gRenderer, createRect(&rect, x + 1, y + 1, w - 2, h - 2));
	createText(numberFont, textColor, val.size() == 1 && val != "0" ? "0" + val : val, x + (50 - textSize) / 2, y + (50 - textSize) / 2, textSize, textSize);
	createText(numberFont, BLACK, index, x + 17, y + 60, 15, 20);
	SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
}
void ArrayElement::changeColor(SDL_Color color) {
	this->color = color;
	this->textColor = color;
}
void ArrayElement::changeColor(SDL_Color elemColor, SDL_Color textColor) {
	this->color = elemColor;
	this->textColor = textColor;
}
void ArrayElement::changeCoordinate(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}