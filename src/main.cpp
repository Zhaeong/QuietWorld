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

              cout << texCol << "\n";
            }
        }

      //Render to screen
      RenderUI(vGameUI);
      mainShip.renderShip();

      
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
