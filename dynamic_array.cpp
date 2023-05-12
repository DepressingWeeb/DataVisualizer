#include "dynamic_array.h"

DynamicArrayVisualizer::DynamicArrayVisualizer() {
	arr = nullptr;
	size = 0;
	bgColor = theme.getBgColor();
}
DynamicArrayVisualizer::~DynamicArrayVisualizer() {
	freeMem();
}
void DynamicArrayVisualizer::freeMem() {
	delete[] arr;
	size = 0;
}
void DynamicArrayVisualizer::visualize() {
	for (int i = 0; i < size; i++) {
		arr[i].display();
	}
}
void DynamicArrayVisualizer::visualize(ArrayElement* arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i].display();
	}
}
void DynamicArrayVisualizer::initialize(vector<string> list) {
	freeMem();
	this->size = list.size();
	if (list.size() == 0) return;
	arr = new ArrayElement[size];
	for (int i = 0; i < size; i++) {
		arr[i] = ArrayElement(100 + i * 50, 100, 50, 50, theme.getElemColor(), theme.getBgElemColor(), theme.getTextColor(), theme.getTextSize(), list[i], to_string(i));
	}
}
void DynamicArrayVisualizer::reCalcCoordinate() {
	for (int i = 0; i < size; i++) {
		arr[i].changeCoordinate(100 + i * 50, 100, 50, 50);
	}
}
void DynamicArrayVisualizer::resetColor() {
	for (int i = 0; i < size; i++) {
		arr[i].changeColor(theme.getElemColor(), theme.getTextColor());
	}
}
void DynamicArrayVisualizer::resetColor(ArrayElement* arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i].changeColor(theme.getElemColor(), theme.getTextColor());
	}
}
void DynamicArrayVisualizer::setArrayInsert(ArrayElement* newArr, ArrayElement* oldArr, int idx, string val, int lim) {
	for (int i = 0; i < lim; i++) {
		if (i == idx) {
			newArr[i].val = val;
		}
		else if (i < idx) {
			newArr[i].val = oldArr[i].val;
		}
		else {
			newArr[i].val = oldArr[i - 1].val;
		}
	}
	for (int i = lim; i < size + 1; i++) {
		newArr[i] = ArrayElement(100 + i * 50, 100, 50, 50, theme.getElemColor(), theme.getBgElemColor(), theme.getTextColor(), theme.getTextSize(), "0", to_string(i));
	}
	return;
}
void DynamicArrayVisualizer::insertStep(int idx, string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	vector<string> code = {
		"int* newArr = new int[n+1];",
		"for (int i = 0; i < indexInsert; i++)",
		"    newArr[i] = oldArr[i];",
		"arr[indexInsert] = valueInsert;",
		"for (int i = indexInsert + 1; i < n + 1; i++)",
		"    newArr[i] = oldArr[i - 1];",
		"delete[] oldArr;"
	};
	int linesColor = -1;
	int totalSteps = (size) * 2 + 5;
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
	ArrayElement* newArr = new ArrayElement[size + 1];
	for (int i = 0; i < size + 1; i++) {
		newArr[i] = ArrayElement(100 + i * 50, 100, 50, 50, theme.getElemColor(), theme.getBgElemColor(), theme.getTextColor(), theme.getTextSize(), "0", to_string(i));
	}
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
		if (currStep == 0) {
			linesColor = -1;
			resetColor(newArr, size + 1);
			if (checkStep)createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep == 1) {
			setArrayInsert(newArr, arr, idx, val, 0);
			linesColor = 0;
			resetColor(newArr, size + 1);
		}
		else if (currStep <= 1 + idx * 2 + 1) {
			resetColor(newArr, size + 1);
			if (currStep % 2 == 0) {
				linesColor = 1;
				setArrayInsert(newArr, arr, idx, val, (currStep - 1) / 2);
			}
			else {
				linesColor = 2;
				setArrayInsert(newArr, arr, idx, val, (currStep - 1) / 2);
			}
		}
		else if (currStep == 2 + idx * 2 + 1) {
			resetColor(newArr, size + 1);
			linesColor = 3;
			setArrayInsert(newArr, arr, idx, val, (currStep - 1) / 2);
			newArr[idx].changeColor(GREEN);
		}
		else if (currStep != totalSteps) {
			resetColor(newArr, size + 1);
			if (currStep % 2 == 0) {
				linesColor = 4;
				setArrayInsert(newArr, arr, idx, val, (currStep - 1) / 2);
			}
			else {
				linesColor = 5;
				setArrayInsert(newArr, arr, idx, val, (currStep - 1) / 2);
			}
		}
		else {
			resetColor(newArr, size + 1);
			linesColor = 6;
			setArrayInsert(newArr, arr, idx, val, size + 1);
		}
		if (currStep)	visualize(newArr, size + 1);
		highlightCode(generalFont, explain, code, linesColor, 100, 200, YELLOW, checkStep);
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
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
	setArrayInsert(newArr, arr, idx, val, size + 1);
	delete[] arr;
	arr = new ArrayElement[size + 1];
	for (int i = 0; i < size + 1; i++) {
		arr[i] = newArr[i];
	}
	delete[] newArr;
	size++;
	resetColor();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
int DynamicArrayVisualizer::search(string val) {
	for (int i = 0; i < size; i++) {
		if (arr[i].val == val) {
			return i;
		}
	}
	return -1;
}
void DynamicArrayVisualizer::searchStep(string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	vector<string> code = { "for (int i = 0; i < n; i++)",
						 "    if (arr[i] == target)",
						 "        return i;",
						 "return NOT_FOUND;" };
	int linesColor = -1;
	int ans = search(val);
	int totalSteps = ans == -1 ? size * 2 + 2 : ans * 2 + 3;
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
		if (currStep == 0) {
			linesColor = -1;
			resetColor();
			if (checkStep)createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep != totalSteps) {
			resetColor();
			if (currStep % 2 == 1) {
				linesColor = 0;
			}
			else {
				linesColor = 1;
				if (arr[(currStep - 2) / 2].val == val)
					arr[(currStep - 2) / 2].changeColor(GREEN);
				else
					arr[(currStep - 2) / 2].changeColor(RED);
			}
			visualize();
		}
		else {
			if (ans == -1)
				linesColor = 3;
			else
				linesColor = 2;
			visualize();
		}
		highlightCode(generalFont, explain, code, linesColor, 100, 200, YELLOW, checkStep);
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
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
	resetColor();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void DynamicArrayVisualizer::update(int idx, string newVal) {
	arr[idx].val = newVal;
}
void DynamicArrayVisualizer::setArrayDelete(ArrayElement* newArr, ArrayElement* oldArr, int idx, int lim) {
	for (int i = 0; i < lim; i++) {
		if (i < idx) {
			newArr[i].val = oldArr[i].val;
		}
		else {
			newArr[i].val = oldArr[i + 1].val;
		}
	}
	for (int i = lim; i < size - 1; i++) {
		newArr[i] = ArrayElement(100 + i * 50, 100, 50, 50, theme.getElemColor(), theme.getBgElemColor(), theme.getTextColor(), theme.getTextSize(), "0", to_string(i));
	}
	return;
}
void DynamicArrayVisualizer::deleteStep(int idx, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	vector<string> code = {
		"int* newArr = new int[n-1];",
		"for (int i = 0; i < indexDelete; i++)",
		"    newArr[i] = oldArr[i];",
		"for (int i = indexDelete + 1; i < n; i++)",
		"    newArr[i - 1] = oldArr[i];",
		"delete[] oldArr;"
	};
	int linesColor = -1;
	int totalSteps = (size - 1) * 2 + 4;
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
	ArrayElement* newArr = new ArrayElement[size - 1];
	for (int i = 0; i < size - 1; i++) {
		newArr[i] = ArrayElement(100 + i * 50, 100, 50, 50, theme.getElemColor(), theme.getBgElemColor(), theme.getTextColor(), theme.getTextSize(), "0", to_string(i));
	}
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
		if (currStep == 0) {
			linesColor = -1;
			resetColor(newArr, size - 1);
			if (checkStep)createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep == 1) {
			setArrayDelete(newArr, arr, idx, 0);
			linesColor = 0;
			resetColor(newArr, size - 1);
		}
		else if (currStep <= 1 + idx * 2 + 1) {
			resetColor(newArr, size - 1);
			if (currStep % 2 == 0) {
				linesColor = 1;
				setArrayDelete(newArr, arr, idx, (currStep - 1) / 2);
			}
			else {
				linesColor = 2;
				setArrayDelete(newArr, arr, idx, (currStep - 1) / 2);
			}
		}
		else if (currStep != totalSteps) {
			resetColor(newArr, size - 1);
			if (currStep % 2 == 1) {
				linesColor = 3;
				setArrayDelete(newArr, arr, idx, (currStep - 2) / 2);
			}
			else {
				linesColor = 4;
				setArrayDelete(newArr, arr, idx, (currStep - 2) / 2);
			}
		}
		else {
			resetColor(newArr, size - 1);
			linesColor = 5;
			setArrayDelete(newArr, arr, idx, size - 1);
		}
		if (currStep)	visualize(newArr, size - 1);
		highlightCode(generalFont, explain, code, linesColor, 100, 200, YELLOW, checkStep);
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
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
	setArrayDelete(newArr, arr, idx, size - 1);
	delete[] arr;
	arr = new ArrayElement[size - 1];
	for (int i = 0; i < size - 1; i++) {
		arr[i] = newArr[i];
	}
	delete[] newArr;
	size--;
	resetColor();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void DynamicArrayVisualizer::updateStep(int idx, string val, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	vector<string> code = { "arr[indexUpdate] = valueUpdate;" };
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
	string oldVal = arr[idx].val;
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
		if (currStep == 0) {
			linesColor = -1;
			resetColor();
			arr[idx].val = oldVal;
			if (checkStep)createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else {
			resetColor();
			linesColor = 0;
			arr[idx].changeColor(GREEN);
			arr[idx].val = val;
			visualize();
		}
		highlightCode(generalFont, explain, code, linesColor, 100, 200, YELLOW, checkStep);
		if (linesColor != -1) {
			for (int i = 0; i < code.size(); i++) {
				if (i == linesColor)
					createText(generalFont, RED, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
				else
					createText(generalFont, BLACK, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
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
	arr[idx].val = val;
	resetColor();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}