#include "game.h"

int StartSDL(SDL_Window **window, SDL_Renderer **renderer)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't initialize SDL: %s",
                 SDL_GetError());
  }

  //SDL_WINDOW_FULLSCREEN
  //SDL_WINDOW_RESIZABLE
  if (SDL_CreateWindowAndRenderer(GAMEWIDTH,
                                  GAMEHEIGHT,
                                  SDL_WINDOW_RESIZABLE,
                                  window,
                                  renderer))
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't create window and renderer: %s",
                 SDL_GetError());
  }

  return 0;
}

SDL_Texture* GetSDLTexture(SDL_Renderer *renderer, string textureLocation)
{
  //Make sure to initialize texture to null or else SDL_DestroyTexture will crash program
  SDL_Texture *texture = NULL;

  SDL_Surface* loadedSurface = IMG_Load( textureLocation.c_str() );

  if( loadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL_image Error: %s\n",
            textureLocation.c_str(),
            IMG_GetError() );
  }
  else
  {
    //Convert surface to display format
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface,
                                                              SDL_PIXELFORMAT_ARGB8888,
                                                              0 );
    if( formattedSurface == NULL )
    {
      printf( "Unable to convert loaded surface to display format! SDL Error: %s\n",
              SDL_GetError() );
    }
    else
    {
      //Create blank streamable texture
      /*
      texture = SDL_CreateTexture( renderer,
                                   SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   formattedSurface->w,
                                   formattedSurface->h );
      */

      texture = SDL_CreateTextureFromSurface( renderer, formattedSurface );
      if( texture == NULL )
      {
        printf( "Unable to create blank texture! SDL Error: %s\n",
                SDL_GetError() );
      }      

      //Get rid of old formatted surface
      SDL_FreeSurface( formattedSurface );
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );

  }
  return texture;
}

void DrawBoundingBox(SDL_Renderer *renderer,
                     int objX,
                     int objY,
                     int objW,
                     int objH,
                     int r,
                     int g,
                     int b)
{
  SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);

  //top
  SDL_RenderDrawLine(renderer,
                     objX,
                     objY,
                     objX + objW,
                     objY);
  //left
  SDL_RenderDrawLine(renderer,
                     objX,
                     objY,
                     objX,
                     objY + objH);

  //right
  SDL_RenderDrawLine(renderer,
                     objX + objW,
                     objY,
                     objX + objW,
                     objY + objH);
  //right
  SDL_RenderDrawLine(renderer,
                     objX,
                     objY +objH,
                     objX + objW,
                     objY + objH);

}

void DrawBoundingBoxCam(SDL_Renderer *renderer,
                        int camX,
                        int camY,
                        int objX,
                        int objY,
                        int objW,
                        int objH,
                        int r,
                        int g,
                        int b)
{
  SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);

  int xPos = objX - camX;
  int yPos = objY - camY;

  //top
  SDL_RenderDrawLine(renderer,
                     xPos,
                     yPos,
                     xPos + objW,
                     yPos);
  //left
  SDL_RenderDrawLine(renderer,
                     xPos,
                     yPos,
                     xPos,
                     yPos + objH);

  //right
  SDL_RenderDrawLine(renderer,
                     xPos + objW,
                     yPos,
                     xPos + objW,
                     yPos + objH);
  //right
  SDL_RenderDrawLine(renderer,
                     xPos,
                     yPos +objH,
                     xPos + objW,
                     yPos + objH);

}

void RenderTexture(SDL_Renderer *renderer, Texture tex)
{
  if(tex.mRender)
  {
    SDL_SetTextureBlendMode(tex.mTexture, SDL_BLENDMODE_BLEND);

    SDL_Rect srcRect;
    SDL_Rect dstRect;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.h = tex.mHeight;
    srcRect.w = tex.mWidth;

    dstRect.x = tex.mX;
    dstRect.y = tex.mY;
    dstRect.h = tex.mHeight;
    dstRect.w = tex.mWidth;

    SDL_RenderCopyEx(renderer, tex.mTexture, &srcRect, &dstRect, tex.mRotation, tex.mCenter, tex.mFlip);
  }
}

