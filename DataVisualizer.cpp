#include <SDL.h>

#include <SDL_image.h>

#include <SDL_ttf.h>

#include <string>

#include <time.h>

#include <vector>

#include <utility>

#include <sstream>

#include <fstream>

#include <tuple>

#include <map>

#define RED {255,0,0}
#define BLUE {0,0,255}
#define GREEN {0,255,0}
#define BLACK {0,0,0}
#define WHITE {255,255,255}
#define YELLOW {255,255,0}
using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 640;
map<string, string> codeExplainMap = {
	{"ListNode* toDel = head;","Create a new Node pointer point to the head of the linked list"},
	{"head = head->next;","Set head pointer to the next Node"},
	{"delete toDel;","Delete everything in the previous head"},
	{"ListNode* curr = head->next;","Set curr pointer to the second node"},
	{"ListNode* prev = head;","Set prev pointer to the first (head) node"},
	{"while(index-- > 1){","Check exit loop if index <= 1 and decrease the index to delete"},
	{"    curr = curr->next;","Set curr pointer to the next node"},
	{"    prev = prev->next;","Set prev pointer to the next node"},
	{"prev->next = curr->next;","Set the pointer of prev to point to the next of curr pointer"},
	{"delete curr;","Deallocate the memory used by the deleted node"},
	{"    head = head->next;","Set head pointer to the next node"},
	{"ListNode* newNode = new ListNode(value,nullptr);","Create the pointer to the insert node"},
	{"newNode->next = head->next;","Set the next pointer of the insert node to next of the head pointer"},
	{"head->next = newNode;","Set the next pointer of head to point to the new node"},
	{"newNode->next = head;","Set the next pointer of the insert node to the first node (head)"},
	{"head = newNode;","Change the head pointer of linked list to the new node"},
	{"while (head){","Check if head is a nullptr,if it is then exit the loop"},
	{"    if( head->val == target )","Check to find whether the value of the pointer is equal to the target"},
	{"        return ans;","Return the index at which the value was found"},
	{"    head = head->next;","Increase the pointer to the next node"},
	{"    ans++;","Increase the answer-index of the node that has the value"},
	{"return NOT_FOUND;","If the list doesn't contain the value ,return NOT_FOUND constant (oftentimes will be -1)"},
	{"while (index--){","Check if the index variable (the index to update) is 0 or not to exit the loop, else decrease it"},
	{"head->val = updateValue;","Set the pointer value to the update value"},
	{"tail->next = newNode;","Set the next pointer of tail to point to the new node"},
	{"tail = tail->next;","Change the tail pointer to the new node"},
	//insert elem array
	{"n++;","Increase the size of array by 1"},
	{"for(int i = n-1; i > indexInsert; i--){","check exit loop and decrease i"},
	{"    arr[i] = arr[i-1];","Set the two current element to be equal"},
	{"arr[indexInsert] = valueInsert;","Update the insert value at insert index"},
	//delete elem array
	{"for(int i = indexDelete; i < n-1; i++){","Check exit loop and increase i by 1"},
	{"    arr[i] = arr[i+1];","Set the two current element to be equal"},
	{"n--;","Decrese the size of array by 1"},
	//search array
	{"for(int i=0;i<n;i++){","Check exit loop and increase i by 1"},
	{"    if(arr[i] == target){","Check if the current element value is equal to the target"},
	{"        targetIndex = i;","Set the returned value to be the current i"},
	{"        return targetIndex; }","Return the index where the value was founded"},
	{"    else continue;","Continue to the next iteration"},
	{"return NOT_FOUND;","If the array doesn't contain the value ,return NOT_FOUND constant (oftentimes will be -1)"},
	//update array
	{"arr[indexUpdate] = valueUpdate;","update the input index to be equal to the value the user input"},
	{"while (listSize--){","Check exit loop if listSize == 0 and decrease the listSize variable"},
	//Insert empty LL
	{"head = new ListNode(value,nullptr);","Create the head of the list"},
	{"tail->next = head;","Set tail next pointer point to head of the list because current list has only one Node to create the typical cycle"},
	{"tail = head;","Set tail pointer equal to head because list has only one node"},
	{"DLLNode* newNode = new DLLNode(value,head,head->next);","Create a new DLLNode that has prev pointer point to head pointer and next pointer point to head->next pointer"},
	{"if (head->next)    head->next->prev = newNode;","Check the next pointer existence. If it is, set its prev pointer point to the newNode "},
	{"DLLNode* newNode = new DLLNode(value,nullptr,nullptr);","Create a new DLLNode that has both prev and next pointer set to nullptr"},
	{"head->prev = newNode;","Set the prev pointer of head point to the newNode"},
	{"    toDel = toDel->next;","Increase pointer to the next Node"},
	{"if (toDel->prev)	toDel->prev->next = toDel->next;","Check the prev pointer existence. If it is, set its next to the deleteNode->next"},
	{"if (toDel->next)	toDel->next->prev = toDel->prev;","Check the next pointer existence. If it is, set its prev to the deleteNode->prev"},
	{"DLLNode* toDel = head;","Set the toDel pointer equal to head pointer"},
	{"if (head)	head->prev = nullptr;","Check the head pointer existence (the list is empty or not). If it is, set its prev to nullptr"},
	{"head = new DLLNode(value,nullptr,nullptr);","Initiate a new head that has both prev and next pointer set to nullptr"},
	{"/* Re-layout the list */","Re-layout the current list to fit the purpose of visualization"}
};
SDL_Rect SCREEN = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

