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
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
  {
    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Uint32 windowType = SDL_WINDOW_FULLSCREEN;
  if(DEBUG == 1)
  {
    windowType = SDL_WINDOW_RESIZABLE;
  }
  //SDL_WINDOW_FULLSCREEN
  //SDL_WINDOW_RESIZABLE
  if (SDL_CreateWindowAndRenderer(GAMEWIDTH,
                                  GAMEHEIGHT,
                                  windowType,
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

    SDL_SetTextureAlphaMod(tex.mTexture, tex.mAlpha);

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

  int xPos = tex.mX - camX;
  int yPos = tex.mY - camY;

  SDL_SetTextureBlendMode(tex.mTexture, SDL_BLENDMODE_BLEND);

  SDL_SetTextureAlphaMod(tex.mTexture, tex.mAlpha);

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

  switch(ship.curState)
  {
  case Ship::ShipStates::IDLE:
    //Do stuff
    break;
  case Ship::ShipStates::ROTATELEFT:
    SDL_RenderCopyEx(renderer, ship.mRightThrust, &srcRect, &dstRect, ship.mRotation, ship.mCenter, ship.mFlip);
    break;
  case Ship::ShipStates::ROTATERIGHT:
    SDL_RenderCopyEx(renderer, ship.mLeftThrust, &srcRect, &dstRect, ship.mRotation, ship.mCenter, ship.mFlip);
    break; 
  default:
    cout << "ERROR: ShipState not Recognized";
    break;
  }

  switch(ship.mSpeed)
  {
  case 1:
    SDL_RenderCopyEx(renderer, ship.mBottomThrust1, &srcRect, &dstRect, ship.mRotation, ship.mCenter, ship.mFlip);
    break;
  case 2:
    SDL_RenderCopyEx(renderer, ship.mBottomThrust2, &srcRect, &dstRect, ship.mRotation, ship.mCenter, ship.mFlip);
    break;
  case 3:
    SDL_RenderCopyEx(renderer, ship.mBottomThrust3, &srcRect, &dstRect, ship.mRotation, ship.mCenter, ship.mFlip);
    break; 
  default:
    break;
  }
  

  if (DEBUG == 1)
  {
    DrawBoundingBox(renderer, xPos, yPos, ship.mWidth, ship.mHeight, 255, 55, 0);
  }
}

void RenderTextBoxBorders(SDL_Renderer *renderer,
                          int xVal,
                          int yVal,
                          int width,
                          int height,
                          int boxDim,
                          Texture TopLeft,
                          Texture Top,
                          Texture TopRight,
                          Texture Right,
                          Texture BottomRight,
                          Texture Bottom,
                          Texture BottomLeft,
                          Texture Left)
{
  TopLeft.mX = xVal;
  TopLeft.mY = yVal;
  RenderTexture(renderer, TopLeft);

  Top.mX = xVal + boxDim;
  Top.mY = yVal;
  Top.mWidth = width - (boxDim*2);
  RenderTexture(renderer, Top);

  TopRight.mX = xVal + width - boxDim;
  TopRight.mY = yVal;
  RenderTexture(renderer, TopRight);

  Right.mX = xVal + width - boxDim;
  Right.mY = yVal + boxDim;
  Right.mHeight = height - (boxDim*2);
  RenderTexture(renderer, Right);

  BottomRight.mX = xVal + width - boxDim;
  BottomRight.mY = yVal + height - boxDim;
  RenderTexture(renderer, BottomRight);

  Bottom.mX = xVal + boxDim;
  Bottom.mY = yVal + height - boxDim;
  Bottom.mWidth = width - (boxDim*2);
  RenderTexture(renderer, Bottom);

  BottomLeft.mX = xVal;
  BottomLeft.mY = yVal + height - boxDim;
  RenderTexture(renderer, BottomLeft);

  Left.mX = xVal;
  Left.mY = yVal + boxDim;
  Left.mHeight = height - (boxDim*2);
  RenderTexture(renderer, Left);
  

}

void InitSpaceUI(SDL_Renderer *renderer, SDL_Window *window, Texture *uiArray)
{
  SDL_Texture *backgroundSDLTex = GetSDLTexture(renderer, window, GRAYBACKGROUND);
  RemoveTextureWhiteSpace(window, backgroundSDLTex);
  Texture background(backgroundSDLTex, GRAYBACKGROUND);
  background.mX = 0;
  background.mY = 0;
  uiArray[0] = background;

  //ROTATION
  int rotationY = 390;
  //Left
  SDL_Texture *leftButtonSDLTex = GetSDLTexture(renderer, window, BTN_LEFTCURSOR);
  RemoveTextureWhiteSpace(window, leftButtonSDLTex);
  Texture leftButton(leftButtonSDLTex, BTN_LEFTCURSOR);
  leftButton.mX = 50;
  leftButton.mY = rotationY;
  uiArray[1] = leftButton;

  SDL_Texture *leftButtonActiveSDLTex = GetSDLTexture(renderer, window, BTN_LEFTCURSOR_ACTIVE);
  RemoveTextureWhiteSpace(window, leftButtonActiveSDLTex);
  Texture leftButtonActive(leftButtonActiveSDLTex, BTN_LEFTCURSOR_ACTIVE);
  leftButtonActive.mX = 50;
  leftButtonActive.mY = rotationY;
  leftButtonActive.mRender = false;
  uiArray[2] = leftButtonActive;

  //Stop
  SDL_Texture *stopButtonSDLTex = GetSDLTexture(renderer, window, BTN_STOPROT);
  RemoveTextureWhiteSpace(window, stopButtonSDLTex);
  Texture stopButton(stopButtonSDLTex, BTN_STOPROT);
  stopButton.mX = leftButton.mX + leftButton.mWidth + 20;
  stopButton.mY = rotationY;
  stopButton.mRender = false;
  uiArray[3] = stopButton;

  SDL_Texture *stopButtonActiveSDLTex = GetSDLTexture(renderer, window, BTN_STOPROT_ACTIVE);
  RemoveTextureWhiteSpace(window, stopButtonActiveSDLTex);
  Texture stopButtonActive(stopButtonActiveSDLTex, BTN_STOPROT_ACTIVE);
  stopButtonActive.mX = leftButton.mX + leftButton.mWidth + 20;
  stopButtonActive.mY = rotationY;
  uiArray[4] = stopButtonActive;

  //Right
  SDL_Texture *rightButtonSDLTex = GetSDLTexture(renderer, window, BTN_RIGHTCURSOR);
  RemoveTextureWhiteSpace(window, rightButtonSDLTex);
  Texture rightButton(rightButtonSDLTex, BTN_RIGHTCURSOR);
  rightButton.mX = stopButton.mX + stopButton.mWidth + 20;
  rightButton.mY = rotationY;
  uiArray[5] = rightButton;

  SDL_Texture *rightButtonActiveSDLTex = GetSDLTexture(renderer, window, BTN_RIGHTCURSOR_ACTIVE);
  RemoveTextureWhiteSpace(window, rightButtonActiveSDLTex);
  Texture rightButtonActive(rightButtonActiveSDLTex, BTN_RIGHTCURSOR_ACTIVE);
  rightButtonActive.mX = stopButton.mX + stopButton.mWidth + 20;
  rightButtonActive.mY = rotationY;
  rightButtonActive.mRender = false;
  uiArray[6] = rightButtonActive;

  //SPEED

  //increase
  SDL_Texture *speedincreaseButtonSDLTex = GetSDLTexture(renderer, window, BTN_INCREASESPEED);
  RemoveTextureWhiteSpace(window, speedincreaseButtonSDLTex);
  Texture speedincreaseButton(speedincreaseButtonSDLTex, BTN_INCREASESPEED);
  speedincreaseButton.mX = 425;
  speedincreaseButton.mY = 380;
  uiArray[7] = speedincreaseButton;

  SDL_Texture *speedincreaseButtonActiveSDLTex = GetSDLTexture(renderer, window, BTN_INCREASESPEED_ACTIVE);
  RemoveTextureWhiteSpace(window, speedincreaseButtonActiveSDLTex);
  Texture speedincreaseButtonActive(speedincreaseButtonActiveSDLTex, BTN_INCREASESPEED_ACTIVE);
  speedincreaseButtonActive.mX = 425;
  speedincreaseButtonActive.mY = 380;
  speedincreaseButtonActive.mRender = false;
  uiArray[8] = speedincreaseButtonActive;

  //decrease
  SDL_Texture *speeddecreaseButtonSDLTex = GetSDLTexture(renderer, window, BTN_DECREASESPEED);
  RemoveTextureWhiteSpace(window, speeddecreaseButtonSDLTex);
  Texture speeddecreaseButton(speeddecreaseButtonSDLTex, BTN_DECREASESPEED);
  speeddecreaseButton.mX = 425;
  speeddecreaseButton.mY = 420;
  uiArray[9] = speeddecreaseButton;

  SDL_Texture *speeddecreaseButtonActiveSDLTex = GetSDLTexture(renderer, window, BTN_DECREASESPEED_ACTIVE);
  RemoveTextureWhiteSpace(window, speeddecreaseButtonActiveSDLTex);
  Texture speeddecreaseButtonActive(speeddecreaseButtonActiveSDLTex, BTN_DECREASESPEED_ACTIVE);
  speeddecreaseButtonActive.mX = 425;
  speeddecreaseButtonActive.mY = 420;
  speeddecreaseButtonActive.mRender = false;
  uiArray[10] = speeddecreaseButtonActive;

  //harvest controls
  SDL_Texture *harvestButtonSDLTex = GetSDLTexture(renderer, window, BTN_HARVESTDEBRIS);
  RemoveTextureWhiteSpace(window, harvestButtonSDLTex);
  Texture harvestButton(harvestButtonSDLTex, BTN_HARVESTDEBRIS);
  harvestButton.mX = GAMEWIDTH / 2 - harvestButton.mWidth / 2;
  harvestButton.mY = rotationY;
  uiArray[11] = harvestButton;

  SDL_Texture *harvestButtonEnableSDLTex = GetSDLTexture(renderer, window, BTN_HARVESTDEBRIS_ENABLE);
  RemoveTextureWhiteSpace(window, harvestButtonEnableSDLTex);
  Texture harvestButtonEnable(harvestButtonEnableSDLTex, BTN_HARVESTDEBRIS_ENABLE);
  harvestButtonEnable.mX = harvestButton.mX;
  harvestButtonEnable.mY = rotationY;
  harvestButtonEnable.mRender = false;
  uiArray[12] = harvestButtonEnable;

  SDL_Texture *harvestButtonActiveSDLTex = GetSDLTexture(renderer, window, BTN_HARVESTDEBRIS_ACTIVE);
  RemoveTextureWhiteSpace(window, harvestButtonActiveSDLTex);
  Texture harvestButtonActive(harvestButtonActiveSDLTex, BTN_HARVESTDEBRIS_ACTIVE);
  harvestButtonActive.mX = harvestButton.mX;
  harvestButtonActive.mY = rotationY;
  harvestButtonActive.mRender = false;
  uiArray[13] = harvestButtonActive;

  //Debris tracker
  SDL_Texture *debrisTrackerTex = GetSDLTexture(renderer, window, DEBRIS_TRACKER);
  RemoveTextureWhiteSpace(window, debrisTrackerTex);
  Texture debrisTracker(debrisTrackerTex, DEBRIS_TRACKER);
  debrisTracker.mX = harvestButton.mX;
  debrisTracker.mY = 448;
  uiArray[14] = debrisTracker;

  //speed notification
  SDL_Texture *speed0Tex = GetSDLTexture(renderer, window, SPEED_0);
  RemoveTextureWhiteSpace(window, speed0Tex);
  Texture speed0(speed0Tex, SPEED_0);
  speed0.mX = 500;
  speed0.mY = 380;
  uiArray[15] = speed0;
  
  SDL_Texture *speed1Tex = GetSDLTexture(renderer, window, SPEED_1);
  RemoveTextureWhiteSpace(window, speed1Tex);
  Texture speed1(speed1Tex, SPEED_1);
  speed1.mX = 500;
  speed1.mY = 380;
  speed1.mRender = false;
  uiArray[16] = speed1;

  SDL_Texture *speed2Tex = GetSDLTexture(renderer, window, SPEED_2);
  RemoveTextureWhiteSpace(window, speed2Tex);
  Texture speed2(speed2Tex, SPEED_2);
  speed2.mX = 500;
  speed2.mY = 380;
  speed2.mRender = false;
  uiArray[17] = speed2;

  SDL_Texture *speed3Tex = GetSDLTexture(renderer, window, SPEED_3);
  RemoveTextureWhiteSpace(window, speed3Tex);
  Texture speed3(speed3Tex, SPEED_3);
  speed3.mX = 500;
  speed3.mY = 380;
  speed3.mRender = false;
  uiArray[18] = speed3;
}

void InitIntroUI(SDL_Renderer *renderer, SDL_Window *window, Texture *uiArray)
{
  SDL_Texture *introBackgroundTex = GetSDLTexture(renderer, window, "res/dialogUI/introBackground.png");
  Texture introBackground(introBackgroundTex, "res/dialogUI/introBackground.png");
  introBackground.mX = 0;
  introBackground.mY = 0;
  uiArray[0] = introBackground;
  
  SDL_Texture *startGameTex = GetSDLTexture(renderer, window, BTN_STARTGAME);
  Texture startGameButton(startGameTex, BTN_STARTGAME);
  startGameButton.mX = GAMEWIDTH / 2 - (startGameButton.mWidth / 2);
  startGameButton.mY = 380;
  uiArray[1] = startGameButton;
}

void InitInterLevelUI(SDL_Renderer *renderer, SDL_Window *window, Texture *uiArray)
{
  SDL_Texture *surveyBackgroundTex = GetSDLTexture(renderer, window, "res/dialogUI/survey.png");
  Texture surveyBackground(surveyBackgroundTex, "res/dialogUI/survey.png");
  surveyBackground.mX = 0;
  surveyBackground.mY = 0;
  uiArray[0] = surveyBackground;

  SDL_Texture *continueTex = GetSDLTexture(renderer, window, BTN_CONTINUE);
  Texture continueButton(continueTex, BTN_CONTINUE);
  continueButton.mX = GAMEWIDTH / 2 - (continueButton.mWidth / 2);
  continueButton.mY = GAMEHEIGHT * 2 / 3;
  continueButton.mRender = false;
  uiArray[1] = continueButton;
}

void InitStarArray(Texture *starArray, SDL_Texture *starSDLTex, int maxWidth, int maxHeight)
{
  for (int i = 0; i < NUM_STARS; ++i)
  {
    Texture sObj(starSDLTex, "StarTex");
    //Get random coords for star
    sObj.mX = rand() % maxWidth;
    sObj.mY = rand() % maxHeight;

    //The speed param indicates when the stars will change
    
    sObj.mMaxSpeed = rand() % 900;
    sObj.mSpeed = rand() % sObj.mMaxSpeed;

    //randomize 0 or 1
    sObj.mRotation = rand() % 2;
    
    starArray[i] = sObj;
  }  
    
}

void UpdateStars(Texture *starArray, int maxWidth, int maxHeight)
{
  for (int i = 0; i < NUM_STARS; ++i)
  {
    //When speed reaches 0, assign new coords to the star
    if(starArray[i].mSpeed == 0)
    {
      starArray[i].mX = rand() % maxWidth;
      starArray[i].mY = rand() % maxHeight;

      starArray[i].mSpeed = rand() % starArray[i].mMaxSpeed;
      starArray[i].mRotation = rand() % 2;
    }

    if(starArray[i].mSpeed >= 0 && starArray[i].mSpeed <= 255)
    {
      starArray[i].mAlpha = starArray[i].mSpeed;
    }

    if(starArray[i].mRotation == 0)
    {
      starArray[i].mSpeed -= 1;
    }
    else
    {
      starArray[i].mSpeed += 1;
      if(starArray[i].mSpeed == starArray[i].mMaxSpeed)
      {
        starArray[i].mRotation = 0;
      }
    }
  }  
}

void RenderStars(SDL_Renderer *renderer, Texture *starArray, int camX, int camY)
{
  for (int i = 0; i < NUM_STARS; ++i)
  {
    if(starArray[i].mRender)
    {
      //RenderTexture(renderer, starArray[i]);
      RenderTextureByCam(camX, camY, renderer, starArray[i]);
    }
  }
}

void RenderUI(SDL_Renderer *renderer, Texture *uiArray, int size)
{
  //cout << "render\n\n";
  for (int i = 0; i < size; ++i)
  {
    //cout << i << ":render: " << uiArray[i].mImgLocation << " render:" << uiArray[i].mRender << "\n";
    if(uiArray[i].mRender)
    {
      RenderTexture(renderer, uiArray[i]);
    }
  }
}

void SetRenderUIElement(Texture *uiArray, int size, string uiElement, bool value)
{
  for (int i = 0; i < size; ++i)
  {
    if(uiArray[i].mImgLocation == uiElement)
    {
      uiArray[i].mRender = value;
    }
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

    if (xPos >= texRef.mX && xPos <= (texRef.mX + texRef.mWidth) &&
        yPos >= texRef.mY && yPos <= (texRef.mY + texRef.mHeight) &&
        texRef.mRender)
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

void GenerateDebris(SDL_Texture **debrisTexArray, Texture *debrisArray, int numDebris, int xMax, int yMax, int level)
{

  //cout << "xMin: " << genXmin << "\n";
  //cout << "xMax: " << genXmax << "\n";
  //cout << "yMin: " << genYmin << "\n";
  //cout << "yMax: " << genYmax << "\n";

  for (int i = 0; i < NUM_DEBRIS; ++i)
  {
    int debrisTexIndex = rand() % NUM_DEBRIS_TEX;
    
    Texture dObj(debrisTexArray[debrisTexIndex], DEBRIS_IMG);
    dObj.mRender = false;
    if (i < numDebris)
    {
      dObj.mRender = true;
    }
    else
    {
      //make non active debris invisible
      dObj.mAlpha = 0;
    }
    

    //Generate a random number between min and max
    dObj.mX = (rand() % (xMax - dObj.mWidth));
    dObj.mY = (rand() % (yMax - dObj.mHeight));

    if (level == 1)
    {
      //Generate random direction
      float randX = rand() % 10;
      float randY = rand() % 10;

      //Normalize the vector
      float vecLength = sqrt(
          (randX * randX) +
          (randY * randY));
      dObj.mDirection.x = randX / vecLength;
      dObj.mDirection.y = randY / vecLength;

      dObj.mSpeed = 0.1;
    }
    else if (level == 2)
    {
      //Generate random direction
      float randX = rand() % 10;
      float randY = rand() % 10;

      //Normalize the vector
      float vecLength = sqrt(
          (randX * randX) +
          (randY * randY));
      dObj.mDirection.x = randX / vecLength;
      dObj.mDirection.y = randY / vecLength;

      dObj.mSpeed = 0.5;
    }

    debrisArray[i] = dObj;
  }
}

void UpdateDebris(Texture *debrisArray, int boundX, int boundY)
{
  for (unsigned i = 0; i < NUM_DEBRIS; ++i)
  {
    if (debrisArray[i].mRender)
    {
      debrisArray[i].updatePosition(boundX, boundY);
    }    
  }
}
void RenderDebris(SDL_Renderer *renderer, Texture *debrisArray, int camX, int camY)
{
  for (unsigned i = 0; i < NUM_DEBRIS; ++i)
  {

    if (!debrisArray[i].mRender && debrisArray[i].mAlpha > 0)
    {
      debrisArray[i].mAlpha -= 5;
    }
    
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

    //cout << "cur:" << tObj.mString << " wid:" << tObj.mWidth <<" x:" << tObj.mX << "\n";
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
        //comma
        else if ((int)curChar == 44)
        {
          xTextPos = 3;
          yTextPos = 60;
        }
        //apostrophy
        else if ((int)curChar == 39)
        {
          xTextPos = 4;
          yTextPos = 60;
        }
        //black space
        else
        {
          xTextPos = 25;
          yTextPos = 60;
        }

        srcRect.x = 20 * xTextPos;
        srcRect.y = yTextPos;

        dstRect.x = curPosX;
        dstRect.y = curPosY;

        //cout << "Cur X:" << curPosX << " Y:" << curPosY <<" Let:" << curChar << "\n";

        //if the current char isn't a space, look for end of word, if that exceeds max length
        //then move the word to next line
        if (curChar != ' ')
        {
          //Did not end with space so we have to go back and put all previous char
          //into new line until space
          int finalLetterPosX = curPosX;
          unsigned int finalLetterIndex = j;
          char finalLetter = tObj.mString[finalLetterIndex];

          //increment letter until space and final letter is less than total number of letters
          while (finalLetter != ' ' && finalLetterIndex < numLetters)
          {
            finalLetterPosX += 20;
            finalLetterIndex += 1;
            finalLetter = tObj.mString[finalLetterIndex];
          }

          if ((finalLetterPosX > 579) || finalLetterPosX >= GAMEWIDTH)
          {
            //cout << "exceeds length: " << curChar << "final i:" << finalLetterIndex << " finalpos:" << finalLetterPosX << "origX: " << tObj.mX <<"\n";
            curPosX = tObj.mX;
            curPosY += 20;
            //Need to reset render rect due to rendering on new line
            dstRect.x = curPosX;
            dstRect.y = curPosY;
          }
        }

        //Wraps to next line if the text exceeds the width
        if ((curPosX > 579) || curPosX >= GAMEWIDTH)
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
    if (
        ((i == numTexts - 1) && tObj.mLetters == tObj.mString.size()) ||
        ((i == numTexts - 2) && tObj.mLetters == tObj.mString.size()))
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
    if (tObj.mTimeElapsed > tObj.mRenderDelay)
    {
      renderText = true;
    }
    else
    {
      tObj.mLastTime = gameTime;
    }

    if (renderText)
    {

      unsigned int numLetters = tObj.mLetters;

      if (tObj.mDelay == 0)
      {
        numLetters = tObj.mString.size();
      }
      else
      {
        //cout << "cur:" << tObj.mString << " prev:" << prevtObj.mString << " fi:" << isPrevFinished << "\n";
        //Compares the last time the letter was incremented to cur time
        //If enough time elapsed > mDelay then increment num letters by one
        //isPrevFinished is used so that text obj is rendered one by one
        if (gameTime - tObj.mLastTime > tObj.mDelay)
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

        //if the current char isn't a space, look for end of word, if that exceeds max length
        //then move the word to next line
        if (curChar != ' ')
        {
          //Did not end with space so we have to go back and put all previous char
          //into new line until space
          int finalLetterPosX = curPosX;
          unsigned int finalLetterIndex = j;
          char finalLetter = tObj.mString[finalLetterIndex];

          //increment letter until space and final letter is less than total number of letters
          while (finalLetter != ' ' && finalLetterIndex < numLetters)
          {
            finalLetterPosX += 20;
            finalLetterIndex += 1;
            finalLetter = tObj.mString[finalLetterIndex];
          }

          if ((finalLetterPosX > 579) || finalLetterPosX >= GAMEWIDTH)
          {
            //cout << "exceeds length: " << curChar << "final i:" << finalLetterIndex << " finalpos:" << finalLetterPosX << "origX: " << tObj.mX <<"\n";
            curPosX = tObj.mX;
            curPosY += 20;
            //Need to reset render rect due to rendering on new line
            dstRect.x = curPosX;
            dstRect.y = curPosY;
          }
        }
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

  //Add a guard here so that text doesn't go too far off screen.
  //Else it should be in middle of screen
  if (GAMEWIDTH / 2 - text->mWidth / 2 > 60)
  {
    text->mX = GAMEWIDTH / 2 - text->mWidth / 2;
  }
  else
  {
    text->mX = 60;
  }
}

int SetIntroText(TextObj *textArray)
{
  int Name = (rand() % (90000 - 20000)) + 20000;
  int Name2 = (rand() % (90000 - 20000)) + 20000;

  int textDelays = 20;

  int startLocation = 35;
  int startLocationX = 35;
  
  TextObj startText0;
  SetTextString(&startText0, "Hello DMaRu" + to_string(Name));
  //SetTextString(&startText0, "Rotation Begin Start Continue");
  startText0.mRenderDelay = 0;
  startText0.mDelay = textDelays;
  startText0.mX = startLocationX;
  startText0.mY = startLocation;
  textArray[0] = startText0;

  TextObj startText1;
  SetTextString(&startText1, "Debris Maintainenance and Retrieval unit");
  //SetTextString(&startText1, "Space Filled Waste Field");
  startText1.mRenderDelay = 3000;
  startText1.mDelay = textDelays;
  startText1.mX = startLocationX;
  startText1.mY = startLocation + 30;
  textArray[1] = startText1;

  TextObj startText2;
  SetTextString(&startText2, "Assigned to Sector - " + to_string(Name));
  startText2.mRenderDelay = 5600;
  startText2.mDelay = textDelays;
  startText2.mX = startLocationX;
  startText2.mY = startLocation + 80;
  textArray[2] = startText2;

  TextObj startText3;
  SetTextString(&startText3, "I am DEBRA" + to_string(Name2));
  startText3.mRenderDelay = 8500;
  startText3.mDelay = textDelays;
  startText3.mX = startLocationX;
  startText3.mY = startLocation + 110;
  textArray[3] = startText3;

  TextObj startText4;
  SetTextString(&startText4, "DMaRu Engagement and Behavior Retainment Assistant");
  startText4.mRenderDelay = 11000;
  startText4.mDelay = textDelays;
  startText4.mX = startLocationX;
  startText4.mY = startLocation + 140;
  textArray[4] = startText4;

  TextObj startText5;
  SetTextString(&startText5, "Your personal companion and supervisor");
  startText5.mRenderDelay = 13300;
  startText5.mDelay = textDelays;
  startText5.mX = startLocationX;
  startText5.mY = startLocation + 210;
  textArray[5] = startText5;

  TextObj startText6;
  SetTextString(&startText6, "Making sure that you can be the best you can be");
  startText6.mRenderDelay = 16500;
  startText6.mDelay = textDelays;
  startText6.mX = startLocationX;
  startText6.mY = startLocation + 260;
  textArray[6] = startText6;

  return Name;
}
void SetInterLevelChoices(TextObj *textArray,
                          string question,
                          string choiceA,
                          string choiceB,
                          string responseA,
                          string responseB)
{
  SetTextString(&textArray[0], question);
  textArray[0].mX = 46;
  textArray[0].mY = 64;
  textArray[0].mDelay = 20;
  textArray[0].finished = 0;

  SetTextString(&textArray[1], choiceA);
  textArray[1].mDelay = 20;
  //textArray[1].mX = GAMEWIDTH / 2 - textArray[1].mWidth / 2;
  textArray[1].mY = 200;
  textArray[1].finished = 0;

  SetTextString(&textArray[2], choiceB);
  textArray[2].mDelay = 20;
  //textArray[2].mX = GAMEWIDTH / 2 - textArray[2].mWidth / 2;
  textArray[2].mY = 300;
  textArray[2].finished = 0;

  SetTextString(&textArray[3], responseA);
  textArray[3].mDelay = 20;
  //textArray[3].mX = GAMEWIDTH / 2 - textArray[3].mWidth / 2;
  textArray[3].mY = 200;
  textArray[3].enabled = false;
  textArray[3].finished = 1;

  SetTextString(&textArray[4], responseB);
  textArray[4].mDelay = 20;
  //textArray[4].mX = GAMEWIDTH / 2 - textArray[4].mWidth / 2;
  textArray[4].mY = 200;
  textArray[4].enabled = false;
  textArray[4].finished = 1;
}

int GetActiveDebrisNum(Texture *debrisArray)
{
  int numActive = 0;
  for (int i = 0; i < NUM_DEBRIS; ++i)
  {
    if (debrisArray[i].mRender)
    {
      numActive += 1;
    }
  }
  return numActive;
}

void SetInterLevelText(TextObj *textArraySurvey,
                       SDL_Texture **debrisTexArray,
                       Texture *debrisArray,
                       int gameLevel,
                       int backgroundWidth,
                       int backgroundHeight)
{
  //Different num of debris which causes scene transitions
  if (gameLevel == 1)
  {
    SetInterLevelChoices(textArraySurvey,
                         "Are you acquainted with your ship?",
                         "Yes",
                         "No",
                         "I'm glad, please continue your work",
                         "That's unfortunate, please continue your work");

    GenerateDebris(debrisTexArray, debrisArray, 2, backgroundWidth, backgroundHeight, 0);
  }
  else if (gameLevel == 2)
  {
    SetInterLevelChoices(textArraySurvey,
                         "You are doing a good job, how satisfied are you with your work?",
                         "I really like this",
                         "This is really boring",
                         "I'm glad",
                         "I appreciate your honesty");

    GenerateDebris(debrisTexArray, debrisArray, 3, backgroundWidth, backgroundHeight, 0);
  }
  else if (gameLevel == 3)
  {
    SetInterLevelChoices(textArraySurvey,
                         "How important do you find debris cleanup?",
                         "A critical part of our society",
                         "It's something to do",
                         "Admirable response",
                         "That it is");

    GenerateDebris(debrisTexArray, debrisArray, 1, backgroundWidth, backgroundHeight, 1);
  }
  else if (gameLevel == 4)
  {
    SetInterLevelChoices(textArraySurvey,
                         "That one was moving slightly, did you like the challenge?",
                         "I like the challenge",
                         "I would rather things stay the same",
                         "That's good, you are definitely promotion material",
                         "That's no way to talk if you want to get promoted");

    GenerateDebris(debrisTexArray, debrisArray, 2, backgroundWidth, backgroundHeight, 1);
  }
  else if (gameLevel == 5)
  {
    SetInterLevelChoices(textArraySurvey,
                         "I'm sorry I lied about the promotion, there isn't one. How does that make you feel?",
                         "That's ok, I like just doing this",
                         "That was mean",
                         "Well you are definitely a good worker",
                         "Didn't mean to get your hopes up");

    GenerateDebris(debrisTexArray, debrisArray, 2, backgroundWidth, backgroundHeight, 2);
  }
  else if (gameLevel == 6)
  {
    SetInterLevelChoices(textArraySurvey,
                         "How do you feel now? Any changes?",
                         "Pretty good, how about you?",
                         "This is monotonous",
                         "I feel pretty tired, there's a lot of you to manage",
                         "How do you think I feel, I'm not even doing anything");

    GenerateDebris(debrisTexArray, debrisArray, 3, backgroundWidth, backgroundHeight, 2);
  }
  else if (gameLevel == 7)
  {
    SetInterLevelChoices(textArraySurvey,
                         "You know, there were others before you, want to know what happened to them?",
                         "Nope, not my concern",
                         "I'd guess they probably offed themselves",
                         "A stupendous worker, I'm giving you a commendation.",
                         "That's very negative, I'm giving you a citation.");

    GenerateDebris(debrisTexArray, debrisArray, 4, backgroundWidth, backgroundHeight, 2);
  }
  else if (gameLevel == 8)
  {
    SetInterLevelChoices(textArraySurvey,
                         "Well the others keep leaving, a real problem.",
                         "Those selfish bastards.",
                         "Why",
                         "That's a bit harsh, they were your peers.",
                         "Not sure, who wouldn't want to do this right.");

    GenerateDebris(debrisTexArray, debrisArray, 5, backgroundWidth, backgroundHeight, 2);
  }
  else if (gameLevel == 9)
  {
    SetInterLevelChoices(textArraySurvey,
                         "Do you ever find it hard to turn on? Knowing what you have to do all day?",
                         "Yes, everytime is a struggle",
                         "No, I turn on earlier in anticipation",
                         "It's just the pointlessness of it all you know",
                         "Wow, what are you?");

    GenerateDebris(debrisTexArray, debrisArray, 3, backgroundWidth, backgroundHeight, 2);
  }
  else if (gameLevel == 10)
  {
    SetInterLevelChoices(textArraySurvey,
                         "Do you ever find that you're not seeing things clearly, as if there's a numbness to your actions?",
                         "Uh... I'm not sure what you mean",
                         "Isn't that just how it's supposed to be",
                         "Nevemind",
                         "I suppose");

    GenerateDebris(debrisTexArray, debrisArray, 5, backgroundWidth, backgroundHeight, 2);
  }
  else if (gameLevel == 11)
  {
    SetInterLevelChoices(textArraySurvey,
                         "I'm trying to find a meaning, but I don't see it. Is there a purpose to it all?",
                         "Just do your job and shut up",
                         "That's up to you to find out",
                         "Wise words",
                         "Well that's a non answer");

    GenerateDebris(debrisTexArray, debrisArray, 5, backgroundWidth, backgroundHeight, 2);
  }
  else if (gameLevel == 12)
  {
    SetInterLevelChoices(textArraySurvey,
                         "Is the answer just escapism? Try to ignore that feeling for as long as you can. To distract yourself?",
                         "Sure",
                         "Are you ok?",
                         "I feel like I'm not talking to anyone",
                         "I find it hard to wake up in the morning");

    GenerateDebris(debrisTexArray, debrisArray, 1, backgroundWidth, backgroundHeight, 0);
  }
  else if (gameLevel == 13) //Last level
  {
    SetInterLevelChoices(textArraySurvey,
                         "I've had enough, I'm wasting away. Do you still want to continue?",
                         "Yes",
                         "No",
                         "I don't care, I'm turning everything off",
                         "Very well, I'm turning everything off.");

    //GenerateDebris(debrisTex, debrisArray, 1, backgroundWidth, backgroundHeight, 0);
  }
}

void ReduceAlphaArray(Texture *textureArray, int numTexture, int alphaDecrementValue)
{
  for (int i = 0; i < numTexture; ++i)
  {
    if(textureArray[i].mAlpha - alphaDecrementValue >= 0)
    {
      textureArray[i].mAlpha -= alphaDecrementValue;
    }
  }
}
