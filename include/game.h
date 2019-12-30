#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <string>
#include <iostream>

#include "game_defs.h"
#include "obj_texture.h"
#include "obj_ship.h"

using namespace std;

//Forward declaration due to obj_texture including this file
//class Texture;

//Foward declaration due to obj_ship including this file
//class Ship;

int StartSDL(SDL_Window **window, SDL_Renderer **renderer);

SDL_Texture* GetSDLTexture(SDL_Renderer *renderer, SDL_Window *window, string textureLocation);

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

void RenderTexture(SDL_Renderer *renderer, Texture tex);

void RenderTextureByCam(int camX, int camY, SDL_Renderer *renderer, Texture tex);

void RemoveTextureWhiteSpace(SDL_Window *window, SDL_Texture *texture);

void RenderShip(SDL_Renderer *renderer, int camX, int camY, Ship ship);

void InitSpaceUI(SDL_Renderer *renderer, SDL_Window *window, Texture *uiArray);

void InitIntroUI(SDL_Renderer *renderer, SDL_Window *window, Texture *uiArray);

void InitInterLevelUI(SDL_Renderer *renderer, SDL_Window *window, Texture *uiArray);

void RenderUI(SDL_Renderer *renderer, Texture *uiArray, int size);

string GetAction(int *mouseXpos, int *mouseYpos);

string TextureMouseCollision(Texture *arrayTexture, int size, int xPos, int yPos);

bool TextureMouseCollisionSingle(Texture mTexture, int xPos, int yPos);

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

void GenerateDebris(SDL_Texture *debrisTex, Texture *debrisArray, int numDebris, int xMax, int yMax, int level);

void UpdateDebris(Texture *debrisArray, int boundX, int boundY);

void RenderDebris(SDL_Renderer *renderer, Texture *debrisArray, int camX, int camY);

void RenderText(SDL_Renderer *renderer, SDL_Texture *fontTexture, TextObj *textArray);

bool RenderSurveyText(SDL_Renderer *renderer, SDL_Texture *fontTexture, TextObj *textArray, int numTexts);

void RenderTextWithDelays(SDL_Renderer *renderer, SDL_Texture *fontTexture, TextObj *textArray, int numTexts);

bool TextureCollide(int x, int y, int width, int height , Texture texB);

void SetTextureColorMod(Texture tex);

void SetShipColorMod(Ship ship);

void SetTextString(TextObj *text, string textContent);

void SetIntroText(TextObj *textArray);

void SetInterLevelChoices(TextObj *textArray,
                          string question,
                          string choiceA,
                          string choiceB,
                          string responseA,
                          string responseB);

int GetActiveDebrisNum(Texture *debrisArray);

void SetInterLevelText(TextObj *textArraySurvey, 
                       SDL_Texture *debrisTex, 
                       Texture *debrisArray, 
                       int gameLevel, 
                       int backgroundWidth, 
                       int backgroundHeight);

#endif // GAME_H
 
