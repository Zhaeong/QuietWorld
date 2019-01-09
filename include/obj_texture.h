#ifndef OBJ_TEXTURE_H
#define OBJ_TEXTURE_H


#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

class Texture
{
public:


  string mImgLocation = "";
  SDL_Renderer *mRenderer = NULL;
  SDL_Texture *mTexture = NULL;

  int mWidth = 0;
  int mHeight = 0;

  void* mPixels = 0;
  int mPitch = 0;


  
  Texture(SDL_Renderer *SRen, string imgLocation);
  void loadTexture();
  
};

#endif // OBJ_TEXTURE_H
