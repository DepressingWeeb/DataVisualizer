#pragma once
#include "list_node.h"
class StackVisualizer {
private:
	SDL_Color bgColor;
	void freeMem();
	void reCalcCoordinate();
	void resetColor();
	void resetArrow();
public:
	ListNode* head;
	int listSize;
	StackVisualizer();
	~StackVisualizer();
	ListNode* getNode(int idx);
	void visualize();
	void initialize(vector<string> list);
	void popStep(bool checkStep);
	void pushStep(string val, bool checkStep);
	void clearStep(bool checkStep);
};