SDL_Window* window = SDL_CreateWindow("Data Visualizer", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer* gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
class Theme {
private:
	SDL_Color bgColor;
	SDL_Color bgElemColor;
	SDL_Color textColor;
	SDL_Color elemColor;
	SDL_Color arrowColor;
	int textSize;
	Theme(SDL_Color bgColor, SDL_Color bgElemColor, SDL_Color textColor,SDL_Color elemColor,SDL_Color arrowColor, int textSize) {
		this->bgColor = bgColor;
		this->bgElemColor = bgElemColor;
		this->textColor = textColor;
		this->elemColor = elemColor;
		this->arrowColor = arrowColor;
		this->textSize = textSize;
	}
public:
	SDL_Color getBgColor() {
		return bgColor;
	}
	SDL_Color getBgElemColor() {
		return bgElemColor;
	}
	SDL_Color getTextColor() {
		return textColor;
	}
	SDL_Color getElemColor() {
		return elemColor;
	}
	SDL_Color getArrowColor() {
		return arrowColor;
	}
	int getTextSize() {
		return textSize;
	}
	void setBgColor(SDL_Color color) {
		bgColor = color;
	}
	void setBgElemColor(SDL_Color color) {
		bgElemColor = color;
	}
	void setTextColor(SDL_Color color) {
		textColor = color;
	}
	void setElemColor(SDL_Color color) {
		elemColor = color;
	}
	void setArrowColor(SDL_Color color) {
		arrowColor = color;
	}
	void setTextSize(int size) {
		textSize = size;
	}
	static Theme getInstance(SDL_Color bgColor, SDL_Color bgElemColor, SDL_Color textColor,SDL_Color elemColor,SDL_Color arrowColor, int textSize) {
		static Theme theme(bgColor, bgElemColor, textColor,elemColor,arrowColor, textSize);
		return theme;
	}

};
//Theme(SDL_Color bgColor, SDL_Color bgElemColor, SDL_Color textColor,SDL_Color elemColor,SDL_Color arrowColor, int textSize)
Theme theme = Theme::getInstance(WHITE, WHITE, BLACK, BLACK,BLACK,30);
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

void createButton(SDL_Texture* buttonSpriteSheet ,SDL_Rect* button, SDL_Rect* buttonOnHover, bool leftMouseDown, int x, int y, int w, int h, void(*func)(),double degree=-1) {
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

void highlightCode(TTF_Font* font,SDL_Texture* explain,vector<string> code, int linesColor, int startX, int startY,SDL_Color color,bool checkStep) {
	if (linesColor == -1) return;
	int mouseX, mouseY;
	string explanation = codeExplainMap[code[linesColor]];
	SDL_Rect rect;
	int letterSizeW = 8;
	SDL_Rect hightlight = { startX, startY + linesColor * 50, code[linesColor].size() * 16, 40 };
	SDL_Rect src = { 0,0,explanation.size()*letterSizeW + 80,50 };
	SDL_Rect dst = { SCREEN_WIDTH-(explanation.size() * letterSizeW + 80), 0, explanation.size() * letterSizeW+80, 50 };
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(gRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	
	SDL_SetRenderDrawColor(gRenderer,color.r,color.g,color.b,color.a);
	if ((mouseX > startX && mouseX<startX + code[linesColor].size() * 16 && mouseY>startY + linesColor * 50 && mouseY < startY + linesColor * 50 + 40)||!checkStep) {
		SDL_RenderFillRect(gRenderer, &hightlight);
		SDL_RenderCopy(gRenderer, explain, &src, &dst);
		createText(font, BLACK, explanation, dst.x+52, dst.y+22, explanation.size() * letterSizeW, 20);

	}
	SDL_SetRenderDrawColor(gRenderer, oldColor.r, oldColor.g, oldColor.b,oldColor.a);
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
void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius,SDL_Color color)
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
void DrawArrow(SDL_Renderer* renderer, int startX, int endX, int startY, int endY, double trirad, SDL_Color color) {
	const double PI = 3.14159265358979323846;
	double rad = PI / 180.0;
	SDL_Color old;
	SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
	double rotation = atan2(startY - endY, endX - startX) + PI / 2.0;
	SDL_RenderDrawLine(renderer, endX + trirad * sin(rotation), endY + trirad * cos(rotation), endX + trirad * sin(rotation - 120.0 * rad), endY + trirad * cos(rotation - 120.0 * rad));
	SDL_RenderDrawLine(renderer, endX + trirad * sin(rotation - 120.0 * rad), endY + trirad * cos(rotation - 120.0 * rad), endX + trirad * sin(rotation + 120.0 * rad), endY + trirad * cos(rotation + 120.0 * rad));
	SDL_RenderDrawLine(renderer, endX + trirad * sin(rotation + 120.0 * rad), endY + trirad * cos(rotation + 120.0 * rad), endX + trirad * sin(rotation), endY + trirad * cos(rotation));
	SDL_SetRenderDrawColor(renderer, old.r, old.g, old.b, old.a);
}
class ArrayElement {
public:
	int x;
	int y;
	int w;
	int h;
	int textSize;
	string index;
	SDL_Color color;
	SDL_Color bgColor;
	SDL_Color textColor;
	string val;
	SDL_Rect rect;
	TTF_Font* numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	ArrayElement() { this->val = "N"; }
	ArrayElement(int x, int y,int w,int h, SDL_Color color,SDL_Color bgColor,SDL_Color textColor,string val,string index) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->color = color;
		this->bgColor = bgColor;
		this->textColor = textColor;
		this->val = val;
		this->index = index;
	}
	//~ArrayElement() { TTF_CloseFont(numberFont); TTF_CloseFont(letterFont); }

	void display() {
		SDL_Color old;
		SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
		SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderDrawRect(gRenderer, createRect(&rect, x, y, w, h));
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
		SDL_RenderFillRect(gRenderer, createRect(&rect, x+1, y+1, w-2, h-2));
		createText(numberFont, textColor, val.size()==1 && val!="0" ? "0" + val : val, x + (50-textSize)/2, y + (50 - textSize)/2, textSize, textSize);
		createText(numberFont, BLACK, index, x + 17, y + 60, 15, 20);
		SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
	}
	void changeColor(SDL_Color color) {
		this->color = color;
		this->textColor = color;
	}
	void changeColor(SDL_Color elemColor, SDL_Color textColor) {
		this->color = elemColor;
		this->textColor = textColor;
	}
	void changeCoordinate(int x, int y, int w, int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

};

class ArrayVisualizer {
private:
	SDL_Color bgColor;
public:
	int n;
	ArrayElement arr[100];
	ArrayElement tempArr[100];
	int stepInsert;
	int stepDelete;
	int stepSearch;
	int stepUpdate;
	int delay;
	int maxStep;
	int speed;
	vector<string> code = {};
	int linesDisplay = -1;
	SDL_Rect buttonPosArray[4][2];
	TTF_Font* numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
	SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
	ArrayVisualizer() { 
		n = 0; 
		stepInsert = -1; 
		stepDelete = -1;
		stepSearch = -1;
		stepUpdate = -1;
		delay = -1;
		maxStep = 1000;
		speed = 0;
		bgColor = theme.getBgColor();
		for (int i = 0; i < 100; i++) arr[i] = ArrayElement();
		SDL_Rect rect;
		for (int i = 0; i < 4; i++) {
			buttonPosArray[i][0] = *createRect(&rect, 1413+180*i, 870, 130, 130);
			buttonPosArray[i][1] = *createRect(&rect, 1413 + 180 * i, 1149, 130, 130);
		}
	}
	~ArrayVisualizer() {
		SDL_DestroyTexture(buttonSpriteSheet);
		TTF_CloseFont(numberFont);
	}
	void resetStep() {
		for (int i = 0; i < 100; i++) {
			if (arr[i].val == "N") break;
			arr[i].changeColor(theme.getElemColor(), theme.getTextColor());
		}
		code = {};
		linesDisplay = -1;
		stepInsert = -1;
		stepDelete = -1;
		stepSearch = -1;
		stepUpdate = -1;
		maxStep = 1000;
	}
	bool checkStepEqualZero() {
		return stepInsert == 0 || stepDelete == 0 || stepSearch == 0 || stepUpdate == 0;
	}
	bool checkAnyStep() {
		return stepInsert >= 0 || stepDelete >= 0 || stepSearch >= 0 || stepUpdate >= 0;
	}
	void visualize() {
		if (!checkStepEqualZero()) {
			for (int i = 0; i < 100; i++) {
				if (arr[i].val == "N") break;
					arr[i].display();
			}
		}
		else {
			for (int i = 0; i < 100; i++) {
				if (tempArr[i].val == "N") break;
					tempArr[i].display();
			}
		}
		if(!checkStepEqualZero())	highlightCode(numberFont, explain, code, linesDisplay, 100, 200, YELLOW, true);
		if (checkStepEqualZero()) return;
		for (int i = 0; i < code.size(); i++) {
			if (i == linesDisplay)
				createText(numberFont, RED, code[i], 100, 200 + i * 50,code[i].size() * 16, 40);
			else
				createText(numberFont, BLACK, code[i], 100, 200 + i * 50,code[i].size() * 16, 40);
		}
	}
	void visualize(ArrayElement customArr[],int size) {
		for (int i = 0; i < size; i++) {
			customArr[i].display();
		}
		for (int i = 0; i < code.size(); i++) {
			if (i == linesDisplay)
				createText(numberFont, RED, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
			else
				createText(numberFont, BLACK, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
		}
	}
	void updateFrame(vector<string> code = {}, int linesColor = -1) {
		int t = 60 / (speed+1);
		SDL_Rect rect;
		bool leftMouseDown = false;
		SDL_Event e;
		while (t--) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			visualize();
			createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], leftMouseDown, 900, 400, 50, 50, nullptr);
			highlightCode(numberFont, explain, code, linesColor, 100, 200, YELLOW, false);
			if (linesColor != -1) {
				for (int i = 0; i < code.size(); i++) {
					if (i == linesColor)
						createText(numberFont, RED, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
					else
						createText(numberFont, BLACK, code[i], 100, 200 + i * 50, code[i].size() * 16, 40);
				}
			}
			if (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
					leftMouseDown = true;
					if (e.button.x > 900 && e.button.x < 950 && e.button.y > 400 && e.button.y < 450) speed = (speed + 1) % 4;
				}
			}

			SDL_RenderPresent(gRenderer);
		}
	}
	bool keyboardEvent(int idx,string val) {
		bool checkSame = false;
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_LEFT) {
				if (stepInsert >= 0) {
					stepInsert = max(stepInsert - 1, 0);
					if (stepInsert > 0) {
						insertStep(idx, val, stepInsert, n - 1);
					}
				}
				else if (stepDelete >= 0) {
					stepDelete = max(stepDelete - 1, 0);
					if (stepDelete > 0) {
						delIndexStep(idx, stepDelete, n + 1);
					}
				}
				else if (stepSearch >= 0) {
					stepSearch = max(stepSearch - 1, 0);
					if (stepSearch > 0) {
						searchStep(val, stepSearch, n);
					}
				}
				else if (stepUpdate >= 0) {
					stepUpdate = max(stepUpdate - 1, 0);
					if (stepUpdate > 0) {
						updateStep(idx, val, stepUpdate, n);
					}
				}

				return true;
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RIGHT) {
				if (stepInsert >= 0) {
					stepInsert = min(stepInsert + 1, maxStep);
					insertStep(idx, val, stepInsert, n - 1);
				}
				else if (stepDelete >= 0) {
					stepDelete = min(stepDelete + 1, maxStep);
					delIndexStep(idx, stepDelete, n + 1);
				}
				else if (stepSearch >= 0) {
					stepSearch = min(stepSearch + 1, maxStep);
					searchStep(val, stepSearch, n);
				}
				else if (stepUpdate >= 0) {
					stepUpdate = min(stepUpdate + 1, maxStep);
					updateStep(idx, val, stepUpdate, n);
				}
				return true;
			}
		}
		return false;
	}
	void initialize(vector<string> strArr) {
		this->n = strArr.size();
		for (int i = 0; i < 100; i++) {
			arr[i] = ArrayElement();
		}
		for (int i = 0; i < n; i++) {
			arr[i] = ArrayElement(100+i*50, 100, 50, 50,theme.getElemColor(),theme.getBgElemColor(),theme.getTextColor(),strArr[i],to_string(i));
		}

	}
	void insertStep(int idx, string val, int step,int n) {
		for (int i = 0; i < 100; i++) arr[i] = tempArr[i];
		int currStep = 0;
		if (step == 0) return;
		vector<string> codeInsert = { 
			"n++;", 
			"for(int i = n-1; i > indexInsert; i--){",
			"    arr[i] = arr[i-1];",
			"}",
			"arr[indexInsert] = valueInsert;" 
		};
		n++;
		arr[n - 1] = ArrayElement(100 + (n - 1) * 50, 100, 50, 50, YELLOW, theme.getBgElemColor(), YELLOW , "0",to_string(n-1));
		currStep++;
		if (currStep == step) { linesDisplay = 0; code = codeInsert; return; }

		for (int i = n - 1; i > idx; i--) {
			currStep++;
			if (currStep == step) { linesDisplay = 1; code = codeInsert; return; }
			arr[i + 1].changeColor(theme.getElemColor(), theme.getTextColor());
			arr[i].val = arr[i - 1].val;
			arr[i].changeColor(RED);
			arr[i - 1].changeColor(BLUE);
			currStep++;
			if (currStep == step) { linesDisplay = 2; code = codeInsert; return; }
		}
		currStep++;
		if (currStep == step) { linesDisplay = 1; code = codeInsert; return; }
		arr[idx + 1].changeColor(theme.getElemColor(), theme.getTextColor());
		arr[idx].val = val;
		currStep++;
		linesDisplay = 4; code = codeInsert; maxStep = currStep; return;

	}
	void insert(int idx, string val) {
		vector<string> codeInsert = { 
			"n++;",
			"for(int i = n-1; i > indexInsert; i--){",
			"    arr[i] = arr[i-1];",
			"}",
			"arr[indexInsert] = valueInsert;"
		};
		
		n++;
		arr[n-1] = ArrayElement(100 + (n-1) * 50, 100, 50, 50, YELLOW, theme.getBgElemColor(), YELLOW, "0",to_string(n-1));
		updateFrame(codeInsert, 0);
		
		for (int i = n-1; i > idx; i--) {
			updateFrame(codeInsert, 1);
			arr[i + 1].changeColor(theme.getElemColor(),theme.getTextColor());
			arr[i].val = arr[i - 1].val;
			arr[i].changeColor(RED);
			arr[i-1].changeColor(BLUE);
			updateFrame(codeInsert,2);
		}
		updateFrame(codeInsert, 1);
		arr[idx + 1].changeColor(theme.getElemColor(), theme.getTextColor());;
		arr[idx].val = val;
		updateFrame(codeInsert,4);
		arr[idx].changeColor(theme.getElemColor(), theme.getTextColor());;
		updateFrame(codeInsert, 4);
	}
	void insertNoVisual(int idx, string val) {
		arr[n - 1] = ArrayElement(100 + (n - 1) * 50, 100, 50, 50, YELLOW, theme.getBgElemColor(),YELLOW, "0", to_string(n - 1));
		for (int i = n - 1; i > idx; i--) {
			arr[i].val = arr[i - 1].val;
		}
		arr[idx].val = val;
	}

	void delIndexStep(int idx, int step, int n) {
		for (int i = 0; i < 100; i++) arr[i] = tempArr[i];
		int currStep = 0;
		if (step == 0) return;
		vector<string> codeDelete = { 
			"for(int i = indexDelete; i < n-1; i++){",
			"    arr[i] = arr[i+1];",
			"}",
			"n--;"
		};
		for (int i = idx; i < n - 1; i++) {
			currStep++;
			if (currStep == step) { linesDisplay = 0; code = codeDelete; return; }
			if (i != idx) arr[i - 1].changeColor(theme.getElemColor(), theme.getTextColor());
			arr[i].changeColor(GREEN);
			arr[i + 1].changeColor(RED);
			arr[i].val = arr[i + 1].val;
			currStep++;
			if (currStep == step) { linesDisplay = 1; code = codeDelete; return; }
		}
		currStep++;
		if (currStep == step) { linesDisplay = 0; code = codeDelete; return; }
		arr[n - 1] = ArrayElement();

		n--;
		arr[n - 1].changeColor(theme.getElemColor(), theme.getTextColor());
		currStep++;
		linesDisplay = 3; code = codeDelete; maxStep = currStep; return;

	}
	void delIndex(int idx){
		vector<string> codeDelete = { "for(int i = indexDelete; i < n-1; i++){", "    arr[i] = arr[i+1];","}","n--;" };
		for (int i = idx; i < n-1; i++) {
			updateFrame(codeDelete,0);
			if (i != idx) arr[i - 1].changeColor(theme.getElemColor(), theme.getTextColor());
			arr[i].changeColor(GREEN);
			arr[i + 1].changeColor(RED);
			arr[i].val = arr[i + 1].val;
			updateFrame(codeDelete,1);
		}
		updateFrame(codeDelete, 0);
		arr[n - 1] = ArrayElement();
		
		n--;
		updateFrame(codeDelete, 3);
		arr[n - 1].changeColor(theme.getElemColor(), theme.getTextColor());


	}
	void delNoVisual(int idx) {
		n++;
		for (int i = idx; i < n - 1; i++) {
			arr[i].val = arr[i + 1].val;
		}
		arr[n - 1] = ArrayElement();

		n--;
		arr[n - 1].changeColor(theme.getElemColor(), theme.getTextColor());
	}
	void searchStep(string val, int step,int n) {
		for (int i = 0; i < 100; i++) arr[i] = tempArr[i];
		int currStep = 0;
		if (step == 0) return;
		vector<string> codeSearch = { 
			"for(int i=0;i<n;i++){",
			"    if(arr[i] == target){",
			"        targetIndex = i;",
			"        return targetIndex; }",
			"    else continue;",
			"}",
			"return NOT_FOUND;"
		};
		for (int i = 0; i < n; i++) {
			currStep++;
			if (currStep == step) { linesDisplay = 0;code = codeSearch; return; }
			if (arr[i].val == val) {
				currStep++;
				if (currStep == step) { linesDisplay = 1; code = codeSearch; return; }
				arr[i].changeColor(GREEN);
				currStep++;
				if (currStep == step) { linesDisplay = 2; code = codeSearch; return; }
				arr[i].changeColor(theme.getElemColor(), theme.getTextColor());
				currStep++;
				if (currStep == step) { linesDisplay = 3; code = codeSearch; maxStep = currStep; return; }
				return;
			}
			else arr[i].changeColor(RED);
			currStep++;
			if (currStep == step) { linesDisplay = 4; code = codeSearch; return; }
			arr[i].changeColor(theme.getElemColor(), theme.getTextColor());
		}
		currStep++;
		if (currStep == step) {
			linesDisplay = 0; code = codeSearch; return;
		}
		currStep++;
		if (currStep == step) {
			linesDisplay = 6; code = codeSearch; maxStep = currStep; return;
		}
		
	}
	void search(string val) {
		vector<string> codeSearch = { "for(int i=0;i<n;i++){", "    if(arr[i] == target){","        targetIndex = i;","        break; }","    else continue;","}"};
		for (int i = 0; i < n; i++) {
			updateFrame(codeSearch, 0);
			if (arr[i].val == val) {
				updateFrame(codeSearch, 1);
				arr[i].changeColor(GREEN);
				updateFrame(codeSearch, 2);
				arr[i].changeColor(theme.getElemColor(), theme.getTextColor());
				updateFrame(codeSearch, 3);
				return;
			}
			else arr[i].changeColor(RED);
			updateFrame(codeSearch, 4);
			arr[i].changeColor(theme.getElemColor(), theme.getTextColor());
		}
		updateFrame(codeSearch, 0);
		updateFrame(codeSearch, 6);

	}
	void updateStep(int idx, string val,int step, int n) {
		for (int i = 0; i < 100; i++) arr[i] = tempArr[i];
		int currStep = 0;
		if (step == 0) return;
		vector<string> codeUpdate = { "arr[indexUpdate] = valueUpdate;" };
		arr[idx].val = val;
		arr[idx].changeColor(GREEN);
		currStep++;
		if (currStep == step) { linesDisplay = 0; code = codeUpdate; return; }
		arr[idx].changeColor(theme.getElemColor(), theme.getTextColor());
		code = codeUpdate;
		linesDisplay = -1;
		maxStep = 2;
	}
	void update(int idx, string val) {
		vector<string> codeUpdate = {"arr[indexUpdate] = valueUpdate;"};
		arr[idx].val = val;
		arr[idx].changeColor(GREEN);
		updateFrame(codeUpdate, 0);
		arr[idx].changeColor(theme.getElemColor(), theme.getTextColor());
	}
};

