#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include <stdio.h>
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
#include "global.h"
#include "utils.h"
#include "UI_components.h"
int main(int, char**)
{
	init();
	numberFont= TTF_OpenFont("resources/Font/SpaceMono-Regular.ttf", 300);
	startingScreen();
	return 0;
}