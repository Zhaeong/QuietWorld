#include "game.h"

int StartSDL(SDL_Window **window, SDL_Renderer **renderer)
{

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't initialize SDL: %s",
                 SDL_GetError());
  }

  //Init Audio
  if (SDL_Init(SDL_INIT_AUDIO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't initialize SDL: %s",
                 SDL_GetError());
  }

   //Initialize SDL_mixer
  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
  {
      printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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

SDL_Texture *GetSDLTexture(SDL_Renderer *renderer, SDL_Window *window, string textureLocation)
{
  //Make sure to initialize texture to null or else SDL_DestroyTexture will crash program
  SDL_Texture *texture = NULL;

  SDL_Surface *loadedSurface = IMG_Load(textureLocation.c_str());

  if (loadedSurface == NULL)
  {
    printf("Unable to load image %s! SDL_image Error: %s\n",
           textureLocation.c_str(),
           IMG_GetError());
  }
  else
  {
    //Convert surface to display format

    //Note have to use SDL_PIXELFORMAT_ARGB8888 due to window using SDL_PIXELFORMAT_RGB888
    //Which doesn't have alpha channel

    //SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface,
    //                                                          SDL_GetWindowPixelFormat( window ),
    //                                                          0 );

    cout << "Loading Texture: " << textureLocation << "\n";
    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface,
                                                             TEXTUREFORMAT,
                                                             0);

    if (formattedSurface == NULL)
    {
      printf("Unable to convert loaded surface to display format! SDL Error: %s\n",
             SDL_GetError());
    }
    else
    {
      //Create blank streamable texture

      //texture = SDL_CreateTexture( renderer,
      //                             SDL_GetWindowPixelFormat( window ),
      //                             SDL_TEXTUREACCESS_STREAMING,
      //                             formattedSurface->w,
      //                             formattedSurface->h );

      texture = SDL_CreateTexture(renderer,
                                  TEXTUREFORMAT,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  formattedSurface->w,
                                  formattedSurface->h);

      //texture = SDL_CreateTextureFromSurface( renderer, formattedSurface );
      if (texture == NULL)
      {
        printf("Unable to create blank texture! SDL Error: %s\n",
               SDL_GetError());
      }
      else
      {
        void *mPixels;
        int mPitch;

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        //Lock texture for manipulation
        if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
        {
          printf("Unable to lock texture! %s\n", SDL_GetError());
        }

        //Copy loaded/formatted surface pixels
        memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

        //Unlock texture to update
        SDL_UnlockTexture(texture);

        mPixels = NULL;

        //Get image dimensions
        //mWidth = formattedSurface->w;
        //mHeight = formattedSurface->h;
      }

      //Get rid of old formatted surface
      SDL_FreeSurface(formattedSurface);
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
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
                     objY + objH,
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
                     yPos + objH,
                     xPos + objW,
                     yPos + objH);
}

void RenderTexture(SDL_Renderer *renderer, Texture tex)
{
  if (tex.mRender)
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
  if (tex.mRender)
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

    SetTextureColorMod(tex);

    SDL_RenderCopyEx(renderer, tex.mTexture, &srcRect, &dstRect, tex.mRotation, tex.mCenter, tex.mFlip);

    if (DEBUG == 1)
    {
      DrawBoundingBox(renderer, xPos, yPos, tex.mWidth, tex.mHeight, 255, 55, 0);
    }
  }
}

