#pragma once
#include "list_node.h"
class QueueVisualizer {
private:
	SDL_Color bgColor;
	void freeMem();
	void reCalcCoordinate();
	void resetColor();
	void resetArrow();
public:
	ListNode* head;
	ListNode* tail;
	int listSize;
	QueueVisualizer();
	~QueueVisualizer();
	ListNode* getNode(int idx);
	void visualize();
	void initialize(vector<string> list);
	void popStep(bool checkStep);
	void pushStep(string val, bool checkStep);
	void insertEmpty(string val, bool checkStep);
	void clearStep(bool checkStep);
};
