#include "stack.h"
StackVisualizer::StackVisualizer() {
	head = nullptr;
	listSize = 0;
	bgColor = theme.getBgColor();
}
StackVisualizer::~StackVisualizer() {
	freeMem();
}
void StackVisualizer::freeMem() {
	ListNode* current = head;
	while (current) {
		ListNode* next = current->next;
		delete current;
		current = next;
	}
	head = nullptr;
	listSize = 0;
}
void StackVisualizer::visualize() {
	ListNode* clone = head;
	while (clone) {
		clone->displayNode();
		clone = clone->next;
	}
}
void StackVisualizer::initialize(vector<string> list) {
	freeMem();
	this->listSize = list.size();
	if (list.size() == 0) return;
	head = new ListNode(list[0], 100, 100, 25);
	auto clone = head;
	for (int i = 1; i < list.size(); i++) {
		clone->next = new ListNode(list[i], 100, 100 + 90 * i, 25);
		clone = clone->next;
	}
	resetArrow();
}
void StackVisualizer::reCalcCoordinate() {
	ListNode* clone = head;
	if (!clone) return;
	clone->centerX = 100;
	clone->centerY = 100;
	clone->radius = 25;
	int i = 1;
	while (true) {
		clone = clone->next;
		if (!clone) break;
		clone->centerX = 100;
		clone->centerY = 100 + 90 * i;
		i++;
	}
}
void StackVisualizer::resetColor() {
	ListNode* clone = head;
	while (clone) {
		clone->setColorNode(theme.getElemColor());
		clone->setColorArrow(theme.getArrowColor());
		clone->setColorText(theme.getTextColor());
		clone = clone->next;
	}
}
void StackVisualizer::resetArrow() {
	ListNode* clone = head;
	while (clone) {
		clone->setArrowPos(clone->centerX, clone->centerY + clone->radius, clone->centerX, clone->centerY + clone->radius + 30);
		clone = clone->next;
	}
}
ListNode* StackVisualizer::getNode(int idx) {
	ListNode* clone = head;
	while (idx--) {
		clone = clone->next;
	}
	return clone;
}
void StackVisualizer::popStep(bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
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
		highlightCode(generalFont, explain, code, linesColor, 200, 100, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			currIndex = -1;
			prevIndex = -1;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 140, 250, 840, 40);
			visualize();
		}
		else if (currStep == 1) {
			linesColor = 0;
			currIndex = 0;
			prevIndex = 0;
			resetColor();
			visualize();
			if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX + currPtr->radius, currPtr->centerY - 10, 10 * 5, 20);
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 2) {
			head = toDelete;
			linesColor = 1;
			currIndex = 0;
			prevIndex = 1;
			resetColor();
			visualize();
			if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX + currPtr->radius, currPtr->centerY - 10, 10 * 5, 20);
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX + prevPtr->radius + 5, prevPtr->centerY - 10, 10 * 4, 20);
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
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		}
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 200, 100 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 200, 100 + i * 50, code[i].size() * 16, 40);
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
void StackVisualizer::pushStep(string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
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
	int headIndex = -1;
	int framesCount = 1;
	ListNode* tmpHead = nullptr;
	vector<string> code = {
		"ListNode* newNode = new ListNode(value,nullptr);",
		"newNode->next = head;",
		"head = newNode;"
	};
	ListNode* newNode = new ListNode(val, head ? head->centerX + 100 : 100, head ? head->centerY : 100, 25);
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
		if (headIndex >= 0)	tmpHead = getNode(headIndex);
		else tmpHead = nullptr;
		highlightCode(generalFont, explain, code, linesColor, 200, 200, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			headIndex = -1;
			newNode->next = nullptr;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 140, 250, 840, 40);
			visualize();
		}
		else if (currStep == 1) {
			newNode->next = nullptr;
			linesColor = 0;
			headIndex = 0;
			resetColor();
			visualize();
			newNode->displayNode();
			if (tmpHead)	createText(generalFont, RED, "Head", tmpHead->centerX - tmpHead->radius + 5, tmpHead->centerY - tmpHead->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 2) {
			newNode->next = tmpHead;
			linesColor = 1;
			headIndex = 0;
			newNode->setArrowPos(newNode->centerX - 25, newNode->centerY, newNode->centerX - 65, newNode->centerY);
			resetColor();
			visualize();
			newNode->displayNode();
			if (tmpHead)	createText(generalFont, RED, "Head", tmpHead->centerX - tmpHead->radius + 5, tmpHead->centerY - tmpHead->radius - 25, 10 * 4, 20);
		}
		else {
			linesColor = 2;
			headIndex = 0;
			newNode->setArrowPos(newNode->centerX - 25, newNode->centerY, newNode->centerX - 65, newNode->centerY);
			resetColor();
			visualize();
			newNode->displayNode();
			createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY - newNode->radius - 25, 10 * 4, 20);
		}
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 200, 200 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 200, 200 + i * 50, code[i].size() * 16, 40);
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
	newNode->next = head;
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
void StackVisualizer::clearStep(bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	int linesColor = -1;
	int totalSteps = listSize * 2 + 1;
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
	vector<string> code = {
		"while(listSize--)",
		"    stack.pop();"
	};
	ListNode* currHead = nullptr;
	ListNode* clone = nullptr;
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
		highlightCode(generalFont, explain, code, linesColor, 200, 200, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			currHead = nullptr;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 140, 250, 840, 40);
			visualize();
		}
		else {
			if (currStep % 2 == 1) {
				linesColor = 0;
			}
			else {
				linesColor = 1;
			}
			currHead = getNode(currStep / 2);
			clone = currHead;
			while (clone) {
				clone->displayNode();
				clone = clone->next;
			}
			if (currHead)	createText(generalFont, RED, "Head", currHead->centerX - currHead->radius + 5, currHead->centerY - currHead->radius - 25, 10 * 4, 20);
		}
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 200, 200 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 200, 200 + i * 50, code[i].size() * 16, 40);
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
	freeMem();
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}