class ListNode {
public:
	string val;
	ListNode* next;
	int centerX;
	int centerY;
	int radius;
	int arrowSX;
	int arrowSY;
	int arrowDX;
	int arrowDY;
	int textSize;
	SDL_Color colorNode;
	SDL_Color colorBg;
	SDL_Color colorArrow;
	SDL_Color colorText;
	TTF_Font* numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);

	ListNode() {
		this->val = "N";
		this->next = nullptr;
		this->centerX = 0;
		this->centerY = 0;
		this->radius = 0;
		this->textSize = theme.getTextSize();
		this->colorNode = theme.getElemColor();
		this->colorArrow = theme.getArrowColor();
		this->colorBg = theme.getBgElemColor();
		this->colorText = theme.getTextColor();
		this->arrowSX = centerX + radius;
		this->arrowSY = centerY;
		this->arrowDX = centerX + radius + 30;
		this->arrowDY = centerY;
	}

	ListNode(string val, int centerX, int centerY, int radius) {
		this->val = val;
		this->next = nullptr;
		this->centerX = centerX;
		this->centerY = centerY;
		this->radius = radius;
		this->textSize = theme.getTextSize();
		this->colorNode = theme.getElemColor();
		this->colorBg = theme.getBgElemColor();
		this->colorArrow = theme.getArrowColor();
		this->colorText = theme.getTextColor();
		this->arrowSX = centerX + radius;
		this->arrowSY = centerY;
		this->arrowDX = centerX + radius + 30;
		this->arrowDY = centerY;
	}
	ListNode(string val, int centerX, int centerY, int radius,ListNode* next) {
		this->val = val;
		this->next = next;
		this->centerX = centerX;
		this->centerY = centerY;
		this->radius = radius;
		this->textSize = theme.getTextSize();
		this->colorNode = theme.getElemColor();
		this->colorBg = theme.getBgElemColor();
		this->colorArrow = theme.getArrowColor();
		this->colorText = theme.getTextColor();
		this->arrowSX = centerX + radius;
		this->arrowSY = centerY;
		this->arrowDX = centerX + radius + 30;
		this->arrowDY = centerY;
	}

	void displayNode() {
		SDL_RenderDrawCircle(gRenderer, centerX, centerY, radius,colorNode);
		SDL_RenderFillCircle(gRenderer, centerX, centerY, radius - 1, colorBg);
		createText(numberFont, colorText, val.size() == 1 && val != "0" ? "0" + val : val, centerX - textSize/2, centerY - textSize/2, textSize, textSize);
		if (next) {
			DrawArrow(gRenderer, arrowSX, arrowDX, arrowSY, arrowDY,8,colorArrow);
		}
	}
	void setColorNode(SDL_Color newColor) {
		colorNode = newColor;
	}
	void setColorText(SDL_Color newColor) {
		colorText = newColor;
	}
	void setColorArrow(SDL_Color newColor) {
		colorArrow = newColor;
	}
	void setColorBg(SDL_Color newColor) {
		colorBg = newColor;
	}
	void setTextSize(int size) {
		textSize = size;
	}
	void setArrowPos(int x1, int y1,int x2,int y2) {
		arrowSX = x1;
		arrowSY = y1;
		arrowDX = x2;
		arrowDY = y2;
	}
	void setCenterPos(int x, int y) {
		centerX = x;
		centerY = y;
	}
};
class DoublyListNode {
public:
	string val;
	DoublyListNode* next;
	DoublyListNode* prev;
	int centerX;
	int centerY;
	int radius;
	int arrowSX;
	int arrowSY;
	int arrowDX;
	int arrowDY;
	int arrowSXPrev;
	int arrowSYPrev;
	int arrowDXPrev;
	int arrowDYPrev;
	int textSize;
	SDL_Color colorNode;
	SDL_Color colorBg;
	SDL_Color colorArrow;
	SDL_Color colorText;
	TTF_Font* numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);

	DoublyListNode() {
		this->val = "N";
		this->next = nullptr;
		this->prev = nullptr;
		this->centerX = 0;
		this->centerY = 0;
		this->radius = 0;
		this->textSize = theme.getTextSize();
		this->colorNode = theme.getElemColor();
		this->colorBg = theme.getBgElemColor();
		this->colorArrow = theme.getArrowColor();
		this->colorText = theme.getTextColor();
		setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius + 30, centerY - 10);
		setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius - 30, centerY + 10);
	}

	DoublyListNode(string val, int centerX, int centerY, int radius) {
		this->val = val;
		this->next = nullptr;
		this->prev = nullptr;
		this->centerX = centerX;
		this->centerY = centerY;
		this->radius = radius;
		this->textSize = theme.getTextSize();
		this->colorNode = theme.getElemColor();
		this->colorBg = theme.getBgElemColor();
		this->colorArrow = theme.getArrowColor();
		this->colorText = theme.getTextColor();
		setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius + 30, centerY - 10);
		setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius - 30, centerY + 10);
	}
	DoublyListNode(string val, int centerX, int centerY, int radius, DoublyListNode* next,DoublyListNode* prev ) {
		this->val = val;
		this->next = next;
		this->prev = prev;
		this->centerX = centerX;
		this->centerY = centerY;
		this->radius = radius;
		this->textSize = theme.getTextSize();
		this->colorNode = theme.getElemColor();
		this->colorBg = theme.getBgElemColor();
		this->colorArrow = theme.getArrowColor();
		this->colorText = theme.getTextColor();
		setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius + 30, centerY - 10);
		setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius - 30, centerY + 10);
	}

	void displayNode() {
		SDL_RenderDrawCircle(gRenderer, centerX, centerY, radius, colorNode);
		SDL_RenderFillCircle(gRenderer, centerX, centerY, radius - 1,colorBg);
		createText(numberFont, colorText, val.size() == 1 && val != "0" ? "0" + val : val, centerX - textSize / 2, centerY - textSize / 2, textSize, textSize);
		if (next) {
			DrawArrow(gRenderer, arrowSX, arrowDX, arrowSY, arrowDY, 8, colorArrow);
		}
		if (prev) {
			DrawArrow(gRenderer, arrowSXPrev, arrowDXPrev, arrowSYPrev, arrowDYPrev, 8, colorArrow);
		}
	}
	void setColorNode(SDL_Color newColor) {
		colorNode = newColor;
	}
	void setColorArrow(SDL_Color newColor) {
		colorArrow = newColor;
	}
	void setColorText(SDL_Color newColor) {
		colorText = newColor;
	}
	void setArrowNextPos(int x1, int y1, int x2, int y2) {
		arrowSX = x1;
		arrowSY = y1;
		arrowDX = x2;
		arrowDY = y2;
	}
	void setArrowPrevPos(int x1, int y1, int x2, int y2) {
		arrowSXPrev = x1;
		arrowSYPrev = y1;
		arrowDXPrev = x2;
		arrowDYPrev = y2;
	}
	void setCenterPos(int x, int y) {
		centerX = x;
		centerY = y;
	}
	void setArrowNextNormal() {
		setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius + 30, centerY - 10);
	}
	void setArrowPrevNormal() {
		setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius - 30, centerY + 10);
	}
	void setArrowNextX2() {
		setArrowNextPos(centerX + radius - 3, centerY - 10, centerX + radius*3 + 30*2+10, centerY - 10);
	}
	void setArrowPrevX2() {
		setArrowPrevPos(centerX - radius + 3, centerY + 10, centerX - radius*3 - 30*2-10, centerY + 10);
	}
	void setArrowNextUp() {
		setArrowNextPos(centerX-10, centerY - radius+3, centerX-10, centerY - radius - 40);
	}
	void setArrowNextDown() {
		setArrowNextPos(centerX + 10, centerY + radius - 3, centerX + 10, centerY + radius + 40);
	}
	void setArrowPrevDown() {
		setArrowPrevPos(centerX+10, centerY + radius-3, centerX+10, centerY + radius + 40);
	}
	void setArrowPrevUp() {
		setArrowPrevPos(centerX - 10, centerY - radius + 3, centerX - 10, centerY - radius - 40);
	}
	void setArrowPrevDownLeft() {
		setArrowPrevPos(centerX - 18, centerY + 18, centerX - 70, centerY + 70);
	}
	void setArrowPrevUpLeft() {
		setArrowPrevPos(centerX - radius+2, centerY-11, centerX - 78, centerY - 66);
	}
	void setArrowNextUpperRight() {
		setArrowNextPos(centerX + radius -2, centerY - 11, centerX + 78, centerY - 66);
	}
	void setArrowNextDownRight() {
		setArrowNextPos(centerX + 18, centerY + 18, centerX + 70, centerY + 70);
	}
};
class LinkedListVisualizer {
private:
	SDL_Color bgColor;
public:
	ListNode* head;
	int listSize;
	LinkedListVisualizer() {
		head = nullptr;
		listSize = 0;
		bgColor = theme.getBgColor();
	}
	void freeMem() {
		ListNode* current = head;
		while (listSize--) {
			ListNode* next = current->next;
			delete current;
			current = next;
		}
		head = nullptr;

	}
	virtual void visualize() {
		ListNode* clone = head;
		while (clone) {
			clone->displayNode();
			clone = clone->next;
		}
	}
	void initialize(vector<string> list) {
		freeMem();
		this->listSize = list.size();
		if (list.size() == 0) return;
		head = new ListNode(list[0], 100, 100, 25);
		auto clone = head;
		for (int i = 1; i < list.size(); i++) {
			clone->next = new ListNode(list[i], 100 + 90*i, 100, 25);
			clone = clone->next;
		}
	}
	void reCalcCoordinate() {
		ListNode* clone = head;
		if (!clone) return;
		clone->centerX = 100;
		clone->centerY = 100;
		clone->radius = 25;
		int i = 1;
		while (true) {
			clone = clone->next;
			if (!clone) break;
			clone->centerX = 100+90*i;
			clone->centerY = 100;
			i++;
		}
	}
	ListNode* getNode(int idx) {
		ListNode* clone = head;
		while (idx--) {
			clone = clone->next;
		}
		return clone;
	}
	void resetColor() {
		ListNode* clone = head;
		while (clone) {
			clone->setColorNode(theme.getElemColor());
			clone->setColorArrow(theme.getArrowColor());
			clone->setColorText(theme.getTextColor());
			clone = clone->next;
		}
	}
	void resetArrow() {
		ListNode* clone = head;
		while (clone) {
			clone->setArrowPos(clone->centerX + clone->radius,clone->centerY, clone->centerX + clone->radius+30,clone->centerY);
			clone = clone->next;
		}
	}
	void insertStep(int idx, string val,bool checkStep) { 
		SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
		SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
		TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
		vector<string> code;
		if(idx)
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
		int totalSteps = idx?idx*2 + 2:3;
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
		int currStep = checkStep ? 0 : 1;
		int currHeadIndex = 0;
		int framesCount = 1;
		ListNode* newNode;
		ListNode* tmp = idx - 1 >= 0 ? getNode(idx - 1) : nullptr;
		if(tmp)	newNode = new ListNode(val, tmp->centerX, tmp->centerY + 100, 25);
		else newNode = new ListNode(val, head->centerX, head->centerY + 100, 25);
		ListNode* oldNode= getNode(idx);
		ListNode* currHead = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50,NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			//createText(generalFont, BLACK, to_string(currStep),0,0,50,50);
			t = 60 / (speed + 1);
			if (framesCount % t == 0&&!checkStep) currStep=min(currStep+1,totalSteps);
			currHead = getNode(currHeadIndex);
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW,checkStep);
			if(currStep==0){
				linesColor = -1;
				currHeadIndex = 0;
				resetColor(); 
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40); 
				visualize();
			}
			else if (currStep < totalSteps - 2) {
				resetColor();
				if (currStep % 2 == 0) {
					getNode(currStep/2)->setColorNode(RED);
					linesColor = 1;
					currHeadIndex = currStep / 2;
				}
				else linesColor = 0;

				visualize();
			}
			else {
				if (currStep == totalSteps - 2) {
					if(tmp)	tmp->next = oldNode;
					newNode->next = nullptr;
					visualize();
					newNode->displayNode();
					linesColor = idx?3:0;

				}
				else if (currStep == totalSteps - 1) {
					newNode->next = oldNode;
					if(tmp)	tmp->next = oldNode;
					if(oldNode)newNode->setArrowPos(idx?newNode->centerX+10:newNode->centerX,idx?newNode->centerY-newNode->radius+5:newNode->centerY - newNode->radius, idx ? oldNode->centerX - 25 : oldNode->centerX, idx?oldNode->centerY+25: oldNode->centerY + 35);
					if(tmp)	tmp->setArrowPos(tmp->centerX + tmp->radius, tmp->centerY, tmp->centerX + tmp->radius + 30, tmp->centerY);
					visualize();
					newNode->displayNode();
					linesColor = idx?4:1;
				}
				else {
					newNode->next = oldNode;
					if(tmp)	tmp->next = newNode;
					//if (oldNode)	newNode->setArrowPos(newNode->centerX, newNode->centerY - newNode->radius, idx ? oldNode->centerX - 25 : oldNode->centerX, oldNode->centerY + 25);
					if (tmp)	tmp->setArrowPos(tmp->centerX,tmp->centerY+tmp->radius,newNode->centerX, newNode->centerY -35);
					visualize();
					if(idx==0) newNode->displayNode();
					linesColor = idx?5:2;
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
			if(currHead==newNode)
				createText(generalFont, RED, "Head",currHead->centerX-currHead->radius+5,currHead->centerY+currHead->radius+5, 10*4, 20);
			else
				createText(generalFont, RED, "Head",currHead->centerX-currHead->radius+5,currHead->centerY-currHead->radius-25, 10*4, 20);
			if (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
			SDL_RenderPresent(gRenderer);
			framesCount++;
		}
		if(tmp)	tmp->next = newNode;
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
	void deleteNode(int idx) {
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
	int search(string val) {
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
	void searchStep(string val,bool checkStep) {
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
		int totalSteps = ans == -1 ? (listSize) * 4 +2 : ans * 4+3;
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
		int currStep = checkStep ? 0 : 1;
		int currHeadIndex = 0;
		int framesCount = 1;
		ListNode* currHead = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			//createText(generalFont, BLACK, to_string(currStep),0,0,50,50);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep=min(currStep+1,totalSteps);
			currHead = getNode(currHeadIndex);
			if (currStep == 0) {
				linesColor = -1;
				currHeadIndex = 0;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
					if (getNode(currHeadIndex)&& getNode(currHeadIndex)->val == val && currStep == totalSteps) {
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
			if(currHead)
				createText(generalFont, RED, "Head", currHead->centerX - currHead->radius + 5, currHead->centerY - currHead->radius - 25, 10 * 4, 20);
			if (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
			SDL_RenderPresent(gRenderer);
			framesCount++;
		}
		resetColor();
		SDL_DestroyTexture(buttonSpriteSheet);
		SDL_DestroyTexture(explain);
		TTF_CloseFont(generalFont);
		return;
	}
	void update(int idx, string newVal) {
		ListNode* clone = head;
		while (idx) {
			clone = clone->next;
			idx--;
		}
		clone->val = newVal;
	}
	void deleteStep(int idx, bool checkStep) {
		SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
		SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
		TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
		vector<string> code= {
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
		int totalSteps = 2+(idx-1)*3+2+1;
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
		int currStep = checkStep ? 0 : 1;
		int currIndex = -1;
		int prevIndex = -1;
		int framesCount = 1;
		ListNode* toDelete = getNode(idx);
		ListNode* prev = getNode(idx - 1);
		ListNode* aft = getNode(idx + 1);
		ListNode* currPtr = nullptr;
		ListNode* prevPtr = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if(!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false,900, 400, 50, 50, nullptr);
			//createText(generalFont, BLACK, to_string(currStep),0,0,50,50);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			if (currIndex >= 0)	currPtr = getNode(currIndex);
			else currPtr = nullptr;
			if (prevIndex >= 0)	prevPtr = getNode(prevIndex);
			else prevPtr = nullptr;
			if (currStep == 0) {
				linesColor = -1;
				currIndex = -1;
				prevIndex = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
					getNode(currStep / 3+1)->setColorNode(RED);
					linesColor = 3;
					currIndex = currStep / 3+1;
				}
				else {
					getNode(currStep / 3 )->setColorNode(GREEN);
					linesColor = 4;
					prevIndex = currStep / 3 ;
				}

				visualize();
			}
			else {
				if (currStep == totalSteps - 1) {
					if (!aft) { prev->next = nullptr; currPtr = toDelete; }
					if (aft)	prev->setArrowPos(prev->arrowSX, prev->arrowSY, aft->centerX - 35, aft->centerY);
					toDelete->setCenterPos(prev->centerX + 90, prev->centerY+100);
					if (aft)	toDelete->setArrowPos(toDelete->centerX + 25, toDelete->centerY - 25, aft->centerX - 25, aft->centerY + 25);
					visualize();
					toDelete->displayNode();
					linesColor = 6;

				}
				else {
					linesColor = 7;
					ListNode* clone = head;
					while (clone) {
						if(clone!=toDelete)	clone->displayNode();
						clone = clone->next;
					}
				}
			}
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW,checkStep);
			if (linesColor != -1) {
				for (int i = 0; i < code.size(); i++) {
					if (i == linesColor)
						createText(generalFont, RED, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
					else
						createText(generalFont, BLACK, code[i], 100, 250 + i * 50, code[i].size() * 16, 40);
				}
			}
			if(currPtr &&currStep!=totalSteps)	createText(generalFont, RED, "Curr", currPtr->centerX - currPtr->radius + 5, currPtr->centerY - currPtr->radius - 25, 10 * 4, 20);
			if (prevPtr)	createText(generalFont, RED, "Prev", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
			if (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void deleteStepHead(bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currIndex = -1;
		int prevIndex = -1;
		int framesCount = 1;
		ListNode* toDelete = head;
		ListNode* aft = head->next;
		ListNode* currPtr = nullptr;
		ListNode* prevPtr = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			if (currIndex >= 0)	currPtr = getNode(currIndex);
			else currPtr = nullptr;
			if (prevIndex >= 0)	prevPtr = getNode(prevIndex);
			else prevPtr = nullptr;
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW,checkStep);
			if (currStep == 0) {
				linesColor = -1;
				currIndex = -1;
				prevIndex = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
				visualize();
			}
			else if (currStep == 1) {
				linesColor = 0;
				currIndex = 0;
				prevIndex = 0;
				resetColor();
				visualize();
				if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius , currPtr->centerY + currPtr->radius + 5, 10 * 5, 20);
				if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
			}
			else if (currStep == 2) {
				head = toDelete;
				linesColor = 1;
				currIndex = 0;
				prevIndex = 1;
				resetColor();
				visualize();
				if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius , currPtr->centerY - currPtr->radius - 25, 10 * 5, 20);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void updateStep(int idx,string val, bool checkStep) {
		SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
		SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
		TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
		vector<string> code = { "while (index--){",
							 "    head = head->next;",
							 "}",
							 "head->val = updateValue;" };
		int linesColor = -1;
		int ans = search(val);
		int totalSteps = idx*2+2;
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
		int currStep = checkStep ? 0 : 1;
		int currHeadIndex = 0;
		int framesCount = 1;
		ListNode* currHead = nullptr;
		ListNode* updateNode = getNode(idx);
		string beforeUpdate = updateNode->val;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			currHead = getNode(currHeadIndex);
			if (currStep == 0) {
				linesColor = -1;
				currHeadIndex = 0;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
				visualize();
			}
			else if(currStep<totalSteps) {
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void insertEmpty(string val,bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int framesCount = 1;
		ListNode* tmpHead = nullptr;
		vector<string> code = {
			"head = new ListNode(value,nullptr);"
		};
		ListNode* newNode = new ListNode(val, 100, 100, 25);
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			else tmpHead = nullptr;
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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


};
class CircularLinkedListVisualizer :public LinkedListVisualizer {
private:
	ListNode* tail;
	SDL_Color bgColor;
public:
	CircularLinkedListVisualizer() {
		head = nullptr;
		tail = nullptr;
		listSize = 0;
		bgColor = theme.getBgColor();
	}
	void visualize() {
		ListNode* clone = head;
		int currrSize = listSize;
		while (currrSize--) {
			clone->displayNode();
			clone = clone->next;
		}
		if (tail && head) {
			SDL_Color old;
			SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
			SDL_Color arrow = theme.getArrowColor();
			SDL_SetRenderDrawColor(gRenderer,arrow.r,arrow.g,arrow.b,255);
			SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
			SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, head->centerX - 75, head->centerY + 50);
			SDL_RenderDrawLine(gRenderer, head->centerX - 75, head->centerY + 50, head->centerX - 75, head->centerY);
			DrawArrow(gRenderer, head->centerX - 75, head->centerX - 35, head->centerY, head->centerY, 8, theme.getArrowColor());
			SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
		}
	}
	void initialize(vector<string> list) {
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
	void insertStep(int idx, string val, bool checkStep) {
		SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
		SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
		TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
		vector<string> code= {
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
		int currStep = checkStep ? 0 : 1;
		int currHeadIndex = 0;
		int framesCount = 1;
		ListNode* newNode;
		ListNode* tmp =  getNode(idx - 1);
		newNode = new ListNode(val, tmp->centerX, tmp->centerY + 100, 25);
		ListNode* oldNode = getNode(idx);
		ListNode* currHead = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			//createText(generalFont, BLACK, to_string(currStep),0,0,50,50);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			currHead = getNode(currHeadIndex);
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				currHeadIndex = 0;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
					//tmp->next = oldNode;
					newNode->next = nullptr;
					visualize();
					newNode->displayNode();
					linesColor = idx ? 3 : 0;

				}
				else if (currStep == totalSteps - 1) {
					newNode->next = oldNode;
					//tmp->next = oldNode;
					newNode->setArrowPos(newNode->centerX + 10,newNode->centerY - newNode->radius + 5 , oldNode->centerX - 25 , oldNode->centerY + 25);
					tmp->setArrowPos(tmp->centerX + tmp->radius, tmp->centerY, tmp->centerX + tmp->radius + 30, tmp->centerY);
					visualize();
					newNode->displayNode();
					linesColor = idx ? 4 : 1;
				}
				else {
					//newNode->next = oldNode;
					//tmp->next = newNode;
					//if (oldNode)	newNode->setArrowPos(newNode->centerX, newNode->centerY - newNode->radius, idx ? oldNode->centerX - 25 : oldNode->centerX, oldNode->centerY + 25);
					tmp->setArrowPos(tmp->centerX, tmp->centerY + tmp->radius, newNode->centerX, newNode->centerY - 35);
					visualize();
					newNode->displayNode();
					linesColor = 5 ;
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void insertStepHead(int idx,string val,bool checkStep){
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
		ListNode* newNode = new ListNode(val, head->centerX , head->centerY+100, 25);
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			if (headIndex >= 0)	tmpHead = getNode(headIndex);
			else tmpHead = nullptr;
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				headIndex = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
				newNode->setArrowPos(newNode->centerX, newNode->centerY-newNode->radius, newNode->centerX, newNode->centerY-newNode->radius-40);
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
					SDL_Color arrow = theme.getArrowColor();
					SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 100);
					//SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 75, head->centerX - 75, head->centerY + 75);
					//SDL_RenderDrawLine(gRenderer, head->centerX - 75, head->centerY + 75, head->centerX - 75, head->centerY);
					DrawArrow(gRenderer, tail->centerX, head->centerX + 35, tail->centerY + 100, head->centerY + 100, 8, theme.getArrowColor());
					SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
				}
				newNode->displayNode();
				if (tmpHead)	createText(generalFont, RED, "Head", tmpHead->centerX - tmpHead->radius + 5, tmpHead->centerY - tmpHead->radius - 25, 10 * 4, 20);
			}
			else {
				linesColor = 3;
				headIndex = 0;
				newNode->setArrowPos(newNode->centerX, newNode->centerY-newNode->radius, newNode->centerX , newNode->centerY - newNode->radius - 40);
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
					SDL_Color arrow = theme.getArrowColor();
					SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 100);
					//SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 75, head->centerX - 75, head->centerY + 75);
					//SDL_RenderDrawLine(gRenderer, head->centerX - 75, head->centerY + 75, head->centerX - 75, head->centerY);
					DrawArrow(gRenderer, tail->centerX, head->centerX + 35, tail->centerY + 100, head->centerY + 100, 8, theme.getArrowColor());
					SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
				}
				newNode->displayNode();
				createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY + newNode->radius +5, 10 * 4, 20);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void insertStepTail(int idx, string val, bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int framesCount = 1;
		ListNode* tmpTail = nullptr;
		vector<string> code = {
			"ListNode* newNode = new ListNode(value,nullptr);",
			"newNode->next = head;",
			"tail->next = newNode;",
			"tail = tail->next"
		};
		ListNode* newNode = new ListNode(val, head->centerX, head->centerY + 100, 25);
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			if (currStep != 0) {
				tmpTail = tail;
			}
			else tmpTail = tail;
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
					SDL_Color arrow = theme.getArrowColor();
					SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 100);
					//SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 75, head->centerX - 75, head->centerY + 75);
					//SDL_RenderDrawLine(gRenderer, head->centerX - 75, head->centerY + 75, head->centerX - 75, head->centerY);
					DrawArrow(gRenderer, tail->centerX, head->centerX + 35, tail->centerY + 100, head->centerY + 100, 8, theme.getArrowColor());
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
					SDL_Color arrow = theme.getArrowColor();
					SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 100);
					//SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 75, head->centerX - 75, head->centerY + 75);
					//SDL_RenderDrawLine(gRenderer, head->centerX - 75, head->centerY + 75, head->centerX - 75, head->centerY);
					DrawArrow(gRenderer, tail->centerX, head->centerX + 35, tail->centerY + 100, head->centerY + 100, 8, theme.getArrowColor());
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void deleteStep(int idx, bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currIndex = -1;
		int prevIndex = -1;
		int framesCount = 1;
		ListNode* toDelete = getNode(idx);
		ListNode* prev = getNode(idx - 1);
		ListNode* aft = getNode(idx + 1);
		ListNode* currPtr = nullptr;
		ListNode* prevPtr = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			//createText(generalFont, BLACK, to_string(currStep),0,0,50,50);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			if (currIndex >= 0)	currPtr = getNode(currIndex);
			else currPtr = nullptr;
			if (prevIndex >= 0)	prevPtr = getNode(prevIndex);
			else prevPtr = nullptr;
			if (currStep == 0) {
				linesColor = -1;
				currIndex = -1;
				prevIndex = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
						//tail->DrawArrow(gRenderer, head->centerX - 75, head->centerX - 35, head->centerY, head->centerY, 8, BLACK);
						prev->setArrowPos(head->centerX - 75, head->centerY, head->centerX - 35, head->centerY);
						SDL_Color old;
						SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
						SDL_Color arrow = theme.getArrowColor();
						SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
						SDL_RenderDrawLine(gRenderer, prev->centerX, prev->centerY + 25, prev->centerX, prev->centerY + 50);
						SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
						currPtr = toDelete;
					}
					if (aft)	prev->setArrowPos(prev->arrowSX, prev->arrowSY, aft->centerX - 35, aft->centerY);
					if(aft)	toDelete->setCenterPos(prev->centerX + 90, prev->centerY + 100);
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
						SDL_Color arrow = theme.getArrowColor();
						SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
						SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
						SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, head->centerX - 75, head->centerY + 50);
						SDL_RenderDrawLine(gRenderer, head->centerX - 75, head->centerY + 50, head->centerX - 75, head->centerY);
						DrawArrow(gRenderer, head->centerX - 75, head->centerX - 35, head->centerY, head->centerY, 8, theme.getArrowColor());
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void searchStep(string val, bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currHeadIndex = 0;
		int framesCount = 1;
		ListNode* currHead = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			//createText(generalFont, BLACK, to_string(currStep),0,0,50,50);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			currHead = getNode(currHeadIndex);
			if (currStep == 0) {
				linesColor = -1;
				currHeadIndex = 0;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
			SDL_RenderPresent(gRenderer);
			framesCount++;
		}
		resetColor();
		SDL_DestroyTexture(buttonSpriteSheet);
		SDL_DestroyTexture(explain);
		TTF_CloseFont(generalFont);
		return;
	}
	void deleteStepHead(bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currIndex = -1;
		int prevIndex = -1;
		int framesCount = 1;
		ListNode* toDelete = head;
		ListNode* aft = head->next;
		ListNode* currPtr = nullptr;
		ListNode* prevPtr = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
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
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
					SDL_Color arrow = theme.getArrowColor();
					SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
					if (tail == aft) {
						SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, aft->centerX-55, aft->centerY + 50);
						SDL_RenderDrawLine(gRenderer, aft->centerX - 55, aft->centerY + 50, aft->centerX - 55, aft->centerY );
					}
					else {
						SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, aft->centerX, aft->centerY + 50);
						DrawArrow(gRenderer, aft->centerX, aft->centerX, aft->centerY + 50, aft->centerY + 35, 8, theme.getArrowColor());
					}
					SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
				}
				else if (!aft) {
					SDL_Color old;
					SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
					SDL_Color arrow = theme.getArrowColor();
					SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, tail->centerX-75, tail->centerY + 50);
					SDL_RenderDrawLine(gRenderer, tail->centerX-75, tail->centerY + 50, tail->centerX-75, tail->centerY);
					DrawArrow(gRenderer, tail->centerX - 75, tail->centerX-35, tail->centerY, tail->centerY, 8, theme.getArrowColor());
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
					SDL_Color arrow = theme.getArrowColor();
					SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
					SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 25, tail->centerX, tail->centerY + 50);
					if (tail == aft) {
						SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, aft->centerX - 55, aft->centerY + 50);
						SDL_RenderDrawLine(gRenderer, aft->centerX - 55, aft->centerY + 50, aft->centerX - 55, aft->centerY);
						DrawArrow(gRenderer, aft->centerX - 55, aft->centerX-35, aft->centerY, aft->centerY, 8, theme.getArrowColor());
					}
					else {
						SDL_RenderDrawLine(gRenderer, tail->centerX, tail->centerY + 50, aft->centerX, aft->centerY + 50);
						DrawArrow(gRenderer, aft->centerX, aft->centerX, aft->centerY + 50, aft->centerY + 35, 8, theme.getArrowColor());
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void insertEmpty(string val, bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int framesCount = 1;
		ListNode* tmpHead = nullptr;
		vector<string> code = {
			"head = new ListNode(value,nullptr);",
			"tail = head;",
			"tail->next = head;"
		};
		ListNode* newNode = new ListNode(val, 100, 100, 25);
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			else tmpHead = nullptr;
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
				visualize();
			}
			else if (currStep == 1) {
				linesColor = 0;
				resetColor();
				visualize();
				newNode->displayNode();
				createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY - newNode->radius - 25, 10 * 4, 20);
			}
			else if(currStep==2) {
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
				SDL_Color arrow = theme.getArrowColor();
				SDL_SetRenderDrawColor(gRenderer, arrow.r, arrow.g, arrow.b, 255);
				SDL_RenderDrawLine(gRenderer, newNode->centerX, newNode->centerY + 25, newNode->centerX, newNode->centerY + 50);
				SDL_RenderDrawLine(gRenderer, newNode->centerX, newNode->centerY + 50, newNode->centerX - 75, newNode->centerY + 50);
				SDL_RenderDrawLine(gRenderer, newNode->centerX - 75, newNode->centerY + 50, newNode->centerX - 75, newNode->centerY);
				DrawArrow(gRenderer, newNode->centerX - 75, newNode->centerX - 35, newNode->centerY, newNode->centerY, 8, theme.getArrowColor());
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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


};
class DoublyLinkedListVisualizer {
private:
	SDL_Color bgColor;
public:
	DoublyListNode* head;
	int listSize;
	DoublyLinkedListVisualizer() {
		head = nullptr;
		listSize = 0;
		bgColor = theme.getBgColor();
	}
	void freeMem() {
		DoublyListNode* current = head;
		while (listSize--) {
			DoublyListNode* next = current->next;
			delete current;
			current = next;
		}
		head = nullptr;

	}
	void visualize() {
		DoublyListNode* clone = head;
		while (clone) {
			clone->displayNode();
			clone = clone->next;
		}
	}
	void initialize(vector<string> list) {
		freeMem();
		this->listSize = list.size();
		if (list.size() == 0) return;
		head = new DoublyListNode(list[0], 100, 100, 25);
		auto clone = head;
		for (int i = 1; i < list.size(); i++) {
			clone->next = new DoublyListNode(list[i], 100 + 90 * i, 100, 25,nullptr,clone);
			clone = clone->next;
		}
	}
	void reCalcCoordinate() {
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
	DoublyListNode* getNode(int idx) {
		DoublyListNode* clone = head;
		while (idx--) {
			clone = clone->next;
		}
		return clone;
	}
	void resetColor() {
		DoublyListNode* clone = head;
		while (clone) {
			clone->setColorNode(theme.getElemColor());
			clone->setColorArrow(theme.getArrowColor());
			clone->setColorText(theme.getTextColor());
			clone = clone->next;
		}
	}
	void resetArrow() {
		DoublyListNode* clone = head;
		while (clone) {
			clone->setArrowNextNormal();
			clone->setArrowPrevNormal();
			clone = clone->next;
		}
	}
	int search(string val) {
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
	void searchStep(string val, bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currHeadIndex = 0;
		int framesCount = 1;
		DoublyListNode* currHead = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			//createText(generalFont, BLACK, to_string(currStep),0,0,50,50);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			currHead = getNode(currHeadIndex);
			if (currStep == 0) {
				linesColor = -1;
				currHeadIndex = 0;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
			SDL_RenderPresent(gRenderer);
			framesCount++;
		}
		resetColor();
		SDL_DestroyTexture(buttonSpriteSheet);
		SDL_DestroyTexture(explain);
		TTF_CloseFont(generalFont);
		return;
	}
	void updateStep(int idx, string val, bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currHeadIndex = 0;
		int framesCount = 1;
		DoublyListNode* currHead = nullptr;
		DoublyListNode* updateNode = getNode(idx);
		string beforeUpdate = updateNode->val;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			currHead = getNode(currHeadIndex);
			if (currStep == 0) {
				linesColor = -1;
				currHeadIndex = 0;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void insertStep(int idx, string val, bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currHeadIndex = 0;
		int framesCount = 1;
		DoublyListNode* newNode;
		DoublyListNode* tmp = getNode(idx - 1);
		newNode = new DoublyListNode(val, tmp->centerX, tmp->centerY + 100, 25);
		DoublyListNode* oldNode = getNode(idx);
		DoublyListNode* currHead = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			//createText(generalFont, BLACK, to_string(currStep),0,0,50,50);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			currHead = getNode(currHeadIndex);
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				currHeadIndex = 0;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
					//tmp->next = oldNode;
					newNode->next = oldNode;
					newNode->prev = tmp;
					newNode->setArrowPrevUp();
					newNode->setArrowNextUpperRight();
					if (oldNode) {
						oldNode->setArrowPrevNormal();
					}
					visualize();
					newNode->displayNode();
					linesColor =3;

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
					linesColor =4;
				}
				else {
					//newNode->next = oldNode;
					tmp->next = newNode;
					//if (oldNode)	newNode->setArrowPos(newNode->centerX, newNode->centerY - newNode->radius, idx ? oldNode->centerX - 25 : oldNode->centerX, oldNode->centerY + 25);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
			SDL_RenderPresent(gRenderer);
			framesCount++;
		}
		newNode->next = oldNode;
		newNode->prev = tmp;
		if(oldNode) oldNode->prev = newNode;
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
	void insertStepHead(string val, bool checkStep) {
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
		DoublyListNode* newNode = new DoublyListNode(val, head?head->centerX:100 , head?head->centerY+100:100 , 25);
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			if (headIndex >= 0 )	tmpHead = getNode(headIndex);
			else tmpHead = nullptr;
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				headIndex = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void deleteStep(int idx, bool checkStep) {
		
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
		int currStep = checkStep ? 0 : 1;
		int currIndex = -1;
		int prevIndex = -1;
		int framesCount = 1;
		DoublyListNode* toDelete = getNode(idx);
		DoublyListNode* prev = getNode(idx - 1);
		DoublyListNode* aft = getNode(idx + 1);
		DoublyListNode* currPtr = nullptr;
		DoublyListNode* prevPtr = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 400, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			if (currIndex >= 0)	currPtr = getNode(currIndex);
			else currPtr = nullptr;
			if (prevIndex >= 0)	prevPtr = getNode(prevIndex);
			else prevPtr = nullptr;
			if (currStep == 0) {
				linesColor = -1;
				currIndex = -1;
				prevIndex = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
					currPtr = toDelete;
					if (aft) {
						//aft->setArrowPrevPos(aft->centerX - aft->radius, aft->centerY, prev->centerX + 35, prev->centerY);
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
					if (!aft) { prev->next = nullptr;  }
					if (aft) {
						prev->setArrowNextX2();
						//aft->setArrowPrevPos(aft->centerX - aft->radius, aft->centerY, prev->centerX + 35, prev->centerY);
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
				if(currStep<totalSteps-3)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY - currPtr->radius - 25, 10 * 5, 20);
				else createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY + currPtr->radius + 5, 10 * 5, 20);
			}
			if (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.x > 850 && e.button.x < 950 && e.button.y > 400 && e.button.y < 450) { quit = true; }
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
			SDL_RenderPresent(gRenderer);
			framesCount++;
		}
		prev->next = aft;
		if(aft) aft->prev = prev;
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
	void deleteStepHead(bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currIndex = -1;
		int prevIndex = -1;
		int framesCount = 1;
		DoublyListNode* toDelete = head;
		DoublyListNode* aft = head->next;
		DoublyListNode* currPtr = nullptr;
		DoublyListNode* prevPtr = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
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
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void insertEmpty(string val, bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int framesCount = 1;
		vector<string> code = {
			"head = new DLLNode(value,nullptr,nullptr);"
		};
		DoublyListNode* newNode = new DoublyListNode(val, 100, 100, 25);
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
};
class StackVisualizer {
private:
	SDL_Color bgColor;
public:
	ListNode* head;
	int listSize;
	StackVisualizer() {
		head = nullptr;
		listSize = 0;
		bgColor = theme.getBgColor();
	}
	void freeMem() {
		ListNode* current = head;
		while (current) {
			ListNode* next = current->next;
			delete current;
			current = next;
		}
		head = nullptr;

	}
	void visualize() {
		ListNode* clone = head;
		while (clone) {
			clone->displayNode();
			clone = clone->next;
		}
	}
	void initialize(vector<string> list) {
		freeMem();
		this->listSize = list.size();
		if (list.size() == 0) return;
		head = new ListNode(list[0], 100, 100, 25);
		auto clone = head;
		for (int i = 1; i < list.size(); i++) {
			clone->next = new ListNode(list[i], 100, 100+90*i, 25);
			clone = clone->next;
		}
		resetArrow();
	}
	void reCalcCoordinate() {
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
			clone->centerY = 100+90*i;
			i++;
		}
	}
	void resetColor() {
		ListNode* clone = head;
		while (clone) {
			clone->setColorNode(theme.getElemColor());
			clone->setColorArrow(theme.getArrowColor());
			clone->setColorText(theme.getTextColor());
			clone = clone->next;
		}
	}
	void resetArrow() {
		ListNode* clone = head;
		while (clone) {
			clone->setArrowPos(clone->centerX, clone->centerY + clone->radius, clone->centerX , clone->centerY + clone->radius + 30);
			clone = clone->next;
		}
	}
	ListNode* getNode(int idx) {
		ListNode* clone = head;
		while (idx--) {
			clone = clone->next;
		}
		return clone;
	}
	void popStep(bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currIndex = -1;
		int prevIndex = -1;
		int framesCount = 1;
		ListNode* toDelete = head;
		ListNode* aft = head->next;
		ListNode* currPtr = nullptr;
		ListNode* prevPtr = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
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
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 140, 250, 840, 40);
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
				//if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX + currPtr->radius + 5, currPtr->centerY - 10, 10 * 4, 20);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void pushStep(string val, bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int headIndex = -1;
		int framesCount = 1;
		ListNode* tmpHead = nullptr;
		vector<string> code = {
			"ListNode* newNode = new ListNode(value,nullptr);",
			"newNode->next = head;",
			"head = newNode;"
		};
		ListNode* newNode = new ListNode(val, head?head->centerX+100:100, head?head->centerY:100 , 25);
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			if (headIndex >= 0)	tmpHead = getNode(headIndex);
			else tmpHead = nullptr;
			highlightCode(generalFont, explain, code, linesColor, 200, 200, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				headIndex = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 140, 250, 840, 40);
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
				newNode->setArrowPos(newNode->centerX - 25, newNode->centerY, newNode->centerX -65, newNode->centerY);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
};
class QueueVisualizer {
private:
	SDL_Color bgColor;
public:
	ListNode* head;
	ListNode* tail;
	int listSize;
	QueueVisualizer() {
		head = nullptr;
		tail = nullptr;
		listSize = 0;
		bgColor = theme.getBgColor();
	}
	void freeMem() {
		ListNode* current = head;
		while (current) {
			ListNode* next = current->next;
			delete current;
			current = next;
		}
		head = nullptr;
		tail = nullptr;

	}
	void visualize() {
		ListNode* clone = head;
		while (clone) {
			clone->displayNode();
			clone = clone->next;
		}
	}
	void initialize(vector<string> list) {
		freeMem();
		this->listSize = list.size();
		if (list.size() == 0) return;
		head = new ListNode(list[0], 100, 100, 25);
		auto clone = head;
		for (int i = 1; i < list.size(); i++) {
			clone->next = new ListNode(list[i], 100 + 90 * i, 100, 25);
			if (i == list.size() - 1) tail = clone;
			clone = clone->next;
		}
		resetArrow();
	}
	void reCalcCoordinate() {
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
	void resetColor() {
		ListNode* clone = head;
		while (clone) {
			clone->setColorNode(theme.getElemColor());
			clone->setColorArrow(theme.getArrowColor());
			clone->setColorText(theme.getTextColor());
			clone = clone->next;
		}
	}
	void resetArrow() {
		ListNode* clone = head;
		while (clone) {
			clone->setArrowPos(clone->centerX + clone->radius, clone->centerY, clone->centerX + clone->radius + 30, clone->centerY);
			clone = clone->next;
		}
	}
	ListNode* getNode(int idx) {
		ListNode* clone = head;
		while (idx--) {
			clone = clone->next;
		}
		return clone;
	}
	void popStep(bool checkStep) {
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
		int currStep = checkStep ? 0 : 1;
		int currIndex = -1;
		int prevIndex = -1;
		int framesCount = 1;
		ListNode* toDelete = head;
		ListNode* aft = head->next;
		ListNode* currPtr = nullptr;
		ListNode* prevPtr = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
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
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
				visualize();
			}
			else if (currStep == 1) {
				linesColor = 0;
				currIndex = 0;
				prevIndex = 0;
				resetColor();
				visualize();
				if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY +currPtr->radius+5, 10 * 5, 20);
				if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius - 25, 10 * 4, 20);
			}
			else if (currStep == 2) {
				head = toDelete;
				linesColor = 1;
				currIndex = 0;
				prevIndex = 1;
				resetColor();
				visualize();
				if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX - currPtr->radius, currPtr->centerY - currPtr->radius-25, 10 * 5, 20);
				if (prevPtr)	createText(generalFont, RED, "Head", prevPtr->centerX - prevPtr->radius + 5, prevPtr->centerY - prevPtr->radius-25, 10 * 4, 20);

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
				//if (currPtr)	createText(generalFont, RED, "toDel", currPtr->centerX + currPtr->radius + 5, currPtr->centerY - 10, 10 * 4, 20);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
	void pushStep(string val, bool checkStep) {
		SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
		SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
		TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
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
		int currStep = checkStep ? 0 : 1;
		int framesCount = 1;
		vector<string> code = {
			"ListNode* newNode = new ListNode(value,nullptr);",
			"tail->next = newNode;",
			"tail = tail->next;"
		};
		ListNode* newNode = new ListNode(val, tail->centerX + 100, tail->centerY, 25);
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
				visualize();
			}
			else if (currStep == 1) {
				tail->next = nullptr;
				linesColor = 0;
				resetColor();
				visualize();
				newNode->displayNode();
				createText(generalFont, RED, "Tail", tail->centerX - tail->radius + 5, tail->centerY - tail->radius - 25, 10 * 4, 20);
			}
			else if (currStep == 2) {
				tail->next = newNode;
				linesColor = 1;
				tail->setArrowPos(tail->centerX + 25, tail->centerY, tail->centerX + 65, tail->centerY);
				resetColor();
				visualize();
				newNode->displayNode();
				createText(generalFont, RED, "Tail", tail->centerX - tail->radius + 5, tail->centerY - tail->radius - 25, 10 * 4, 20);
			}
			else {
				linesColor = 2;
				tail->setArrowPos(tail->centerX + 25, tail->centerY, tail->centerX + 65, tail->centerY);
				resetColor();
				visualize();
				newNode->displayNode();
				createText(generalFont, RED, "Tail", newNode->centerX - newNode->radius + 5, newNode->centerY - newNode->radius - 25, 10 * 4, 20);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
			SDL_RenderPresent(gRenderer);
			framesCount++;
		}
		tail->next = newNode;
		tail = tail->next;
		listSize++;
		reCalcCoordinate();
		resetColor();
		resetArrow();
		SDL_DestroyTexture(buttonSpriteSheet);
		SDL_DestroyTexture(explain);
		TTF_CloseFont(generalFont);
		return;

	}
	void insertEmpty(string val, bool checkStep) {
		SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.png");
		SDL_Texture* explain = loadImgTexture("resources/Prompt/Explanation.png");
		TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 92);
		int linesColor = -1;
		int totalSteps = 2;
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
		int currStep = checkStep ? 0 : 1;
		int framesCount = 1;
		ListNode* tmpHead = nullptr;
		vector<string> code = {
			"head = new ListNode(value,nullptr);",
			"tail = head;"
		};
		ListNode* newNode = new ListNode(val, 100, 100, 25);
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
			if (!checkStep)	createButton(buttonSpriteSheet, &buttonPosArray[speed][0], &buttonPosArray[speed][1], false, 900, 400, 50, 50, nullptr);
			t = 60 / (speed + 1);
			if (framesCount % t == 0 && !checkStep) currStep = min(currStep + 1, totalSteps);
			else tmpHead = nullptr;
			highlightCode(generalFont, explain, code, linesColor, 100, 250, YELLOW, checkStep);
			if (currStep == 0) {
				linesColor = -1;
				resetColor();
				createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
				visualize();
			}
			else if (currStep==1) {
				linesColor = 0;
				resetColor();
				visualize();
				newNode->displayNode();
				createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY - newNode->radius - 25, 10 * 4, 20);
			}
			else {
				linesColor = 1;
				resetColor();
				visualize();
				newNode->displayNode();
				createText(generalFont, RED, "Head", newNode->centerX - newNode->radius + 5, newNode->centerY - newNode->radius - 25, 10 * 4, 20);
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
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					quitGame();
					break;
				case SDL_MOUSEBUTTONDOWN:
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
};
bool alertInitialize(bool& alert1, bool& alert2,bool& alert3, const string& s1,const string& s2) {
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
		try{
			temp = stoi(tmp);
		}
		catch (...){
			alert2 = true;
			alert1 = false;
			alert3 = false;
			return false;
		}
		
		if (temp < 0 || temp >= 100) {
			alert2 = true;
			alert1 = false;
			alert3 = false;
			return false;
		}

	}
	return true;



}
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
	bool leftMouseDown = false;
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
		SDL_RenderCopy(gRenderer,form , NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], leftMouseDown, 370, 385, 150, 75, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], leftMouseDown, 370, 500, 150, 75, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 0, 0, 50, 50, NULL);
		createText(generalFont, { 0 , 0 , 0 }, text1,170, 184, text1.size() * 20,40);
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
		leftMouseDown = false;
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_TEXTINPUT:
				if (activeField == 1) text1 += e.text.text;
				else if (activeField == 2) text2 += e.text.text;
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) { 
					quit = true; 
					ans.push_back("Keep");
					SDL_DestroyTexture(buttonSpriteSheet);
					SDL_DestroyTexture(form);
					TTF_CloseFont(generalFont);
					return ans;
				}
				if (e.button.x > 156 && e.button.x < 763 && e.button.y > 176 && e.button.y <233) activeField = 1;
				else if (e.button.x > 156 && e.button.x < 763 && e.button.y > 292 && e.button.y < 342) activeField = 2;
				else activeField = 0;
				if (e.button.x > 370 && e.button.x < 520 && e.button.y > 385 && e.button.y < 460) { 
					if(alertInitialize(alert1Frame,alert2Frame,alert3Frame,trim(text1),trim(text2)))
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
			while (!in.eof()) {
				in >> temp;
				ans.push_back(temp);
			}
		}
		else if (text2!="") {
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

tuple <int, string, bool,bool> indexValueFormInput(string formName,int indexLimit) {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	SDL_Texture* form = loadImgTexture("resources/Form/"+formName);
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
	bool leftMouseDown = false;
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
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], leftMouseDown, 370, 385, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], leftMouseDown, 370, 500, 200, 66, NULL);
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
		leftMouseDown = false;
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_TEXTINPUT:
				if (activeField == 1) text1 += e.text.text;
				else if (activeField == 2) text2 += e.text.text;
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
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
	if (text1 != "" && text2!="") {
		return make_tuple(stoi(trim(text1)), trim(text2), checkStep, true);
	}
	else return make_tuple(0, "-1", false, false);

}
tuple <int, bool, bool> indexFormInput(string formName,int indexLimit) {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf",50);
	SDL_Texture* form = loadImgTexture("resources/Form/"+formName);
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
	bool leftMouseDown = false;
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
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], leftMouseDown, 370, 375, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], leftMouseDown, 370, 500, 200, 66, NULL);
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
			createText(generalFont, RED,alert1 , 190, 300, alert1.size() * 12, 25);
		}
		else if (alert2Frame) {
			createText(generalFont, RED,alert2 , 190, 300, alert2.size() * 12, 25);
		}
		leftMouseDown = false;
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_TEXTINPUT:
				if (activeField == 1) text1 += e.text.text;
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
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
	bool leftMouseDown = false;
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
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], leftMouseDown, 370, 375, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], leftMouseDown, 370, 500, 200, 66, NULL);
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
		leftMouseDown = false;
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_TEXTINPUT:
				if (activeField == 1) text1 += e.text.text;
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
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
					quit = true;
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

