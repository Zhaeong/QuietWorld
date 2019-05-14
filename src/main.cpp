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
  StartSDL(&window, &renderer);

  //Initiate Camera coords
  int camX = 0;
  int camY = 0;
  int camW = GAMEWIDTH;
  int camH = GAMEHEIGHT * 2 / 3;

  
  //Create UI arrays
  Texture uiSpaceArray[NUM_SPACE_UI];
  InitSpaceUI(renderer, uiSpaceArray);

  Texture uiIntroArray[NUM_INTRO_UI];
  InitIntroUI(renderer, uiIntroArray);

  //Create ship char
  SDL_Texture *shipTex = GetSDLTexture(renderer,  "res/ship/ship.png");
  Ship mainShip(shipTex);


  CenterCamOnPlayer(&camX, &camY, camW, camH,
                    mainShip.mPosition.x,
                    mainShip.mPosition.y,
                    mainShip.mWidth,
                    mainShip.mHeight);
  
  //Create texture handling

  SDL_Texture *fontTex = GetSDLTexture(renderer, "res/text/mainText.png");
  TextObj textArray[NUM_TEXT];

  TextObj debugText;
  debugText.mString = "heyyy guys";
  debugText.mDelay = 200;

  textArray[0] = debugText;  

  //Create debris Array
  Texture debrisArray[NUM_DEBRIS];

  SDL_Texture *debrisTex = GetSDLTexture(renderer, DEBRIS_IMG);

  //Initialize random seed as game width
  srand(3234);

  //Generate debris in 3x3 shape, so when player moves to one of the
  //bounds it'll regen that part appearing to be infinite

  //Mid
  GenerateDebris(debrisTex, debrisArray, 0, 10, camX, camY);
  
  //Top Left
  GenerateDebris(debrisTex, debrisArray, 10, 20, camX - DEBRISWIDTH, camY - DEBRISHEIGHT);
  //Top
  GenerateDebris(debrisTex, debrisArray, 20, 30, camX, camY - DEBRISHEIGHT);
  //Top Right
  GenerateDebris(debrisTex, debrisArray, 30, 40, camX + DEBRISWIDTH, camY - DEBRISHEIGHT);  
  
  //Left
  GenerateDebris(debrisTex, debrisArray, 40, 50, camX - DEBRISWIDTH, camY);
  //Right
  GenerateDebris(debrisTex, debrisArray, 50, 60, camX + DEBRISWIDTH, camY);

  //Bottom Left
  GenerateDebris(debrisTex, debrisArray, 60, 70, camX - DEBRISWIDTH, camY + DEBRISHEIGHT);
  //Bottom
  GenerateDebris(debrisTex, debrisArray, 70, 80, camX, camY + DEBRISHEIGHT);
  //Bottom Right
  GenerateDebris(debrisTex, debrisArray, 80, 90, camX + DEBRISWIDTH, camY + DEBRISHEIGHT);  

  //Keep track of cur bound so that when player leaves regen and update coord
  int curBoundX = camX;
  int curBoundY = camY;

  //RemoveDebris(&vDebris, camX, camY);
  bool runGame = true;

  int debrisIndex = -1;

  unsigned int gameTime = 0;

  unsigned int numDebris = 0;

  string gameState = STATE_PAUSE;

  while (runGame)
  {    
    frameStart = SDL_GetTicks();

    //The color at which the screen will be if alpha = 0 on all textures
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    SDL_RenderClear(renderer);    

    ////////////////////////////////////////////////////////////////////////
    //Main Game Code
    ////////////////////////////////////////////////////////////////////////    

    //Update GameTime
    gameTime = SDL_GetTicks();
    
    //Get input
    int xMouse = 0;
    int yMouse = 0;
      
    string eventType = GetAction(&xMouse, &yMouse);

    if(eventType == "EXIT")
    {
      runGame = false;
    }

    if(gameState == STATE_GAME)
    {

      if(eventType != "NONE")
      {
        cout << eventType << "\n";
        if(eventType == "MOUSE_DOWN")
        {
          string texCol = TextureMouseCollision(uiSpaceArray, NUM_SPACE_UI, xMouse, yMouse);

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
          else if(texCol == BTN_HARVESTDEBRIS_ACTIVE)
          {
            if(debrisIndex != -1)
            {
              debrisArray[debrisIndex].mRender = false;
              numDebris += 1;

              if(numDebris == 10)
              {
                gameState = STATE_PAUSE;             
              }
            }
          }         

          cout << texCol << "\n";
        }
      }

    
      int worldMouseX = camX + xMouse;
      int worldMouseY = camY + yMouse;

      bool debrisCol = false;

      for (int i = 0; i < NUM_DEBRIS; ++i)
      {
        Texture dObj = debrisArray[i];
  
        if(TextureCollide(mainShip.mPosition.x, mainShip.mPosition.y, mainShip.mWidth, mainShip.mHeight, dObj))
        {
          debrisIndex = i;
          debrisCol = true;
        }
      
      }
      if(debrisCol)
      {
        uiSpaceArray[6].mRender = false;
        uiSpaceArray[7].mRender = true;
      }
      else
      {
        uiSpaceArray[6].mRender = true;
        uiSpaceArray[7].mRender = false;
        debrisIndex = -1;
      }    
    
      //Update game state
      mainShip.updateBasedOnState();

      //Update camera position
      MoveCameraBaseOnShip(renderer, &camX, &camY, camW, camH,
                           mainShip.mPosition.x, mainShip.mPosition.y, mainShip.mWidth, mainShip.mHeight,
                           mainShip.mSpeed);

    

      //Check if player has breached the debris bounds    
      CheckDebrisField(debrisTex,
                       debrisArray,
                       &curBoundX, &curBoundY,
                       mainShip.mPosition.x, mainShip.mPosition.y);

      //Set num of debris gathered
      textArray[0].mString = to_string(numDebris);

      ////////////////////
      //Render to screen//
      ////////////////////
    
      //Render Debris
      RenderDebris(renderer, debrisArray, camX, camY);
    
      //Render Ship
      RenderShip(renderer, camX, camY, mainShip);
    
      //Render UI
      RenderUI(renderer, uiSpaceArray, NUM_SPACE_UI);
      
      //Render DEBUG items if turned on
      if(DEBUG == 1)
      {       
        
        textArray[1].mY = 30;
        textArray[1].mString = "x:" + to_string(xMouse) + " y:" + to_string(yMouse);

        textArray[2].mY = 60;
        textArray[2].mString = "x:" + to_string(worldMouseX) + " y:" + to_string(worldMouseY);

        textArray[3].mY = 90;
        textArray[3].mString = "GameTime:" + to_string(gameTime);
      
        SDL_SetRenderDrawColor(renderer, 100, 255, 255, SDL_ALPHA_OPAQUE);
      
        SDL_RenderDrawLine(renderer,
                           mainShip.mPosition.x,
                           mainShip.mPosition.y,
                           mainShip.mPosition.x + mainShip.mDirection.x * 10,
                           mainShip.mPosition.y + mainShip.mDirection.y * 10 );
       
          
          
      }

    }
    else if(gameState == STATE_PAUSE)
    {

      if(numDebris == 0)
      {
        textArray[0].mString = "Designation DMAR114123, Function Debris Maintenance and Retrieval";
        textArray[0].mDelay = 200;
      }
      else if(numDebris == 10)
      {
        textArray[0].mString = "You are doing a good job";
        textArray[0].mDelay = 200;
      }

      if(eventType == "MOUSE_DOWN")
      {
        string texCol = TextureMouseCollision(uiIntroArray, NUM_INTRO_UI, xMouse, yMouse);

        if(texCol == BTN_STARTGAME)
        {
          textArray[0].mString = "";
          textArray[0].mLetters = 0;
          gameState = STATE_GAME;           
        }        
      }    

      //Render UI
      RenderUI(renderer, uiIntroArray, NUM_INTRO_UI);
      
    }
   


    //Render text    
    RenderText(renderer, fontTex, textArray);
      
    //Swap buffers to present backbuffer to screen
    SDL_RenderPresent(renderer);

    ////////////////////////////////////////////////////////////////////////
    //End of main game code
    ////////////////////////////////////////////////////////////////////////

    frameTime = SDL_GetTicks() - frameStart;
    
    if(frameDelay > frameTime)
    {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  return 0;
}
