#ifndef OBJ_TEXTURE_H
#define OBJ_TEXTURE_H


#include <SDL.h>
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

  int mX = 0;
  int mY = 0;

  double mRotation = 0;
  SDL_Point *mCenter = NULL;
  SDL_RendererFlip mFlip = SDL_FLIP_NONE;

  void* mPixels = 0;
  int mPitch = 0;

  Texture(SDL_Renderer *SRen, string imgLocation);
  void loadTexture();
  void renderTexture();
  
};

#endif // OBJ_TEXTURE_H
