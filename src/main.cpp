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

    StartGame(window, renderer);

    //Texture gameText(renderer, "res/png/leftCursor.png");
    

    while (1)
    {
        frameStart = SDL_GetTicks();


        //The color at which the screen will be if alpha = 0 on all textures
        SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);

        SDL_RenderClear(renderer);

 
        frameTime = SDL_GetTicks() - frameStart;

        //Swap buffers to present backbuffer to screen
        SDL_RenderPresent(renderer);

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}