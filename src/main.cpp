#include "main.h"

int main(int argc, char* argv[])
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  const int FPS = 60;

  //How many miliseconds per frame
  const int frameDelay = 1000/FPS;

  Uint32 frameStart;
  float frameTime;

  cout << "Starting Quietworld";

  //Initiate SDL
  StartSDL(&window, &renderer);

  //Initiate Camera coords
  int camX = 0;
  int camY = 0;
  int camW = GAMEWIDTH;
  int camH = GAMEHEIGHT * 2 / 3;

  //Create UI vector 
  vector<Texture> vGameUI;
  InitSpaceUI(renderer, vGameUI);

  //Create ship char
  Ship mainShip;

  Texture shipTex(renderer, "res/ship/ship.png");

  mainShip.vShipTextures.push_back(shipTex);

  CenterCamOnPlayer(&camX, &camY, camW, camH,
                    mainShip.mPosition.x,
                    mainShip.mPosition.y,
                    mainShip.mWidth,
                    mainShip.mHeight);
  
  //Create texture handling
  SDL_Texture *mainFontText = GetFontText(renderer, "res/text/mainText.png");
  Text textDisplay(renderer, mainFontText);

  textDisplay.addTextObj("You are Valued", 0 ,GAMEHEIGHT - 50, 0);
  textDisplay.addTextObj("", 0, 0, 0);
  textDisplay.addTextObj("", 0, 30, 0);
  textDisplay.addTextObj("", 0, 60, 0);

  //Create debris Array
  vector<Texture> vDebris;

  //Initialize random seed as game width
  srand(3234);

  GenerateDebris(renderer, &vDebris, camX, camY);
  

  bool runGame = true;

  int debrisIndex = -1;
  bool debrisUI = false;

  while (runGame)
  {
    frameStart = SDL_GetTicks();

    //The color at which the screen will be if alpha = 0 on all textures
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    SDL_RenderClear(renderer);

    frameTime = SDL_GetTicks() - frameStart;

    ////////////////////////////////////////////////////////////////////////
    //Main Game Code
    ////////////////////////////////////////////////////////////////////////

      
    //Get input
    int xMouse = 0;
    int yMouse = 0;
      
    string eventType = GetAction(&xMouse, &yMouse);

    if(eventType == "EXIT")
    {
      runGame = false;
    }

    if(eventType != "NONE")
    {
      cout << eventType << "\n";
      if(eventType == "MOUSE_DOWN")
      {
        string texCol = TextureMouseCollision(vGameUI, xMouse, yMouse);

        if(texCol == BTN_LEFTCURSOR)
        {
          mainShip.curState = Ship::ShipStates::ROTATELEFT;
                  
        }
        else if(texCol == BTN_RIGHTCURSOR)
        {
          mainShip.curState = Ship::ShipStates::ROTATERIGHT;
        }
        else if(texCol == BTN_STOPROT)
        {
          mainShip.curState = Ship::ShipStates::IDLE;
        }
        else if(texCol == BTN_INCREASESPEED)
        {
          mainShip.changeSpeed(1);
        }
        else if(texCol == BTN_DECREASESPEED)
        {
          mainShip.changeSpeed(-1);
        }
        else if(texCol == BTN_HARVESTDEBRIS)
        {
          if(debrisIndex != -1)
          {
            vDebris.erase(vDebris.begin() + debrisIndex);
          }
        }

        cout << texCol << "\n";
      }
    }

    int worldMouseX = camX + xMouse;
    int worldMouseY = camY + yMouse;
      

    bool debrisCol = false;
    for (unsigned i = 0; i < vDebris.size(); ++i)
    {
      Texture dObj = vDebris.at(i);
      if(TextureCollide(mainShip.mPosition.x, mainShip.mPosition.y, mainShip.mWidth, mainShip.mHeight, dObj))
      {
        debrisCol = true;
        debrisIndex = i;
        if(!debrisUI)
        {
          debrisUI = true;
          Texture harvestDebrisButton(renderer, BTN_HARVESTDEBRIS);
          harvestDebrisButton.mX = GAMEWIDTH - 60;
          harvestDebrisButton.mY = GAMEHEIGHT * 2/3 + 50;
          vGameUI.push_back(harvestDebrisButton);
                  
        }
      }
    }

    //Check if current ship is over debris
    if(!debrisCol)
    {
      //Remove harvest icon from UI
      if(debrisUI)
      {
        vGameUI.pop_back();
        debrisUI = false;
      }
      debrisIndex = -1;
          
    }
      
    //Update game state
    mainShip.updateBasedOnState();

    //Update camera position
    MoveCameraBaseOnShip(renderer, &camX, &camY, camW, camH,
                         mainShip.mPosition.x, mainShip.mPosition.y, mainShip.mWidth, mainShip.mHeight,
                         mainShip.mSpeed);

    ////////////////////
    //Render to screen//
    ////////////////////

    //Render space debris

    RenderDebris(vDebris, camX, camY);
      
    //Render Ship
    mainShip.renderShip(camX, camY);

    //Render UI
    RenderUI(vGameUI);
      
    //Render text
    textDisplay.drawText();


    //Render DEBUG items if turned on
    if(DEBUG == 1)
    {
      textDisplay.mTextArray[1].textString = "x:" + to_string(xMouse) + " y:" + to_string(yMouse);
      textDisplay.mTextArray[2].textString = "x:" + to_string(worldMouseX) + " y:" + to_string(worldMouseY);
      textDisplay.mTextArray[3].textString = "Cam x:" + to_string(camX) + " y:" + to_string(camY);
      
      SDL_SetRenderDrawColor(renderer, 100, 255, 255, SDL_ALPHA_OPAQUE);
      
      SDL_RenderDrawLine(renderer,
                         mainShip.mPosition.x,
                         mainShip.mPosition.y,
                         mainShip.mPosition.x + mainShip.mDirection.x * 10,
                         mainShip.mPosition.y + mainShip.mDirection.y * 10 );

      DrawBoundingBoxCam(renderer,
                         camX,
                         camY,
                         mainShip.mPosition.x,
                         mainShip.mPosition.y,
                         mainShip.mWidth,
                         mainShip.mHeight,
                         255,
                         0,
                         0);
          
          
          
    }
      
    //Swap buffers to present backbuffer to screen
    SDL_RenderPresent(renderer);


    ////////////////////////////////////////////////////////////////////////
    //End of main game code
    ////////////////////////////////////////////////////////////////////////
      
    if(frameDelay > frameTime)
    {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  return 0;
}
