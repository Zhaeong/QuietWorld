#include "obj_texture.h"
Texture::Texture(){}

Texture::Texture(SDL_Texture *sTex, string imgLocation)
{
  mTexture = sTex;
  mImgLocation = imgLocation;
  mX = 0;
  mY = 0;
  SDL_QueryTexture(sTex, NULL, NULL, &mWidth, &mHeight);

}


void Texture::rotateByDegree(int degrees)
{
  mRotation = (mRotation + degrees) % 360;
}
/*
void Texture::renderTexture()
{
  SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

  SDL_Rect srcRect;
  SDL_Rect dstRect;

  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.h = mHeight;
  srcRect.w = mWidth;

  dstRect.x = mX;
  dstRect.y = mY;
  dstRect.h = mHeight;
  dstRect.w = mWidth;

  SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &dstRect, mRotation, mCenter, mFlip);
}

void Texture::renderTextureByCam(int camX, int camY)
{
  //Convert character world coord to screen coord
  int xPos = mX - camX;
  int yPos = mY - camY;
 
  SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

  SDL_Rect srcRect;
  SDL_Rect dstRect;

  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.h = mHeight;
  srcRect.w = mWidth;

  dstRect.x = xPos;
  dstRect.y = yPos;
  dstRect.h = mHeight;
  dstRect.w = mWidth;

  SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &dstRect, mRotation, mCenter, mFlip);

  if(DEBUG == 1)
  {
    DrawBoundingBox(mRenderer, xPos, yPos, mWidth, mHeight, 255, 55, 0);
  }
  
}
*/
