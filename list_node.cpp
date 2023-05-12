#include "list_node.h"
ListNode::ListNode() {
	this->val = "N";
	this->next = nullptr;
	this->centerX = 0;
	this->centerY = 0;
	this->radius = 0;
	this->textSize = theme.getTextSize();
	this->colorNode = theme.getElemColor();
	this->colorArrow = theme.getArrowColor();
	this->colorBg = theme.getBgElemColor();
	this->colorText = theme.getTextColor();
	this->arrowSX = centerX + radius;
	this->arrowSY = centerY;
	this->arrowDX = centerX + radius + 30;
	this->arrowDY = centerY;
}
ListNode::ListNode(string val, int centerX, int centerY, int radius) {
	this->val = val;
	this->next = nullptr;
	this->centerX = centerX;
	this->centerY = centerY;
	this->radius = radius;
	this->textSize = theme.getTextSize();
	this->colorNode = theme.getElemColor();
	this->colorBg = theme.getBgElemColor();
	this->colorArrow = theme.getArrowColor();
	this->colorText = theme.getTextColor();
	this->arrowSX = centerX + radius;
	this->arrowSY = centerY;
	this->arrowDX = centerX + radius + 30;
	this->arrowDY = centerY;
}
ListNode::ListNode(string val, int centerX, int centerY, int radius, ListNode* next) {
	this->val = val;
	this->next = next;
	this->centerX = centerX;
	this->centerY = centerY;
	this->radius = radius;
	this->textSize = theme.getTextSize();
	this->colorNode = theme.getElemColor();
	this->colorBg = theme.getBgElemColor();
	this->colorArrow = theme.getArrowColor();
	this->colorText = theme.getTextColor();
	this->arrowSX = centerX + radius;
	this->arrowSY = centerY;
	this->arrowDX = centerX + radius + 30;
	this->arrowDY = centerY;
}
void ListNode::displayNode() {
	SDL_RenderDrawCircle(gRenderer, centerX, centerY, radius, colorNode);
	SDL_RenderFillCircle(gRenderer, centerX, centerY, radius - 1, colorBg);
	createText(numberFont, colorText, val.size() == 1 && val != "0" ? "0" + val : val, centerX - textSize / 2, centerY - textSize / 2, textSize, textSize);
	if (next) {
		DrawArrow(gRenderer, arrowSX, arrowDX, arrowSY, arrowDY, 8, colorArrow);
	}
}
void ListNode::setColorNode(SDL_Color newColor) {
	colorNode = newColor;
}
void ListNode::setColorText(SDL_Color newColor) {
	colorText = newColor;
}
void ListNode::setColorArrow(SDL_Color newColor) {
	colorArrow = newColor;
}
void ListNode::setColorBg(SDL_Color newColor) {
	colorBg = newColor;
}
void ListNode::setTextSize(int size) {
	textSize = size;
}
void ListNode::setArrowPos(int x1, int y1, int x2, int y2) {
	arrowSX = x1;
	arrowSY = y1;
	arrowDX = x2;
	arrowDY = y2;
}
void ListNode::setCenterPos(int x, int y) {
	centerX = x;
	centerY = y;
}
DoublyListNode::DoublyListNode() {
	this->val = "N";
	this->next = nullptr;
	this->prev = nullptr;
	this->centerX = 0;
	this->centerY = 0;
	this->radius = 0;
	this->textSize = theme.getTextSize();
	this->colorNode = theme.getElemColor();
	this->colorBg = theme.getBgElemColor();
	this->colorArrow = theme.getArrowColor();
	this->colorText = theme.getTextColor();
	setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius + 30, centerY - 10);
	setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius - 30, centerY + 10);
}
DoublyListNode::DoublyListNode(string val, int centerX, int centerY, int radius) {
	this->val = val;
	this->next = nullptr;
	this->prev = nullptr;
	this->centerX = centerX;
	this->centerY = centerY;
	this->radius = radius;
	this->textSize = theme.getTextSize();
	this->colorNode = theme.getElemColor();
	this->colorBg = theme.getBgElemColor();
	this->colorArrow = theme.getArrowColor();
	this->colorText = theme.getTextColor();
	setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius + 30, centerY - 10);
	setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius - 30, centerY + 10);
}
DoublyListNode::DoublyListNode(string val, int centerX, int centerY, int radius, DoublyListNode* next, DoublyListNode* prev) {
	this->val = val;
	this->next = next;
	this->prev = prev;
	this->centerX = centerX;
	this->centerY = centerY;
	this->radius = radius;
	this->textSize = theme.getTextSize();
	this->colorNode = theme.getElemColor();
	this->colorBg = theme.getBgElemColor();
	this->colorArrow = theme.getArrowColor();
	this->colorText = theme.getTextColor();
	setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius + 30, centerY - 10);
	setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius - 30, centerY + 10);
}
void DoublyListNode::displayNode() {
	SDL_RenderDrawCircle(gRenderer, centerX, centerY, radius, colorNode);
	SDL_RenderFillCircle(gRenderer, centerX, centerY, radius - 1, colorBg);
	createText(numberFont, colorText, val.size() == 1 && val != "0" ? "0" + val : val, centerX - textSize / 2, centerY - textSize / 2, textSize, textSize);
	if (next) {
		DrawArrow(gRenderer, arrowSX, arrowDX, arrowSY, arrowDY, 8, colorArrow);
	}
	if (prev) {
		DrawArrow(gRenderer, arrowSXPrev, arrowDXPrev, arrowSYPrev, arrowDYPrev, 8, colorArrow);
	}
}
void DoublyListNode::setColorNode(SDL_Color newColor) {
	colorNode = newColor;
}
void DoublyListNode::setColorArrow(SDL_Color newColor) {
	colorArrow = newColor;
}
void DoublyListNode::setColorText(SDL_Color newColor) {
	colorText = newColor;
}
void DoublyListNode::setArrowNextPos(int x1, int y1, int x2, int y2) {
	arrowSX = x1;
	arrowSY = y1;
	arrowDX = x2;
	arrowDY = y2;
}
void DoublyListNode::setArrowPrevPos(int x1, int y1, int x2, int y2) {
	arrowSXPrev = x1;
	arrowSYPrev = y1;
	arrowDXPrev = x2;
	arrowDYPrev = y2;
}
void DoublyListNode::setCenterPos(int x, int y) {
	centerX = x;
	centerY = y;
}
void DoublyListNode::setArrowNextNormal() {
	setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius + 30, centerY - 10);
}
void DoublyListNode::setArrowPrevNormal() {
	setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius - 30, centerY + 10);
}
void DoublyListNode::setArrowNextX2() {
	setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius * 3 + 30 * 2 + 10, centerY - 10);
}
void DoublyListNode::setArrowPrevX2() {
	setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius * 3 - 30 * 2 - 10, centerY + 10);
}
void DoublyListNode::setArrowNextUp() {
	setArrowNextPos(centerX - 10, centerY - radius + 3, centerX - 10, centerY - radius - 40);
}
void DoublyListNode::setArrowNextDown() {
	setArrowNextPos(centerX + 10, centerY + radius - 3, centerX + 10, centerY + radius + 40);
}
void DoublyListNode::setArrowPrevDown() {
	setArrowPrevPos(centerX + 10, centerY + radius - 3, centerX + 10, centerY + radius + 40);
}
void DoublyListNode::setArrowPrevUp() {
	setArrowPrevPos(centerX - 10, centerY - radius + 3, centerX - 10, centerY - radius - 40);
}
void DoublyListNode::setArrowPrevDownLeft() {
	setArrowPrevPos(centerX - 18, centerY + 18, centerX - 70, centerY + 70);
}
void DoublyListNode::setArrowPrevUpLeft() {
	setArrowPrevPos(centerX - radius + 2, centerY - 11, centerX - 78, centerY - 66);
}
void DoublyListNode::setArrowNextUpperRight() {
	setArrowNextPos(centerX + radius - 2, centerY - 11, centerX + 78, centerY - 66);
}
void DoublyListNode::setArrowNextDownRight() {
	setArrowNextPos(centerX + 18, centerY + 18, centerX + 70, centerY + 70);
}
