#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <string>
#include <iostream>
#include <vector>

#include "obj_texture.h"

using namespace std;

int StartGame(SDL_Window **window, SDL_Renderer **renderer);

void InitSpaceUI(SDL_Renderer *renderer, vector<Texture> &vGameUI);
void RenderUI(vector<Texture> vGameUI);

#endif // GAME_H
 
