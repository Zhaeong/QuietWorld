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

  mDirection.x = 0.3;
  mDirection.y = 1;

  SDL_QueryTexture(sTex, NULL, NULL, &mWidth, &mHeight);
  SDL_GetTextureColorMod(mTexture, &mColorR, &mColorG, &mColorB);

}

void Texture::rotateByDegree(int degrees)
{
  mRotation = (mRotation + degrees) % 360;
}

void Texture::updatePosition(int curLevelBoundX, int curLevelBoundY)
{

  float newPosX = mX + mDirection.x * mSpeed;
  float newPosY = mY + mDirection.y * mSpeed; 

  //If haven't hit a wall
  if((newPosX >= 0 && (newPosX + mWidth) <= curLevelBoundX)
    &&(newPosY >= 0 && (newPosY + mHeight) <= curLevelBoundY))
  {
    mX = newPosX;
    mY = newPosY; 
  }
  else
  {
    vect2 norm = {0, 0};
    //Change direction once hit right wall
    if((newPosX + mWidth) >= curLevelBoundX)
    {
      //Normal of right wall
      norm = {-1, 0};
    }

    //Change direction once hit left wall
    if((newPosX) <= 0)
    {
      //Normal of left wall
      norm = {1, 0};
    }

    //Change direction once hit bottom wall
    if((newPosY + mHeight) >= curLevelBoundY)
    {
      //Normal of bottom wall
      norm = {0, -1};
    }

    //Change direction once hit top wall
    if((newPosY) <= 0)
    {
      //Normal of top wall
      norm = {0, 1};
    }

    //Reflection direction vector
    //r: reflection vector 
    //d: direction vector
    //n: normal of the boundary wall
    //r = d - 2(d dot n)n    
    float dotProd = vect2dot(norm, mDirection);

    float newXdir = mDirection.x - 2 * dotProd * norm.x;
    float newYdir = mDirection.y - 2 * dotProd * norm.y;

    mDirection.x = newXdir;
    mDirection.y = newYdir;
    
  }   
}