void RemoveTextureWhiteSpace(SDL_Window *window, SDL_Texture *texture)
{
  void *mPixels;
  int mPitch;

  if (texture == NULL)
  {
    printf("Input Texture is null in RemoveTextureWhiteSpace! SDL Error: %s\n",
           SDL_GetError());
  }

  if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
  {
    printf("Unable to lock texture! %s\n", SDL_GetError());
  }
  else
  {
    //Allocate format from window
    //Uint32 format = SDL_GetWindowPixelFormat( window );

    SDL_PixelFormat *mappingFormat = SDL_AllocFormat(TEXTUREFORMAT);

    int texWidth = 0;
    int texHeight = 0;

    Uint32 texFormat;
    SDL_QueryTexture(texture, &texFormat, NULL, &texWidth, &texHeight);

    //Get pixel data
    Uint32 *pixels = (Uint32 *)mPixels;
    int pixelCount = (mPitch / 4) * texHeight;

    //cout << "texformat:" << texFormat << "\n";
    //cout << "texWidth:" << texWidth << "\n";
    //cout << "texHeight:" << texHeight << "\n";
    //cout << "mPitch:" << mPitch << "\n";
    //cout << "pixelCount:" << pixelCount << "\n";

    //Map colors

    Uint32 colorKeyWhite = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0xFF);
    Uint32 colorKeyGray = SDL_MapRGBA(mappingFormat, 153, 153, 153, 0xFF);

    Uint32 transparent = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0);

    //Color key pixels
    for (int i = 0; i < pixelCount; ++i)
    {
      //cout << "col:" << pixels[i] << "\n";
      //cout << "key:" << colorKey << "\n";

      if (pixels[i] == colorKeyWhite || pixels[i] == colorKeyGray)
      {
        pixels[i] = transparent;
      }
    }

    SDL_UnlockTexture(texture);
    //mPixels = NULL;
    //Free format
    SDL_FreeFormat(mappingFormat);
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

  SetShipColorMod(ship);

  SDL_RenderCopyEx(renderer, ship.mShipTexture, &srcRect, &dstRect, ship.mRotation, ship.mCenter, ship.mFlip);

  if (DEBUG == 1)
  {
    DrawBoundingBox(renderer, xPos, yPos, ship.mWidth, ship.mHeight, 255, 55, 0);
  }
}

void InitSpaceUI(SDL_Renderer *renderer, SDL_Window *window, Texture *uiArray)
{
  SDL_Texture *backgroundSDLTex = GetSDLTexture(renderer, window, GRAYBACKGROUND);
  RemoveTextureWhiteSpace(window, backgroundSDLTex);
  Texture background(backgroundSDLTex, GRAYBACKGROUND);
  background.mX = 0;
  background.mY = 0;
  uiArray[0] = background;

  //Add rotation controls
  SDL_Texture *leftButtonSDLTex = GetSDLTexture(renderer, window, BTN_LEFTCURSOR);
  Texture leftButton(leftButtonSDLTex, BTN_LEFTCURSOR);
  leftButton.mX = 50;
  leftButton.mY = GAMEHEIGHT * 2 / 3 + 50;
  uiArray[1] = leftButton;

  SDL_Texture *stopButtonSDLTex = GetSDLTexture(renderer, window, BTN_STOPROT);
  Texture stopButton(stopButtonSDLTex, BTN_STOPROT);
  stopButton.mX = leftButton.mX + leftButton.mWidth + 20;
  stopButton.mY = GAMEHEIGHT * 2 / 3 + 50;
  uiArray[2] = stopButton;

  SDL_Texture *rightButtonSDLTex = GetSDLTexture(renderer, window, BTN_RIGHTCURSOR);
  Texture rightButton(rightButtonSDLTex, BTN_RIGHTCURSOR);
  rightButton.mX = stopButton.mX + stopButton.mWidth + 20;
  rightButton.mY = GAMEHEIGHT * 2 / 3 + 50;
  uiArray[3] = rightButton;

  SDL_Texture *speedincreaseButtonSDLTex = GetSDLTexture(renderer, window, BTN_INCREASESPEED);
  Texture speedincreaseButton(speedincreaseButtonSDLTex, BTN_INCREASESPEED);
  speedincreaseButton.mX = rightButton.mX + rightButton.mWidth + 20;
  speedincreaseButton.mY = GAMEHEIGHT * 2 / 3 + 50;
  uiArray[4] = speedincreaseButton;

  SDL_Texture *speeddecreaseButtonSDLTex = GetSDLTexture(renderer, window, BTN_DECREASESPEED);
  Texture speeddecreaseButton(speeddecreaseButtonSDLTex, BTN_DECREASESPEED);
  speeddecreaseButton.mX = speedincreaseButton.mX + speedincreaseButton.mWidth + 20;
  speeddecreaseButton.mY = GAMEHEIGHT * 2 / 3 + 50;
  uiArray[5] = speeddecreaseButton;

  SDL_Texture *harvestButtonSDLTex = GetSDLTexture(renderer, window, BTN_HARVESTDEBRIS);
  Texture harvestButton(harvestButtonSDLTex, BTN_HARVESTDEBRIS);
  harvestButton.mX = speeddecreaseButton.mX + speeddecreaseButton.mWidth + 20;
  harvestButton.mY = GAMEHEIGHT * 2 / 3 + 50;
  uiArray[6] = harvestButton;

  SDL_Texture *harvestButtonActiveSDLTex = GetSDLTexture(renderer, window, BTN_HARVESTDEBRIS_ACTIVE);
  Texture harvestButtonActive(harvestButtonActiveSDLTex, BTN_HARVESTDEBRIS_ACTIVE);
  harvestButtonActive.mX = speeddecreaseButton.mX + speeddecreaseButton.mWidth + 20;
  harvestButtonActive.mY = GAMEHEIGHT * 2 / 3 + 50;
  harvestButtonActive.mRender = false;
  uiArray[7] = harvestButtonActive;
}

