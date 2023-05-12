#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "global.h"

void init();
SDL_Rect* createRect(SDL_Rect* rect, int x, int y, int w, int h);
SDL_Texture* loadImgTexture(string path);
void quitGame();
void createButton(SDL_Texture* buttonSpriteSheet, SDL_Rect* button, SDL_Rect* buttonOnHover, bool leftMouseDown, int x, int y, int w, int h, void(*func)(), double degree = -1);
void highlightCode(TTF_Font* font, SDL_Texture* explain, vector<string> code, int linesColor, int startX, int startY, SDL_Color color, bool checkStep);
void createText(TTF_Font* font, SDL_Color color, string text, int x, int y, int w, int h);
bool isNumber(string s);
bool isExist(const string& name);
string ltrim(const string& s);
string rtrim(const string& s);
string trim(const string& s);
bool isEqualColor(SDL_Color c1, SDL_Color c2);
void SDL_RenderDrawCircle(SDL_Renderer* renderer, int32_t x, int32_t y, int32_t radius, SDL_Color color);
void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color);
void DrawArrow(SDL_Renderer* renderer, int startX, int endX, int startY, int endY, float trirad, SDL_Color color);
int highlightNode(bool isStack, int startX, int startY, SDL_Color color, int listSize);
int highlightArrElem(int startX, int startY, SDL_Color color, int size);
void ImGuiControlStep(int& step, int maxStep, bool checkStep, int& speed, int& t, bool& quit, bool& stop);
bool alertInitialize(bool& alert1, bool& alert2, bool& alert3, const string& s1, const string& s2);
void initResource();

