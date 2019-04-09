#include "obj_text.h"


Text::Text(SDL_Renderer *SRen, SDL_Texture *STex)
{
  mRenderer = SRen;
  mTexture = STex;
}


//ASCII 'A' == 65
//ASCII 'B' == 66
//ASCII 'a' == 97
//ASCII ' ' == 32
//ASCII '0' == 48
//ASCII '1' == 49
void Text::drawText()
{
  SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

  SDL_Rect srcRect;
  SDL_Rect dstRect;

  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.h = 20;
  srcRect.w = 20;


  dstRect.x = 0;
  dstRect.y = 0;
  dstRect.h = 20;
  dstRect.w = 20;

  for(unsigned int i = 0; i < mTextArray.size(); i++)
  {

    TextObj& tObj = mTextArray[i];

    int curPos = tObj.mX;
  
    for(string::size_type j = 0; j < tObj.textString.size(); ++j) {

      char curChar = tObj.textString[j];
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
      //minus sign
      else if((int)curChar == 45)
      {
        xTextPos = 0;
        yTextPos = 60;
      }
      //dot
      else if((int)curChar == 46)
      {
        xTextPos = 1;
        yTextPos = 60;
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
      dstRect.y = tObj.mY;

      SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_NONE);

      curPos += 20;
    }
      
  }
  

}

void Text::addTextObj(string textString, int x, int y, int delay)
{
  TextObj newTextObj = {textString, x, y, delay};
  mTextArray.push_back(newTextObj);
  
}

void Text::clearText()
{
  mTextArray.clear();
}