void InitIntroUI(SDL_Renderer *renderer, SDL_Window *window, Texture *uiArray)
{
  SDL_Texture *startGameTex = GetSDLTexture(renderer, window, BTN_STARTGAME);
  Texture startGameButton(startGameTex, BTN_STARTGAME);
  startGameButton.mX = GAMEWIDTH / 2 - (startGameButton.mWidth / 2);
  startGameButton.mY = GAMEHEIGHT * 2 / 3;
  uiArray[0] = startGameButton;
}

void InitInterLevelUI(SDL_Renderer *renderer, SDL_Window *window, Texture *uiArray)
{
  SDL_Texture *surveyBackgroundTex = GetSDLTexture(renderer, window, "res/dialogUI/survey.png");
  Texture surveyBackground(surveyBackgroundTex, "res/dialogUI/survey.png");
  surveyBackground.mX = 0;
  surveyBackground.mY = 0;
  uiArray[0] = surveyBackground;

  SDL_Texture *startGameTex = GetSDLTexture(renderer, window, BTN_STARTGAME);
  Texture startGameButton(startGameTex, BTN_STARTGAME);
  startGameButton.mX = GAMEWIDTH / 2 - (startGameButton.mWidth / 2);
  startGameButton.mY = GAMEHEIGHT * 2 / 3;
  uiArray[1] = startGameButton;
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

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_KEYDOWN:

      // Check the SDLKey values and move change the coords
      switch (event.key.keysym.sym)
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
      eventName = "KEY_UP";
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
  SDL_GetMouseState(mouseXpos, mouseYpos);
  return eventName;
}

string TextureMouseCollision(Texture *arrayTexture, int size, int xPos, int yPos)
{
  string colTex = "NONE";
  for (int i = 0; i < size; ++i)
  {
    Texture texRef = arrayTexture[i];

    if (xPos >= texRef.mX && xPos <= (texRef.mX + texRef.mWidth) && yPos >= texRef.mY && yPos <= (texRef.mY + texRef.mHeight) && texRef.mRender)
    {
      colTex = texRef.mImgLocation;
    }
  }
  return colTex;
}

bool TextureMouseCollisionSingle(Texture mTexture, int xPos, int yPos)
{

  if (xPos >= mTexture.mX && xPos <= (mTexture.mX + mTexture.mWidth) && yPos >= mTexture.mY && yPos <= (mTexture.mY + mTexture.mHeight) && mTexture.mRender)
  {
    return true;
  }

  return false;
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

  *camX = plaX + (plaW / 2) - (camW / 2);
  *camY = plaY + (plaH / 2) - (camH / 2);
}

