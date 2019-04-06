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

void InitSpaceUI(SDL_Renderer *renderer, vector<Texture> &vGameUI)
{
  vGameUI.clear();

  //Add background
  Texture uiBackground(renderer, GRAYBACKGROUND);
  uiBackground.mX = 0;
  uiBackground.mY = GAMEHEIGHT * 2/3;

  uiBackground.mWidth = GAMEWIDTH;
  uiBackground.mHeight = GAMEHEIGHT / 3;
  vGameUI.push_back(uiBackground);

  //Add rotation controls  
  Texture leftButton(renderer, BTN_LEFTCURSOR);
  leftButton.mX = 50;
  leftButton.mY = GAMEHEIGHT * 2/3 + 50;
  vGameUI.push_back(leftButton);
  
  Texture stopButton(renderer, BTN_STOPROT);
  stopButton.mX = leftButton.mX + leftButton.mWidth + 20;
  stopButton.mY = GAMEHEIGHT * 2/3 + 50;
  vGameUI.push_back(stopButton);

  Texture rightButton(renderer, BTN_RIGHTCURSOR);
  rightButton.mX = stopButton.mX + stopButton.mWidth + 20;
  rightButton.mY = GAMEHEIGHT * 2/3 + 50;
  vGameUI.push_back(rightButton);

  Texture speedincreaseButton(renderer, BTN_INCREASESPEED);
  speedincreaseButton.mX = rightButton.mX + rightButton.mWidth + 20;
  speedincreaseButton.mY = GAMEHEIGHT * 2/3 + 50;
  vGameUI.push_back(speedincreaseButton);

  Texture speeddecreaseButton(renderer, BTN_DECREASESPEED);
  speeddecreaseButton.mX = speedincreaseButton.mX + speedincreaseButton.mWidth + 20;
  speeddecreaseButton.mY = GAMEHEIGHT * 2/3 + 50;
  vGameUI.push_back(speeddecreaseButton);

  
}

void RenderUI(vector<Texture> vGameUI)
{
  for (unsigned i = 0; i < vGameUI.size(); ++i)
  {
    vGameUI.at(i).renderTexture();
  }
}

string GetAction(int *mouseXpos, int *mouseYpos)
{
  string eventName = "NONE";
  /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
  /* more events on the event queue, our while loop will exit when */
  /* that occurs.                
   */

  SDL_Event event;
  
  while( SDL_PollEvent( &event ) )
  {
    switch( event.type )
    {
    case SDL_KEYDOWN:

      /* Check the SDLKey values and move change the coords */
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

string TextureMouseCollision(vector<Texture> vTexture, int xPos, int yPos)
{
  string colTex = "NONE";
  for (unsigned i = 0; i < vTexture.size(); ++i)
  {
    Texture texRef = vTexture.at(i);
      
    vTexture.at(i).renderTexture();
    if(xPos >= texRef.mX
       && xPos <= (texRef.mX + texRef.mWidth)
       && yPos >= texRef.mY
       && yPos <= (texRef.mY + texRef.mHeight))
    {
      colTex = texRef.mImgLocation;
    }
  }
  return colTex;
}

SDL_Texture* GetFontText(SDL_Renderer *SRen, string textLocation)
{
  cout << "Loading: " << textLocation << "\n";
  
  //Make sure to initialize texture to null or else SDL_DestroyTexture will crash program
  SDL_Texture *mTexture = NULL;

  void* mPixels = 0;
  int mPitch = 0;

  SDL_Surface* loadedSurface = IMG_Load( textLocation.c_str() );

  if( loadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL_image Error: %s\n",
            textLocation.c_str(),
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
      mTexture = SDL_CreateTexture( SRen,
                                    SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    formattedSurface->w,
                                    formattedSurface->h );
      if( mTexture == NULL )
      {
        printf( "Unable to create blank texture! SDL Error: %s\n",
                SDL_GetError() );
      }
      else
      {

        //Lock texture for manipulation
        SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch );

        //Copy loaded/formatted surface pixels
        memcpy( mPixels, formattedSurface->pixels,
                formattedSurface->pitch * formattedSurface->h );

        //Unlock texture to update
        SDL_UnlockTexture( mTexture );
        mPixels = NULL;

              
      }

      //Get rid of old formatted surface
      SDL_FreeSurface( formattedSurface );
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );

    cout << "Finish Loading: " << textLocation << "\n";

      
  }
  
  return mTexture;
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


void RenderDebris(vector<Texture> vDebris, int camX, int camY)
{
  for (unsigned i = 0; i < vDebris.size(); ++i)
  {
    Texture dObj = vDebris.at(i);
    dObj.renderTextureByCam(camX, camY);      
  }
}


bool TextureCollide(int x, int y, int width, int height , Texture texB)
{
  //Check horizontal collision
  bool horizontalCol = false;
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

void GenerateDebris(SDL_Renderer *renderer,  vector<Texture> *vDebris, int xCord, int yCord)
{
  int genXmin = xCord;
  int genXmax = xCord + GAMEWIDTH;

  int genYmin = yCord;
  int genYmax = yCord + GAMEHEIGHT;  

  /*
  cout << "xMin: " << genXmin << "\n";
  cout << "xMax: " << genXmax << "\n";
  cout << "yMin: " << genYmin << "\n";
  cout << "yMax: " << genYmax << "\n";
  */
  
  for(unsigned i = 0; i < 20; ++i)
  {
    Texture dObj(renderer, "res/debris/debris1.png");

    //Generate a random number between min and max
    dObj.mX = (rand() % (genXmax - genXmin)) + genXmin;
    dObj.mY = (rand() % (genYmax - genYmin)) + genYmin;

    vDebris->push_back(dObj);

    //cout << "x: " << dObj.mX << " y: " << dObj.mY << "\n";
  }
}

void RemoveDebris(vector<Texture> *vDebris, int xCord, int yCord)
{
  int xMin = xCord;
  int xMax = xCord + GAMEWIDTH;

  int yMin = yCord;
  int yMax = yCord + GAMEHEIGHT;

  //A weird quirk of STL where erase() invalidates the iterator since it'll reduce the vector size,
  //so we need to make it = erase since it'll return a valid iterator
  for(vector<Texture>::iterator it = vDebris->begin(); it != vDebris->end();)
  {
    int debrisX = it->mX;
    int debrisY = it->mY;

    if(debrisX >= xMin &&
       debrisX <= xMax &&
       debrisY >= yMin &&
       debrisY <= yMax)
    {
      it = vDebris->erase(it);      
    }
    else
    {
      ++it;
    }
  }

}

void CheckDebrisField(SDL_Renderer *renderer,
                      vector<Texture> *vDebris,
                      int *originX,int *originY,
                      int plaX, int plaY,
                      int plaW, int plaH)
{

  //If player moves past left bound, delete debris on right, top, bottom
  //and regen top, bottom, left
  if(plaX < *originX)
  {
    //Top
    RemoveDebris(vDebris, *originX, *originY - GAMEHEIGHT);
    //Bottom
    RemoveDebris(vDebris, *originX, *originY + GAMEHEIGHT);
    //Right    
    RemoveDebris(vDebris, *originX + GAMEWIDTH, *originY);

    //Set new originX
    *originX = *originX - GAMEWIDTH;
    
    //Top
    GenerateDebris(renderer, vDebris, *originX, *originY - GAMEHEIGHT);
    //Bottom
    GenerateDebris(renderer, vDebris, *originX, *originY + GAMEHEIGHT);
    //Left
    GenerateDebris(renderer, vDebris, *originX - GAMEWIDTH, *originY);
    
  }
}
