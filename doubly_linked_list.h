#pragma once
#include "utils.h"
#include "list_node.h"
class DoublyLinkedListVisualizer {
private:
	SDL_Color bgColor;
	void freeMem();
	void reCalcCoordinate();
	void resetColor();
	void resetArrow();
	int search(string val);
public:
	DoublyListNode* head;
	int listSize;
	DoublyLinkedListVisualizer();
	~DoublyLinkedListVisualizer();
	void visualize();
	void initialize(vector<string> list);
	DoublyListNode* getNode(int idx);
	void searchStep(string val, bool checkStep);
	void updateStep(int idx, string val, bool checkStep);
	void insertStep(int idx, string val, bool checkStep);
	void insertStepHead(string val, bool checkStep);
	void deleteStep(int idx, bool checkStep);
	void deleteStepHead(bool checkStep);
	void insertEmpty(string val, bool checkStep);
};
