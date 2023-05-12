#pragma once
#include "utils.h"
#include "static_array.h"
#include "dynamic_array.h"
#include "singly_linked_list.h"
#include "doubly_linked_list.h"
#include "circular_linked_list.h"
#include "stack.h"
#include "queue.h"

vector<string> initializeFormInput();
tuple <int, string, bool, bool> indexValueFormInput(string formName, int indexLimit);
tuple <int, bool, bool> indexFormInput(string formName, int indexLimit);
tuple <string, bool, bool> valueFormInput(string formName);
void ImGuiChangeColorNode(ListNode* node, bool& shownWindow);
void ImGuiChangeColorDoublyNode(DoublyListNode* node, bool& shownWindow);
void ImGuiChangeColorArrElem(ArrayElement& elem, bool& shownWindow);
void ImGuiChangeColorElem(vector<SDL_Color>& choice, int& textSize, bool& shownWindow);
void staticArrayVisualizing();
void dynamicArrayVisualizing();
void linkedListVisualizing();
void circularLinkedListVisualizing();
void doublyLinkedListVisualizing();
void stackVisualizing();
void queueVisualizing();
void linkedListMenu();
void arrayMenu();
void themeOptions();
void mainMenu();
void startingScreen();
