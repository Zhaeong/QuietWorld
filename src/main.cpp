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

              if(texCol == LEFTCURSOR)
                {

                  mainShip.curState = Ship::ShipStates::ROTATELEFT;
                  mainShip.changeDirection();
                }
              else if(texCol == RIGHTCURSOR)
                {

                  mainShip.curState = Ship::ShipStates::ROTATERIGHT;
                }

              cout << texCol << "\n";
            }
        }

      //Update game state
      mainShip.updateBasedOnState();

      //Render to screen

      
      
      cout << "x:" << mainShip.mDirection.x << " y:" << mainShip.mDirection.y << "\n";
      
      RenderUI(vGameUI);
      //mainShip.renderShip();

      SDL_SetRenderDrawColor(renderer, 100, 255, 255, SDL_ALPHA_OPAQUE);

      SDL_RenderDrawLine(renderer,
                         mainShip.mPosition.x,
                         mainShip.mPosition.y,
                         mainShip.mDirection.x * 10,
                         mainShip.mDirection.y * 10 );

      
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
