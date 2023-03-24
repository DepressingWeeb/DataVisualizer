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

#define RED {255,0,0}
#define BLUE {0,0,255}
#define GREEN {0,255,0}
#define BLACK {0,0,0}
#define WHITE {255,255,255}
#define YELLOW {255,255,0}
using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 640;
SDL_Rect SCREEN = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

SDL_Window* window = SDL_CreateWindow("Data Visualizer", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer* gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
vector<string> globalCode = {};
int linesDisplay=-1;

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

void createButton(SDL_Texture* buttonSpriteSheet ,SDL_Rect* button, SDL_Rect* buttonOnHover, bool leftMouseDown, int x, int y, int w, int h, void(*func)()) {
	int mouseX, mouseY;
	SDL_Rect buttonRect;
	SDL_GetMouseState(&mouseX, &mouseY);
	if (x < mouseX && mouseX < x + w && y < mouseY && mouseY < y + h) {
		if (leftMouseDown && func != NULL) func();
		SDL_RenderCopy(gRenderer, buttonSpriteSheet, buttonOnHover, createRect(&buttonRect, x, y, w, h));
	}
	else {
		SDL_RenderCopy(gRenderer, buttonSpriteSheet, button, createRect(&buttonRect, x, y, w, h));
	}
}

void createText(TTF_Font* font, SDL_Color color, string text, int x, int y, int w, int h) {
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	SDL_FreeSurface(textSurface);
	SDL_Rect rect;
	SDL_RenderCopy(gRenderer, texture, NULL, createRect(&rect, x, y, w, h));
	SDL_DestroyTexture(texture);
}

class ArrayElement {
public:
	int x;
	int y;
	int w;
	int h;
	string index;
	SDL_Color color;
	string val;
	SDL_Rect rect;
	TTF_Font* numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	TTF_Font* letterFont = TTF_OpenFont("resources/Font/ClementFive.ttf", 500);
	ArrayElement() { this->val = "N"; }
	ArrayElement(int x, int y,int w,int h, SDL_Color color,string val,string index) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->color = color;
		this->val = val;
		this->index = index;
	}
	//~ArrayElement() { TTF_CloseFont(numberFont); TTF_CloseFont(letterFont); }

	void display() {
		SDL_Color old;
		SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.b, &old.a);
		SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderDrawRect(gRenderer, createRect(&rect, x, y, w, h));
		createText(numberFont, color, val.size()==1 && val!="0" ? "0" + val : val, x + 10, y + 10, 30, 30);
		createText(numberFont, color, index, x + 17, y + 60, 15, 20);
		SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
	}
	void changeColor(SDL_Color color) {
		this->color = color;
	}
	void changeCoordinate(int x, int y, int w, int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

};

