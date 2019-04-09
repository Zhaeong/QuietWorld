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

SDL_Texture* GetSDLTexture(SDL_Renderer *renderer, string textureLocation);

void DrawBoundingBox(SDL_Renderer *renderer,
                     int objX,
                     int objY,
                     int objW,
                     int objH,
                     int r,
                     int g,
                     int b);
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

void RenderTextureByCam(int camX, int camY, SDL_Renderer *renderer, Texture tex);

void InitSpaceUI(SDL_Renderer *renderer, Texture *uiArray);

void RenderUI(SDL_Renderer *renderer, Texture *uiArray, int size);

string GetAction(int *mouseXpos, int *mouseYpos);

string TextureMouseCollision(Texture *arrayTexture, int size, int xPos, int yPos);
/*


SDL_Texture* GetFontText(SDL_Renderer *SRen, string textLocation);

void CenterCamOnPlayer(int *camX,
                       int *camY,
                       int camW,
                       int camH,
                       int plaX,
                       int plaY,
                       int plaW,
                       int plaH);





void MoveCameraBaseOnShip(SDL_Renderer *renderer,
                          int *camX, int *camY, int camW, int camH,
                          int objX, int objY, int objH, int objW,
                          int speed);

void RenderDebris(vector<Texture> vDebris, int camX, int camY);

bool TextureCollide(int x, int y, int width, int height , Texture texB);

void GenerateDebris(SDL_Renderer *renderer, vector<Texture> *vDebris, int camX, int camY);

void RemoveDebris(vector<Texture> *vDebris, int xCord, int yCord);

void CheckDebrisField(SDL_Renderer *renderer,
                      vector<Texture> *vDebris,
                      int *originX,int *originY,
                      int plaX, int plaY,
                      int plaW, int plaH);
*/
#endif // GAME_H
 
