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

void DrawBoundingBoxCam(SDL_Renderer *renderer,
                     int camX,
                     int camY,
                     int objX,
                     int objY,
                     int objW,
                     int objH,
                     int r,
                     int g,
                     int b);

void DrawBoundingBox(SDL_Renderer *renderer,
                     int objX,
                     int objY,
                     int objW,
                     int objH,
                     int r,
                     int g,
                     int b);

void MoveCameraBaseOnShip(int *camX, int *camY, int objX, int objY, int objH, int objW);

void RenderDebris(vector<Texture> vDebris, int camX, int camY);
#endif // GAME_H
 
