#pragma once
#include "utils.h"
#include "array_element.h"
class DynamicArrayVisualizer {
private:
	SDL_Color bgColor;
	void freeMem();
	void reCalcCoordinate();
	void resetColor();
	void resetColor(ArrayElement* arr, int size);
	void setArrayInsert(ArrayElement* newArr, ArrayElement* oldArr, int idx, string val, int lim);
	void update(int idx, string newVal);
	int search(string val);
	void setArrayDelete(ArrayElement* newArr, ArrayElement* oldArr, int idx, int lim);
public:
	ArrayElement* arr;
	int size;
	DynamicArrayVisualizer();
	~DynamicArrayVisualizer();
	void visualize();
	void visualize(ArrayElement* arr, int size);
	void initialize(vector<string> list);
	void insertStep(int idx, string val, bool checkStep);
	void searchStep(string val, bool checkStep);
	void deleteStep(int idx, bool checkStep);
	void updateStep(int idx, string val, bool checkStep);
};