class ArrayVisualizer {
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
	SDL_Rect buttonPosArray[4];
	TTF_Font* numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.jpg");
	ArrayVisualizer() { 
		n = 0; 
		stepInsert = -1; 
		stepDelete = -1;
		stepSearch = -1;
		stepUpdate = -1;
		delay = -1;
		maxStep = 1000;
		speed = 0;
		for (int i = 0; i < 100; i++) arr[i] = ArrayElement();
		SDL_Rect rect;
		for (int i = 0; i < 4; i++) {
			buttonPosArray[i] = *createRect(&rect, 1493+180*i, 883, 130, 130);
		}
	}
	void resetStep() {
		for (int i = 0; i < 100; i++) {
			if (arr[i].val == "N") break;
			arr[i].changeColor(BLACK);
		}
		stepInsert = -1;
		stepDelete = -1;
		stepSearch = -1;
		stepUpdate = -1;
		maxStep = 1000;
	}
	void visualize() {
		for (int i = 0; i < 100; i++) {
			if (arr[i].val=="N") break;
			arr[i].display();
		}
		for (int i = 0; i < globalCode.size(); i++) {
			if (i == linesDisplay)
				createText(numberFont, RED, globalCode[i], 100, 200 + i * 50, globalCode[i].size() * 16, 40);
			else
				createText(numberFont, BLACK, globalCode[i], 100, 200 + i * 50, globalCode[i].size() * 16, 40);
		}
	}
	void visualize(ArrayElement customArr[],int size) {
		for (int i = 0; i < size; i++) {
			customArr[i].display();
		}
		for (int i = 0; i < globalCode.size(); i++) {
			if (i == linesDisplay)
				createText(numberFont, RED, globalCode[i], 100, 200 + i * 50, globalCode[i].size() * 16, 40);
			else
				createText(numberFont, BLACK, globalCode[i], 100, 200 + i * 50, globalCode[i].size() * 16, 40);
		}
	}
	void updateFrame(vector<string> code = {}, int linesColor = -1) {
		int t = 24 / (speed+1);
		SDL_Rect rect;
		bool leftMouseDown = false;
		SDL_Event e;
		while (t--) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			visualize();
			createButton(buttonSpriteSheet, &buttonPosArray[speed], &buttonPosArray[speed], leftMouseDown, 900, 400, 50, 50, nullptr);
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
		if (currentKeyStates[SDL_SCANCODE_LEFT] && delay==-1) {
			if (stepInsert >= 0) {
				stepInsert = max(stepInsert - 1, 0);
				if (stepInsert > 0) {
					insertStep(idx, val, stepInsert, n-1);
					delay = 0;
				}		
			}
			else if (stepDelete >= 0) {
				stepDelete = max(stepDelete - 1, 0);
				if (stepDelete > 0) {
					delIndexStep(idx, stepDelete, n + 1);
					delay = 0;
				}
			}
			else if (stepSearch >= 0) {
				stepSearch = max(stepSearch - 1, 0);
				if (stepSearch > 0) {
					searchStep(val, stepSearch, n);
					delay = 0;
				}
			}
			else if (stepUpdate >= 0) {
				stepUpdate = max(stepUpdate - 1, 0);
				if (stepUpdate > 0) {
					updateStep(idx,val, stepUpdate, n);
					delay = 0;
				}
			}

			return true;
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT] && delay==-1) {
			if (stepInsert >= 0) {
				stepInsert = min(stepInsert+1, maxStep);
				insertStep(idx, val, stepInsert,n-1);
				delay = 0;
			}
			else if (stepDelete >= 0) {
				stepDelete = min(stepDelete + 1, maxStep);
				delIndexStep(idx, stepDelete, n + 1);
				delay = 0;
			}
			else if (stepSearch >= 0) {
				stepSearch = min(stepSearch + 1, maxStep);
				searchStep(val, stepSearch, n );
				delay = 0;
			}
			else if (stepUpdate >= 0) {
				stepUpdate = min(stepUpdate + 1, maxStep);
				updateStep(idx, val, stepUpdate, n);
				delay = 0;
			}
			return true;
		}
		if (delay >= 0) {
			delay++;
			if (delay == 5)
				delay = -1;
		}
		return false;
	}
	void initialize(vector<string> strArr) {
		this->n = strArr.size();
		for (int i = 0; i < 100; i++) {
			arr[i] = ArrayElement();
		}
		for (int i = 0; i < n; i++) {
			arr[i] = ArrayElement(100+i*50, 100, 50, 50, { 0,0,0 }, strArr[i],to_string(i));
		}

	}
	void insertStep(int idx, string val, int step,int n) {
		for (int i = 0; i < 100; i++) arr[i] = tempArr[i];
		int currStep = 0;
		vector<string> codeInsert = { "n++;", "for(int i=n-1;i>indexInsert;i--){","  arr[i]=arr[i-1];","}","arr[indexInsert]=valueInsert;" };
		n++;
		arr[n - 1] = ArrayElement(100 + (n - 1) * 50, 100, 50, 50, YELLOW, "0",to_string(n-1));
		currStep++;
		if (currStep == step) { linesDisplay = 0; globalCode = codeInsert; return; }

		for (int i = n - 1; i > idx; i--) {
			currStep++;
			if (currStep == step) { linesDisplay = 1; globalCode = codeInsert; return; }
			arr[i + 1].changeColor(BLACK);
			arr[i].val = arr[i - 1].val;
			arr[i].changeColor(RED);
			arr[i - 1].changeColor(BLUE);
			currStep++;
			if (currStep == step) { linesDisplay = 2; globalCode = codeInsert; return; }
		}
		arr[idx + 1].changeColor(BLACK);
		arr[idx].val = val;
		currStep++;
		linesDisplay = 4; globalCode = codeInsert; maxStep = currStep; return;

	}
	void insert(int idx, string val) {
		vector<string> codeInsert = { "n++;", "for(int i=n-1;i>indexInsert;i--){","  arr[i]=arr[i-1];","}","arr[indexInsert]=valueInsert;" };
		
		n++;
		arr[n-1] = ArrayElement(100 + (n-1) * 50, 100, 50, 50, { 255,255,0 }, "0",to_string(n-1));
		updateFrame(codeInsert, 0);
		
		for (int i = n-1; i > idx; i--) {
			updateFrame(codeInsert, 1);
			arr[i + 1].changeColor(BLACK);
			arr[i].val = arr[i - 1].val;
			arr[i].changeColor(RED);
			arr[i-1].changeColor(BLUE);
			updateFrame(codeInsert,2);
		}
		arr[idx + 1].changeColor({ 0,0,0,255 });
		arr[idx].val = val;
		updateFrame(codeInsert,4);
		arr[idx].changeColor({ 0,0,0,255 });
		updateFrame(codeInsert, 4);
	}

	void delIndexStep(int idx, int step, int n) {
		for (int i = 0; i < 100; i++) arr[i] = tempArr[i];
		int currStep = 0;
		vector<string> codeDelete = { "for(int i=indexDelete;i<n-1;i++){", "    arr[i]=arr[i+1];","}","n--;" };
		for (int i = idx; i < n - 1; i++) {
			currStep++;
			if (currStep == step) { linesDisplay = 0; globalCode = codeDelete; return; }
			if (i != idx) arr[i - 1].changeColor(BLACK);
			arr[i].changeColor(GREEN);
			arr[i + 1].changeColor(RED);
			arr[i].val = arr[i + 1].val;
			currStep++;
			if (currStep == step) { linesDisplay = 1; globalCode = codeDelete; return; }
		}
		arr[n - 1] = ArrayElement();

		n--;
		arr[n - 1].changeColor({ 0,0,0 });
		currStep++;
		linesDisplay = 3; globalCode = codeDelete; maxStep = currStep; return;

	}
	void delIndex(int idx){
		vector<string> codeDelete = { "for(int i=indexDelete;i<n-1;i++){", "    arr[i]=arr[i+1];","}","n--;" };
		for (int i = idx; i < n-1; i++) {
			updateFrame(codeDelete,0);
			if (i != idx) arr[i - 1].changeColor(BLACK);
			arr[i].changeColor(GREEN);
			arr[i + 1].changeColor(RED);
			arr[i].val = arr[i + 1].val;
			updateFrame(codeDelete,1);
		}
		arr[n - 1] = ArrayElement();
		
		n--;
		updateFrame(codeDelete, 3);
		arr[n - 1].changeColor({ 0,0,0 });


	}
	void searchStep(string val, int step,int n) {
		for (int i = 0; i < 100; i++) arr[i] = tempArr[i];
		int currStep = 0;
		vector<string> codeSearch = { "for(int i=0;i<n;i++){", "    if(arr[i]==target){","        targetIndex=i;","        break; }","    else continue;","}" };
		for (int i = 0; i < n; i++) {
			currStep++;
			if (currStep == step) { linesDisplay = 0; globalCode = codeSearch; return; }
			if (arr[i].val == val) {
				currStep++;
				if (currStep == step) { linesDisplay = 1; globalCode = codeSearch; return; }
				arr[i].changeColor(GREEN);
				currStep++;
				if (currStep == step) { linesDisplay = 2; globalCode = codeSearch; return; }
				arr[i].changeColor(BLACK);
				currStep++;
				if (currStep == step) { linesDisplay = 3; globalCode = codeSearch; maxStep = currStep; return; }
				return;
			}
			else arr[i].changeColor(RED);
			currStep++;
			if (currStep == step) { linesDisplay = 4; globalCode = codeSearch; return; }
			arr[i].changeColor(BLACK);
		}
		maxStep = currStep;
	}
	void search(string val) {
		vector<string> codeSearch = { "for(int i=0;i<n;i++){", "    if(arr[i]==target){","        targetIndex=i;","        break; }","    else continue;","}"};
		for (int i = 0; i < n; i++) {
			updateFrame(codeSearch, 0);
			if (arr[i].val == val) {
				updateFrame(codeSearch, 1);
				arr[i].changeColor(GREEN);
				updateFrame(codeSearch, 2);
				arr[i].changeColor(BLACK);
				updateFrame(codeSearch, 3);
				return;
			}
			else arr[i].changeColor(RED);
			updateFrame(codeSearch, 4);
			arr[i].changeColor(BLACK);
		}

	}
	void updateStep(int idx, string val,int step, int n) {
		for (int i = 0; i < 100; i++) arr[i] = tempArr[i];
		int currStep = 0;
		vector<string> codeUpdate = { "arr[indexUpdate]=valueUpdate;" };
		arr[idx].val = val;
		arr[idx].changeColor(GREEN);
		currStep++;
		if (currStep == step) { linesDisplay = 0; globalCode = codeUpdate; return; }
		arr[idx].changeColor(BLACK);
		globalCode = codeUpdate;
		linesDisplay = -1;
		maxStep = 2;
	}
	void update(int idx, string val) {
		vector<string> codeUpdate = {"arr[indexUpdate]=valueUpdate;"};
		arr[idx].val = val;
		arr[idx].changeColor(GREEN);
		updateFrame(codeUpdate, 0);
		arr[idx].changeColor(BLACK);
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
	SDL_Color colorNode;
	SDL_Color colorArrow;
	TTF_Font* numberFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);

	ListNode() {
		this->val = "N";
		this->next = nullptr;
		this->centerX = 0;
		this->centerY = 0;
		this->radius = 0;
		this->colorNode = BLACK;
		this->colorArrow = BLACK;
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
		this->colorNode = BLACK;
		this->colorArrow = BLACK;
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
		this->colorNode = BLACK;
		this->colorArrow = BLACK;
		this->arrowSX = centerX + radius;
		this->arrowSY = centerY;
		this->arrowDX = centerX + radius + 30;
		this->arrowDY = centerY;
	}
	void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius,SDL_Color color)
	{
		const int32_t diameter = (radius * 2);

		int32_t x = (radius - 1);
		int32_t y = 0;
		int32_t tx = 1;
		int32_t ty = 1;
		int32_t error = (tx - diameter);

		SDL_Color old;
		SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.b,&old.a);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		while (x >= y)
		{
			SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

			if (error <= 0)
			{
				++y;
				error += ty;
				ty += 2;
			}

			if (error > 0)
			{
				--x;
				tx += 2;
				error += (tx - diameter);
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

	void displayNode() {
		DrawCircle(gRenderer, centerX, centerY, radius,colorNode);
		createText(numberFont, colorNode, val.size() == 1 && val != "0" ? "0" + val : val, centerX - 15, centerY - 15, 30, 30);
		if (next) {
			DrawArrow(gRenderer, arrowSX, arrowDX, arrowSY, arrowDY,8,colorArrow);
		}
	}
	void setColorNode(SDL_Color newColor) {
		colorNode = newColor;
	}
	void setColorArrow(SDL_Color newColor) {
		colorArrow = newColor;
	}
	void setArrowPos(int x1, int y1,int x2,int y2) {
		arrowSX = x1;
		arrowSY = y1;
		arrowDX = x2;
		arrowDY = y2;
	}
};

class LinkedListVisualizer {
public:
	ListNode* head;
	int listSize;
	LinkedListVisualizer() {
		head = nullptr;
		listSize = 0;
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
	void initialize( vector<string> list) {
		freeMem();
		this->listSize = list.size();
		head = new ListNode(list[0], 100, 100, 25);
		auto clone = head;
		for (int i = 1; i < list.size(); i++) {
			clone->next = new ListNode(list[i], 100 + 90*i, 100, 25);
			clone = clone->next;
		}
	}
	void reCalcCoordinate() {
		ListNode* clone = head;
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
			clone->setColorNode(BLACK);
			clone->setColorArrow(BLACK);
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
	void insert(int idx, string val) {
		if (idx == 0) {
			ListNode* newHead = new ListNode(val, 100, 100, 25, head);
			head = newHead;
			//Re-coordinate
			reCalcCoordinate();
		}
		else {
			ListNode* clone = head;
			while (idx-1) {
				clone = clone->next;
				idx--;
			}
			clone->next= new ListNode(val, 100, 100, 25, clone->next);
			reCalcCoordinate();
		}
	}
	void insertStep(int idx, string val) { 
		SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.jpg");
		TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 100);
		vector<string> code;
		if(idx)
			code = { 
			"while(index > 1){",
			"    head=head->next;",
			"}",
			"ListNode * newNode = new Node(value,nullptr); ",
			"newNode->next = head->next; ",
			"head->next = newNode; "
		};
		else
			code = {
			"ListNode * newNode = new Node(value,nullptr); ",
			"newNode->next = head ",
			"head = newNode; "
		};
		int linesColor = -1;
		int totalSteps = idx?idx*2 + 2:3;
		SDL_Rect rect;
		SDL_Rect endButton = *createRect(&rect, 1493, 178, 100, 50);
		SDL_Rect endButtonHover = *createRect(&rect, 1763, 178, 100, 50);
		SDL_Event e;
		bool quit = false;
		int currStep = 0;
		int currHeadIndex = 0;
		ListNode* newNode;
		ListNode* tmp = idx - 1 >= 0 ? getNode(idx - 1) : nullptr;
		if(tmp)	newNode = new ListNode(val, tmp->centerX, tmp->centerY + 100, 25);
		else newNode = new ListNode(val, head->centerX, head->centerY + 100, 25);
		ListNode* oldNode= getNode(idx);
		ListNode* currHead = nullptr;
		while (!quit) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			createButton(buttonSpriteSheet, &endButton, &endButtonHover, false, 850, 500, 100, 50,NULL);
			//createText(generalFont, BLACK, to_string(currStep),0,0,50,50);
			currHead = getNode(currHeadIndex);
			if(currStep==0){
				linesColor = -1;
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
				case SDL_KEYDOWN:
					
						switch (e.key.keysym.sym)
						{
						case SDLK_LEFT: currStep = max(currStep - 1, 0); break;
						case SDLK_RIGHT: currStep = min(currStep + 1, totalSteps); break;
						}
					
				}
			}
			SDL_RenderPresent(gRenderer);
		}
		if(tmp)	tmp->next = newNode;
		newNode->next = oldNode;
		if (idx == 0) head = newNode;
		reCalcCoordinate();
		resetColor();
		resetArrow();
		SDL_DestroyTexture(buttonSpriteSheet);
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
	void search(string val) {
		ListNode* clone = head;
		while (clone) {
			if (clone->val == val)
				break;
			else
				clone = clone->next;

		}
	}
	void update(int idx, string newVal) {
		ListNode* clone = head;
		while (idx) {
			clone = clone->next;
			idx--;
		}
		clone->val = newVal;
	}

};

vector<string> initializeFormInput() {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	SDL_Texture* form = loadImgTexture("resources/Form/InitForm.png");
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.jpg");
	SDL_Rect buttonPosArray[15][2];
	vector<string> ans;
	bool checkRandom = false;
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 909, 178 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 639, 178 + 144 * i, 200, 100);
	}
	bool quit = false;
	bool leftMouseDown = false;
	int activeField = 0;
	string text1 = "";
	string text2 = "";
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_StartTextInput();
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer,form , NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], leftMouseDown, 370, 385, 150, 75, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[7][0], &buttonPosArray[7][1], leftMouseDown, 370, 500, 150, 75, NULL);
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
				if (e.button.x > 156 && e.button.x < 763 && e.button.y > 176 && e.button.y <233) activeField = 1;
				else if (e.button.x > 156 && e.button.x < 763 && e.button.y > 292 && e.button.y < 342) activeField = 2;
				else activeField = 0;
				if (e.button.x > 370 && e.button.x < 520 && e.button.y > 385 && e.button.y < 460 ) quit = true;
				if (e.button.x > 370 && e.button.x < 520 && e.button.y > 500 && e.button.y < 575) {
					quit = true; checkRandom = true;
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

tuple <int, string, bool,bool> indexValueFormInput(string formName) {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	SDL_Texture* form = loadImgTexture("resources/Form/"+formName);
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.jpg");
	SDL_Rect buttonPosArray[2][2];
	vector<string> ans;
	bool checkStep = false;
	for (int i = 0; i < 2; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 909, 1330 + 144 * i, 200, 66);
		buttonPosArray[i][1] = *createRect(&rect, 639, 1330 + 144 * i, 200, 66);
	}
	bool quit = false;
	bool leftMouseDown = false;
	int activeField = 0;
	string text1 = "";
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_StartTextInput();
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, form, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], leftMouseDown, 370, 375, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], leftMouseDown, 370, 500, 200, 66, NULL);
		createText(generalFont, { 0 , 0 , 0 }, text1, 183, 246, text1.size() * 20, 40);
		if (SDL_GetTicks64() % 1000 >= 500) {
			switch (activeField) {
			case 1:
				SDL_RenderDrawLine(gRenderer, 183 + text1.size() * 20, 246, 183 + text1.size() * 20, 283);
				break;
			}
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
				if (e.button.x > 169 && e.button.x < 794 && e.button.y > 228 && e.button.y < 299) activeField = 1;
				else activeField = 0;
				if (e.button.x > 370 && e.button.x < 570 && e.button.y > 375 && e.button.y < 441) { quit = true; checkStep = true; }
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
		stringstream ss(text1);
		int idx;
		string val;
		ss >> idx >> val;
		return make_tuple(idx, val, checkStep, true);
	}
	else return make_tuple(0, "-1", false, false);

}
tuple <int, bool, bool> indexFormInput(string formName) {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	SDL_Texture* form = loadImgTexture("resources/Form/"+formName);
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.jpg");
	SDL_Rect buttonPosArray[2][2];
	vector<string> ans;
	bool checkStep = false;
	for (int i = 0; i < 2; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 909, 1330 + 144 * i, 200, 66);
		buttonPosArray[i][1] = *createRect(&rect, 639, 1330 + 144 * i, 200, 66);
	}
	bool quit = false;
	bool leftMouseDown = false;
	int activeField = 0;
	string text1 = "";
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_StartTextInput();
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, form, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], leftMouseDown, 370, 375, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], leftMouseDown, 370, 500, 200, 66, NULL);
		createText(generalFont, { 0 , 0 , 0 }, text1, 183, 246, text1.size() * 20, 40);
		if (SDL_GetTicks64() % 1000 >= 500) {
			switch (activeField) {
			case 1:
				SDL_RenderDrawLine(gRenderer, 183 + text1.size() * 20, 246, 183 + text1.size() * 20, 283);
				break;
			}
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
				if (e.button.x > 169 && e.button.x < 794 && e.button.y > 228 && e.button.y < 299) activeField = 1;
				else activeField = 0;
				if (e.button.x > 370 && e.button.x < 570 && e.button.y > 375 && e.button.y < 441) { quit = true; checkStep = true; }
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
		int idx = stoi(text1);
		return make_tuple(idx, checkStep, true);
	}
	else return make_tuple(0, false, false);

}
tuple <string, bool, bool> valueFormInput(string formName) {
	SDL_Rect rect;
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	SDL_Texture* form = loadImgTexture("resources/Form/" + formName);
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.jpg");
	SDL_Rect buttonPosArray[2][2];
	vector<string> ans;
	bool checkStep = false;
	for (int i = 0; i < 2; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 909, 1330 + 144 * i, 200, 66);
		buttonPosArray[i][1] = *createRect(&rect, 639, 1330 + 144 * i, 200, 66);
	}
	bool quit = false;
	bool leftMouseDown = false;
	int activeField = 0;
	string text1 = "";
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_StartTextInput();
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, form, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], leftMouseDown, 370, 375, 200, 66, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], leftMouseDown, 370, 500, 200, 66, NULL);
		createText(generalFont, { 0 , 0 , 0 }, text1, 183, 246, text1.size() * 20, 40);
		if (SDL_GetTicks64() % 1000 >= 500) {
			switch (activeField) {
			case 1:
				SDL_RenderDrawLine(gRenderer, 183 + text1.size() * 20, 246, 183 + text1.size() * 20, 283);
				break;
			}
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
				if (e.button.x > 169 && e.button.x < 794 && e.button.y > 228 && e.button.y < 299) activeField = 1;
				else activeField = 0;
				if (e.button.x > 370 && e.button.x < 570 && e.button.y > 375 && e.button.y < 441) { quit = true; checkStep = true; }
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
		return make_tuple(text1, checkStep, true);
	}
	else return make_tuple("0", false, false);

}