void RenderTextureByCam(int camX, int camY, SDL_Renderer *renderer, Texture tex)
{
  if(tex.mRender)
  {
    //Convert character world coord to screen coord
    int xPos = tex.mX - camX;
    int yPos = tex.mY - camY;
 
    SDL_SetTextureBlendMode(tex.mTexture, SDL_BLENDMODE_BLEND);

    SDL_Rect srcRect;
    SDL_Rect dstRect;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.h = tex.mHeight;
    srcRect.w = tex.mWidth;

    dstRect.x = xPos;
    dstRect.y = yPos;
    dstRect.h = tex.mHeight;
    dstRect.w = tex.mWidth;

    SDL_RenderCopyEx(renderer, tex.mTexture, &srcRect, &dstRect, tex.mRotation, tex.mCenter, tex.mFlip);

    if(DEBUG == 1)
    {
      DrawBoundingBox(renderer, xPos, yPos, tex.mWidth, tex.mHeight, 255, 55, 0);
    }
  }
}

void RenderShip(SDL_Renderer *renderer, int camX, int camY, Ship ship)
{
    //Convert character world coord to screen coord
  int xPos = ship.mPosition.x - camX;
  int yPos = ship.mPosition.y - camY;
 
  SDL_SetTextureBlendMode(ship.mShipTexture, SDL_BLENDMODE_BLEND);

  SDL_Rect srcRect;
  SDL_Rect dstRect;

  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.h = ship.mHeight;
  srcRect.w = ship.mWidth;

  dstRect.x = xPos;
  dstRect.y = yPos;
  dstRect.h = ship.mHeight;
  dstRect.w = ship.mWidth;

  SDL_RenderCopyEx(renderer, ship.mShipTexture, &srcRect, &dstRect, ship.mRotation, ship.mCenter, ship.mFlip);

  if(DEBUG == 1)
  {
    DrawBoundingBox(renderer, xPos, yPos, ship.mWidth, ship.mHeight, 255, 55, 0);
  }
}

void InitSpaceUI(SDL_Renderer *renderer, Texture *uiArray)
{ 
  SDL_Texture *backgroundSDLTex = GetSDLTexture(renderer, GRAYBACKGROUND);
  Texture background(backgroundSDLTex, GRAYBACKGROUND);
  background.mX = 0;
  background.mY = GAMEHEIGHT * 2/3;
  uiArray[0] = background;

  //Add rotation controls  
  SDL_Texture *leftButtonSDLTex = GetSDLTexture(renderer, BTN_LEFTCURSOR);
  Texture leftButton(leftButtonSDLTex, BTN_LEFTCURSOR);
  leftButton.mX = 50;
  leftButton.mY = GAMEHEIGHT * 2/3 + 50;
  uiArray[1] = leftButton;
  
  SDL_Texture *stopButtonSDLTex = GetSDLTexture(renderer, BTN_STOPROT);
  Texture stopButton(stopButtonSDLTex, BTN_STOPROT);
  stopButton.mX = leftButton.mX + leftButton.mWidth + 20;
  stopButton.mY = GAMEHEIGHT * 2/3 + 50;
  uiArray[2] = stopButton;

  SDL_Texture *rightButtonSDLTex = GetSDLTexture(renderer, BTN_RIGHTCURSOR);
  Texture rightButton(rightButtonSDLTex, BTN_RIGHTCURSOR);
  rightButton.mX = stopButton.mX + stopButton.mWidth + 20;
  rightButton.mY = GAMEHEIGHT * 2/3 + 50;
  uiArray[3] = rightButton;

  SDL_Texture *speedincreaseButtonSDLTex = GetSDLTexture(renderer, BTN_INCREASESPEED);
  Texture speedincreaseButton(speedincreaseButtonSDLTex, BTN_INCREASESPEED);
  speedincreaseButton.mX = rightButton.mX + rightButton.mWidth + 20;
  speedincreaseButton.mY = GAMEHEIGHT * 2/3 + 50;
  uiArray[4] = speedincreaseButton;

  SDL_Texture *speeddecreaseButtonSDLTex = GetSDLTexture(renderer, BTN_DECREASESPEED);
  Texture speeddecreaseButton(speeddecreaseButtonSDLTex, BTN_DECREASESPEED);
  speeddecreaseButton.mX = speedincreaseButton.mX + speedincreaseButton.mWidth + 20;
  speeddecreaseButton.mY = GAMEHEIGHT * 2/3 + 50;
  uiArray[5] = speeddecreaseButton;

  SDL_Texture *harvestButtonSDLTex = GetSDLTexture(renderer, BTN_HARVESTDEBRIS);
  Texture harvestButton(harvestButtonSDLTex, BTN_HARVESTDEBRIS);
  harvestButton.mX = speeddecreaseButton.mX + speeddecreaseButton.mWidth + 20;
  harvestButton.mY = GAMEHEIGHT * 2/3 + 50;
  uiArray[6] = harvestButton;

  SDL_Texture *harvestButtonActiveSDLTex = GetSDLTexture(renderer, BTN_HARVESTDEBRIS_ACTIVE);
  Texture harvestButtonActive(harvestButtonActiveSDLTex, BTN_HARVESTDEBRIS_ACTIVE);
  harvestButtonActive.mX = speeddecreaseButton.mX + speeddecreaseButton.mWidth + 20;
  harvestButtonActive.mY = GAMEHEIGHT * 2/3 + 50;
  harvestButtonActive.mRender = false;
  uiArray[7] = harvestButtonActive;

  
}

