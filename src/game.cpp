#include "game.h"

int StartGame(SDL_Window **window, SDL_Renderer **renderer)
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

string TextureCollision(vector<Texture> vTexture, int xPos, int yPos)
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
