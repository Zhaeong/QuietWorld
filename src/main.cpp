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
  StartGame(&window, &renderer);

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
  textDisplay.addTextObj("heyeyeye", 0, 0, 0);

  //Create debris Array
  vector<Texture> vDebris;
  Texture dObj(renderer, "res/debris/debris1.png");
  dObj.mX = 245;
  dObj.mY = 0;
  vDebris.push_back(dObj);

  bool runGame = true;

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
              string texCol = TextureCollision(vGameUI, xMouse, yMouse);

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

              cout << texCol << "\n";
            }
        }

      textDisplay.mTextArray[0].textString = "x:" + to_string(mainShip.mPosition.x) + " y:" + to_string(mainShip.mPosition.y)  ;
      
      //Update game state
      mainShip.updateBasedOnState();

      ////////////////////
      //Render to screen//
      ////////////////////

      //Render space debris

      RenderDebris(vDebris, camX, camY);
      
      //Render UI
      RenderUI(vGameUI);

      //Render Ship
      mainShip.renderShip(camX, camY); 
      
      //Render text
      textDisplay.drawText();


      //Render DEBUG items if turned on
      if(DEBUG == 1)
        {
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
          
          DrawBoundingBox(renderer,
                          camX,
                          camY,
                          GAMEWIDTH/2,
                          GAMEHEIGHT/2,
                          0,
                          255,
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