void RenderUI(SDL_Renderer *renderer, Texture *uiArray, int size)
{
  for (int i = 0; i < size; ++i)
  {
    RenderTexture(renderer, uiArray[i]);
  }
}


string GetAction(int *mouseXpos, int *mouseYpos)
{
  string eventName = "NONE";
  // Poll for events. SDL_PollEvent() returns 0 when there are no  
  // more events on the event queue, our while loop will exit when 
  // that occurs.                
  

  SDL_Event event;
  
  while( SDL_PollEvent( &event ) )
  {
    switch( event.type )
    {
    case SDL_KEYDOWN:

      // Check the SDLKey values and move change the coords
      switch( event.key.keysym.sym )
      {
      case SDLK_ESCAPE:
        eventName = "EXIT";
        break;
      case SDLK_LEFT:
        eventName = "MOVE_LEFT";
        break;
      case SDLK_RIGHT:
        eventName = "MOVE_RIGHT";
        break;
      case SDLK_UP:
        eventName = "MOVE_UP";
        break;
      case SDLK_DOWN:
        eventName = "MOVE_DOWN";
        break;
      case SDLK_e:
        eventName = "KEY_E";
        break;
      default:
        eventName = "KEY_DOWN";
        break;
      }
      break;

    case SDL_KEYUP:
      eventName =  "KEY_UP";
      break;

    case SDL_MOUSEBUTTONDOWN:
      eventName = "MOUSE_DOWN";
      break;

    case SDL_MOUSEBUTTONUP:
      eventName = "MOUSE_UP";
      break;

    case SDL_QUIT:
      eventName = "EXIT";

    default:
      break;
    }
  }
  SDL_GetMouseState( mouseXpos, mouseYpos );
  return eventName;
}

string TextureMouseCollision(Texture *arrayTexture, int size, int xPos, int yPos)
{
  string colTex = "NONE";
  for (int i = 0; i < size; ++i)
  {
    Texture texRef = arrayTexture[i];
  
    if(xPos >= texRef.mX
       && xPos <= (texRef.mX + texRef.mWidth)
       && yPos >= texRef.mY
       && yPos <= (texRef.mY + texRef.mHeight)
       && texRef.mRender)
    {
      colTex = texRef.mImgLocation;
    }
  }
  return colTex;
}

void CenterCamOnPlayer(int *camX,
                       int *camY,
                       int camW,
                       int camH,
                       int plaX,
                       int plaY,
                       int plaW,
                       int plaH)
{
  
  *camX = plaX + (plaW/2) - (camW/2);
  *camY = plaY + (plaH/2) - (camH/2);
}

void MoveCameraBaseOnShip(SDL_Renderer *renderer,
                          int *camX, int *camY, int camW, int camH,
                          int objX, int objY, int objH, int objW,
                          int objSpeed)
{
  int boundD = 150;

  int boundX = camW/2 - boundD/2;
  int boundY = camH/2 - boundD/2;

  //Convert obj from world coord to screen coord
  int objXscreen = objX - *camX;
  int objYscreen = objY - *camY;  

  if(objXscreen < boundX)
  {
    *camX -= objSpeed;
  }
  else if(objXscreen + objW > boundX + boundD)
  {
    *camX += objSpeed;
  }

  if(objYscreen < boundY)
  {
    *camY -= objSpeed;
  }
  else if(objYscreen + objH > boundY + boundD)
  {
    *camY += objSpeed;
  }
  
  if(DEBUG == 1)
  {
    DrawBoundingBox(renderer,
                    boundX,
                    boundY,
                    boundD,
                    boundD,
                    0,
                    255,
                    0);
  }
}