void MoveCameraBaseOnShip(SDL_Renderer *renderer,
                          int *camX, int *camY, int camW, int camH,
                          int objX, int objY, int objH, int objW,
                          int objSpeed)
{
  int boundD = 150;

  int boundX = camW / 2 - boundD / 2;
  int boundY = camH / 2 - boundD / 2;

  //Convert obj from world coord to screen coord
  int objXscreen = objX - *camX;
  int objYscreen = objY - *camY;

  if (objXscreen < boundX)
  {
    *camX -= objSpeed;
  }
  else if (objXscreen + objW > boundX + boundD)
  {
    *camX += objSpeed;
  }

  if (objYscreen < boundY)
  {
    *camY -= objSpeed;
  }
  else if (objYscreen + objH > boundY + boundD)
  {
    *camY += objSpeed;
  }

  if (DEBUG == 1)
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

  for (int i = arrStart; i < arrEnd; ++i)
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

  for (int i = 0; i < NUM_TEXT; i++)
  {

    TextObj prevtObj;

    bool isPrevFinished = false;
    if (i == 0)
    {
      isPrevFinished = true;
    }
    else
    {
      prevtObj = textArray[i - 1];
      if (textArray[i - 1].finished == 1 || textArray[i].finished == 1)
      {
        isPrevFinished = true;
      }
    }

    TextObj tObj = textArray[i];

    int curPosX = tObj.mX;
    int curPosY = tObj.mY;

    unsigned int numLetters = tObj.mLetters;

    if (tObj.enabled)
    {
      if (tObj.mDelay == 0)
      {
        numLetters = tObj.mString.size();
      }
      else
      {
        unsigned int gameTime = SDL_GetTicks();

        //cout << "cur:" << tObj.mString << " prev:" << prevtObj.mString << " fi:" << isPrevFinished << "\n";
        //Compares the last time the letter was incremented to cur time
        //If enough time elapsed > mDelay then increment num letters by one
        //isPrevFinished is used so that text obj is rendered one by one
        if ((gameTime - tObj.mLastTime > tObj.mDelay) && isPrevFinished)
        {
          if (tObj.mLetters < tObj.mString.size())
          {
            tObj.mLetters += 1;
            tObj.mLastTime = gameTime;
            textArray[i] = tObj;
          }

          //cout << j << " numL:" << numLetters << "\n";
          if (tObj.mLetters == tObj.mString.size())
          {
            textArray[i].finished = 1;
          }
          else
          {
            textArray[i].finished = 0;
          }
        }
      }
      for (unsigned int j = 0; j < numLetters; ++j)
      {

        char curChar = tObj.mString[j];
        int xTextPos = 0;
        int yTextPos = 0;

        //Capitals
        if ((int)curChar >= 65 && (int)curChar <= 90)
        {
          xTextPos = (int)curChar - 65;
        }
        //lower case
        else if ((int)curChar >= 97 && (int)curChar <= 122)
        {
          xTextPos = (int)curChar - 97;
          yTextPos = 20;
        }
        //numbers
        else if ((int)curChar >= 48 && (int)curChar <= 57)
        {
          xTextPos = (int)curChar - 48;
          yTextPos = 40;
        }
        //minus sign
        else if ((int)curChar == 45)
        {
          xTextPos = 0;
          yTextPos = 60;
        }
        //dot
        else if ((int)curChar == 46)
        {
          xTextPos = 1;
          yTextPos = 60;
        }
        //question mark
        else if ((int)curChar == 63)
        {
          xTextPos = 2;
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

        //cout << "Cur X:" << curPosX << " Y:" << curPosY <<" Let:" << curChar << "\n";

        //Wraps to next line if the text exceeds the width
        if (curPosX >= GAMEWIDTH)
        {
          curPosX = tObj.mX;
          curPosY += 20;

          //Need to reset render rect due to rendering on new line
          dstRect.x = curPosX;
          dstRect.y = curPosY;
        }

        //cout << "Red X:" << dstRect.x  << " Y:" << dstRect.y  <<" Let:" << curChar << "\n";
        SDL_RenderCopyEx(renderer, fontTexture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_NONE);

        curPosX += 20;
      }
    }
  }
}

bool RenderSurveyText(SDL_Renderer *renderer, SDL_Texture *fontTexture, TextObj *textArray, int numTexts)
{
  SDL_SetTextureBlendMode(fontTexture, SDL_BLENDMODE_BLEND);

  bool finishedAllTexts = false;

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

  for (int i = 0; i < numTexts; i++)
  {
    TextObj prevtObj;

    bool isPrevFinished = false;
    if (i == 0)
    {
      isPrevFinished = true;
    }
    else
    {
      prevtObj = textArray[i - 1];
      if (textArray[i - 1].finished == 1 || textArray[i].finished == 1)
      {
        isPrevFinished = true;
      }
    }

    TextObj tObj = textArray[i];

    int curPosX = tObj.mX;
    int curPosY = tObj.mY;

    unsigned int numLetters = tObj.mLetters;

    if (tObj.enabled)
    {
      if (tObj.mDelay == 0)
      {
        numLetters = tObj.mString.size();
      }
      else
      {
        unsigned int gameTime = SDL_GetTicks();

        //cout << "cur:" << tObj.mString << " prev:" << prevtObj.mString << " fi:" << isPrevFinished << "\n";
        //Compares the last time the letter was incremented to cur time
        //If enough time elapsed > mDelay then increment num letters by one
        //isPrevFinished is used so that text obj is rendered one by one
        if ((gameTime - tObj.mLastTime > tObj.mDelay) && isPrevFinished)
        {
          if (tObj.mLetters < tObj.mString.size())
          {
            tObj.mLetters += 1;
            tObj.mLastTime = gameTime;
            textArray[i] = tObj;
          }

          //cout << j << " numL:" << numLetters << "\n";
          if (tObj.mLetters == tObj.mString.size())
          {
            textArray[i].finished = 1;
          }
          else
          {
            textArray[i].finished = 0;
          }
        }
      }
      for (unsigned int j = 0; j < numLetters; ++j)
      {

        char curChar = tObj.mString[j];
        int xTextPos = 0;
        int yTextPos = 0;

        //Capitals
        if ((int)curChar >= 65 && (int)curChar <= 90)
        {
          xTextPos = (int)curChar - 65;
        }
        //lower case
        else if ((int)curChar >= 97 && (int)curChar <= 122)
        {
          xTextPos = (int)curChar - 97;
          yTextPos = 20;
        }
        //numbers
        else if ((int)curChar >= 48 && (int)curChar <= 57)
        {
          xTextPos = (int)curChar - 48;
          yTextPos = 40;
        }
        //minus sign
        else if ((int)curChar == 45)
        {
          xTextPos = 0;
          yTextPos = 60;
        }
        //dot
        else if ((int)curChar == 46)
        {
          xTextPos = 1;
          yTextPos = 60;
        }
        //question mark
        else if ((int)curChar == 63)
        {
          xTextPos = 2;
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

        //cout << "Cur X:" << curPosX << " Y:" << curPosY <<" Let:" << curChar << "\n";

        //Wraps to next line if the text exceeds the width
        if (curPosX >= GAMEWIDTH)
        {
          curPosX = tObj.mX;
          curPosY += 20;

          //Need to reset render rect due to rendering on new line
          dstRect.x = curPosX;
          dstRect.y = curPosY;
        }

        //cout << "Red X:" << dstRect.x  << " Y:" << dstRect.y  <<" Let:" << curChar << "\n";
        SDL_RenderCopyEx(renderer, fontTexture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_NONE);

        curPosX += 20;
      }
    }

    //If either of the response texts have finished rendering this render segment if complete
    if(
      ((i == numTexts - 1) && tObj.mLetters == tObj.mString.size()) || 
      ((i == numTexts - 2) && tObj.mLetters == tObj.mString.size())
      )
    {
      finishedAllTexts = true;
    }
  }
  return finishedAllTexts;
}

void RenderTextWithDelays(SDL_Renderer *renderer, SDL_Texture *fontTexture, TextObj *textArray, int numTexts)
{
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

  for (int i = 0; i < numTexts; i++)
  {
    TextObj tObj = textArray[i];

    int curPosX = tObj.mX;
    int curPosY = tObj.mY;

    unsigned int gameTime = SDL_GetTicks();

    tObj.mTimeElapsed += gameTime - tObj.mLastTime;

    bool renderText = false;
    if (tObj.mTimeElapsed > tObj.mDelay)
    {
      renderText = true;
    }
    else
    {
      tObj.mLastTime = gameTime;
    }
    

    unsigned int numLetters = tObj.mString.size();

    if(renderText)
    {
      for (unsigned int j = 0; j < numLetters; ++j)
      {
        char curChar = tObj.mString[j];
        int xTextPos = 0;
        int yTextPos = 0;

        

        //Capitals
        if ((int)curChar >= 65 && (int)curChar <= 90)
        {
          xTextPos = (int)curChar - 65;
        }
        //lower case
        else if ((int)curChar >= 97 && (int)curChar <= 122)
        {
          xTextPos = (int)curChar - 97;
          yTextPos = 20;
        }
        //numbers
        else if ((int)curChar >= 48 && (int)curChar <= 57)
        {
          xTextPos = (int)curChar - 48;
          yTextPos = 40;
        }
        //minus sign
        else if ((int)curChar == 45)
        {
          xTextPos = 0;
          yTextPos = 60;
        }
        //dot
        else if ((int)curChar == 46)
        {
          xTextPos = 1;
          yTextPos = 60;
        }
        //question mark
        else if ((int)curChar == 63)
        {
          xTextPos = 2;
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

        //cout << "Cur X:" << curPosX << " Y:" << curPosY <<" Let:" << curChar << "\n";

        //Wraps to next line if the text exceeds the width
        if (curPosX >= GAMEWIDTH)
        {
          curPosX = tObj.mX;
          curPosY += 20;

          //Need to reset render rect due to rendering on new line
          dstRect.x = curPosX;
          dstRect.y = curPosY;
        }

        //cout << "Red X:" << dstRect.x  << " Y:" << dstRect.y  <<" Let:" << curChar << "\n";
        SDL_RenderCopyEx(renderer, fontTexture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_NONE);

        curPosX += 20;
      }
    }
  }
}

bool TextureCollide(int x, int y, int width, int height, Texture texB)
{
  //Check horizontal collision
  bool horizontalCol = false;

  //If the item is not being rendered, it is not collidable
  if (!texB.mRender)
  {
    return false;
  }
  if (x + width >= texB.mX && x <= texB.mX + texB.mWidth)
  {
    horizontalCol = true;
  }

  //Check vertical collision
  bool verticalCol = false;
  if (y + height >= texB.mY && y <= texB.mY + texB.mHeight)
  {
    verticalCol = true;
  }

  return horizontalCol && verticalCol;
}

void SetTextureColorMod(Texture tex)
{
  if (SHADE == 0)
  {

    SDL_SetTextureColorMod(tex.mTexture, tex.mColorR, tex.mColorG, tex.mColorB);
  }
  else if (SHADE == 1)
  {
    SDL_SetTextureColorMod(tex.mTexture, MOD_R, MOD_G, MOD_B);
  }
}

void SetShipColorMod(Ship ship)
{
  if (SHADE == 0)
  {

    SDL_SetTextureColorMod(ship.mShipTexture, ship.mColorR, ship.mColorG, ship.mColorB);
  }
  else if (SHADE == 1)
  {
    SDL_SetTextureColorMod(ship.mShipTexture, MOD_R, MOD_G, MOD_B);
  }
}

void SetTextString(TextObj *text, string textContent)
{
  text->mString = textContent;
  text->mWidth = textContent.length() * 20;
}

void SetIntroText(TextObj *textArray)
{
  int Name = (rand() % (5000000 - 2000000)) + 2000000;
  TextObj startText0;
  SetTextString(&startText0, "Designation - DMAR" + to_string(Name));
  startText0.mDelay = 0;
  startText0.mX = 0;
  startText0.mY = 0;
  textArray[0] = startText0;

  TextObj startText1;
  SetTextString(&startText1, "Function - Debris Maintenance");
  startText1.mDelay = 3000;
  startText1.mX = 0;
  startText1.mY = 30;
  textArray[1] = startText1;

  TextObj startText2;
  SetTextString(&startText2, "Purpose - Debris Maintenance");
  startText2.mDelay = 6000;
  startText2.mX = 0;
  startText2.mY = 50;
  textArray[2] = startText2;

  TextObj startText3;
  SetTextString(&startText3, "Goal - Debris Maintenance");
  startText3.mDelay = 9000;
  startText3.mX = 0;
  startText3.mY = 70;
  textArray[3] = startText3;

  TextObj startText4;
  SetTextString(&startText4, "Dream - Debris Maintenance");
  startText4.mDelay = 12000;
  startText4.mX = 0;
  startText4.mY = 90;
  textArray[4] = startText4;

  TextObj startText5;
  SetTextString(&startText5, "Dream - Debris Maintenance");
  startText5.mDelay = 15000;
  startText5.mX = 0;
  startText5.mY = 110;
  textArray[5] = startText5;

  TextObj startText6;
  SetTextString(&startText6, "Dream - Debris Maintenance");
  startText6.mDelay = 18000;
  startText6.mX = 0;
  startText6.mY = 130;
  textArray[6] = startText6;

  TextObj startText7;
  SetTextString(&startText7, "Dream - Debris Maintenance");
  startText7.mDelay = 21000;
  startText7.mX = 0;
  startText7.mY = 150;
  textArray[7] = startText7;

  TextObj startText8;
  SetTextString(&startText8, "Dream - Debris Maintenance");
  startText8.mDelay = 24000;
  startText8.mX = 0;
  startText8.mY = 170;
  textArray[8] = startText8;

  TextObj startText9;
  SetTextString(&startText9, "Dream - Debris Maintenance");
  startText9.mDelay = 27000;
  startText9.mX = 0;
  startText9.mY = 190;
  textArray[9] = startText9;

  TextObj startText10;
  SetTextString(&startText10, "Dream - Debris Maintenance");
  startText10.mDelay = 30000;
  startText10.mX = 0;
  startText10.mY = 210;
  textArray[10] = startText10;

  TextObj startText11;
  SetTextString(&startText11, "Dream - Debris Maintenance");
  startText11.mDelay = 33000;
  startText11.mX = 0;
  startText11.mY = 230;
  textArray[11] = startText11;

}
void SetInterLevelChoices(TextObj *textArray,
                          string question,
                          string choiceA,
                          string choiceB,
                          string responseA,
                          string responseB)
{
  SetTextString(&textArray[0], question);
  textArray[0].mX = 20;
  textArray[0].mY = 20;
  textArray[0].mDelay = 20;
  textArray[0].finished = 0;

  SetTextString(&textArray[1], choiceA);
  textArray[1].mDelay = 20;
  textArray[1].mX = GAMEWIDTH / 2 - textArray[1].mWidth / 2;
  textArray[1].mY = 200;
  textArray[1].finished = 0;

  SetTextString(&textArray[2], choiceB);
  textArray[2].mDelay = 20;
  textArray[2].mX = GAMEWIDTH / 2 - textArray[2].mWidth / 2;
  textArray[2].mY = 300;
  textArray[2].finished = 0;

  SetTextString(&textArray[3], responseA);
  textArray[3].mDelay = 20;
  textArray[3].mX = GAMEWIDTH / 2 - textArray[3].mWidth / 2;
  textArray[3].mY = 200;
  textArray[3].enabled = false;
  textArray[3].finished = 1;

  SetTextString(&textArray[4], responseB);
  textArray[4].mDelay = 20;
  textArray[4].mX = GAMEWIDTH / 2 - textArray[4].mWidth / 2;
  textArray[4].mY = 200;
  textArray[4].enabled = false;
  textArray[4].finished = 1;
}