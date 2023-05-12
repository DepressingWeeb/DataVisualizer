#include "singly_linked_list.h"
LinkedListVisualizer::LinkedListVisualizer() {
	head = nullptr;
	listSize = 0;
	bgColor = theme.getBgColor();
}
LinkedListVisualizer::~LinkedListVisualizer() {
	freeMem();
}
void LinkedListVisualizer::freeMem() {
	ListNode* current = head;
	while (listSize--) {
		ListNode* next = current->next;
		delete current;
		current = next;
	}
	head = nullptr;
}
void LinkedListVisualizer::visualize() {
	ListNode* clone = head;
	while (clone) {
		clone->displayNode();
		clone = clone->next;
	}
}
void LinkedListVisualizer::initialize(vector<string> list) {
	freeMem();
	this->listSize = list.size();
	if (list.size() == 0) return;
	head = new ListNode(list[0], 100, 100, 25);
	auto clone = head;
	for (int i = 1; i < list.size(); i++) {
		clone->next = new ListNode(list[i], 100 + 90 * i, 100, 25);
		clone = clone->next;
	}
}
void LinkedListVisualizer::reCalcCoordinate() {
	ListNode* clone = head;
	if (!clone) return;
	clone->centerX = 100;
	clone->centerY = 100;
	clone->radius = 25;
	int i = 1;
	while (true) {
		clone = clone->next;
		if (!clone) break;
		clone->centerX = 100 + 90 * i;
		clone->centerY = 100;
		i++;
	}
}
ListNode* LinkedListVisualizer::getNode(int idx) {
	ListNode* clone = head;
	while (idx--) {
		clone = clone->next;
	}
	return clone;
}
void LinkedListVisualizer::resetColor() {
	ListNode* clone = head;
	while (clone) {
		clone->setColorNode(theme.getElemColor());
		clone->setColorArrow(theme.getArrowColor());
		clone->setColorText(theme.getTextColor());
		clone = clone->next;
	}
}
void LinkedListVisualizer::resetArrow() {
	ListNode* clone = head;
	while (clone) {
		clone->setArrowPos(clone->centerX + clone->radius, clone->centerY, clone->centerX + clone->radius + 30, clone->centerY);
		clone = clone->next;
	}
}
void LinkedListVisualizer::insertStep(int idx, string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
	vector<string> code;
	if (idx)
		code = {
		"while(index-- > 1){",
		"    head = head->next;",
		"}",
		"ListNode* newNode = new ListNode(value,nullptr);",
		"newNode->next = head->next;",
		"head->next = newNode;"
	};
	else
		code = {
		"ListNode* newNode = new ListNode(value,nullptr);",
		"newNode->next = head;",
		"head = newNode;"
	};
	int linesColor = -1;
	int totalSteps = idx ? idx * 2 + 2 : 3;
	SDL_Rect rect;
	SDL_Rect endButton = *createRect(&rect, 1413, 165, 100, 50);
	SDL_Rect endButtonHover = *createRect(&rect, 1683, 165, 100, 50);
	int speed = 0;
	int t = 60 / (speed + 1);
	SDL_Rect buttonPosArray[4][2];
	for (int i = 0; i < 4; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 1413 + 180 * i, 870, 130, 130);
		buttonPosArray[i][1] = *createRect(&rect, 1413 + 180 * i, 1149, 130, 130);
	}
	SDL_Event e;
	bool quit = false;
	bool stop = false;
	int currStep = checkStep ? 0 : 1;
	int currHeadIndex = 0;
	int framesCount = 1;
	ListNode* newNode;
	ListNode* tmp = idx - 1 >= 0 ? getNode(idx - 1) : nullptr;
	if (tmp)	newNode = new ListNode(val, tmp->centerX, tmp->centerY + 100, 25);
	else newNode = new ListNode(val, head->centerX, head->centerY + 100, 25);
	ListNode* oldNode = getNode(idx);
	ListNode* currHead = nullptr;
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_RenderFillRect(gRenderer, &SCREEN);
		createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
		if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
		t = 60 / (speed + 1);
		ImGuiControlStep(currStep, totalSteps, checkStep, speed, t, quit, stop);
		if (framesCount % t == 0 && !checkStep && !stop) currStep = min(currStep + 1, totalSteps);
		currHead = getNode(currHeadIndex);
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			currHeadIndex = 0;
			if (tmp) tmp->next = oldNode;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep < totalSteps - 2) {
			resetColor();
			if (currStep % 2 == 0) {
				getNode(currStep / 2)->setColorNode(RED);
				linesColor = 1;
				currHeadIndex = currStep / 2;
			}
			else linesColor = 0;
			visualize();
		}
		else {
			if (currStep == totalSteps - 2) {
				if (tmp)	tmp->next = oldNode;
				newNode->next = nullptr;
				visualize();
				newNode->displayNode();
				linesColor = idx ? 3 : 0;
			}
			else if (currStep == totalSteps - 1) {
				newNode->next = oldNode;
				if (tmp)	tmp->next = oldNode;
				if (oldNode)newNode->setArrowPos(idx ? newNode->centerX + 10 : newNode->centerX, idx ? newNode->centerY - newNode->radius + 5 : newNode->centerY - newNode->radius, idx ? oldNode->centerX - 25 : oldNode->centerX, idx ? oldNode->centerY + 25 : oldNode->centerY + 35);
				if (tmp)	tmp->setArrowPos(tmp->centerX + tmp->radius, tmp->centerY, tmp->centerX + tmp->radius + 30, tmp->centerY);
				visualize();
				newNode->displayNode();
				linesColor = idx ? 4 : 1;
			}
			else {
				newNode->next = oldNode;
				if (tmp)	tmp->next = newNode;
				if (tmp)	tmp->setArrowPos(tmp->centerX, tmp->centerY + tmp->radius, newNode->centerX, newNode->centerY - 35);
				visualize();
				if (idx == 0) newNode->displayNode();
				linesColor = idx ? 5 : 2;
				currHead = idx ? currHead : newNode;
			}
		}
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
			}
		}
		if (currHead == newNode)
			createText(generalFont, RED, "Head", currHead->centerX - currHead->radius + 5, currHead->centerY + currHead->radius + 5, 10 * 4, 20);
		else
			createText(generalFont, RED, "Head", currHead->centerX - currHead->radius + 5, currHead->centerY - currHead->radius - 25, 10 * 4, 20);
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ImGui::GetIO().WantCaptureMouse) break;
				if (e.button.x > 850 && e.button.x < 950 && e.button.y > 500 && e.button.y < 550) { quit = true; }
				else if (!checkStep && e.button.x > 900 && e.button.x < 950 && e.button.y > 400 && e.button.y < 450) speed = (speed + 1) % 4;
			case SDL_KEYDOWN:
				if (checkStep) {
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT: currStep = max(currStep - 1, 0); break;
					case SDLK_RIGHT: currStep = min(currStep + 1, totalSteps); break;
					}
				}
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
		framesCount++;
	}
	if (tmp)	tmp->next = newNode;
	newNode->next = oldNode;
	if (idx == 0) head = newNode;
	listSize++;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void LinkedListVisualizer::deleteNode(int idx) {
	if (idx == 0) {
		ListNode* clone = head;
		head = clone->next;
		clone->next = nullptr;
		delete clone;
		reCalcCoordinate();
	}
	else {
		ListNode* prev = head;
		ListNode* curr = head->next;
		while (idx - 1) {
			prev = prev->next;
			curr = curr->next;
			idx--;
		}
		prev->next = curr->next;
		curr->next = nullptr;
		delete curr;
		reCalcCoordinate();
	}
}
int LinkedListVisualizer::search(string val) {
	int ans = 0;
	ListNode* clone = head;
	while (clone) {
		if (clone->val == val)
			return ans;
		else
			clone = clone->next;
		ans++;
	}
	return -1;
}
void LinkedListVisualizer::searchStep(string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	vector<string> code = { "while (head){",
						 "    if( head->val == target )",
						 "        return ans;",
						 "    head = head->next;",
						 "    ans++;",
						 "}",
						 "return NOT_FOUND;" };
	int linesColor = -1;
	int ans = search(val);
	int totalSteps = ans == -1 ? (listSize) * 4 + 2 : ans * 4 + 3;
	SDL_Rect rect;
	SDL_Rect endButton = *createRect(&rect, 1413, 165, 100, 50);
	SDL_Rect endButtonHover = *createRect(&rect, 1683, 165, 100, 50);
	int speed = 0;
	int t = 60 / (speed + 1);
	SDL_Rect buttonPosArray[4][2];
	for (int i = 0; i < 4; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 1413 + 180 * i, 870, 130, 130);
		buttonPosArray[i][1] = *createRect(&rect, 1413 + 180 * i, 1149, 130, 130);
	}
	SDL_Event e;
	bool quit = false;
	bool stop = false;
	int currStep = checkStep ? 0 : 1;
	int currHeadIndex = 0;
	int framesCount = 1;
	ListNode* currHead = nullptr;
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	while (!quit) {
		SDL_RenderClear(gRenderer);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
		if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
		t = 60 / (speed + 1);
		ImGuiControlStep(currStep, totalSteps, checkStep, speed, t, quit, stop);
		if (framesCount % t == 0 && !checkStep && !stop) currStep = min(currStep + 1, totalSteps);
		currHead = getNode(currHeadIndex);
		if (currStep == 0) {
			linesColor = -1;
			currHeadIndex = 0;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else {
			resetColor();
			if (currStep % 4 == 1) {
				linesColor = 0;
			}
			else if (currStep % 4 == 2) {
				if (currStep == totalSteps)
					linesColor = 6;
				else {
					linesColor = 1;
					currHeadIndex = currStep / 4;
				}
			}
			else if (currStep % 4 == 3) {
				if (getNode(currHeadIndex) && getNode(currHeadIndex)->val == val && currStep == totalSteps) {
					linesColor = 2;
					currHead->setColorNode(GREEN);
				}
				else {
					linesColor = 3;
					currHeadIndex = currStep / 4 + 1;
					if (currHead) currHead->setColorNode(RED);
				}
			}
			else linesColor = 4;
			visualize();
		}
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
			}
		}
		if (currHead)
			createText(generalFont, RED, "Head", currHead->centerX - currHead->radius + 5, currHead->centerY - currHead->radius - 25, 10 * 4, 20);
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ImGui::GetIO().WantCaptureMouse) break;
				if (e.button.x > 850 && e.button.x < 950 && e.button.y > 500 && e.button.y < 550) { quit = true; }
				else if (!checkStep && e.button.x > 900 && e.button.x < 950 && e.button.y > 400 && e.button.y < 450) speed = (speed + 1) % 4;
			case SDL_KEYDOWN:
				if (checkStep) {
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT: currStep = max(currStep - 1, 0); break;
					case SDLK_RIGHT: currStep = min(currStep + 1, totalSteps); break;
					}
				}
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
		framesCount++;
	}
	resetColor();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void LinkedListVisualizer::update(int idx, string newVal) {
	ListNode* clone = head;
	while (idx) {
		clone = clone->next;
		idx--;
	}
	clone->val = newVal;
}
void LinkedListVisualizer::deleteStep(int idx, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
	vector<string> code = {
		"ListNode* curr = head->next;",
		"ListNode* prev = head;",
		"while(index-- > 1){",
		"    curr = curr->next;",
		"    prev = prev->next;",
		"}",
		"prev->next = curr->next;",
		"delete curr;",
	};
	int linesColor = -1;
	int totalSteps = 2 + (idx - 1) * 3 + 2 + 1;
	SDL_Rect rect;
	SDL_Rect endButton = *createRect(&rect, 1413, 165, 100, 50);
	SDL_Rect endButtonHover = *createRect(&rect, 1683, 165, 100, 50);
	int speed = 0;
	int t = 60 / (speed + 1);
	SDL_Rect buttonPosArray[4][2];
	for (int i = 0; i < 4; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 1413 + 180 * i, 870, 130, 130);
		buttonPosArray[i][1] = *createRect(&rect, 1413 + 180 * i, 1149, 130, 130);
	}
	SDL_Event e;
	bool quit = false;
	bool stop = false;
	int currStep = checkStep ? 0 : 1;
	int currIndex = -1;
	int prevIndex = -1;
	int framesCount = 1;
	ListNode* toDelete = getNode(idx);
	ListNode* prev = getNode(idx - 1);
	ListNode* aft = getNode(idx + 1);
	ListNode* currPtr = nullptr;
	ListNode* prevPtr = nullptr;
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	while (!quit) {
		SDL_RenderClear(gRenderer);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
		if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
		t = 60 / (speed + 1);
		ImGuiControlStep(currStep, totalSteps, checkStep, speed, t, quit, stop);
		if (framesCount % t == 0 && !checkStep && !stop) currStep = min(currStep + 1, totalSteps);
		if (currIndex >= 0)	currPtr = getNode(currIndex);
		else currPtr = nullptr;
		if (prevIndex >= 0)	prevPtr = getNode(prevIndex);
		else prevPtr = nullptr;
		if (currStep == 0) {
			linesColor = -1;
			currIndex = -1;
			prevIndex = -1;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep == 1) {
			linesColor = 0;
			currIndex = 1;
			prevIndex = -1;
			resetColor();
			visualize();
		}
		else if (currStep == 2) {
			linesColor = 1;
			currIndex = 1;
			prevIndex = 0;
			resetColor();
			visualize();
		}
		else if (currStep < totalSteps - 1) {
			resetColor();
			reCalcCoordinate();
			resetArrow();
			prev->next = toDelete;
			if (currStep % 3 == 0) {
				linesColor = 2;
			}
			else if (currStep % 3 == 1) {
				getNode(currStep / 3 + 1)->setColorNode(RED);
				linesColor = 3;
				currIndex = currStep / 3 + 1;
			}
			else {
				getNode(currStep / 3)->setColorNode(GREEN);
				linesColor = 4;
				prevIndex = currStep / 3;
			}
			visualize();
		}
		else {
			if (currStep == totalSteps - 1) {
				if (!aft) { prev->next = nullptr; currPtr = toDelete; }
				if (aft)	prev->setArrowPos(prev->arrowSX, prev->arrowSY, aft->centerX - 35, aft->centerY);
				toDelete->setCenterPos(prev->centerX + 90, prev->centerY + 100);
				if (aft)	toDelete->setArrowPos(toDelete->centerX + 25, toDelete->centerY - 25, aft->centerX - 25, aft->centerY + 25);
				visualize();
				toDelete->displayNode();
				linesColor = 6;
			}
			else {
				linesColor = 7;
				ListNode* clone = head;
				while (clone) {
					if (clone != toDelete)	clone->displayNode();
					clone = clone->next;
				}
			}
		}
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
			}
		}
		if (currPtr && currStep != totalSteps)	createText(generalFont, RED, "Curr", currPtr->centerX - currPtr->radius + 5, currPtr->centerY - currPtr->radius - 25, 10 * 4, 20);
		if (prevPtr)	createText(generalFont, RED, "Prev", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ImGui::GetIO().WantCaptureMouse) break;
				if (e.button.x > 850 && e.button.x < 950 && e.button.y > 500 && e.button.y < 550) { quit = true; }
				else if (!checkStep && e.button.x > 900 && e.button.x < 950 && e.button.y > 400 && e.button.y < 450) speed = (speed + 1) % 4;
			case SDL_KEYDOWN:
				if (checkStep) {
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT: currStep = max(currStep - 1, 0); break;
					case SDLK_RIGHT: currStep = min(currStep + 1, totalSteps); break;
					}
				}
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
		framesCount++;
	}
	prev->next = toDelete->next;
	toDelete->next = nullptr;
	delete toDelete;
	listSize--;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void LinkedListVisualizer::deleteStepHead(bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	vector<string> code = {
		"ListNode* toDel = head;",
		"head = head->next;",
		"delete toDel;"
	};
	int linesColor = -1;
	int totalSteps = 3;
	SDL_Rect rect;
	SDL_Rect endButton = *createRect(&rect, 1413, 165, 100, 50);
	SDL_Rect endButtonHover = *createRect(&rect, 1683, 165, 100, 50);
	int speed = 0;
	int t = 60 / (speed + 1);
	SDL_Rect buttonPosArray[4][2];
	for (int i = 0; i < 4; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 1413 + 180 * i, 870, 130, 130);
		buttonPosArray[i][1] = *createRect(&rect, 1413 + 180 * i, 1149, 130, 130);
	}
	SDL_Event e;
	bool quit = false;
	bool stop = false;
	int currStep = checkStep ? 0 : 1;
	int currIndex = -1;
	int prevIndex = -1;
	int framesCount = 1;
	ListNode* toDelete = head;
	ListNode* aft = head->next;
	ListNode* currPtr = nullptr;
	ListNode* prevPtr = nullptr;
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	while (!quit) {
		SDL_RenderClear(gRenderer);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
		if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
		t = 60 / (speed + 1);
		ImGuiControlStep(currStep, totalSteps, checkStep, speed, t, quit, stop);
		if (framesCount % t == 0 && !checkStep && !stop) currStep = min(currStep + 1, totalSteps);
		if (currIndex >= 0)	currPtr = getNode(currIndex);
		else currPtr = nullptr;
		if (prevIndex >= 0)	prevPtr = getNode(prevIndex);
		else prevPtr = nullptr;
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			currIndex = -1;
			prevIndex = -1;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep == 1) {
			linesColor = 0;
			currIndex = 0;
			prevIndex = 0;
			resetColor();
			visualize();
			if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY + currPtr->radius + 5, 10 * 5, 20);
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 2) {
			head = toDelete;
			linesColor = 1;
			currIndex = 0;
			prevIndex = 1;
			resetColor();
			visualize();
			if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY - currPtr->radius - 25, 10 * 5, 20);
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		}
		else {
			linesColor = 2;
			currIndex = -1;
			prevIndex = 1;
			resetColor();
			ListNode* clone = head->next;
			while (clone) {
				clone->displayNode();
				clone = clone->next;
			}
			if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY - currPtr->radius - 25, 10 * 5, 20);
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		}
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
			}
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ImGui::GetIO().WantCaptureMouse) break;
				if (e.button.x > 850 && e.button.x < 950 && e.button.y > 500 && e.button.y < 550) { quit = true; }
				else if (!checkStep && e.button.x > 900 && e.button.x < 950 && e.button.y > 400 && e.button.y < 450) speed = (speed + 1) % 4;
			case SDL_KEYDOWN:
				if (checkStep) {
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT: currStep = max(currStep - 1, 0); break;
					case SDLK_RIGHT: currStep = min(currStep + 1, totalSteps); break;
					}
				}
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
		framesCount++;
	}
	delete toDelete;
	head = aft;
	listSize--;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void LinkedListVisualizer::updateStep(int idx, string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
	vector<string> code = { "while (index--){",
						 "    head = head->next;",
						 "}",
						 "head->val = updateValue;" };
	int linesColor = -1;
	int ans = search(val);
	int totalSteps = idx * 2 + 2;
	SDL_Rect rect;
	SDL_Rect endButton = *createRect(&rect, 1413, 165, 100, 50);
	SDL_Rect endButtonHover = *createRect(&rect, 1683, 165, 100, 50);
	int speed = 0;
	int t = 60 / (speed + 1);
	SDL_Rect buttonPosArray[4][2];
	for (int i = 0; i < 4; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 1413 + 180 * i, 870, 130, 130);
		buttonPosArray[i][1] = *createRect(&rect, 1413 + 180 * i, 1149, 130, 130);
	}
	SDL_Event e;
	bool quit = false;
	bool stop = false;
	int currStep = checkStep ? 0 : 1;
	int currHeadIndex = 0;
	int framesCount = 1;
	ListNode* currHead = nullptr;
	ListNode* updateNode = getNode(idx);
	string beforeUpdate = updateNode->val;
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	while (!quit) {
		SDL_RenderClear(gRenderer);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
		if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
		t = 60 / (speed + 1);
		ImGuiControlStep(currStep, totalSteps, checkStep, speed, t, quit, stop);
		if (framesCount % t == 0 && !checkStep && !stop) currStep = min(currStep + 1, totalSteps);
		currHead = getNode(currHeadIndex);
		if (currStep == 0) {
			linesColor = -1;
			currHeadIndex = 0;
			updateNode->val = beforeUpdate;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep < totalSteps) {
			resetColor();
			updateNode->val = beforeUpdate;
			if (currStep % 2 == 1) {
				linesColor = 0;
			}
			else {
				linesColor = 1;
				currHeadIndex = currStep / 2;
				currHead->setColorNode(RED);
			}
			visualize();
		}
		else {
			linesColor = 3;
			updateNode->val = val;
			currHead->setColorNode(GREEN);
			visualize();
		}
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
			}
		}
		if (currHead)
			createText(generalFont, RED, "Head", currHead->centerX - currHead->radius + 5, currHead->centerY - currHead->radius - 25, 10 * 4, 20);
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ImGui::GetIO().WantCaptureMouse) break;
				if (e.button.x > 850 && e.button.x < 950 && e.button.y > 500 && e.button.y < 550) { quit = true; }
				else if (!checkStep && e.button.x > 900 && e.button.x < 950 && e.button.y > 400 && e.button.y < 450) speed = (speed + 1) % 4;
			case SDL_KEYDOWN:
				if (checkStep) {
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT: currStep = max(currStep - 1, 0); break;
					case SDLK_RIGHT: currStep = min(currStep + 1, totalSteps); break;
					}
				}
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
		framesCount++;
	}
	updateNode->val = val;
	resetColor();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void LinkedListVisualizer::insertEmpty(string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	int linesColor = -1;
	int totalSteps = 1;
	SDL_Rect rect;
	SDL_Rect endButton = *createRect(&rect, 1413, 165, 100, 50);
	SDL_Rect endButtonHover = *createRect(&rect, 1683, 165, 100, 50);
	int speed = 0;
	int t = 60 / (speed + 1);
	SDL_Rect buttonPosArray[4][2];
	for (int i = 0; i < 4; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 1413 + 180 * i, 870, 130, 130);
		buttonPosArray[i][1] = *createRect(&rect, 1413 + 180 * i, 1149, 130, 130);
	}
	SDL_Event e;
	bool quit = false;
	bool stop = false;
	int currStep = checkStep ? 0 : 1;
	int framesCount = 1;
	ListNode* tmpHead = nullptr;
	vector<string> code = {
		"head = new ListNode(value,nullptr);"
	};
	ListNode* newNode = new ListNode(val, 100, 100, 25);
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	while (!quit) {
		SDL_RenderClear(gRenderer);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
		if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
		t = 60 / (speed + 1);
		ImGuiControlStep(currStep, totalSteps, checkStep, speed, t, quit, stop);
		if (framesCount % t == 0 && !checkStep && !stop) currStep = min(currStep + 1, totalSteps);
		else tmpHead = nullptr;
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else {
			linesColor = 0;
			resetColor();
			visualize();
			newNode->displayNode();
			createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY - newNode->radius - 25, 10 * 4, 20);
		}
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
			}
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ImGui::GetIO().WantCaptureMouse) break;
				if (e.button.x > 850 && e.button.x < 950 && e.button.y > 500 && e.button.y < 550) { quit = true; }
				else if (!checkStep && e.button.x > 900 && e.button.x < 950 && e.button.y > 400 && e.button.y < 450) speed = (speed + 1) % 4;
			case SDL_KEYDOWN:
				if (checkStep) {
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT: currStep = max(currStep - 1, 0); break;
					case SDLK_RIGHT: currStep = min(currStep + 1, totalSteps); break;
					}
				}
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
		framesCount++;
	}
	head = newNode;
	listSize++;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}