void GenerateDebris(SDL_Texture *debrisTex, Texture *debrisArray, int arrStart, int arrEnd, int xCord, int yCord)
{
  int genXmin = xCord;
  int genXmax = xCord + DEBRISWIDTH;

  int genYmin = yCord;
  int genYmax = yCord + DEBRISHEIGHT;  

 
  //cout << "xMin: " << genXmin << "\n";
  //cout << "xMax: " << genXmax << "\n";
  //cout << "yMin: " << genYmin << "\n";
  //cout << "yMax: " << genYmax << "\n";
  
  
  for(int i = arrStart; i < arrEnd; ++i)
  {
    Texture dObj(debrisTex, DEBRIS_IMG);

    //Generate a random number between min and max
    dObj.mX = (rand() % (genXmax - genXmin)) + genXmin;
    dObj.mY = (rand() % (genYmax - genYmin)) + genYmin;

    debrisArray[i] = dObj;

    //cout << "x: " << dObj.mX << " y: " << dObj.mY << "\n";
  }
}

void RenderDebris(SDL_Renderer *renderer, Texture *debrisArray, int camX, int camY)
{
  for (unsigned i = 0; i < NUM_DEBRIS; ++i)
  {
    RenderTextureByCam(camX, camY, renderer, debrisArray[i]);    
  }
}

void RenderText(SDL_Renderer *renderer, SDL_Texture *fontTexture, TextObj *textArray)
{
  SDL_SetTextureBlendMode(fontTexture, SDL_BLENDMODE_BLEND);

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

  for(int i = 0; i < NUM_TEXT; i++)
  {
    
    TextObj tObj = textArray[i];

    int curPosX = tObj.mX;
    int curPosY = tObj.mY;

    unsigned int numLetters = tObj.mLetters;

   
    if(tObj.mDelay == 0)
    {
      numLetters = tObj.mString.size();
    }
    else
    {
      unsigned int gameTime = SDL_GetTicks();

      //Compares the last time the letter was incremented to cur time
      //If enough time elpased > mDelay then increment num letters by one
      if(gameTime - tObj.mLastTime > tObj.mDelay)
      {
        if(tObj.mLetters < tObj.mString.size())
        {
          tObj.mLetters += 1;
          tObj.mLastTime = gameTime;
          textArray[i] = tObj;
        }
        
      }

    }
    for(unsigned int j = 0; j < numLetters; ++j) {

      char curChar = tObj.mString[j];
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
    
      dstRect.x = curPosX;
      dstRect.y = curPosY;

      SDL_RenderCopyEx(renderer, fontTexture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_NONE);

      curPosX += 20;

      //Wraps to next line if the text exceeds the width
      if(curPosX > GAMEWIDTH)
      {
        curPosX = tObj.mX;
        curPosY += 20;
      }
    }
  }
}



bool TextureCollide(int x, int y, int width, int height , Texture texB)
{
  //Check horizontal collision
  bool horizontalCol = false;

  //If the item is not being rendered, it is not collidable
  if(!texB.mRender)
  {
    return false;
  }
  if(x + width >= texB.mX && x <= texB.mX + texB.mWidth)
  {
    horizontalCol = true;
  }

  //Check vertical collision
  bool verticalCol = false;
  if(y + height >= texB.mY && y <= texB.mY + texB.mHeight)
  {
    verticalCol = true;
  }

  return horizontalCol && verticalCol;
}

//A becomes B, B is destoryed
void SwapArrayPointers(Texture *debrisArray, int startA, int endA, int startB, int endB)
{
  if(endA - startA == endB - startB)
  {
    int range = endA - startA;
    
    for(int i = 0; i < range; ++i)
    {      
      debrisArray[startB + i] = debrisArray[startA + i];
    }
  }
  else
  {
    cout << "Error Swaparraypointers bound mismatch\n";
  }
}

