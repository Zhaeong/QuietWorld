#ifndef OBJ_TEXT_H
#define OBJ_TEXT_H


#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;


struct TextObj
{
  string textString = "";
  int mX = 0;
  int mY = 0;
  int textDelay = 0;
};

class Text
{
public:

  SDL_Renderer *mRenderer = NULL;
  SDL_Texture *mTexture = NULL;

  vector<TextObj> mTextArray;
  
  Text(SDL_Renderer *SRen, SDL_Texture *STex);

  void drawText();
  void addTextObj(string textString, int x, int y, int delay);
  void clearText();
  
};


#endif // OBJ_TEXT_H