void arrayVisualizing() {
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
	buttonPosArray[10][1] = *createRect(&rect, 1819, 1029, 50, 50);;
	SDL_Rect endButton = *createRect(&rect, 1413, 165, 100, 50);
	SDL_Rect endButtonHover = *createRect(&rect, 1683, 165, 100, 50);
	SDL_Color bgColor = theme.getBgColor();
	bool quit = false;
	bool leftMouseDown = false;
	ArrayVisualizer arrayVisualizer;
	vector<string> v = { "11","23","24","35"};
	arrayVisualizer.initialize(v);
	int cnt = 0;
	bool isKeyPress = false;
	int delay = -1;
	int idx=-1;
	bool guideTextVisible = false;
	string val="";
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		arrayVisualizer.visualize();
		if (!arrayVisualizer.checkAnyStep()) {
			createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
			createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
			createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
			createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
			createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
			createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		}
		else {
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50, NULL);
		}
		//isKeyPress = arrayVisualizer.keyboardEvent(idx,val);
		if (arrayVisualizer.checkStepEqualZero()) createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
		//if (cnt == 400) { 
		//	arrayVisualizer.stepInsert = 0; 
		//	arrayVisualizer.n++;
		//	for (int i = 0; i < 100; i++) 
		//		arrayVisualizer.tempArr[i] = arrayVisualizer.arr[i]; 
		//}
		//if (cnt == 300) { arrayVisualizer.insert(1,"23"); }
		if (SDL_PollEvent(&e)) {
			switch (e.type){
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (arrayVisualizer.checkAnyStep() && e.button.x > 850 && e.button.x < 950 && e.button.y > 500 && e.button.y < 550) { 
					if (arrayVisualizer.stepInsert >= 0) {
						for (int i = 0; i < 100; i++)
							arrayVisualizer.arr[i] = arrayVisualizer.tempArr[i];
						arrayVisualizer.insertNoVisual(idx,val);
					}
					else if (arrayVisualizer.stepDelete >= 0) {
						for (int i = 0; i < 100; i++)
							arrayVisualizer.arr[i] = arrayVisualizer.tempArr[i];
						arrayVisualizer.delNoVisual(idx);
					}
					else if (arrayVisualizer.stepUpdate >= 0) {
						for (int i = 0; i < 100; i++)
							arrayVisualizer.arr[i] = arrayVisualizer.tempArr[i];
						arrayVisualizer.arr[idx].val = val;
					}
					arrayVisualizer.resetStep(); 
					break;
				}
				if (arrayVisualizer.checkAnyStep()) break;
				leftMouseDown = true;
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray=initializeFormInput();
					arrayVisualizer.resetStep();
					if((tempArray.size()!=0&&tempArray[0]!="Keep")||(tempArray.size()==0))
						arrayVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png",arrayVisualizer.n+1);
					arrayVisualizer.resetStep();
					if (isValid) {
						if (checkStep) {
							arrayVisualizer.n++;
							arrayVisualizer.stepInsert = 0; 
							guideTextVisible = true;
							for (int i = 0; i < 100; i++) 
								arrayVisualizer.tempArr[i] = arrayVisualizer.arr[i]; 
						}
						else {
							arrayVisualizer.insert(idx, val);
						}
					}
				}
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png",arrayVisualizer.n);
					arrayVisualizer.resetStep();
					if (isValid) {
						if (checkStep) {
							arrayVisualizer.n--;
							arrayVisualizer.stepDelete = 0;
							guideTextVisible = true;
							for (int i = 0; i < 100; i++)
								arrayVisualizer.tempArr[i] = arrayVisualizer.arr[i];
						}
						else {
							arrayVisualizer.delIndex(idx);
						}
					}
				}
				else if (e.button.x > 600 && e.button.x < 800 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(val, checkStep, isValid) = valueFormInput("SearchForm.png");
					arrayVisualizer.resetStep();
					if (isValid) {
						if (checkStep) {
							arrayVisualizer.stepSearch = 0;
							guideTextVisible = true;
							for (int i = 0; i < 100; i++)
								arrayVisualizer.tempArr[i] = arrayVisualizer.arr[i];
						}
						else {
							arrayVisualizer.search(val);
						}
					}
				}
				else if (e.button.x > 800 && e.button.x < 1000 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("UpdateForm.png",arrayVisualizer.n);
					arrayVisualizer.resetStep();
					if (isValid) {
						if (checkStep) {
							arrayVisualizer.stepUpdate = 0;
							guideTextVisible = true;
							for (int i = 0; i < 100; i++)
								arrayVisualizer.tempArr[i] = arrayVisualizer.arr[i];
						}
						else {
							arrayVisualizer.update(idx, val);
						}
					}
				}
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_LEFT) {
					if (arrayVisualizer.stepInsert >= 0) {
						arrayVisualizer.stepInsert = max(arrayVisualizer.stepInsert - 1, 0);
						if (arrayVisualizer.stepInsert > 0) {
							arrayVisualizer.insertStep(idx, val, arrayVisualizer.stepInsert, arrayVisualizer.n - 1);
						}
					}
					else if (arrayVisualizer.stepDelete >= 0) {
						arrayVisualizer.stepDelete = max(arrayVisualizer.stepDelete - 1, 0);
						if (arrayVisualizer.stepDelete > 0) {
							arrayVisualizer.delIndexStep(idx, arrayVisualizer.stepDelete, arrayVisualizer.n + 1);
						}
					}
					else if (arrayVisualizer.stepSearch >= 0) {
						arrayVisualizer.stepSearch = max(arrayVisualizer.stepSearch - 1, 0);
						if (arrayVisualizer.stepSearch > 0) {
							arrayVisualizer.searchStep(val, arrayVisualizer.stepSearch, arrayVisualizer.n);
						}
					}
					else if (arrayVisualizer.stepUpdate >= 0) {
						arrayVisualizer.stepUpdate = max(arrayVisualizer.stepUpdate - 1, 0);
						if (arrayVisualizer.stepUpdate > 0) {
							arrayVisualizer.updateStep(idx, val, arrayVisualizer.stepUpdate, arrayVisualizer.n);
						}
					}

				}
				else if (e.key.keysym.sym == SDLK_RIGHT) {
					if (arrayVisualizer.stepInsert >= 0) {
						arrayVisualizer.stepInsert = min(arrayVisualizer.stepInsert + 1, arrayVisualizer.maxStep);
						arrayVisualizer.insertStep(idx, val, arrayVisualizer.stepInsert, arrayVisualizer.n - 1);
					}
					else if (arrayVisualizer.stepDelete >= 0) {
						arrayVisualizer.stepDelete = min(arrayVisualizer.stepDelete + 1, arrayVisualizer.maxStep);
						arrayVisualizer.delIndexStep(idx, arrayVisualizer.stepDelete, arrayVisualizer.n + 1);
					}
					else if (arrayVisualizer.stepSearch >= 0) {
						arrayVisualizer.stepSearch = min(arrayVisualizer.stepSearch + 1, arrayVisualizer.maxStep);
						arrayVisualizer.searchStep(val, arrayVisualizer.stepSearch, arrayVisualizer.n);
					}
					else if (arrayVisualizer.stepUpdate >= 0) {
						arrayVisualizer.stepUpdate = min(arrayVisualizer.stepUpdate + 1, arrayVisualizer.maxStep);
						arrayVisualizer.updateStep(idx, val, arrayVisualizer.stepUpdate, arrayVisualizer.n);
					}
				}
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	TTF_CloseFont(generalFont);
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
	bool leftMouseDown = false;
	SDL_Color bgColor = theme.getBgColor();
	LinkedListVisualizer listVisualizer;
	vector<string> v = { "11","23","24","35" };
	listVisualizer.initialize(v);
	int cnt = 0;
	bool isKeyPress = false;
	int delay = -1;
	int idx = -1;
	bool guideTextVisible = false;
	string val = "";
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		listVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		//isKeyPress = arrayVisualizer.keyboardEvent(idx, val);
		if (isKeyPress) guideTextVisible = false;
		if (guideTextVisible) createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					//arrayVisualizer.resetStep();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						listVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png",listVisualizer.listSize+1);
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
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png",listVisualizer.listSize);
					if (isValid) {
						if(idx)
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
					tie(idx,val, checkStep, isValid) = indexValueFormInput("UpdateForm.png",listVisualizer.listSize);
					if (isValid) {
						listVisualizer.updateStep(idx,val, checkStep);
					}
				}
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	TTF_CloseFont(generalFont);
	listVisualizer.freeMem();
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
	bool leftMouseDown = false;
	SDL_Color bgColor = theme.getBgColor();
	CircularLinkedListVisualizer listVisualizer;
	vector<string> v = { "11","23","24","35" };
	listVisualizer.initialize(v);
	int cnt = 0;
	bool isKeyPress = false;
	int delay = -1;
	int idx = -1;
	bool guideTextVisible = false;
	string val = "";
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		listVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		//isKeyPress = arrayVisualizer.keyboardEvent(idx, val);
		if (isKeyPress) guideTextVisible = false;
		if (guideTextVisible) createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					//arrayVisualizer.resetStep();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						listVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png",listVisualizer.listSize+1);
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
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png",listVisualizer.listSize);
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
					tie(idx, val, checkStep, isValid) = indexValueFormInput("UpdateForm.png",listVisualizer.listSize);
					if (isValid) {
						listVisualizer.updateStep(idx, val, checkStep);
					}
				}
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	TTF_CloseFont(generalFont);
	listVisualizer.freeMem();
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
	bool leftMouseDown = false;
	SDL_Color bgColor = theme.getBgColor();
	DoublyLinkedListVisualizer listVisualizer;
	vector<string> v = { "11","23","24","35" };
	listVisualizer.initialize(v);
	int cnt = 0;
	bool isKeyPress = false;
	int delay = -1;
	int idx = -1;
	bool guideTextVisible = false;
	string val = "";
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		listVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		//isKeyPress = arrayVisualizer.keyboardEvent(idx, val);
		if (isKeyPress) guideTextVisible = false;
		if (guideTextVisible) createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					//arrayVisualizer.resetStep();
					if ((tempArray.size() != 0 && tempArray[0] != "Keep") || (tempArray.size() == 0))
						listVisualizer.initialize(tempArray);
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png",listVisualizer.listSize+1);
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
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png",listVisualizer.listSize);
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
					tie(idx, val, checkStep, isValid) = indexValueFormInput("UpdateForm.png",listVisualizer.listSize);
					if (isValid) {
						listVisualizer.updateStep(idx, val, checkStep);
					}
				}
			}
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	TTF_CloseFont(generalFont);
	listVisualizer.freeMem();
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
		buttonPosArray[i][0] = *createRect(&rect, 829, 1317 + 144 * (i-8), 200, 66);
		buttonPosArray[i][1] = *createRect(&rect, 559, 1317 + 144 * (i-8), 200, 66);
	}
	buttonPosArray[10][0] = *createRect(&rect, 2000, 1029, 50, 50);
	buttonPosArray[10][1] = *createRect(&rect, 1819, 1029, 50, 50);
	bool quit = false;
	bool leftMouseDown = false;
	bool visiblePop = false;
	StackVisualizer stackVisualizer;
	vector<string> v = { "11","23","24","35" };
	SDL_Color bgColor = theme.getBgColor();
	stackVisualizer.initialize(v);
	int cnt = 0;
	bool isKeyPress = false;
	int delay = -1;
	int idx = -1;
	bool guideTextVisible = false;
	string val = "";
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		stackVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[5][0], &buttonPosArray[5][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[6][0], &buttonPosArray[6][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		//isKeyPress = arrayVisualizer.keyboardEvent(idx, val);
		if (isKeyPress) guideTextVisible = false;
		if (guideTextVisible) createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 200, 250, 800, 40);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEMOTION:
				if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					visiblePop = true;
				}
				else if (visiblePop && e.button.x > 400 && e.button.x < 900 && e.button.y > 440 && e.button.y < 640) visiblePop = true;
				else visiblePop = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					//arrayVisualizer.resetStep();
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
				else if (e.button.x > 650 && e.button.x < 850 && e.button.y > 460 && e.button.y < 526 && visiblePop) {
					stackVisualizer.popStep(true);
					visiblePop = false;
				}
				else if (e.button.x > 650 && e.button.x < 850 && e.button.y > 550 && e.button.y < 616 && visiblePop) {
					stackVisualizer.popStep(false);
					visiblePop = false;
				}

			}
		}
		if (visiblePop) {
			SDL_RenderCopy(gRenderer, popForm, NULL, createRect(&rect, 610, 440, 289, 200));
			createButton(buttonSpriteSheet, &buttonPosArray[8][0], &buttonPosArray[8][1], leftMouseDown, 650, 460, 200, 66, NULL);
			createButton(buttonSpriteSheet, &buttonPosArray[9][0], &buttonPosArray[9][1], leftMouseDown, 650, 550, 200, 66, NULL);
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(popForm);
	TTF_CloseFont(generalFont);
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
	bool quit = false;
	bool leftMouseDown = false;
	bool visiblePop = false;
	SDL_Color bgColor = theme.getBgColor();
	QueueVisualizer queueVisualizer;
	vector<string> v = { "11","23","24","35" };
	queueVisualizer.initialize(v);
	int cnt = 0;
	bool isKeyPress = false;
	int delay = -1;
	int idx = -1;
	bool guideTextVisible = false;
	string val = "";
	while (!quit) {
		cnt++;
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		queueVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[5][0], &buttonPosArray[5][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[6][0], &buttonPosArray[6][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[10][0], &buttonPosArray[10][1], false, 0, 0, 50, 50, NULL);
		//isKeyPress = arrayVisualizer.keyboardEvent(idx, val);
		if (isKeyPress) guideTextVisible = false;
		if (guideTextVisible) createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 200, 250, 800, 40);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEMOTION:
				if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					visiblePop = true;
				}
				else if (visiblePop && e.button.x > 400 && e.button.x < 900 && e.button.y > 440 && e.button.y < 640) visiblePop = true;
				else visiblePop = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
				if (e.button.x > 0 && e.button.x < 50 && e.button.y > 0 && e.button.y < 50) quit = true;
				else if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					//arrayVisualizer.resetStep();
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
				else if (e.button.x > 650 && e.button.x < 850 && e.button.y > 460 && e.button.y < 526 && visiblePop) {
					queueVisualizer.popStep(true);
					visiblePop = false;
				}
				else if (e.button.x > 650 && e.button.x < 850 && e.button.y > 550 && e.button.y < 616 && visiblePop) {
					queueVisualizer.popStep(false);
					visiblePop = false;
				}

			}
		}
		if (visiblePop) {
			SDL_RenderCopy(gRenderer, popForm, NULL, createRect(&rect, 610, 440, 289, 200));
			createButton(buttonSpriteSheet, &buttonPosArray[8][0], &buttonPosArray[8][1], leftMouseDown, 650, 460, 200, 66, NULL);
			createButton(buttonSpriteSheet, &buttonPosArray[9][0], &buttonPosArray[9][1], leftMouseDown, 650, 550, 200, 66, NULL);
		}
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyTexture(buttonSpriteSheet);
	SDL_DestroyTexture(popForm);
	TTF_CloseFont(generalFont);
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
	bool leftMouseDown = false;
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
				leftMouseDown = true;
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
	SDL_Rect up=*createRect(&rect, 316, 383, 24, 16);
	SDL_Rect down= *createRect(&rect, 316, 399, 24, 16);
	bool quit = false;
	bool leftMouseDown = false;
	bool checkBack = false;
	vector<SDL_Color> choice = { theme.getBgColor(),theme.getBgElemColor(),theme.getTextColor(),theme.getElemColor(),theme.getArrowColor() };
	int textSize = theme.getTextSize();
	vector<pair<SDL_Rect,SDL_Color>> rects;
	ListNode* demoNode = new ListNode("88", 374+25, 467+110, 25, new ListNode());
	ArrayElement demoElem = ArrayElement(374, 467 + 15, 50, 50, choice[3], choice[1], choice[2], "88", "");
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
			rects.push_back(make_pair(*createRect(&rect, 240 + 110 * i, 80 + 60 * j, 95, 32),colorMap[i]));
		}
	}
	SDL_Color old;
	SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	SDL_StartTextInput();
	int mouseX, mouseY;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, themeOptions, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &apply, &applyHover, false, 800, 570, 180, 60, NULL);
		createButton(buttonSpriteSheet, &back, &backHover, false, 0, 0, 50, 50, NULL);
		createText(numberFont, BLACK, to_string(textSize), 275, 390, 2 * 10, 20);
		/*for (int i = 0; i < 5; i++) {
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(gRenderer, createRect(&rect,xMap[choice[i]],80+60*i,95,32));
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
		}*/
		for (int i = 0; i < rects.size(); i++) {
			if (isEqualColor(rects[i].second, choice[i % 5])) {
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
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
				else if (e.button.x > 800 && e.button.x < 980 && e.button.y > 570 && e.button.y < 630) {
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
		SDL_RenderPresent(gRenderer);


	}
	SDL_StopTextInput();
	SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
	//Theme(SDL_Color bgColor, SDL_Color bgElemColor, SDL_Color textColor,SDL_Color elemColor,SDL_Color arrowColor, int textSize)
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
	buttonPosArray[4][0] = *createRect(&rect, 12, 746, 109, 118);
	buttonPosArray[4][1] = *createRect(&rect, 271, 746, 109, 118);
	bool quit = false;
	bool leftMouseDown = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, background, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 300, 100, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 400, 210, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 300, 320, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 400, 430, 200, 100, NULL);
		//createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 875, 515, 109, 118, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 875, 10, 109, 118, NULL);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
				if (e.button.x > 300 && e.button.x < 500 && e.button.y > 100 && e.button.y < 200) arrayVisualizing();
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
int main(int argc, char* args[]) {
	init();
	mainMenu();
	return 0;
}