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

    while (1)
    {
        frameStart = SDL_GetTicks();
	
        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}