void arrayVisualizing() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.jpg");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 909, 178 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 639, 178 + 144 * i, 200, 100);

	}
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
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		arrayVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
		isKeyPress = arrayVisualizer.keyboardEvent(idx,val);
		if (isKeyPress) guideTextVisible = false;
		if (guideTextVisible) createText(generalFont, BLACK, "<Use LEFT ARROW key(<-) or RIGHT ARROW key(->) to change steps>", 50, 250, 900, 40);
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
				leftMouseDown = true;
				if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray=initializeFormInput();
					arrayVisualizer.resetStep();
					arrayVisualizer.initialize(tempArray);
					globalCode = {};
					linesDisplay = -1;
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png");
					arrayVisualizer.resetStep();
					globalCode = {};
					linesDisplay = -1;
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
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png");
					arrayVisualizer.resetStep();
					globalCode = {};
					linesDisplay = -1;
					arrayVisualizer.maxStep = 1000;
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
					globalCode = {};
					linesDisplay = -1;
					arrayVisualizer.maxStep = 1000;
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
					tie(idx, val, checkStep, isValid) = indexValueFormInput("UpdateForm.png");
					arrayVisualizer.resetStep();
					globalCode = {};
					linesDisplay = -1;
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
			}
		}
		SDL_RenderPresent(gRenderer);
	}
}
void linkedListVisualizing() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.jpg");
	TTF_Font* generalFont = TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 200);
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 8; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 909, 178 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 639, 178 + 144 * i, 200, 100);

	}
	bool quit = false;
	bool leftMouseDown = false;
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
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		listVisualizer.visualize();
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 0, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 200, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 400, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 600, 530, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[4][0], &buttonPosArray[4][1], false, 800, 530, 200, 100, NULL);
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
				if (e.button.x > 0 && e.button.x < 200 && e.button.y > 530 && e.button.y < 630) {
					vector<string> tempArray = initializeFormInput();
					//arrayVisualizer.resetStep();
					listVisualizer.initialize(tempArray);
					globalCode = {};
					linesDisplay = -1;
				}
				else if (e.button.x > 200 && e.button.x < 400 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, val, checkStep, isValid) = indexValueFormInput("InsertForm.png");
					globalCode = {};
					linesDisplay = -1;
					if (isValid) {
						if (checkStep) {
							listVisualizer.insertStep(idx, val);
						}
						else {
							listVisualizer.insert(idx, val);
						}
					}
				}
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 530 && e.button.y < 630) {
					bool checkStep;
					bool isValid;
					tie(idx, checkStep, isValid) = indexFormInput("DeleteForm.png");
					//arrayVisualizer.resetStep();
					globalCode = {};
					linesDisplay = -1;
					//arrayVisualizer.maxStep = 1000;
					if (isValid) {
						if (checkStep) {
						}
						else {
							listVisualizer.deleteNode(idx);
						}
					}
				}
			}
		}
		SDL_RenderPresent(gRenderer);
	}
}
void mainMenu() {
	SDL_Event e;
	SDL_Rect rect;
	SDL_Texture* buttonSpriteSheet = loadImgTexture("resources/Button/ButtonSpriteSheet.jpg");
	SDL_Texture* background = loadImgTexture("resources/Background/Background.jpg");
	SDL_Rect buttonPosArray[15][2];
	for (int i = 0; i < 4; i++) {
		buttonPosArray[i][0] = *createRect(&rect, 1493, 322 + 144 * i, 200, 100);
		buttonPosArray[i][1] = *createRect(&rect, 1763, 322 + 144 * i, 200, 100);
	}
	bool quit = false;
	bool leftMouseDown = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderCopy(gRenderer, background, NULL, &SCREEN);
		createButton(buttonSpriteSheet, &buttonPosArray[0][0], &buttonPosArray[0][1], false, 300, 100, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[1][0], &buttonPosArray[1][1], false, 400, 210, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[2][0], &buttonPosArray[2][1], false, 300, 320, 200, 100, NULL);
		createButton(buttonSpriteSheet, &buttonPosArray[3][0], &buttonPosArray[3][1], false, 400, 430, 200, 100, NULL);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				leftMouseDown = true;
				if (e.button.x > 300 && e.button.x < 500 && e.button.y > 100 && e.button.y < 200) arrayVisualizing();
				else if (e.button.x > 400 && e.button.x < 600 && e.button.y > 210 && e.button.y < 310) linkedListVisualizing();
			}
		}
		SDL_RenderPresent(gRenderer);


	}
	return;
}
int main(int argc, char* args[]) {
	init();
	mainMenu();
	return 0;
}