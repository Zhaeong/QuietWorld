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

  StartGame(&window, &renderer);

  //Create UI vector 
  vector<Texture> vGameUI;
  InitSpaceUI(renderer, vGameUI);

  

  //Create ship char
  Ship mainShip;

  Texture shipTex(renderer, "res/ship/ship.png");

  mainShip.vShipTextures.push_back(shipTex);

  bool runGame = true;

  cout << "dirX:" << mainShip.mDirection.x << " dirY:" << mainShip.mDirection.y << "\n";

  //mainShip.changeRotation(true);
      
  cout << "dirX:" << mainShip.mDirection.x << " dirY:" << mainShip.mDirection.y << "\n";
  
  while (runGame)
    {
      frameStart = SDL_GetTicks();

      //The color at which the screen will be if alpha = 0 on all textures
      SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);

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

      //Update game state
      mainShip.updateBasedOnState();

      //Render to screen

      
      RenderUI(vGameUI);
      mainShip.renderShip();

      SDL_SetRenderDrawColor(renderer, 100, 255, 255, SDL_ALPHA_OPAQUE);

      SDL_RenderDrawLine(renderer,
                         mainShip.mPosition.x,
                         mainShip.mPosition.y,
                         mainShip.mPosition.x + mainShip.mDirection.x * 10,
                         mainShip.mPosition.y + mainShip.mDirection.y * 10 );

      
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
