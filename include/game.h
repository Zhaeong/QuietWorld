#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <string>
#include <iostream>
#include <vector>


#include "game_defs.h"
#include "obj_texture.h"

using namespace std;


int StartGame(SDL_Window **window, SDL_Renderer **renderer);

void InitSpaceUI(SDL_Renderer *renderer, vector<Texture> &vGameUI);
void RenderUI(vector<Texture> vGameUI);

string GetAction(int *mouseXpos, int *mouseYpos);

string TextureCollision(vector<Texture> vTexture, int xPos, int yPos);

SDL_Texture* GetFontText(SDL_Renderer *SRen, string textLocation);

void CenterCamOnPlayer(int *camX,
                       int *camY,
                       int camW,
                       int camH,
                       int plaX,
                       int plaY,
                       int plaW,
                       int plaH);

void DrawBoundingBox(SDL_Renderer *renderer,
                     int camX,
                     int camY,
                     int objX,
                     int objY,
                     int objW,
                     int objH);
#endif // GAME_H
 
