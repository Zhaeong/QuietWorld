#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <string>
#include <iostream>
#include <vector>

#include "game_defs.h"
#include "obj_texture.h"

using namespace std;

//Forward declaration due to obj_texture including this file
class Texture;

int StartSDL(SDL_Window **window, SDL_Renderer **renderer);

void InitSpaceUI(SDL_Renderer *renderer, vector<Texture> &vGameUI);
void RenderUI(vector<Texture> vGameUI);

string GetAction(int *mouseXpos, int *mouseYpos);

string TextureMouseCollision(vector<Texture> vTexture, int xPos, int yPos);

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

void MoveCameraBaseOnShip(SDL_Renderer *renderer,
                          int *camX, int *camY, int camW, int camH,
                          int objX, int objY, int objH, int objW,
                          int speed);

void RenderDebris(vector<Texture> vDebris, int camX, int camY);

bool TextureCollide(int x, int y, int width, int height , Texture texB);

#endif // GAME_H
 
