#pragma once
#include "singly_linked_list.h"
class CircularLinkedListVisualizer:public LinkedListVisualizer {
private:
	ListNode* tail;
public:
	CircularLinkedListVisualizer();
	void visualize();
	void initialize(vector<string> list);
	void insertStep(int idx, string val, bool checkStep);
	void insertStepHead(int idx, string val, bool checkStep);
	void insertStepTail(int idx, string val, bool checkStep);
	void deleteStep(int idx, bool checkStep);
	void searchStep(string val, bool checkStep);
	void deleteStepHead(bool checkStep);
	void insertEmpty(string val, bool checkStep);
};
