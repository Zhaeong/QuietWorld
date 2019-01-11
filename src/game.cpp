#include "game.h"

int StartGame(SDL_Window **window, SDL_Renderer **renderer)
{
    int screenWidth = 640;
    int screenHeight = 480;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't initialize SDL: %s",
                     SDL_GetError());
    }


    if (SDL_CreateWindowAndRenderer(screenWidth,
                                    screenHeight,
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
  Texture gameText(renderer, "res/png/leftCursor.png");
  vGameUI.push_back(gameText);
}

void RenderUI(vector<Texture>  vGameUI)
{
  for (unsigned i = 0; i < vGameUI.size(); ++i)
    {
      vGameUI.at(i).renderTexture();
    }
}