void CheckDebrisField(SDL_Texture *debrisTex,
                      Texture *debrisArray,
                      int *originX,int *originY,
                      int plaX, int plaY)
{

  //If player moves past left bound, delete the right column
  //and regen the left column
  if(plaX < *originX)
  {
    //Swap Arrays
    //Order is important since we dont want to erase arrays we still use
    //Start with the right column since that's all getting removed

    //Copy Top to Top Right
    SwapArrayPointers(debrisArray, 20, 30, 30, 40);

    //Copy Mid to Right
    SwapArrayPointers(debrisArray, 0, 10, 50, 60);

    //Copy Bottom to Bottom Right
    SwapArrayPointers(debrisArray, 70, 80, 80, 90);   
    
    //Copy Top left to Top
    SwapArrayPointers(debrisArray, 10, 20, 20, 30);    
    
    //Copy Left to Mid
    SwapArrayPointers(debrisArray, 40, 50, 0, 10);    

    //Copy Bottom Left to Bottom
    SwapArrayPointers(debrisArray, 60, 70, 70, 80);    
    
    //Set new originX
    *originX = *originX - GAMEWIDTH;
    
    //Top Left
    GenerateDebris(debrisTex, debrisArray, 10, 20,  *originX - GAMEWIDTH, *originY - GAMEHEIGHT);
    //Left
    GenerateDebris(debrisTex, debrisArray, 40, 50, *originX - GAMEWIDTH, *originY);
    //Bottom Left
    GenerateDebris(debrisTex, debrisArray, 60, 70, *originX - GAMEWIDTH, *originY + GAMEHEIGHT);

  }
  //If player moves past right bound, delete the left column
  //and regen the right column
  else if(plaX > *originX + GAMEWIDTH)
  {
    //Copy Top to Top Left
    SwapArrayPointers(debrisArray, 20, 30, 10, 20);

    //Copy Mid to Left
    SwapArrayPointers(debrisArray, 0, 10, 40, 50);

    //Copy Bottom to Bottom Left
    SwapArrayPointers(debrisArray, 70, 80, 60, 70);   
    
    //Copy Top right to Top
    SwapArrayPointers(debrisArray, 30, 40, 20, 30);    
    
    //Copy right to Mid
    SwapArrayPointers(debrisArray, 50, 60, 0, 10);    

    //Copy bottom right to Bottom   
    SwapArrayPointers(debrisArray, 80, 90, 70, 80);
    
    //Set new originX
    *originX = *originX + GAMEWIDTH;

    //Top Right
    GenerateDebris(debrisTex, debrisArray, 30, 40, *originX + GAMEWIDTH, *originY - GAMEHEIGHT);
    //Right
    GenerateDebris(debrisTex, debrisArray, 50, 60, *originX + GAMEWIDTH, *originY);
    //Bottom Right    
    GenerateDebris(debrisTex, debrisArray, 80, 90, *originX + GAMEWIDTH, *originY - GAMEHEIGHT);
  }
  //If player moves past top bound, delete the bottom row
  //and regen the top row
  else if(plaY < *originY)
  {
    //Copy Left to Bottom Left
    SwapArrayPointers(debrisArray, 40, 50, 60, 70);
    
    //Copy Mid to Bottom
    SwapArrayPointers(debrisArray, 0, 10, 70, 80);

    //Copy Right to Bottom Right
    SwapArrayPointers(debrisArray, 50, 60, 80, 90);   
    
    //Copy Top Left to Left
    SwapArrayPointers(debrisArray, 10, 20, 40, 50);    
    
    //Copy Top to Mid
    SwapArrayPointers(debrisArray, 20, 30, 0, 10);    

    //Copy Top right to right
    SwapArrayPointers(debrisArray, 30, 40, 50, 60);
    
    //Set new originX
    *originY = *originY - GAMEHEIGHT;

    //Top Left
    GenerateDebris(debrisTex, debrisArray, 10, 20, *originX - GAMEWIDTH, *originY - GAMEHEIGHT);
    //Top
    GenerateDebris(debrisTex, debrisArray, 20, 30, *originX, *originY - GAMEHEIGHT);
    //Top Right    
    GenerateDebris(debrisTex, debrisArray, 30, 40, *originX + GAMEWIDTH, *originY - GAMEHEIGHT);
  }
  //If player moves past bottom bound, delete the top row
  //and regen the bottom row
  else if(plaY  > *originY + GAMEHEIGHT)
  {
    
    //Copy Left to Top Left
    SwapArrayPointers(debrisArray, 40, 50, 10, 20);
    
    //Copy Mid to Top
    SwapArrayPointers(debrisArray, 0, 10, 20, 30);

    //Copy Right to Top Right
    SwapArrayPointers(debrisArray, 50, 60, 30, 40);   
    
    //Copy Bottom Left to Left
    SwapArrayPointers(debrisArray, 60, 70, 40, 50);    
    
    //Copy Bottom to Mid
    SwapArrayPointers(debrisArray, 70, 80, 0, 10);    

    //Copy Bottom right to right
    SwapArrayPointers(debrisArray, 80, 90, 50, 60);
    
    //Set new originX
    *originY = *originY + GAMEHEIGHT;

    //Bottom Left
    GenerateDebris(debrisTex, debrisArray, 60, 70, *originX - GAMEWIDTH, *originY + GAMEHEIGHT);
    //Bottom
    GenerateDebris(debrisTex, debrisArray, 70, 80, *originX, *originY + GAMEHEIGHT);
    //Bottom Right
    GenerateDebris(debrisTex, debrisArray, 80, 90, *originX + GAMEWIDTH, *originY + GAMEHEIGHT);
  }
}
