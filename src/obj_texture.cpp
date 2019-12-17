#include "obj_texture.h"
Texture::Texture(){}

Texture::Texture(SDL_Texture *sTex, string imgLocation)
{
  mTexture = sTex;
  mImgLocation = imgLocation;
  mX = 0;
  mY = 0;

  //Debris specific stuff
  mSpeed = 0;
  mDirection.x = 1;
  mDirection.y = 0;

  SDL_QueryTexture(sTex, NULL, NULL, &mWidth, &mHeight);
  SDL_GetTextureColorMod(mTexture, &mColorR, &mColorG, &mColorB);

}

void Texture::rotateByDegree(int degrees)
{
  mRotation = (mRotation + degrees) % 360;
}

void Texture::updatePosition()
{
  cout << "debx: " << mX << "\n";

  mX += mDirection.x * mSpeed;
  mY += mDirection.y * mSpeed; 
      
}