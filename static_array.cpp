#include "global.h"
#include "utils.h"
#include "array_element.h"
#include "static_array.h"
StaticArrayVisualizer::StaticArrayVisualizer() {
	size = 0;
	bgColor = theme.getBgColor();
}
void StaticArrayVisualizer::visualize() {
	for (int i = 0; i < size; i++) {
		arr[i].display();
	}
}
void StaticArrayVisualizer::visualize(ArrayElement* arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i].display();
	}
}
void StaticArrayVisualizer::initialize(vector<string> list) {
	this->size = min((int)list.size(), 30);
	if (list.size() == 0) return;
	for (int i = 0; i < size; i++) {
		arr[i] = ArrayElement(100 + i * 50, 100, 50, 50, theme.getElemColor(), theme.getBgElemColor(), theme.getTextColor(), theme.getTextSize(), list[i], to_string(i));
	}
}
void StaticArrayVisualizer::reCalcCoordinate() {
	for (int i = 0; i < size; i++) {
		arr[i].changeCoordinate(100 + i * 50, 100, 50, 50);
	}
}
void StaticArrayVisualizer::resetColor() {
	for (int i = 0; i < size; i++) {
		arr[i].changeColor(theme.getElemColor(), theme.getTextColor());
	}
}
void StaticArrayVisualizer::resetColor(ArrayElement* arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i].changeColor(theme.getElemColor(), theme.getTextColor());
	}
}
void StaticArrayVisualizer::setArrayInsert(ArrayElement* arr, ArrayElement* clone, int idx, string val, int lim) {
	for (int i = 0; i < size; i++) {
		clone[i] = arr[i];
	}
	clone[size].val = "0";
	for (int i = 0; i < lim; i++) {
		if (size - i == idx) {
			clone[idx].val = val;
			break;
		}
		clone[size - i].val = clone[size - i - 1].val;
	}
}
void StaticArrayVisualizer::insertStep(int idx, string val, bool checkStep) {
	if (size == 30) return;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	vector<string> code = {
		"n++;",
		"for(int i = n-1; i > indexInsert; i--){",
		"    arr[i] = arr[i-1];",
		"}",
		"arr[indexInsert] = valueInsert;"
	};
	int linesColor = -1;
	int totalSteps = (size - idx) * 2 + 3;
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
	ArrayElement clone[30];
	for (int i = 0; i < size + 1; i++) {
		clone[i] = arr[i];
	}
	clone[size] = ArrayElement(100 + size * 50, 100, 50, 50, YELLOW, theme.getBgElemColor(), YELLOW, theme.getTextSize(), "0", to_string(size));
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
			resetColor(clone, size + 1);
			createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep == 1) {
			linesColor = 0;
			resetColor(clone, size + 1);
			setArrayInsert(arr, clone, idx, val, currStep / 2);
			visualize(clone, size + 1);
		}
		else if (currStep < totalSteps) {
			resetColor(clone, size + 1);
			if (currStep % 2 == 0) {
				linesColor = 1;
				setArrayInsert(arr, clone, idx, val, (currStep - 1) / 2);
				visualize(clone, size + 1);
			}
			else {
				linesColor = 2;
				setArrayInsert(arr, clone, idx, val, (currStep - 1) / 2);
				clone[size + 1 - (currStep - 1) / 2].changeColor(GREEN);
				clone[size - (currStep - 1) / 2].changeColor(GREEN);
				visualize(clone, size + 1);
			}
		}
		else {
			resetColor(clone, size + 1);
			linesColor = 4;
			setArrayInsert(arr, clone, idx, val, (currStep - 1) / 2);
			clone[idx].changeColor(GREEN);
			visualize(clone, size + 1);
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
	setArrayInsert(arr, clone, idx, val, size - idx + 1);
	for (int i = 0; i < size + 1; i++) {
		arr[i] = clone[i];
	}
	size++;
	resetColor();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
int StaticArrayVisualizer::search(string val) {
	for (int i = 0; i < size; i++) {
		if (arr[i].val == val) {
			return i;
		}
	}
	return -1;
}
void StaticArrayVisualizer::searchStep(string val, bool checkStep) {
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
void StaticArrayVisualizer::update(int idx, string newVal) {
	arr[idx].val = newVal;
}
void StaticArrayVisualizer::setArrayDelete(ArrayElement* arr, ArrayElement* clone, int idx, int lim) {
	for (int i = 0; i < size; i++) {
		clone[i] = arr[i];
	}
	for (int i = 0; i < lim; i++) {
		clone[i + idx].val = clone[i + idx + 1].val;
	}
}
void StaticArrayVisualizer::deleteStep(int idx, bool checkStep) {
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	vector<string> code = {
		"for(int i = indexDelete; i < n-1; i++){",
		"    arr[i] = arr[i+1];",
		"}",
		"n--;"
	};
	int linesColor = -1;
	int totalSteps = (size - idx - 1) * 2 + 2;
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
	ArrayElement clone[30];
	for (int i = 0; i < size; i++) {
		clone[i] = arr[i];
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
			resetColor(clone, size);
			createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
			visualize();
		}
		else if (currStep != totalSteps) {
			resetColor(clone, size);
			if (currStep % 2 == 1) {
				linesColor = 0;
				setArrayDelete(arr, clone, idx, currStep / 2);
				visualize(clone, size);
			}
			else {
				linesColor = 1;
				setArrayDelete(arr, clone, idx, currStep / 2);
				clone[idx + currStep / 2 - 1].changeColor(GREEN);
				clone[idx + currStep / 2].changeColor(GREEN);
				visualize(clone, size);
			}
		}
		else {
			resetColor(clone, size);
			linesColor = 3;
			visualize(clone, size - 1);
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
	setArrayDelete(arr, clone, idx, size - idx - 1);
	for (int i = 0; i < size - 1; i++) {
		arr[i] = clone[i];
	}
	size--;
	resetColor();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(explain);
	TTF_CloseFont(generalFont);
	return;
}
void StaticArrayVisualizer::updateStep(int idx, string val, bool checkStep) {
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