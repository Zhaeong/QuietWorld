#ifndef OBJ_TEXT_H
#define OBJ_TEXT_H


#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <cstring>

using namespace std;
class Text
{
 public:

  int mX = 0;
  int mY = 0;
  SDL_Renderer *mRenderer = NULL;
  SDL_Texture *mTexture = NULL;

  string mTextDisplay = "";

  
  Text(SDL_Renderer *SRen, SDL_Texture *STex, string textDisplay, int x, int y);

  void drawText();
};


#endif // OBJ_TEXT_H
