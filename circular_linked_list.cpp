#include "circular_linked_list.h"
CircularLinkedListVisualizer::CircularLinkedListVisualizer() {
	tail = nullptr;
}
void CircularLinkedListVisualizer::visualize() {
	ListNode* clone = head;
	int currrSize = listSize;
	while (currrSize--) {
		clone->displayNode();
		clone = clone->next;
	}
	if (tail && head) {
		SDL_Color old;
		SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
		SDL_Color arrow = tail->colorArrow;
		SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
		SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
		SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, head->centerX - 75, head->centerY + 50);
		SDL_RenderDrawLine(gRenderer, head->centerX - 75, head->centerY + 50, head->centerX - 75, head->centerY);
		DrawArrow(gRenderer, head->centerX - 75, head->centerX - 35, head->centerY, head->centerY, 8, tail->colorArrow);
		SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
	}
}
void CircularLinkedListVisualizer::initialize(vector<string> list) {
	freeMem();
	this->listSize = list.size();
	if (list.size() == 0) return;
	head = new ListNode(list[0], 100, 100, 25);
	ListNode* clone = head;
	for (int i = 1; i < list.size(); i++) {
		clone->next = new ListNode(list[i], 100 + 90 * i, 100, 25);
		clone = clone->next;
	}
	tail = clone;
}
void CircularLinkedListVisualizer::insertStep(int idx, string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
	vector<string> code = {
		"while(index-- > 1){",
		"    head = head->next;",
		"}",
		"ListNode* newNode = new ListNode(value,nullptr);",
		"newNode->next = head->next;",
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
	ListNode* newNode;
	ListNode* tmp = getNode(idx - 1);
	newNode = new ListNode(val, tmp->centerX, tmp->centerY + 100, 25);
	ListNode* oldNode = getNode(idx);
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
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			currHeadIndex = 0;
			tmp->next = oldNode;
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
				newNode->next = nullptr;
				visualize();
				newNode->displayNode();
				linesColor = idx ? 3 : 0;
			}
			else if (currStep == totalSteps - 1) {
				newNode->next = oldNode;
				newNode->setArrowPos(newNode->centerX + 10, newNode->centerY - newNode->radius + 5, oldNode->centerX - 25, oldNode->centerY + 25);
				tmp->setArrowPos(tmp->centerX + tmp->radius, tmp->centerY, tmp->centerX + tmp->radius + 30, tmp->centerY);
				visualize();
				newNode->displayNode();
				linesColor = idx ? 4 : 1;
			}
			else {
				tmp->setArrowPos(tmp->centerX, tmp->centerY + tmp->radius, newNode->centerX, newNode->centerY - 35);
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
	tmp->next = newNode;
	newNode->next = oldNode;
	listSize++;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void CircularLinkedListVisualizer::insertStepHead(int idx, string val, bool checkStep) {
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
	ListNode* tmpHead = nullptr;
	vector<string> code = {
		"ListNode* newNode = new ListNode(value,nullptr);",
		"newNode->next = head;",
		"tail->next = newNode;",
		"head = newNode;"
	};
	ListNode* newNode = new ListNode(val, head->centerX, head->centerY + 100, 25);
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
			linesColor = 1;
			headIndex = 0;
			newNode->setArrowPos(newNode->centerX, newNode->centerY - newNode->radius, newNode->centerX, newNode->centerY - newNode->radius - 40);
			resetColor();
			visualize();
			newNode->displayNode();
			if (tmpHead)	createText(generalFont, RED, "Head", tmpHead->centerX - tmpHead->radius + 5, tmpHead->centerY - tmpHead->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 3) {
			linesColor = 2;
			headIndex = 0;
			newNode->setArrowPos(newNode->centerX, newNode->centerY - newNode->radius, newNode->centerX, newNode->centerY - newNode->radius - 40);
			resetColor();
			ListNode* clone = head;
			int currrSize = listSize;
			while (currrSize--) {
				clone->displayNode();
				clone = clone->next;
			}
			if (tail) {
				SDL_Color old;
				SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
				SDL_Color arrow = tail->colorArrow;
				SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
				SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 100);
				DrawArrow(gRenderer, tail->centerX, head->centerX + 35, tail->centerY + 100, head->centerY + 100, 8, tail->colorArrow);
				SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
			}
			newNode->displayNode();
			if (tmpHead)	createText(generalFont, RED, "Head", tmpHead->centerX - tmpHead->radius + 5, tmpHead->centerY - tmpHead->radius - 25, 10 * 4, 20);
		}
		else {
			linesColor = 3;
			headIndex = 0;
			newNode->setArrowPos(newNode->centerX, newNode->centerY - newNode->radius, newNode->centerX, newNode->centerY - newNode->radius - 40);
			resetColor();
			ListNode* clone = head;
			int currrSize = listSize;
			while (currrSize--) {
				clone->displayNode();
				clone = clone->next;
			}
			if (tail) {
				SDL_Color old;
				SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
				SDL_Color arrow = tail->colorArrow;
				SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
				SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 100);
				DrawArrow(gRenderer, tail->centerX, head->centerX + 35, tail->centerY + 100, head->centerY + 100, 8, tail->colorArrow);
				SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
			}
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
void CircularLinkedListVisualizer::insertStepTail(int idx, string val, bool checkStep) {
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
	int framesCount = 1;
	ListNode* tmpTail = nullptr;
	vector<string> code = {
		"ListNode* newNode = new ListNode(value,nullptr);",
		"newNode->next = head;",
		"tail->next = newNode;",
		"tail = tail->next;"
	};
	ListNode* newNode = new ListNode(val, head->centerX, head->centerY + 100, 25);
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
		if (currStep != 0) {
			tmpTail = tail;
		}
		else tmpTail = tail;
		highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
		if (currStep == 0) {
			linesColor = -1;
			newNode->next = nullptr;
			resetColor();
			reCalcCoordinate();
			resetArrow();
			if (checkStep)	createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep == 1) {
			newNode->next = nullptr;
			linesColor = 0;
			resetColor();
			visualize();
			newNode->displayNode();
			if (tmpTail)	createText(generalFont, RED, "Tail", tmpTail->centerX - tmpTail->radius + 5, tmpTail->centerY - tmpTail->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 2) {
			newNode->next = tmpTail;
			linesColor = 1;
			newNode->setArrowPos(newNode->centerX, newNode->centerY - newNode->radius, newNode->centerX, newNode->centerY - newNode->radius - 40);
			resetColor();
			visualize();
			newNode->displayNode();
			if (tmpTail)	createText(generalFont, RED, "Tail", tmpTail->centerX - tmpTail->radius + 5, tmpTail->centerY - tmpTail->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 3) {
			linesColor = 2;
			newNode->setArrowPos(newNode->centerX, newNode->centerY - newNode->radius, newNode->centerX, newNode->centerY - newNode->radius - 40);
			resetColor();
			ListNode* clone = head;
			int currrSize = listSize;
			while (currrSize--) {
				clone->displayNode();
				clone = clone->next;
			}
			if (tail) {
				SDL_Color old;
				SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
				SDL_Color arrow = tail->colorArrow;
				SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
				SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 100);
				DrawArrow(gRenderer, tail->centerX, head->centerX + 35, tail->centerY + 100, head->centerY + 100, 8, tail->colorArrow);
				SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
			}
			newNode->displayNode();
			if (tmpTail)	createText(generalFont, RED, "Tail", tmpTail->centerX - tmpTail->radius + 5, tmpTail->centerY - tmpTail->radius - 25, 10 * 4, 20);
		}
		else {
			linesColor = 3;
			newNode->setArrowPos(newNode->centerX, newNode->centerY - newNode->radius, newNode->centerX, newNode->centerY - newNode->radius - 40);
			resetColor();
			ListNode* clone = head;
			int currrSize = listSize;
			while (currrSize--) {
				clone->displayNode();
				clone = clone->next;
			}
			if (tail) {
				SDL_Color old;
				SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
				SDL_Color arrow = tail->colorArrow;
				SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
				SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 100);
				DrawArrow(gRenderer, tail->centerX, head->centerX + 35, tail->centerY + 100, head->centerY + 100, 8, tail->colorArrow);
				SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
			}
			newNode->displayNode();
			createText(generalFont, RED, "Tail", newNode->centerX - newNode->radius + 5, newNode->centerY + newNode->radius + 5, 10 * 4, 20);
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
	tail->next = newNode;
	tail = tail->next;
	newNode->next = nullptr;
	listSize++;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void CircularLinkedListVisualizer::deleteStep(int idx, bool checkStep) {
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
			prev->next = toDelete;
			toDelete->next = aft;
			if (!aft) tail = toDelete;
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
				if (!aft) {
					tail = toDelete;
					prev->setArrowPos(head->centerX - 75, head->centerY, head->centerX - 35, head->centerY);
					SDL_Color old;
					SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
					SDL_Color arrow = tail->colorArrow;
					SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
					SDL_RenderDrawLine(gRenderer, prev->centerX, prev->centerY + 25, prev->centerX, prev->centerY + 50);
					SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
					currPtr = toDelete;
				}
				if (aft)	prev->setArrowPos(prev->arrowSX, prev->arrowSY, aft->centerX - 35, aft->centerY);
				if (aft)	toDelete->setCenterPos(prev->centerX + 90, prev->centerY + 100);
				if (aft)	toDelete->setArrowPos(toDelete->centerX + 25, toDelete->centerY - 25, aft->centerX - 25, aft->centerY + 25);
				visualize();
				toDelete->displayNode();
				linesColor = 6;
			}
			else {
				if (!aft) tail = prev;
				linesColor = 7;
				ListNode* clone = head;
				while (clone) {
					if (clone != toDelete)	clone->displayNode();
					clone = clone->next;
				}
				if (tail) {
					SDL_Color old;
					SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
					SDL_Color arrow = tail->colorArrow;
					SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, head->centerX - 75, head->centerY + 50);
					SDL_RenderDrawLine(gRenderer, head->centerX - 75, head->centerY + 50, head->centerX - 75, head->centerY);
					DrawArrow(gRenderer, head->centerX - 75, head->centerX - 35, head->centerY, head->centerY, 8, tail->colorArrow);
					SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
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
	if (!aft) tail = prev;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void CircularLinkedListVisualizer::searchStep(string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	vector<string> code = { "while (listSize--){",
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
void CircularLinkedListVisualizer::deleteStepHead(bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	vector<string> code = {
		"ListNode* toDel = head;",
		"head = head->next;",
		"tail->next = head;",
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
			linesColor = 1;
			currIndex = 0;
			prevIndex = 1;
			resetColor();
			visualize();
			if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY - currPtr->radius - 25, 10 * 5, 20);
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 3) {
			currIndex = 0;
			prevIndex = 1;
			resetColor();
			linesColor = 2;
			ListNode* clone = head;
			int currrSize = listSize;
			while (currrSize--) {
				clone->displayNode();
				clone = clone->next;
			}
			if (tail && aft) {
				SDL_Color old;
				SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
				SDL_Color arrow = tail->colorArrow;
				SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
				SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
				if (tail == aft) {
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, aft->centerX - 55, aft->centerY + 50);
					SDL_RenderDrawLine(gRenderer, aft->centerX - 55, aft->centerY + 50, aft->centerX - 55, aft->centerY);
				}
				else {
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, aft->centerX, aft->centerY + 50);
					DrawArrow(gRenderer, aft->centerX, aft->centerX, aft->centerY + 50, aft->centerY + 35, 8, tail->colorArrow);
				}
				SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
			}
			else if (!aft) {
				SDL_Color old;
				SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
				SDL_Color arrow = tail->colorArrow;
				SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
				SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
				SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, tail->centerX - 75, tail->centerY + 50);
				SDL_RenderDrawLine(gRenderer, tail->centerX - 75, tail->centerY + 50, tail->centerX - 75, tail->centerY);
				DrawArrow(gRenderer, tail->centerX - 75, tail->centerX - 35, tail->centerY, tail->centerY, 8, tail->colorArrow);
				SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
			}
			if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY - currPtr->radius - 25, 10 * 5, 20);
			if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
		}
		else {
			linesColor = 3;
			currIndex = -1;
			prevIndex = 1;
			resetColor();
			ListNode* clone = head->next;
			while (clone) {
				clone->displayNode();
				clone = clone->next;
			}
			if (tail && aft) {
				SDL_Color old;
				SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
				SDL_Color arrow = tail->colorArrow;
				SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
				SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
				if (tail == aft) {
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, aft->centerX - 55, aft->centerY + 50);
					SDL_RenderDrawLine(gRenderer, aft->centerX - 55, aft->centerY + 50, aft->centerX - 55, aft->centerY);
					DrawArrow(gRenderer, aft->centerX - 55, aft->centerX - 35, aft->centerY, aft->centerY, 8, tail->colorArrow);
				}
				else {
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, aft->centerX, aft->centerY + 50);
					DrawArrow(gRenderer, aft->centerX, aft->centerX, aft->centerY + 50, aft->centerY + 35, 8, tail->colorArrow);
				}
				SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
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
void CircularLinkedListVisualizer::insertEmpty(string val, bool checkStep) {
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
	int framesCount = 1;
	ListNode* tmpHead = nullptr;
	vector<string> code = {
		"head = new ListNode(value,nullptr);",
		"tail = head;",
		"tail->next = head;"
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
		else if (currStep == 1) {
			linesColor = 0;
			resetColor();
			visualize();
			newNode->displayNode();
			createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY - newNode->radius - 25, 10 * 4, 20);
		}
		else if (currStep == 2) {
			linesColor = 1;
			resetColor();
			visualize();
			newNode->displayNode();
			createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY - newNode->radius - 25, 10 * 4, 20);
			createText(generalFont, RED, "Tail", newNode->centerX - newNode->radius + 5, newNode->centerY + newNode->radius + 5, 10 * 4, 20);
		}
		else {
			linesColor = 2;
			resetColor();
			visualize();
			newNode->displayNode();
			createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY - newNode->radius - 25, 10 * 4, 20);
			createText(generalFont, RED, "Tail", newNode->centerX - newNode->radius + 5, newNode->centerY + newNode->radius + 5, 10 * 4, 20);
			SDL_Color old;
			SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
			SDL_Color arrow = tail->colorArrow;
			SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
			SDL_RenderDrawLine(gRenderer, newNode->centerX, newNode->centerY + 25, newNode->centerX, newNode->centerY + 50);
			SDL_RenderDrawLine(gRenderer, newNode->centerX, newNode->centerY + 50, newNode->centerX - 75, newNode->centerY + 50);
			SDL_RenderDrawLine(gRenderer, newNode->centerX - 75, newNode->centerY + 50, newNode->centerX - 75, newNode->centerY);
			DrawArrow(gRenderer, newNode->centerX - 75, newNode->centerX - 35, newNode->centerY, newNode->centerY, 8, tail->colorArrow);
			SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
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
	tail = newNode;
	listSize++;
	reCalcCoordinate();
	resetColor();
	resetArrow();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}