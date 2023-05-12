#pragma once
#include "global.h"
#include "utils.h"
#include "array_element.h"
class StaticArrayVisualizer {
private:
	SDL_Color bgColor;
	void reCalcCoordinate();
	void resetColor();
	void resetColor(ArrayElement* arr, int size);
	void setArrayInsert(ArrayElement* arr, ArrayElement* clone, int idx, string val, int lim);
	void update(int idx, string newVal);
	int search(string val);
	void setArrayDelete(ArrayElement* arr, ArrayElement* clone, int idx, int lim);
public:
	ArrayElement arr[30];
	int size;
	StaticArrayVisualizer();
	void visualize();
	void visualize(ArrayElement* arr, int size);
	void initialize(vector<string> list);
	void insertStep(int idx, string val, bool checkStep);
	void searchStep(string val, bool checkStep);
	void deleteStep(int idx, bool checkStep);
	void updateStep(int idx, string val, bool checkStep);
};