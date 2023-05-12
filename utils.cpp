#pragma once
#include "utils.h"
void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	IMG_Init(IMG_INIT_JPG);
	TTF_Init();
}
SDL_Rect* createRect(SDL_Rect* rect, int x, int y, int w, int h) {
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	return rect;
}
SDL_Texture* loadImgTexture(string path) {
	SDL_Texture* newTexture = NULL;
	newTexture = IMG_LoadTexture(gRenderer, path.c_str());
	return newTexture;
}
void quitGame() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(gRenderer);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(0);
}
void createButton(SDL_Texture* buttonSpriteSheet, SDL_Rect* button, SDL_Rect* buttonOnHover, bool leftMouseDown, int x, int y, int w, int h, void(*func)(), double degree) {
	int mouseX, mouseY;
	SDL_Rect buttonRect;
	SDL_GetMouseState(&mouseX, &mouseY);
	if (x < mouseX && mouseX < x + w && y < mouseY && mouseY < y + h) {
		if (leftMouseDown && func != NULL) func();
		if (degree == -1)
			SDL_RenderCopy(gRenderer, buttonSpriteSheet, buttonOnHover, createRect(&buttonRect, x, y, w, h));
		else
			SDL_RenderCopyEx(gRenderer, buttonSpriteSheet, buttonOnHover, createRect(&buttonRect, x, y, w, h), degree, NULL, SDL_FLIP_NONE);
	}
	else {
		SDL_RenderCopy(gRenderer, buttonSpriteSheet, button, createRect(&buttonRect, x, y, w, h));
	}
}
void createText(TTF_Font* font, SDL_Color color, string text, int x, int y, int w, int h) {
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	SDL_FreeSurface(textSurface);
	SDL_Rect rect;
	SDL_RenderCopy(gRenderer, texture, NULL, createRect(&rect, x, y, w, h));
	SDL_DestroyTexture(texture);
}
void highlightCode(TTF_Font* font, SDL_Texture* explain, vector<string> code, int linesColor, int startX, int startY, SDL_Color color, bool checkStep) {
	if (linesColor == -1) return;
	int mouseX, mouseY;
	string explanation = codeExplainMap[code[linesColor]];
	SDL_Rect rect;
	int letterSizeW = 8;
	SDL_Rect hightlight = { startX, startY + linesColor * 50, code[linesColor].size() * 16, 40 };
	SDL_Rect src = { 0,0,explanation.size() * letterSizeW + 80,50 };
	SDL_Rect dst = { SCREEN_WIDTH - (explanation.size() * letterSizeW + 80), 0, explanation.size() * letterSizeW + 80, 50 };
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(gRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
	if ((mouseX > startX && mouseX<startX + code[linesColor].size() * 16 && mouseY>startY + linesColor * 50 && mouseY < startY + linesColor * 50 + 40) || !checkStep) {
		SDL_RenderFillRect(gRenderer, &hightlight);
		SDL_RenderCopy(gRenderer, explain, &src, &dst);
		createText(font, BLACK, explanation, dst.x + 52, dst.y + 22, explanation.size() * letterSizeW, 20);
	}
	SDL_SetRenderDrawColor(gRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}
bool isNumber(string s) {
	return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos;
}
bool isExist(const string& name) {
	ifstream f(name.c_str());
	return f.good();
}
string ltrim(const string& s)
{
	size_t start = s.find_first_not_of(" \n\r\t\f\v");
	return (start == std::string::npos) ? "" : s.substr(start);
}
string rtrim(const string& s)
{
	size_t end = s.find_last_not_of(" \n\r\t\f\v");
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
string trim(const string& s) {
	return rtrim(ltrim(s));
}
bool isEqualColor(SDL_Color c1, SDL_Color c2) {
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}
void SDL_RenderDrawCircle(SDL_Renderer* renderer, int32_t x, int32_t y, int32_t radius, SDL_Color color)
{
	int offsetx, offsety, d;
	int status;
	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;
	SDL_Color old;
	SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	while (offsety >= offsetx) {
		status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
		status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
		status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
		status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
		status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
		status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
		status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
		status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);
		if (status < 0) {
			status = -1;
			break;
		}
		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}
	SDL_SetRenderDrawColor(renderer, old.r, old.g, old.b, old.a);
}
void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color)
{
	int offsetx, offsety, d;
	int status;
	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;
	SDL_Color old;
	SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	while (offsety >= offsetx) {
		status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
			x + offsety, y + offsetx);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
			x + offsetx, y + offsety);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
			x + offsetx, y - offsety);
		status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
			x + offsety, y - offsetx);
		if (status < 0) {
			status = -1;
			break;
		}
		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}
	SDL_SetRenderDrawColor(renderer, old.r, old.g, old.b, old.a);
}
void DrawArrow(SDL_Renderer* renderer, int startX, int endX, int startY, int endY, float trirad, SDL_Color color) {
	const double PI = 3.14159265358979323846;
	double rad = PI / 180.0;
	SDL_Color old;
	SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
	float rotation = atan2(startY - endY, endX - startX) + PI / 2.0;
	SDL_Vertex triangleVertex[3] =
	{
	 {
	  { endX + trirad * sin(rotation),endY + trirad * cos(rotation)}, /* first point location */
	  color, /* first color */
	  { 0.f, 0.f }
	 },
	 {
	  { endX + trirad * sin(rotation - 120.0 * rad), endY + trirad * cos(rotation - 120.0 * rad) }, /* second point location */
	  color, /* second color */
	  { 0.f, 0.f }
	 },
	 {
	  { endX + trirad * sin(rotation + 120.0 * rad), endY + trirad * cos(rotation + 120.0 * rad) }, /* third point location */
	  color, /* third color */
	  { 0.f, 0.f }
	 }
	};
	SDL_RenderGeometry(renderer, NULL, triangleVertex, 3, NULL, 0);
	SDL_SetRenderDrawColor(renderer, old.r, old.g, old.b, old.a);
}
int highlightNode(bool isStack, int startX, int startY, SDL_Color color, int listSize) {
	int mouseX, mouseY;
	SDL_Rect rect;
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(gRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
	int idx = -1;
	if (!isStack) {
		for (int i = 0; i < listSize; i++) {
			int centerX = startX + 90 * i;
			int centerY = startY;
			int d = sqrt(abs(mouseX - centerX) * abs(mouseX - centerX) + abs(mouseY - centerY) * abs(mouseY - centerY));
			if (d <= 25) {
				SDL_RenderDrawCircle(gRenderer, centerX, centerY, 27, color);
				idx = i;
				break;
			}
		}
	}
	else {
		for (int i = 0; i < listSize; i++) {
			int centerX = startX;
			int centerY = startY + 90 * i;
			int d = sqrt(abs(mouseX - centerX) * abs(mouseX - centerX) + abs(mouseY - centerY) * abs(mouseY - centerY));
			if (d <= 25) {
				SDL_RenderDrawCircle(gRenderer, centerX, centerY, 27, color);
				idx = i;
				break;
			}
		}
	}
	SDL_SetRenderDrawColor(gRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	return idx;
}
int highlightArrElem(int startX, int startY, SDL_Color color, int size) {
	int mouseX, mouseY;
	SDL_Rect rect;
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(gRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
	int idx = -1;
	for (int i = 0; i < size; i++) {
		int x = startX + 50 * i;
		int y = startY;
		if (mouseX > x && mouseX<x + 50 && mouseY>y && mouseY < y + 50) {
			SDL_RenderDrawRect(gRenderer, createRect(&rect, x - 1, y - 1, 52, 52));
			idx = i;
			break;
		}
	}
	return idx;
}
void ImGuiControlStep(int& step, int maxStep, bool checkStep, int& speed, int& t, bool& quit, bool& stop) {
	const string stepString = to_string(step) + "/" + to_string(maxStep);
	int speedRaw = speed + 1;
	ImGui::SetNextWindowSize({ 300,150 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Control");
	ImGui::Text("Current step :    ");
	ImGui::SameLine();
	ImGui::Text(stepString.c_str());
	ImGui::ProgressBar((float)step / (float)maxStep);
	if (!checkStep) {
		ImGui::SliderInt("Speed", &speedRaw, 1, 4);
		if (ImGui::Button("Run from beginning")) step = 0;
		ImGui::SameLine();
		if (ImGui::Button("Stop")) stop = true;
		ImGui::SameLine();
		if (ImGui::Button("Run ")) stop = false;
		ImGui::SameLine();
		if (ImGui::Button("End ")) quit = true;
		if (stop) {
			if (ImGui::Button("Previous Step")) step = max(step - 1, 0);
			ImGui::SameLine();
			if (ImGui::Button("  Next Step  ")) step = min(step + 1, maxStep);
		}
	}
	else {
		if (ImGui::Button("Previous Step")) step = max(step - 1, 0);
		ImGui::SameLine();
		if (ImGui::Button("  Next Step  ")) step = min(step + 1, maxStep);
		ImGui::SameLine();
		if (ImGui::Button("End ")) quit = true;
	}
	ImGui::End();
	speed = speedRaw - 1;
}
bool alertInitialize(bool& alert1, bool& alert2, bool& alert3, const string& s1, const string& s2) {
	if (s1.size() == 0 && s2.size() == 0) return true;
	if (s1.size() != 0) {
		if (isExist(s1)) return true;
		else {
			alert3 = true;
			alert1 = false;
			alert2 = false;
			return false;
		}
	}
	istringstream ss(s2);
	string tmp;
	int temp;
	while (ss >> tmp) {
		if (!isNumber(tmp)) {
			alert1 = true;
			alert2 = false;
			alert3 = false;
			return false;
		}
		try {
			temp = stoi(tmp);
		}
		catch (...) {
			alert2 = true;
			alert1 = false;
			alert3 = false;
			return false;
		}
		if (tmp.size() >= 3 || temp < 0 || temp >= 100) {
			alert2 = true;
			alert1 = false;
			alert3 = false;
			return false;
		}
	}
	return true;
}
void initResource() {
	numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
}