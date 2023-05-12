#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
class Theme {
private:
	SDL_Color bgColor;
	SDL_Color bgElemColor;
	SDL_Color textColor;
	SDL_Color elemColor;
	SDL_Color arrowColor;
	int textSize;
	Theme(SDL_Color bgColor, SDL_Color bgElemColor, SDL_Color textColor, SDL_Color elemColor, SDL_Color arrowColor, int textSize) {
		this->bgColor = bgColor;
		this->bgElemColor = bgElemColor;
		this->textColor = textColor;
		this->elemColor = elemColor;
		this->arrowColor = arrowColor;
		this->textSize = textSize;
	}
public:
	SDL_Color getBgColor() {
		return bgColor;
	}
	SDL_Color getBgElemColor() {
		return bgElemColor;
	}
	SDL_Color getTextColor() {
		return textColor;
	}
	SDL_Color getElemColor() {
		return elemColor;
	}
	SDL_Color getArrowColor() {
		return arrowColor;
	}
	int getTextSize() {
		return textSize;
	}
	void setBgColor(SDL_Color color) {
		bgColor = color;
	}
	void setBgElemColor(SDL_Color color) {
		bgElemColor = color;
	}
	void setTextColor(SDL_Color color) {
		textColor = color;
	}
	void setElemColor(SDL_Color color) {
		elemColor = color;
	}
	void setArrowColor(SDL_Color color) {
		arrowColor = color;
	}
	void setTextSize(int size) {
		textSize = size;
	}
	static Theme getInstance(SDL_Color bgColor, SDL_Color bgElemColor, SDL_Color textColor, SDL_Color elemColor, SDL_Color arrowColor, int textSize) {
		static Theme theme(bgColor, bgElemColor, textColor, elemColor, arrowColor, textSize);
		return theme;
	}
};
