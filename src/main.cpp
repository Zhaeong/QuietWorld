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

  //Create text texture

  SDL_Texture *mainFontText = GetFontText(renderer, "res/text/mainText.png");

  //Create ship char
  Ship mainShip;

  Texture shipTex(renderer, "res/ship/ship.png");

  mainShip.vShipTextures.push_back(shipTex);

  CenterCamOnPlayer(&camX, &camY, camW, camH,
                                    mainShip.mPosition.x,
                                    mainShip.mPosition.y,
                                    mainShip.mWidth,
                                    mainShip.mHeight);
  

  //Create textObj to handle text display
  Text textDisplay(renderer, mainFontText);

  textDisplay.addTextObj("heyeyeye", 0, 0, 0);

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

      textDisplay.mTextArray[0].textString = "x is: " + to_string(mainShip.mPosition.x);
      
      //Update game state
      mainShip.updateBasedOnState();

      //Render to screen
      
      RenderUI(vGameUI);
      mainShip.renderShip(camX, camY);

      SDL_SetRenderDrawColor(renderer, 100, 255, 255, SDL_ALPHA_OPAQUE);
      
      if(DEBUG == 1)
        {
          SDL_RenderDrawLine(renderer,
                             mainShip.mPosition.x,
                             mainShip.mPosition.y,
                             mainShip.mPosition.x + mainShip.mDirection.x * 10,
                             mainShip.mPosition.y + mainShip.mDirection.y * 10 );

          DrawBoundingBox(renderer,
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
      


      //Render text
      textDisplay.drawText();
      
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
