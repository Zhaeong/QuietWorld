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

  //Create text texture

  SDL_Texture *mainFontText = getFontText(renderer, "res/text/mainText.png");

  //Create ship char
  Ship mainShip;

  Texture shipTex(renderer, "res/ship/ship.png");

  mainShip.vShipTextures.push_back(shipTex);

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

      Text *newText = new Text(renderer, mainFontText, "ShipX " + to_string(mainShip.mPosition.x), 100, 10);
      Text *newText2 = new Text(renderer, mainFontText, "ShipY " + to_string(mainShip.mPosition.y), 100, 30);

      
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


      //delete any objects created
      delete newText;
      delete newText2;

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
