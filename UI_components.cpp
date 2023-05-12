#include "UI_components.h"
vector<string> initializeFormInput() {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	SDL_Texture* form = loadImgTexture("resources/Form/InitForm.png");
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Rect buttonPosArray[15][2];
	vector<string> ans;
	bool checkRandom = false;
	for (int i = 0; i < 2; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 1413, 1317 + 144 * i, 150, 75);
		buttonPosArray[i][1] = *createRect(&rect, 1649, 1317 + 144 * i, 150, 75);
	}
	buttonPosArray[2][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[2][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	int activeField = 0;
	bool alert1Frame = false;
	bool alert2Frame = false;
	bool alert3Frame = false;
	const string alert1 = "Wrong input format";
	const string alert2 = "Value must be in the range [0,100)";
	const string alert3 = "File does not exist";
	string text1 = "";
	string text2 = "";
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_StartTextInput();
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, form, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 370, 385, 150, 75, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 370, 500, 150, 75, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 0, 0, 50, 50, NULL);
		createText(generalFont, { 0 , 0 , 0 }, text1, 170, 184, text1.size() * 20, 40);
		createText(generalFont, { 0 , 0 , 0 }, text2, 170, 300, text2.size() * 20, 40);
		if (SDL_GetTicks64() % 1000 >= 500) {
			switch (activeField) {
			case 1:
				SDL_RenderDrawLine(gRenderer, 170 + text1.size() * 20, 184, 170 + text1.size() * 20, 219);
				break;
			case 2:
				SDL_RenderDrawLine(gRenderer, 170 + text2.size() * 20, 300, 170 + text2.size() * 20, 334);
				break;
			}
		}
		if (alert1Frame) {
			createText(generalFont, RED, alert1, 420, 255, alert1.size() * 12, 25);
		}
		else if (alert2Frame) {
			createText(generalFont, RED, alert2, 420, 255, alert2.size() * 12, 25);
		}
		else if (alert3Frame) {
			createText(generalFont, RED, alert3, 420, 140, alert3.size() * 12, 25);
		}
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_TEXTINPUT:
				if (activeField == 1 && text1.size() < 30) text1 += e.text.text;
				else if (activeField == 2 && text2.size() < 30) text2 += e.text.text;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) {
					quit = true;
					ans.push_back("Keep");
					SDL_DestroyTexture(buttonSpriteSheet);
					SDL_DestroyTexture(form);
					TTF_CloseFont(generalFont);
					return ans;
				}
				if (e.button.x > 156 && e.button.x < 763 && e.button.y > 176 && e.button.y < 233) activeField = 1;
				else if (e.button.x > 156 && e.button.x < 763 && e.button.y > 292 && e.button.y < 342) activeField = 2;
				else activeField = 0;
				if (e.button.x > 370 && e.button.x < 520 && e.button.y > 385 && e.button.y < 460) {
					if (alertInitialize(alert1Frame, alert2Frame, alert3Frame, trim(text1), trim(text2)))
						quit = true;
				}
				if (e.button.x > 370 && e.button.x < 520 && e.button.y > 500 && e.button.y < 575) {
					if (alertInitialize(alert1Frame, alert2Frame, alert3Frame, trim(text1), trim(text2))) {
						quit = true;
						checkRandom = true;
					}
				}
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_BACKSPACE) {
					if (activeField == 1 && text1.size() != 0) text1.pop_back();
					else if (activeField == 2 && text2.size() != 0) text2.pop_back();
				}
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_StopTextInput();
	text1 = trim(text1);
	text2 = trim(text2);
	if (!checkRandom) {
		string temp;
		ifstream in(text1);
		if (text1 != "" && in.good()) {
			while (in >> temp) {
				if (isNumber(temp) && temp.size() < 3)	ans.push_back(temp);
			}
		}
		else if (text2 != "") {
			stringstream ss(text2);
			while (!ss.eof()) {
				ss >> temp;
				ans.push_back(temp);
			}
		}
		else {
			return ans;
		}
		in.close();
	}
	else {
		srand(time(0));
		for (int i = 0; i < rand() % 4 + 1; i++) {
			ans.push_back(to_string(rand() % 99 + 1));
		}
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(form);
	TTF_CloseFont(generalFont);
	return ans;
}
tuple <int, string, bool, bool> indexValueFormInput(string formName, int indexLimit) {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	SDL_Texture* form = loadImgTexture("resources/Form/" + formName);
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Rect buttonPosArray[3][2];
	vector<string> ans;
	bool checkStep = false;
	for (int i = 0; i < 2; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 1317 + 144 * i, 200, 66);
		buttonPosArray[i][1] = *createRect(&rect, 559, 1317 + 144 * i, 200, 66);
	}
	buttonPosArray[2][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[2][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	int activeField = 0;
	bool alert1Frame = false;
	bool alert2Frame = false;
	bool alert3Frame = false;
	string text1 = "";
	string text2 = "";
	const string alert1 = "Wrong input format";
	const string alert2 = "Index must be in the range [0," + to_string(indexLimit) + ")";
	const string alert3 = "Value must be in the range [0,100)";
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_StartTextInput();
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, form, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 370, 385, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 370, 500, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 0, 0, 50, 50, NULL);
		createText(generalFont, { 0 , 0 , 0 }, text1, 170, 184, text1.size() * 20, 40);
		createText(generalFont, { 0 , 0 , 0 }, text2, 170, 300, text2.size() * 20, 40);
		if (SDL_GetTicks64() % 1000 >= 500) {
			switch (activeField) {
			case 1:
				SDL_RenderDrawLine(gRenderer, 170 + text1.size() * 20, 184, 170 + text1.size() * 20, 219);
				break;
			case 2:
				SDL_RenderDrawLine(gRenderer, 170 + text2.size() * 20, 300, 170 + text2.size() * 20, 334);
				break;
			}
		}
		if (alert1Frame) {
			createText(generalFont, RED, alert1, 170, 353, alert1.size() * 12, 25);
		}
		else if (alert2Frame) {
			createText(generalFont, RED, alert2, 170, 353, alert2.size() * 12, 25);
		}
		else if (alert3Frame) {
			createText(generalFont, RED, alert3, 170, 353, alert3.size() * 12, 25);
		}
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_TEXTINPUT:
				if (activeField == 1 && text1.size() < 30) text1 += e.text.text;
				else if (activeField == 2 && text2.size() < 30) text2 += e.text.text;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) { quit = true; text1 = ""; }
				if (e.button.x > 156 && e.button.x < 763 && e.button.y > 176 && e.button.y < 233) activeField = 1;
				else if (e.button.x > 156 && e.button.x < 763 && e.button.y > 292 && e.button.y < 342) activeField = 2;
				else activeField = 0;
				if (e.button.x > 370 && e.button.x < 570 && e.button.y > 385 && e.button.y < 451) {
					string tmp = trim(text1);
					string tmp1 = trim(text2);
					if (!isNumber(tmp)) { alert1Frame = true; alert2Frame = false; alert3Frame = false; }
					else if (tmp.size() >= 3 || stoi(tmp) >= indexLimit) { alert2Frame = true; alert1Frame = false; alert3Frame = false; }
					else if (!isNumber(tmp1)) { alert1Frame = true; alert2Frame = false; alert3Frame = false; }
					else if (tmp1.size() >= 3) { alert3Frame = true; alert1Frame = false; alert2Frame = false; }
					else {
						quit = true;
						checkStep = true;
					}
				}
				if (e.button.x > 370 && e.button.x < 570 && e.button.y > 500 && e.button.y < 566) {
					string tmp = trim(text1);
					string tmp1 = trim(text2);
					if (!isNumber(tmp)) { alert1Frame = true; alert2Frame = false; alert3Frame = false; }
					else if (tmp.size() >= 3 || stoi(tmp) >= indexLimit) { alert2Frame = true; alert1Frame = false; alert3Frame = false; }
					else if (!isNumber(tmp1)) { alert1Frame = true; alert2Frame = false; alert3Frame = false; }
					else if (tmp1.size() >= 3) { alert3Frame = true; alert1Frame = false; alert2Frame = false; }
					else {
						quit = true;
					}
				}
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_BACKSPACE) {
					if (activeField == 1 && text1.size() != 0) text1.pop_back();
					else if (activeField == 2 && text2.size() != 0) text2.pop_back();
				}
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_StopTextInput();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(form);
	TTF_CloseFont(generalFont);
	if (text1 != "" && text2 != "") {
		return make_tuple(stoi(trim(text1)), trim(text2), checkStep, true);
	}
	else return make_tuple(0, "-1", false, false);
}
tuple <int, bool, bool> indexFormInput(string formName, int indexLimit) {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 50);
	SDL_Texture* form = loadImgTexture("resources/Form/" + formName);
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* wrongFormat = loadImgTexture("resources/Prompt/Alert.png");
	SDL_Texture* wrongIndex = loadImgTexture("resources/Prompt/Alert1.png");
	SDL_Rect buttonPosArray[3][2];
	vector<string> ans;
	bool checkStep = false;
	for (int i = 0; i < 2; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 1317 + 144 * i, 200, 66);
		buttonPosArray[i][1] = *createRect(&rect, 559, 1317 + 144 * i, 200, 66);
	}
	buttonPosArray[2][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[2][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	int activeField = 0;
	bool alert1Frame = false;
	bool alert2Frame = false;
	string text1 = "";
	const string alert1 = "Wrong input format";
	const string alert2 = "Index must be in the range [0," + to_string(indexLimit) + ")";
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_StartTextInput();
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, form, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 370, 375, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 370, 500, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 0, 0, 50, 50, NULL);
		createText(generalFont, { 0 , 0 , 0 }, text1, 183, 246, text1.size() * 20, 40);
		if (SDL_GetTicks64() % 1000 >= 500) {
			switch (activeField) {
			case 1:
				SDL_RenderDrawLine(gRenderer, 183 + text1.size() * 20, 246, 183 + text1.size() * 20, 283);
				break;
			}
		}
		if (alert1Frame) {
			createText(generalFont, RED, alert1, 190, 300, alert1.size() * 12, 25);
		}
		else if (alert2Frame) {
			createText(generalFont, RED, alert2, 190, 300, alert2.size() * 12, 25);
		}
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_TEXTINPUT:
				if (activeField == 1 && text1.size() < 30) text1 += e.text.text;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) { quit = true; text1 = ""; }
				if (e.button.x > 169 && e.button.x < 794 && e.button.y > 228 && e.button.y < 299) activeField = 1;
				else activeField = 0;
				if (e.button.x > 370 && e.button.x < 570 && e.button.y > 375 && e.button.y < 441) {
					string tmp = trim(text1);
					if (!isNumber(tmp)) { alert1Frame = true; alert2Frame = false; }
					else if (tmp.size() >= 3 || stoi(tmp) >= indexLimit) { alert2Frame = true; alert1Frame = false; }
					else {
						quit = true;
						checkStep = true;
					}
				}
				if (e.button.x > 370 && e.button.x < 570 && e.button.y > 500 && e.button.y < 566) {
					string tmp = trim(text1);
					if (!isNumber(tmp)) { alert1Frame = true; alert2Frame = false; }
					else if (tmp.size() >= 3 || stoi(tmp) >= indexLimit) { alert2Frame = true; alert1Frame = false; }
					else {
						quit = true;
					}
				}
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_BACKSPACE) {
					if (activeField == 1 && text1.size() != 0) text1.pop_back();
				}
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_StopTextInput();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(form);
	SDL_DestroyTexture(wrongFormat);
	SDL_DestroyTexture(wrongIndex);
	TTF_CloseFont(generalFont);
	if (text1 != "") {
		int idx = stoi(trim(text1));
		return make_tuple(idx, checkStep, true);
	}
	else return make_tuple(0, false, false);
}
tuple <string, bool, bool> valueFormInput(string formName) {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	SDL_Texture* form = loadImgTexture("resources/Form/" + formName);
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Rect buttonPosArray[3][2];
	vector<string> ans;
	bool checkStep = false;
	for (int i = 0; i < 2; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 1317 + 144 * i, 200, 66);
		buttonPosArray[i][1] = *createRect(&rect, 559, 1317 + 144 * i, 200, 66);
	}
	buttonPosArray[2][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[2][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	int activeField = 0;
	string text1 = "";
	bool alert1Frame = false;
	bool alert2Frame = false;
	const string alert1 = "Wrong input format";
	const string alert2 = "Value must be in the range [0,100)";
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_StartTextInput();
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, form, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 370, 375, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 370, 500, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 0, 0, 50, 50, NULL);
		createText(generalFont, { 0 , 0 , 0 }, text1, 183, 246, text1.size() * 20, 40);
		if (SDL_GetTicks64() % 1000 >= 500) {
			switch (activeField) {
			case 1:
				SDL_RenderDrawLine(gRenderer, 183 + text1.size() * 20, 246, 183 + text1.size() * 20, 283);
				break;
			}
		}
		if (alert1Frame) {
			createText(generalFont, RED, alert1, 190, 300, alert1.size() * 12, 25);
		}
		else if (alert2Frame) {
			createText(generalFont, RED, alert2, 190, 300, alert2.size() * 12, 25);
		}
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_TEXTINPUT:
				if (activeField == 1 && text1.size() < 30) text1 += e.text.text;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) { quit = true; text1 = ""; }
				if (e.button.x > 169 && e.button.x < 794 && e.button.y > 228 && e.button.y < 299) activeField = 1;
				else activeField = 0;
				if (e.button.x > 370 && e.button.x < 570 && e.button.y > 375 && e.button.y < 441) {
					string tmp = trim(text1);
					if (!isNumber(tmp)) { alert1Frame = true; alert2Frame = false; }
					else if (tmp.size() >= 3) { alert2Frame = true; alert1Frame = false; }
					else {
						quit = true;
						checkStep = true;
					}
				}
				if (e.button.x > 370 && e.button.x < 570 && e.button.y > 500 && e.button.y < 566) {
					string tmp = trim(text1);
					if (!isNumber(tmp)) { alert1Frame = true; alert2Frame = false; }
					else if (tmp.size() >= 3) { alert2Frame = true; alert1Frame = false; }
					else {
						quit = true;
					}
				}
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_BACKSPACE) {
					if (activeField == 1 && text1.size() != 0) text1.pop_back();
				}
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_StopTextInput();
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(form);
	TTF_CloseFont(generalFont);
	if (text1 != "") {
		return make_tuple(trim(text1), checkStep, true);
	}
	else return make_tuple("0", false, false);
}
void ImGuiChangeColorNode(ListNode* node, bool& shownWindow) {
	SDL_Color colorNode = node->colorNode;
	SDL_Color colorText = node->colorText;
	SDL_Color colorBg = node->colorBg;
	SDL_Color colorArrow = node->colorArrow;
	int val = stoi(node->val);
	ImVec4 colorNodeF = ImVec4((float)colorNode.r / 255.0, (float)colorNode.g / 255.0, (float)colorNode.b / 255.0, 1.00f);
	ImVec4 colorTextF = ImVec4((float)colorText.r / 255.0, (float)colorText.g / 255.0, (float)colorText.b / 255.0, 1.00f);
	ImVec4 colorBgF = ImVec4((float)colorBg.r / 255.0, (float)colorBg.g / 255.0, (float)colorBg.b / 255.0, 1.00f);
	ImVec4 colorArrowF = ImVec4((float)colorArrow.r / 255.0, (float)colorArrow.g / 255.0, (float)colorArrow.b / 255.0, 1.00f);
	ImGui::Begin("Options", &shownWindow);
	ImGui::Text("Node size :          25");
	ImGui::InputInt("Node value", &val);
	ImGui::ColorEdit3("Node color", (float*)&colorNodeF);
	ImGui::ColorEdit3("Text color", (float*)&colorTextF);
	ImGui::ColorEdit3("Node background color", (float*)&colorBgF);
	ImGui::ColorEdit3("Arrow color", (float*)&colorArrowF);
	ImGui::SliderInt("Text size", &node->textSize, 20, 40);
	ImGui::End();
	node->colorNode = { (Uint8)(colorNodeF.x * 255), (Uint8)(colorNodeF.y * 255), (Uint8)(colorNodeF.z * 255) };
	node->colorText = { (Uint8)(colorTextF.x * 255), (Uint8)(colorTextF.y * 255), (Uint8)(colorTextF.z * 255) };
	node->colorBg = { (Uint8)(colorBgF.x * 255), (Uint8)(colorBgF.y * 255), (Uint8)(colorBgF.z * 255) };
	node->colorArrow = { (Uint8)(colorArrowF.x * 255), (Uint8)(colorArrowF.y * 255), (Uint8)(colorArrowF.z * 255) };
	val = min(99, val);
	val = max(val, 0);
	node->val = to_string(val);
}
void ImGuiChangeColorDoublyNode(DoublyListNode* node, bool& shownWindow) {
	SDL_Color colorNode = node->colorNode;
	SDL_Color colorText = node->colorText;
	SDL_Color colorBg = node->colorBg;
	SDL_Color colorArrow = node->colorArrow;
	int val = stoi(node->val);
	ImVec4 colorNodeF = ImVec4((float)colorNode.r / 255.0, (float)colorNode.g / 255.0, (float)colorNode.b / 255.0, 1.00f);
	ImVec4 colorTextF = ImVec4((float)colorText.r / 255.0, (float)colorText.g / 255.0, (float)colorText.b / 255.0, 1.00f);
	ImVec4 colorBgF = ImVec4((float)colorBg.r / 255.0, (float)colorBg.g / 255.0, (float)colorBg.b / 255.0, 1.00f);
	ImVec4 colorArrowF = ImVec4((float)colorArrow.r / 255.0, (float)colorArrow.g / 255.0, (float)colorArrow.b / 255.0, 1.00f);
	ImGui::Begin("Options", &shownWindow);
	ImGui::Text("Node size :          25");
	ImGui::InputInt("Node value", &val);
	ImGui::ColorEdit3("Node color           ", (float*)&colorNodeF);
	ImGui::ColorEdit3("Text color           ", (float*)&colorTextF);
	ImGui::ColorEdit3("Node background color", (float*)&colorBgF);
	ImGui::ColorEdit3("Arrow color          ", (float*)&colorArrowF);
	ImGui::SliderInt("Text size", &node->textSize, 20, 40);
	ImGui::End();
	node->colorNode = { (Uint8)(colorNodeF.x * 255), (Uint8)(colorNodeF.y * 255), (Uint8)(colorNodeF.z * 255) };
	node->colorText = { (Uint8)(colorTextF.x * 255), (Uint8)(colorTextF.y * 255), (Uint8)(colorTextF.z * 255) };
	node->colorBg = { (Uint8)(colorBgF.x * 255), (Uint8)(colorBgF.y * 255), (Uint8)(colorBgF.z * 255) };
	node->colorArrow = { (Uint8)(colorArrowF.x * 255), (Uint8)(colorArrowF.y * 255), (Uint8)(colorArrowF.z * 255) };
	val = min(99, val);
	val = max(val, 0);
	node->val = to_string(val);
}
void ImGuiChangeColorArrElem(ArrayElement& elem, bool& shownWindow) {
	SDL_Color colorNode = elem.color;
	SDL_Color colorText = elem.textColor;
	SDL_Color colorBg = elem.bgColor;
	int val = stoi(elem.val);
	ImVec4 colorNodeF = ImVec4((float)colorNode.r / 255.0, (float)colorNode.g / 255.0, (float)colorNode.b / 255.0, 1.00f);
	ImVec4 colorTextF = ImVec4((float)colorText.r / 255.0, (float)colorText.g / 255.0, (float)colorText.b / 255.0, 1.00f);
	ImVec4 colorBgF = ImVec4((float)colorBg.r / 255.0, (float)colorBg.g / 255.0, (float)colorBg.b / 255.0, 1.00f);
	ImGui::Begin("Options", &shownWindow);
	ImGui::Text("Element size :          25");
	ImGui::InputInt("Element value", &val);
	ImGui::ColorEdit3("Element color", (float*)&colorNodeF);
	ImGui::ColorEdit3("Text color", (float*)&colorTextF);
	ImGui::ColorEdit3("Element background color", (float*)&colorBgF);
	ImGui::SliderInt("Text size", &elem.textSize, 20, 40);
	ImGui::End();
	elem.color = { (Uint8)(colorNodeF.x * 255), (Uint8)(colorNodeF.y * 255), (Uint8)(colorNodeF.z * 255) };
	elem.textColor = { (Uint8)(colorTextF.x * 255), (Uint8)(colorTextF.y * 255), (Uint8)(colorTextF.z * 255) };
	elem.bgColor = { (Uint8)(colorBgF.x * 255), (Uint8)(colorBgF.y * 255), (Uint8)(colorBgF.z * 255) };
	val = min(99, val);
	val = max(val, 0);
	elem.val = to_string(val);
}
void ImGuiChangeColorElem(vector<SDL_Color>& choice, int& textSize, bool& shownWindow) {
	SDL_Color colorNode = choice[3];
	SDL_Color colorText = choice[2];
	SDL_Color colorBg = choice[0];
	SDL_Color colorElemBg = choice[1];
	SDL_Color colorArrow = choice[4];
	ImVec4 colorNodeF = ImVec4((float)colorNode.r / 255.0, (float)colorNode.g / 255.0, (float)colorNode.b / 255.0, 1.00f);
	ImVec4 colorTextF = ImVec4((float)colorText.r / 255.0, (float)colorText.g / 255.0, (float)colorText.b / 255.0, 1.00f);
	ImVec4 colorBgF = ImVec4((float)colorBg.r / 255.0, (float)colorBg.g / 255.0, (float)colorBg.b / 255.0, 1.00f);
	ImVec4 colorElemBgF = ImVec4((float)colorElemBg.r / 255.0, (float)colorElemBg.g / 255.0, (float)colorElemBg.b / 255.0, 1.00f);
	ImVec4 colorArrowF = ImVec4((float)colorArrow.r / 255.0, (float)colorArrow.g / 255.0, (float)colorArrow.b / 255.0, 1.00f);
	ImGui::SetNextWindowSize({ 500,200 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Options", &shownWindow);
	ImGui::Text("Node size :          25");
	ImGui::ColorEdit3("Node color           ", (float*)&colorNodeF);
	ImGui::ColorEdit3("Text color           ", (float*)&colorTextF);
	ImGui::ColorEdit3("Background color     ", (float*)&colorBgF);
	ImGui::ColorEdit3("Node background color", (float*)&colorElemBgF);
	ImGui::ColorEdit3("Arrow color          ", (float*)&colorArrowF);
	ImGui::SliderInt("Text size", &textSize, 20, 40);
	ImGui::End();
	choice[3] = { (Uint8)(colorNodeF.x * 255), (Uint8)(colorNodeF.y * 255), (Uint8)(colorNodeF.z * 255) };
	choice[2] = { (Uint8)(colorTextF.x * 255), (Uint8)(colorTextF.y * 255), (Uint8)(colorTextF.z * 255) };
	choice[0] = { (Uint8)(colorBgF.x * 255), (Uint8)(colorBgF.y * 255), (Uint8)(colorBgF.z * 255) };
	choice[1] = { (Uint8)(colorElemBgF.x * 255), (Uint8)(colorElemBgF.y * 255), (Uint8)(colorElemBgF.z * 255) };
	choice[4] = { (Uint8)(colorArrowF.x * 255), (Uint8)(colorArrowF.y * 255), (Uint8)(colorArrowF.z * 255) };
}
void staticArrayVisualizing() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 165 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 559, 165 + 144 * i, 200, 100);
	}
	buttonPosArray[10][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[10][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	SDL_Color bgColor = theme.getBgColor();
	StaticArrayVisualizer arrayVisualizer;
	vector<string> v = { "11","23","24","35" };
	arrayVisualizer.initialize(v);
	int cnt = 0;
	int idx = -1;
	string val = "";
	int highlightIdx = -1;
	int activeNode = -1;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, gRenderer);
	ImGui_ImplSDLRenderer_Init(gRenderer);
	bool shownImGuiWindow = true;
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_RenderFillRect(gRenderer, &SCREEN);
		arrayVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		highlightIdx = highlightArrElem(100, 100, YELLOW, arrayVisualizer.size);
		if (activeNode != -1 && shownImGuiWindow) {
			ImGuiChangeColorArrElem(arrayVisualizer.arr[activeNode], shownImGuiWindow);
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (highlightIdx != -1) {
					shownImGuiWindow = true;
					activeNode = highlightIdx;
				}
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						arrayVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png", arrayVisualizer.size + 1);
					if (isValid) {
						arrayVisualizer.insertStep(idx, val, checkStep);
					}
				}
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png", arrayVisualizer.size);
					if (isValid) {
						arrayVisualizer.deleteStep(idx, checkStep);
					}
				}
				else if (e.button.x > 600 && e.button.x < 800 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(val, checkStep, isValid) = valueFormInput("SearchForm.png");
					if (isValid) {
						arrayVisualizer.searchStep(val, checkStep);
					}
				}
				else if (e.button.x > 800 && e.button.x < 1000 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("UpdateForm.png", arrayVisualizer.size);
					if (isValid) {
						arrayVisualizer.updateStep(idx, val, checkStep);
					}
				}
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	TTF_CloseFont(generalFont);
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return;
}
void dynamicArrayVisualizing() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 165 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 559, 165 + 144 * i, 200, 100);
	}
	buttonPosArray[10][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[10][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	SDL_Color bgColor = theme.getBgColor();
	DynamicArrayVisualizer arrayVisualizer;
	vector<string> v = { "11","23","24","35" };
	arrayVisualizer.initialize(v);
	int cnt = 0;
	int idx = -1;
	string val = "";
	int highlightIdx = -1;
	int activeNode = -1;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, gRenderer);
	ImGui_ImplSDLRenderer_Init(gRenderer);
	bool shownImGuiWindow = true;
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_RenderFillRect(gRenderer, &SCREEN);
		arrayVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		highlightIdx = highlightArrElem(100, 100, YELLOW, arrayVisualizer.size);
		if (activeNode != -1 && shownImGuiWindow) {
			ImGuiChangeColorArrElem(arrayVisualizer.arr[activeNode], shownImGuiWindow);
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (highlightIdx != -1) {
					shownImGuiWindow = true;
					activeNode = highlightIdx;
				}
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						arrayVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png", arrayVisualizer.size + 1);
					if (isValid) {
						arrayVisualizer.insertStep(idx, val, checkStep);
					}
				}
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png", arrayVisualizer.size);
					if (isValid) {
						arrayVisualizer.deleteStep(idx, checkStep);
					}
				}
				else if (e.button.x > 600 && e.button.x < 800 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(val, checkStep, isValid) = valueFormInput("SearchForm.png");
					if (isValid) {
						arrayVisualizer.searchStep(val, checkStep);
					}
				}
				else if (e.button.x > 800 && e.button.x < 1000 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("UpdateForm.png", arrayVisualizer.size);
					if (isValid) {
						arrayVisualizer.updateStep(idx, val, checkStep);
					}
				}
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	TTF_CloseFont(generalFont);
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return;
}
void linkedListVisualizing() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 165 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 559, 165 + 144 * i, 200, 100);
	}
	buttonPosArray[10][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[10][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	SDL_Color bgColor = theme.getBgColor();
	LinkedListVisualizer listVisualizer;
	vector<string> v = { "11","23","24","35" };
	listVisualizer.initialize(v);
	int cnt = 0;
	int idx = -1;
	string val = "";
	int highlightIdx = -1;
	int activeNode = -1;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, gRenderer);
	ImGui_ImplSDLRenderer_Init(gRenderer);
	bool shownImGuiWindow = true;
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_RenderFillRect(gRenderer, &SCREEN);
		listVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		highlightIdx = highlightNode(false, 100, 100, YELLOW, listVisualizer.listSize);
		if (activeNode != -1 && shownImGuiWindow) {
			ImGuiChangeColorNode(listVisualizer.getNode(activeNode), shownImGuiWindow);
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (highlightIdx != -1) {
					shownImGuiWindow = true;
					activeNode = highlightIdx;
				}
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						listVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png", listVisualizer.listSize + 1);
					if (isValid) {
						if (listVisualizer.listSize != 0)
							listVisualizer.insertStep(idx, val, checkStep);
						else
							listVisualizer.insertEmpty(val, checkStep);
					}
				}
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png", listVisualizer.listSize);
					if (isValid) {
						if (idx)
							listVisualizer.deleteStep(idx, checkStep);
						else
							listVisualizer.deleteStepHead(checkStep);
					}
				}
				else if (e.button.x > 600 && e.button.x < 800 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(val, checkStep, isValid) = valueFormInput("SearchForm.png");
					if (isValid) {
						listVisualizer.searchStep(val, checkStep);
					}
				}
				else if (e.button.x > 800 && e.button.x < 1000 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("UpdateForm.png", listVisualizer.listSize);
					if (isValid) {
						listVisualizer.updateStep(idx, val, checkStep);
					}
				}
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	TTF_CloseFont(generalFont);
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return;
}
void circularLinkedListVisualizing() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 165 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 559, 165 + 144 * i, 200, 100);
	}
	buttonPosArray[10][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[10][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	SDL_Color bgColor = theme.getBgColor();
	CircularLinkedListVisualizer listVisualizer;
	vector<string> v = { "11","23","24","35" };
	listVisualizer.initialize(v);
	int cnt = 0;
	int idx = -1;
	string val = "";
	int highlightIdx = -1;
	int activeNode = -1;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, gRenderer);
	ImGui_ImplSDLRenderer_Init(gRenderer);
	bool shownImGuiWindow = true;
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_RenderFillRect(gRenderer, &SCREEN);
		listVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		highlightIdx = highlightNode(false, 100, 100, YELLOW, listVisualizer.listSize);
		if (activeNode != -1 && shownImGuiWindow) {
			ImGuiChangeColorNode(listVisualizer.getNode(activeNode), shownImGuiWindow);
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (highlightIdx != -1) {
					shownImGuiWindow = true;
					activeNode = highlightIdx;
				}
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						listVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png", listVisualizer.listSize + 1);
					if (isValid) {
						if (idx == 0) {
							if (listVisualizer.listSize != 0)
								listVisualizer.insertStepHead(idx, val, checkStep);
							else
								listVisualizer.insertEmpty(val, checkStep);
						}
						else if (idx == listVisualizer.listSize) {
							listVisualizer.insertStepTail(idx, val, checkStep);
						}
						else {
							listVisualizer.insertStep(idx, val, checkStep);
						}
					}
				}
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png", listVisualizer.listSize);
					if (isValid) {
						if (idx)
							listVisualizer.deleteStep(idx, checkStep);
						else
							listVisualizer.deleteStepHead(checkStep);
					}
				}
				else if (e.button.x > 600 && e.button.x < 800 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(val, checkStep, isValid) = valueFormInput("SearchForm.png");
					if (isValid) {
						listVisualizer.searchStep(val, checkStep);
					}
				}
				else if (e.button.x > 800 && e.button.x < 1000 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("UpdateForm.png", listVisualizer.listSize);
					if (isValid) {
						listVisualizer.updateStep(idx, val, checkStep);
					}
				}
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	TTF_CloseFont(generalFont);
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return;
}
void doublyLinkedListVisualizing() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 165 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 559, 165 + 144 * i, 200, 100);
	}
	buttonPosArray[10][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[10][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	SDL_Color bgColor = theme.getBgColor();
	DoublyLinkedListVisualizer listVisualizer;
	vector<string> v = { "11","23","24","35" };
	listVisualizer.initialize(v);
	int cnt = 0;
	int idx = -1;
	string val = "";
	int highlightIdx = -1;
	int activeNode = -1;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, gRenderer);
	ImGui_ImplSDLRenderer_Init(gRenderer);
	bool shownImGuiWindow = true;
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_RenderFillRect(gRenderer, &SCREEN);
		listVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		highlightIdx = highlightNode(false, 100, 100, YELLOW, listVisualizer.listSize);
		if (activeNode != -1 && shownImGuiWindow) {
			ImGuiChangeColorDoublyNode(listVisualizer.getNode(activeNode), shownImGuiWindow);
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (highlightIdx != -1) {
					shownImGuiWindow = true;
					activeNode = highlightIdx;
				}
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						listVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png", listVisualizer.listSize + 1);
					if (isValid) {
						if (idx == 0) {
							if (listVisualizer.listSize != 0)
								listVisualizer.insertStepHead(val, checkStep);
							else
								listVisualizer.insertEmpty(val, checkStep);
						}
						else {
							listVisualizer.insertStep(idx, val, checkStep);
						}
					}
				}
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png", listVisualizer.listSize);
					if (isValid) {
						if (idx)
							listVisualizer.deleteStep(idx, checkStep);
						else
							listVisualizer.deleteStepHead(checkStep);
					}
				}
				else if (e.button.x > 600 && e.button.x < 800 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(val, checkStep, isValid) = valueFormInput("SearchForm.png");
					if (isValid) {
						listVisualizer.searchStep(val, checkStep);
					}
				}
				else if (e.button.x > 800 && e.button.x < 1000 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("UpdateForm.png", listVisualizer.listSize);
					if (isValid) {
						listVisualizer.updateStep(idx, val, checkStep);
					}
				}
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	TTF_CloseFont(generalFont);
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return;
}
void stackVisualizing() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* popForm = loadImgTexture("resources/Form/PopForm.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 165 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 559, 165 + 144 * i, 200, 100);
	}
	for (int i = 8; i < 10; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 1317 + 144 * (i - 8), 200, 66);
		buttonPosArray[i][1] = *createRect(&rect, 559, 1317 + 144 * (i - 8), 200, 66);
	}
	buttonPosArray[10][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[10][1] = *createRect(&rect, 1819, 1029, 50, 50);
	buttonPosArray[11][0] = *createRect(&rect, 271, 1434, 200, 100);
	buttonPosArray[11][1] = *createRect(&rect, 12, 1434, 200, 100);
	bool quit = false;
	StackVisualizer stackVisualizer;
	vector<string> v = { "11","23","24","35" };
	SDL_Color bgColor = theme.getBgColor();
	stackVisualizer.initialize(v);
	int cnt = 0;
	int idx = -1;
	string val = "";
	int highlightIdx = -1;
	int activeNode = -1;
	int activeButton = -1;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, gRenderer);
	ImGui_ImplSDLRenderer_Init(gRenderer);
	bool shownImGuiWindow = true;
	bool clear = false;
	bool alert = false;
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_RenderFillRect(gRenderer, &SCREEN);
		stackVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[5][0], &buttonPosArray[5][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[6][0], &buttonPosArray[6][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[11][0], &buttonPosArray[11][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		highlightIdx = highlightNode(true, 100, 100, YELLOW, stackVisualizer.listSize);
		if (activeNode != -1 && shownImGuiWindow) {
			ImGuiChangeColorNode(stackVisualizer.getNode(activeNode), shownImGuiWindow);
		}
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Visualizing method", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 7 * 10) / 2);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "RUN METHOD");
			if (ImGui::Button("Run step-by-step")) {
				ImGui::CloseCurrentPopup();
				if (clear)	activeButton = 1;
				else {
					if (stackVisualizer.listSize != 0) {
						activeButton = 2;
					}
				}
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Run all-at-once ")) {
				ImGui::CloseCurrentPopup();
				if (clear)	activeButton = 3;
				else {
					if (stackVisualizer.listSize != 0) {
						activeButton = 4;
					}
				}
			}
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 5 * 9) / 2);
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		switch (activeButton)
		{
		case 1:
			stackVisualizer.clearStep(true);
			activeButton = -1;
			break;
		case 2:
			stackVisualizer.popStep(true);
			activeButton = -1;
			break;
		case 3:
			stackVisualizer.clearStep(false);
			activeButton = -1;
			break;
		case 4:
			stackVisualizer.popStep(false);
			activeButton = -1;
			break;
		default:
			break;
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (highlightIdx != -1) {
					shownImGuiWindow = true;
					activeNode = highlightIdx;
				}
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						stackVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(val, checkStep, isValid) = valueFormInput("PushForm.png");
					if (isValid) {
						stackVisualizer.pushStep(val, checkStep);
					}
				}
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					clear = false;
					ImGui::OpenPopup("Visualizing method");
				}
				else if (e.button.x > 600 && e.button.x < 800 && e.button.y > 530 && e.button.y < 630) {
					ImGui::OpenPopup("Visualizing method");
					clear = true;
				}
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(popForm);
	TTF_CloseFont(generalFont);
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return;
}
void queueVisualizing() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* popForm = loadImgTexture("resources/Form/PopForm.png");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 165 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 559, 165 + 144 * i, 200, 100);
	}
	for (int i = 8; i < 10; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 829, 1317 + 144 * (i - 8), 200, 66);
		buttonPosArray[i][1] = *createRect(&rect, 559, 1317 + 144 * (i - 8), 200, 66);
	}
	buttonPosArray[10][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[10][1] = *createRect(&rect, 1819, 1029, 50, 50);
	buttonPosArray[11][0] = *createRect(&rect, 271, 1434, 200, 100);
	buttonPosArray[11][1] = *createRect(&rect, 12, 1434, 200, 100);
	bool quit = false;
	SDL_Color bgColor = theme.getBgColor();
	QueueVisualizer queueVisualizer;
	vector<string> v = { "11","23","24","35" };
	queueVisualizer.initialize(v);
	int cnt = 0;
	int idx = -1;
	int activeButton = -1;
	string val = "";
	int highlightIdx = -1;
	int activeNode = -1;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, gRenderer);
	ImGui_ImplSDLRenderer_Init(gRenderer);
	bool shownImGuiWindow = true;
	bool clear = false;
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_RenderFillRect(gRenderer, &SCREEN);
		queueVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[5][0], &buttonPosArray[5][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[6][0], &buttonPosArray[6][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[11][0], &buttonPosArray[11][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		highlightIdx = highlightNode(false, 100, 100, YELLOW, queueVisualizer.listSize);
		if (activeNode != -1 && shownImGuiWindow) {
			ImGuiChangeColorNode(queueVisualizer.getNode(activeNode), shownImGuiWindow);
		}
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Visualizing method", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 7 * 10) / 2);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "RUN METHOD");
			if (ImGui::Button("Run step-by-step")) {
				ImGui::CloseCurrentPopup();
				if (clear)	activeButton = 1;
				else if (queueVisualizer.listSize != 0) activeButton = 2;
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Run all-at-once ")) {
				ImGui::CloseCurrentPopup();
				if (clear)	activeButton = 3;
				else if (queueVisualizer.listSize != 0) activeButton = 4;
			}
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 5 * 9) / 2);
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		switch (activeButton)
		{
		case 1:
			queueVisualizer.clearStep(true);
			activeButton = -1;
			break;
		case 2:
			queueVisualizer.popStep(true);
			activeButton = -1;
			break;
		case 3:
			queueVisualizer.clearStep(false);
			activeButton = -1;
			break;
		case 4:
			queueVisualizer.popStep(false);
			activeButton = -1;
			break;
		default:
			break;
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (highlightIdx != -1) {
					shownImGuiWindow = true;
					activeNode = highlightIdx;
				}
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						queueVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(val, checkStep, isValid) = valueFormInput("PushForm.png");
					if (isValid) {
						if (queueVisualizer.listSize != 0)
							queueVisualizer.pushStep(val, checkStep);
						else
							queueVisualizer.insertEmpty(val, checkStep);
					}
				}
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					clear = false;
					ImGui::OpenPopup("Visualizing method");
				}
				else if (e.button.x > 600 && e.button.x < 800 && e.button.y > 530 && e.button.y < 630) {
					ImGui::OpenPopup("Visualizing method");
					clear = true;
				}
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(popForm);
	TTF_CloseFont(generalFont);
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return;
}
void linkedListMenu() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* background = loadImgTexture("resources/Background/Background.jpg");
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 3; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 12, 309 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 271, 309 + 144 * i, 200, 100);
	}
	buttonPosArray[3][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[3][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, background, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 300, 100, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 400, 210, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 300, 320, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 0, 0, 50, 50, NULL);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 300 && e.button.x < 500 && e.button.y > 100 && e.button.y < 200) linkedListVisualizing();
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 210 && e.button.y < 310) doublyLinkedListVisualizing();
				else if (e.button.x > 300 && e.button.x < 500 && e.button.y > 320 && e.button.y < 420) circularLinkedListVisualizing();
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(background);
	return;
}
void arrayMenu() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* background = loadImgTexture("resources/Background/Background.jpg");
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 2; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 12, 1116 + 155 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 271, 1116 + 155 * i, 200, 100);
	}
	buttonPosArray[2][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[2][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, background, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 300, 100, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 400, 210, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 0, 0, 50, 50, NULL);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 300 && e.button.x < 500 && e.button.y > 100 && e.button.y < 200) staticArrayVisualizing();
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 210 && e.button.y < 310) dynamicArrayVisualizing();
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(background);
	return;
}
void themeOptions() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* themeOptions = loadImgTexture("resources/Theme/ThemeOptions.png");
	TTF_Font* numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 90);
	SDL_Rect apply = *createRect(&rect, 12, 911, 180, 60);
	SDL_Rect applyHover = *createRect(&rect, 271, 905, 180, 66);
	SDL_Rect back = *createRect(&rect, 2000, 1029, 50, 50);
	SDL_Rect backHover = *createRect(&rect, 1819, 1029, 50, 50);
	SDL_Rect custom = *createRect(&rect, 12, 1023, 132, 46);
	SDL_Rect customHover = *createRect(&rect, 271, 1023, 132, 46);
	SDL_Rect up = *createRect(&rect, 316, 383, 24, 16);
	SDL_Rect down = *createRect(&rect, 316, 399, 24, 16);
	bool quit = false;
	bool checkBack = false;
	vector<SDL_Color> choice = { theme.getBgColor(),theme.getBgElemColor(),theme.getTextColor(),theme.getElemColor(),theme.getArrowColor() };
	int textSize = theme.getTextSize();
	vector<pair<SDL_Rect, SDL_Color>> rects;
	ListNode* demoNode = new ListNode("88", 374 + 25, 467 + 110, 25, new ListNode());
	ArrayElement demoElem = ArrayElement(374, 467 + 15, 50, 50, choice[3], choice[1], choice[2], textSize, "88", "");
	vector<SDL_Color> colorMap = {
		{0,0,0},
		{255,255,255},
		{252,129,129},
		{246,224,94},
		{104,211,145},
		{99, 179, 237},
		{118, 228, 247}
	};
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 5; j++) {
			rects.push_back(make_pair(*createRect(&rect, 240 + 110 * i, 80 + 60 * j, 95, 32), colorMap[i]));
		}
	}
	SDL_Color old;
	SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	int mouseX, mouseY;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, gRenderer);
	ImGui_ImplSDLRenderer_Init(gRenderer);
	bool ImGuiWindowVisible = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		SDL_RenderCopy(gRenderer, themeOptions, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &apply, &applyHover, false, 800, 570, 180, 60, NULL);
		createButton(buttonSpriteSheet, &back, &backHover, false, 0, 0, 50, 50, NULL);
		createButton(buttonSpriteSheet, &custom, &customHover, false, 722, 380, 132, 46, NULL);
		createText(numberFont, BLACK, to_string(textSize), 275, 390, 2 * 10, 20);
		if (ImGuiWindowVisible) {
			ImGuiChangeColorElem(choice, textSize, ImGuiWindowVisible);
		}
		for (int i = 0; i < rects.size(); i++) {
			if (isEqualColor(rects[i].second, choice[i % 5])) {
				SDL_SetRenderDrawColor(gRenderer, 128, 90, 213, 255);
				SDL_RenderDrawRect(gRenderer, &rects[i].first);
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
			}
		}
		SDL_SetRenderDrawColor(gRenderer, choice[0].r, choice[0].g, choice[0].b, choice[0].a);
		SDL_RenderFillRect(gRenderer, createRect(&rect, 324, 467, 360, 160));
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
		demoNode->setColorNode(choice[3]);
		demoNode->setColorText(choice[2]);
		demoNode->setColorArrow(choice[4]);
		demoNode->setColorBg(choice[1]);
		demoNode->setTextSize(textSize);
		demoNode->displayNode();
		demoElem.bgColor = choice[1];
		demoElem.textColor = choice[2];
		demoElem.color = choice[3];
		demoElem.textSize = textSize;
		demoElem.display();
		SDL_GetMouseState(&mouseX, &mouseY);
		for (pair<SDL_Rect, SDL_Color> p : rects) {
			if (mouseX > p.first.x && mouseX < p.first.x + p.first.w && mouseY > p.first.y && mouseY < p.first.y + p.first.h) {
				SDL_RenderDrawRect(gRenderer, &p.first);
			}
		}
		if (mouseX > up.x && mouseX < up.x + up.w && mouseY > up.y && mouseY < up.y + up.h) {
			SDL_RenderDrawRect(gRenderer, &up);
		}
		else if (mouseX > down.x && mouseX < down.x + down.w && mouseY > down.y && mouseY < down.y + down.h) {
			SDL_RenderDrawRect(gRenderer, &down);
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) {
					quit = true;
					checkBack = true;
				}
				else if (e.button.x > 722 && e.button.x < 854 && e.button.y > 380 && e.button.y < 426 && !ImGuiWindowVisible) {
					ImGuiWindowVisible = !ImGuiWindowVisible;
				}
				if (ImGuiWindowVisible) break;
				if (e.button.x > 800 && e.button.x < 980 && e.button.y > 570 && e.button.y < 630) {
					quit = true;
				}
				else if (e.button.x > up.x && e.button.x < up.x + up.w && e.button.y > up.y && e.button.y < up.y + up.h) {
					SDL_RenderDrawRect(gRenderer, &up);
					textSize = min(textSize + 1, 40);
				}
				else if (e.button.x > down.x && e.button.x < down.x + down.w && e.button.y > down.y && e.button.y < down.y + down.h) {
					SDL_RenderDrawRect(gRenderer, &down);
					textSize = max(textSize - 1, 20);
				}
				else {
					for (pair<SDL_Rect, SDL_Color> p : rects) {
						if (e.button.x > p.first.x && e.button.x < p.first.x + p.first.w && e.button.y > p.first.y && e.button.y < p.first.y + p.first.h) {
							switch (p.first.y) {
							case 80:
								choice[0] = p.second;
								break;
							case 140:
								choice[1] = p.second;
								break;
							case 200:
								choice[2] = p.second;
								break;
							case 260:
								choice[3] = p.second;
								break;
							case 320:
								choice[4] = p.second;
								break;
							default:
								break;
							}
						}
					}
				}
			default:
				break;
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
	if (!checkBack) {
		theme.setBgColor(choice[0]);
		theme.setBgElemColor(choice[1]);
		theme.setTextColor(choice[2]);
		theme.setElemColor(choice[3]);
		theme.setArrowColor(choice[4]);
		theme.setTextSize(textSize);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(themeOptions);
	TTF_CloseFont(numberFont);
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return;
}
void mainMenu() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* background = loadImgTexture("resources/Background/Background.jpg");
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 4; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 1413, 309 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 1683, 309 + 144 * i, 200, 100);
	}
	buttonPosArray[4][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[4][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, background, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 300, 100, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 400, 210, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 300, 320, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 400, 430, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 0, 0, 50, 50, NULL);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 300 && e.button.x < 500 && e.button.y > 100 && e.button.y < 200) arrayMenu();
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 210 && e.button.y < 310) linkedListMenu();
				else if (e.button.x > 300 && e.button.x < 500 && e.button.y > 320 && e.button.y < 420) stackVisualizing();
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 430 && e.button.y < 530) queueVisualizing();
				else if (e.button.x > 875 && e.button.x < 984 && e.button.y > 10 && e.button.y < 128) themeOptions();
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(background);
	return;
}
void startingScreen() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* background = loadImgTexture("resources/Background/main_bg.png");
	SDL_Rect buttonPosArray[3][2];
	for (int i = 0; i < 2; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 40, 171 + 56 * i, 106, 36);
		buttonPosArray[i][1] = *createRect(&rect, 239, 171 + 56 * i, 106, 36);
	}
	bool quit = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, background, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 431, 463, 106, 36, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 431, 516, 106, 36, NULL);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.x > 431 && e.button.x < 431 + 106 && e.button.y > 463 && e.button.y < 463 + 36) mainMenu();
				else if (e.button.x > 431 && e.button.x < 431 + 106 && e.button.y > 516 && e.button.y < 516 + 36) themeOptions();
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(background);
	return;
}