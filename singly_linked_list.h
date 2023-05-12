#pragma once
#include "utils.h"
#include "list_node.h"
class LinkedListVisualizer {
protected:
	SDL_Color bgColor;
	void freeMem();
	void reCalcCoordinate();
	void resetColor();
	void resetArrow();
	void deleteNode(int idx);
	int search(string val);
	void update(int idx, string newVal);
public:
	ListNode* head;
	int listSize;
	LinkedListVisualizer();
	~LinkedListVisualizer();
	virtual void visualize();
	void initialize(vector<string> list);
	ListNode* getNode(int idx);
	void insertStep(int idx, string val, bool checkStep);
	void searchStep(string val, bool checkStep);
	void deleteStep(int idx, bool checkStep);
	void deleteStepHead(bool checkStep);
	void updateStep(int idx, string val, bool checkStep);
	void insertEmpty(string val, bool checkStep);
};
