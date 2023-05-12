#include "doubly_linked_list.h"
DoublyLinkedListVisualizer::DoublyLinkedListVisualizer() {
	head = nullptr;
	listSize = 0;
	bgColor = theme.getBgColor();
}
DoublyLinkedListVisualizer::~DoublyLinkedListVisualizer() {
	freeMem();
}
void DoublyLinkedListVisualizer::freeMem() {
	DoublyListNode* current = head;
	while (listSize--) {
		DoublyListNode* next = current->next;
		delete current;
		current = next;
	}
	head = nullptr;
}

void DoublyLinkedListVisualizer::visualize() {
	DoublyListNode* clone = head;
	while (clone) {
		clone->displayNode();
		clone = clone->next;
	}
}
void DoublyLinkedListVisualizer::initialize(vector<string> list) {
	freeMem();
	this->listSize = list.size();
	if (list.size() == 0) return;
	head = new DoublyListNode(list[0], 100, 100, 25);
	auto clone = head;
	for (int i = 1; i < list.size(); i++) {
		clone->next = new DoublyListNode(list[i], 100 + 90 * i, 100, 25, nullptr, clone);
		clone = clone->next;
	}
}
void DoublyLinkedListVisualizer::reCalcCoordinate() {
	DoublyListNode* clone = head;
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
DoublyListNode* DoublyLinkedListVisualizer::getNode(int idx) {
	DoublyListNode* clone = head;
	while (idx--) {
		clone = clone->next;
	}
	return clone;
}
void DoublyLinkedListVisualizer::resetColor() {
	DoublyListNode* clone = head;
	while (clone) {
		clone->setColorNode(theme.getElemColor());
		clone->setColorArrow(theme.getArrowColor());
		clone->setColorText(theme.getTextColor());
		clone = clone->next;
	}
}
void DoublyLinkedListVisualizer::resetArrow() {
	DoublyListNode* clone = head;
	while (clone) {
		clone->setArrowNextNormal();
		clone->setArrowPrevNormal();
		clone = clone->next;
	}
}
int DoublyLinkedListVisualizer::search(string val) {
	int ans = 0;
	DoublyListNode* clone = head;
	while (clone) {
		if (clone->val == val)
			return ans;
		else
			clone = clone->next;
		ans++;
	}
	return -1;
}
void DoublyLinkedListVisualizer::searchStep(string val, bool checkStep) {
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
	DoublyListNode* currHead = nullptr;
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
void DoublyLinkedListVisualizer::updateStep(int idx, string val, bool checkStep) {
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
	DoublyListNode* currHead = nullptr;
	DoublyListNode* updateNode = getNode(idx);
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
void DoublyLinkedListVisualizer::insertStep(int idx, string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
	vector<string> code = {
		"while(index-- > 1){",
		"    head = head->next;",
		"}",
		"DLLNode* newNode = new DLLNode(value,head,head->next);",
		"if (head->next)    head->next->prev = newNode;",
		"head->next = newNode;"
	};
	int linesColor = -1;
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
	DoublyListNode* newNode;
	DoublyListNode* tmp = getNode(idx - 1);
	newNode = new DoublyListNode(val, tmp->centerX, tmp->centerY + 100, 25);
	DoublyListNode* oldNode = getNode(idx);
	DoublyListNode* currHead = nullptr;
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
		if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 300, 50, 50, nullptr);
		t = 60 / (speed + 1);
		ImGuiControlStep(currStep, totalSteps, checkStep, speed, t, quit, stop);
		if (framesCount % t == 0 && !checkStep && !stop) currStep = min(currStep + 1, totalSteps);
		currHead = getNode(currHeadIndex);
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			currHeadIndex = 0;
			tmp->next = oldNode;
			if (oldNode)oldNode->prev = tmp;
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
				newNode->next = oldNode;
				newNode->prev = tmp;
				newNode->setArrowPrevUp();
				newNode->setArrowNextUpperRight();
				if (oldNode) {
					oldNode->setArrowPrevNormal();
				}
				visualize();
				newNode->displayNode();
				linesColor = 3;
			}
			else if (currStep == totalSteps - 1) {
				tmp->next = oldNode;
				tmp->setArrowNextNormal();
				if (oldNode) {
					oldNode->prev = newNode;
					oldNode->setArrowPrevDownLeft();
				}
				visualize();
				newNode->displayNode();
				linesColor = 4;
			}
			else {
				tmp->next = newNode;
				tmp->setArrowNextDown();
				visualize();
				newNode->displayNode();
				linesColor = 5;
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
				else if (!checkStep && e.button.x > 900 && e.button.x < 950 && e.button.y > 300 && e.button.y < 350) speed = (speed + 1) % 4;
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
	newNode->next = oldNode;
	newNode->prev = tmp;
	if (oldNode) oldNode->prev = newNode;
	tmp->next = newNode;
	listSize++;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void DoublyLinkedListVisualizer::insertStepHead(string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	int linesColor = -1;
	int totalSteps = 4;
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
	DoublyListNode* tmpHead = nullptr;
	vector<string> code = {
		"DLLNode* newNode = new DLLNode(value,nullptr,nullptr);",
		"newNode->next = head;",
		"head->prev = newNode;",
		"head = newNode;"
	};
	DoublyListNode* newNode = new DoublyListNode(val, head ? head->centerX : 100, head ? head->centerY + 100 : 100, 25);
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
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			headIndex = -1;
			newNode->next = nullptr;
			newNode->prev = nullptr;
			head->prev = nullptr;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
			tmpHead->prev = nullptr;
			linesColor = 1;
			headIndex = 0;
			newNode->setArrowNextUp();
			resetColor();
			visualize();
			newNode->displayNode();
			if (tmpHead)	createText(generalFont, RED, "Head", tmpHead->centerX - tmpHead->radius + 5, tmpHead->centerY - tmpHead->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 3) {
			tmpHead->prev = newNode;
			tmpHead->setArrowPrevDown();
			newNode->setArrowNextUp();
			linesColor = 2;
			headIndex = 0;
			resetColor();
			visualize();
			newNode->displayNode();
			if (tmpHead)	createText(generalFont, RED, "Head", tmpHead->centerX - tmpHead->radius + 5, tmpHead->centerY - tmpHead->radius - 25, 10 * 4, 20);
		}
		else {
			linesColor = 3;
			headIndex = 0;
			newNode->setArrowNextUp();
			resetColor();
			visualize();
			newNode->displayNode();
			createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY + newNode->radius + 5, 10 * 4, 20);
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
	newNode->next = head;
	head->prev = newNode;
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
void DoublyLinkedListVisualizer::deleteStep(int idx, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
	vector<string> code = {
		"DLLNode* toDel = head;",
		"while (index--){",
		"    toDel = toDel->next;",
		"}",
		"/* Re-layout the list */",
		"if (toDel->prev)	toDel->prev->next = toDel->next;",
		"if (toDel->next)	toDel->next->prev = toDel->prev;",
		"delete toDel;"
	};
	int linesColor = -1;
	int totalSteps = 1 + (idx) * 2 + 4 + 1;
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
	DoublyListNode* toDelete = getNode(idx);
	DoublyListNode* prev = getNode(idx - 1);
	DoublyListNode* aft = getNode(idx + 1);
	DoublyListNode* currPtr = nullptr;
	DoublyListNode* prevPtr = nullptr;
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	while (!quit) {
		SDL_RenderClear(gRenderer);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 400, 100, 50, NULL);
		if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 300, 50, 50, nullptr);
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
			prev->next = toDelete;
			toDelete->next = aft;
			if (aft) aft->prev = toDelete;
			toDelete->prev = prev;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep == 1) {
			linesColor = 0;
			currIndex = 0;
			resetColor();
			visualize();
		}
		else if (currStep < totalSteps - 3) {
			resetColor();
			reCalcCoordinate();
			resetArrow();
			prev->next = toDelete;
			if (currStep % 2 == 0) {
				linesColor = 1;
			}
			else {
				currIndex = currStep / 2;
				getNode(currStep / 2)->setColorNode(RED);
				linesColor = 2;
			}
			visualize();
		}
		else {
			if (currStep == totalSteps - 3) {
				prev->setArrowNextDownRight();
				toDelete->setArrowPrevUpLeft();
				prev->next = toDelete;
				currPtr = toDelete;
				if (aft) {
					aft->setArrowPrevDownLeft();
				}
				toDelete->setCenterPos(prev->centerX + 90, prev->centerY + 100);
				if (aft) {
					toDelete->setArrowNextUpperRight();
				}
				if (prev) {
					toDelete->setArrowPrevUpLeft();
				}
				visualize();
				toDelete->displayNode();
				linesColor = 4;
			}
			else if (currStep == totalSteps - 2) {
				currPtr = toDelete;
				if (!aft) { prev->next = nullptr; }
				if (aft) {
					prev->setArrowNextX2();
					aft->setArrowPrevDownLeft();
				}
				toDelete->setCenterPos(prev->centerX + 90, prev->centerY + 100);
				if (aft) {
					toDelete->setArrowNextUpperRight();
				}
				if (prev) {
					toDelete->setArrowPrevUpLeft();
				}
				visualize();
				toDelete->displayNode();
				linesColor = 5;
			}
			else if (currStep == totalSteps - 1) {
				currPtr = toDelete;
				if (aft) aft->setArrowPrevX2();
				linesColor = 6;
				visualize();
				toDelete->displayNode();
			}
			else {
				linesColor = 7;
				DoublyListNode* clone = head;
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
		if (currPtr && currStep != totalSteps) {
			if (currStep < totalSteps - 3)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY - currPtr->radius - 25, 10 * 5, 20);
			else createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY + currPtr->radius + 5, 10 * 5, 20);
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
				if (e.button.x > 850 && e.button.x < 950 && e.button.y > 400 && e.button.y < 450) { quit = true; }
				else if (!checkStep && e.button.x > 900 && e.button.x < 950 && e.button.y > 300 && e.button.y < 350) speed = (speed + 1) % 4;
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
	prev->next = aft;
	if (aft) aft->prev = prev;
	toDelete->next = nullptr;
	toDelete->prev = nullptr;
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
void DoublyLinkedListVisualizer::deleteStepHead(bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	vector<string> code = {
		"DLLNode* toDel = head;",
		"head = head->next;",
		"if (head)	head->prev = nullptr;",
		"delete toDel;"
	};
	int linesColor = -1;
	int totalSteps = 4;
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
	DoublyListNode* toDelete = head;
	DoublyListNode* aft = head->next;
	DoublyListNode* currPtr = nullptr;
	DoublyListNode* prevPtr = nullptr;
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
			toDelete->next = aft;
			if (aft) aft->prev = toDelete;
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
			if (aft) aft->prev = head;
			linesColor = 1;
			currIndex = 0;
			prevIndex = 1;
			resetColor();
			visualize();
			if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY - currPtr->radius - 25, 10 * 5, 20);
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 3) {
			if (aft) aft->prev = nullptr;
			linesColor = 2;
			currIndex = 0;
			prevIndex = 1;
			resetColor();
			visualize();
			if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY - currPtr->radius - 25, 10 * 5, 20);
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		}
		else {
			linesColor = 3;
			currIndex = -1;
			prevIndex = 1;
			resetColor();
			DoublyListNode* clone = head->next;
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
	if (aft) aft->prev = nullptr;
	listSize--;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void DoublyLinkedListVisualizer::insertEmpty(string val, bool checkStep) {
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
	vector<string> code = {
		"head = new DLLNode(value,nullptr,nullptr);"
	};
	DoublyListNode* newNode = new DoublyListNode(val, 100, 100, 25);
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