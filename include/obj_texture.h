#ifndef OBJ_TEXTURE_H
#define OBJ_TEXTURE_H


#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <cstring>

#include "game_defs.h"


using namespace std;

class Texture
{
public:
  
  string mImgLocation = "";
  SDL_Texture *mTexture = NULL;

  int mWidth = 0;
  int mHeight = 0;

  float mX = 0;
  float mY = 0;

  //Original colors
  Uint8 mColorR;
  Uint8 mColorG;
  Uint8 mColorB;

  int mRotation = 0;
  SDL_Point *mCenter = NULL;
  SDL_RendererFlip mFlip = SDL_FLIP_NONE;

  bool mRender = true;

  void* mPixels = 0;
  int mPitch = 0;

  //Debris specific stuff
  float mSpeed = 0;
  vect2 mDirection;


  Texture();
  Texture(SDL_Texture *sTex, string imgLocation);

  void rotateByDegree(int degrees);
  void updatePosition();
  
};

#endif // OBJ_TEXTURE_H
