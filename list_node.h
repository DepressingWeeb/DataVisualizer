#pragma once
#include "global.h"
#include "utils.h"
class ListNode {
public:
	string val;
	ListNode* next;
	int centerX;
	int centerY;
	int radius;
	int arrowSX;
	int arrowSY;
	int arrowDX;
	int arrowDY;
	int textSize;
	SDL_Color colorNode;
	SDL_Color colorBg;
	SDL_Color colorArrow;
	SDL_Color colorText;
	ListNode();
	ListNode(string val, int centerX, int centerY, int radius);
	ListNode(string val, int centerX, int centerY, int radius, ListNode* next);
	void displayNode();
	void setColorNode(SDL_Color newColor);
	void setColorText(SDL_Color newColor);
	void setColorArrow(SDL_Color newColor);
	void setColorBg(SDL_Color newColor);
	void setTextSize(int size);
	void setArrowPos(int x1, int y1, int x2, int y2);
	void setCenterPos(int x, int y);
};
class DoublyListNode {
public:
	string val;
	DoublyListNode* next;
	DoublyListNode* prev;
	int centerX;
	int centerY;
	int radius;
	int arrowSX;
	int arrowSY;
	int arrowDX;
	int arrowDY;
	int arrowSXPrev;
	int arrowSYPrev;
	int arrowDXPrev;
	int arrowDYPrev;
	int textSize;
	SDL_Color colorNode;
	SDL_Color colorBg;
	SDL_Color colorArrow;
	SDL_Color colorText;
	TTF_Font* numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	DoublyListNode();
	DoublyListNode(string val, int centerX, int centerY, int radius);
	DoublyListNode(string val, int centerX, int centerY, int radius, DoublyListNode* next, DoublyListNode* prev);
	void displayNode();
	void setColorNode(SDL_Color newColor);
	void setColorArrow(SDL_Color newColor);
	void setColorText(SDL_Color newColor);
	void setArrowNextPos(int x1, int y1, int x2, int y2);
	void setArrowPrevPos(int x1, int y1, int x2, int y2);
	void setCenterPos(int x, int y);
	void setArrowNextNormal();
	void setArrowPrevNormal();
	void setArrowNextX2();
	void setArrowPrevX2();
	void setArrowNextUp();
	void setArrowNextDown();
	void setArrowPrevDown();
	void setArrowPrevUp();
	void setArrowPrevDownLeft();
	void setArrowPrevUpLeft();
	void setArrowNextUpperRight();
	void setArrowNextDownRight();
};