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
