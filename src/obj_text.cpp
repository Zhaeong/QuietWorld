#include "obj_text.h"


Text::Text(SDL_Renderer *SRen, SDL_Texture *STex, string textDisplay, int x, int y)
{
  mRenderer = SRen;
  mTexture = STex;
  mTextDisplay = textDisplay;
  mX = x;
  mY = y;

  drawText();
}

void Text::drawText()
{
  SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

  SDL_Rect srcRect;
  SDL_Rect dstRect;

  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.h = 20;
  srcRect.w = 20;


  dstRect.x = mX;
  dstRect.y = mY;
  dstRect.h = 20;
  dstRect.w = 20;


  //ASCII 'A' == 65
  //ASCII 'B' == 66
  //ASCII 'a' == 97
  //ASCII ' ' == 32
  //ASCII '0' == 48
  //ASCII '1' == 49
  int curPos = mX;
  
  for(std::string::size_type i = 0; i < mTextDisplay.size(); ++i) {

    char curChar = mTextDisplay[i];
    int xTextPos = 0;
    int yTextPos = 0;

    //Capitals
    if((int)curChar >= 65 && (int)curChar <= 90) 
      {
        xTextPos = (int)curChar - 65;
      }
    //lower case
    else if((int)curChar >= 97 && (int)curChar <= 122)
      {
        xTextPos = (int)curChar - 97;
        yTextPos = 20;
      }
    //numbers
    else if((int)curChar >= 48 && (int)curChar <= 57)
      {
        xTextPos = (int)curChar - 48;
        yTextPos = 40;
      }
    //black space
    else
      {
        xTextPos = 200;
        yTextPos = 40;
      }
    
    

    srcRect.x = 20 * xTextPos;
    srcRect.y = yTextPos;
    
    dstRect.x = curPos;
    dstRect.y = mY;

    SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_NONE);

    curPos += 20;
  }

}
