#include "main.h"

int SHADE;

//Need to use this main signature (int argv, char** args)
//Because SDL alters default program entry point resolving
//SDL calls your entry point expecting a specific signature,
//which is no longer subject to compiler interpretation
int main(int argv, char **args)
{
  //Declarations for sdl window displaying
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  const int FPS = 60;

  SHADE = 1;
  //How many miliseconds per frame
  const int frameDelay = 1000 / FPS;

  Uint32 frameStart;
  float frameTime;

  cout << "Starting Quietworld";

  //Initiate SDL
  StartSDL(&window, &renderer);

  // load WAV files

  //Load music
  Mix_Music *introMus = Mix_LoadMUS("res/wavs/intro.wav");
  if (introMus == NULL)
  {
    printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Music *levelOneMus = Mix_LoadMUS("res/wavs/laurapalmer.wav");
  if (levelOneMus == NULL)
  {
    printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Music *interLevelMus = Mix_LoadMUS("res/wavs/engagementparty.wav");
  if (interLevelMus == NULL)
  {
    printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
  }

  //Play music
  //Second param is number of lopps -1 for infinite
  Mix_PlayMusic( introMus, 1 );

  //Sounds
  Mix_Chunk *gScratch = NULL;

  //Load sound effects
  gScratch = Mix_LoadWAV("res/wavs/fNote.wav");
  if (gScratch == NULL)
  {
    printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
  }

  //Initiate Camera coords
  int camX = 0;
  int camY = 0;
  int camW = GAMEWIDTH;
  int camH = GAMEHEIGHT * 2 / 3;

  //Create UI arrays
  Texture uiSpaceArray[NUM_SPACE_UI];
  InitSpaceUI(renderer, window, uiSpaceArray);

  Texture uiIntroArray[NUM_INTRO_UI];
  InitIntroUI(renderer, window, uiIntroArray);

  Texture uiInterLevelArray[NUM_INTERUI];
  InitInterLevelUI(renderer, window, uiInterLevelArray);

  //Init Dialog textures
  SDL_Texture *tutorialDiag = GetSDLTexture(renderer, window, "res/dialogUI/tutorial.png");
  RemoveTextureWhiteSpace(window, tutorialDiag);
  Texture tutorialDiagTex = Texture(tutorialDiag, "res/dialogUI/tutorial.png");

  SDL_Texture *dialogOK = GetSDLTexture(renderer, window, "res/dialogUI/dialogOK.png");
  Texture dialogOKTex = Texture(dialogOK, "res/dialogUI/dialogOK.png");
  dialogOKTex.mX = GAMEWIDTH / 2 - (dialogOKTex.mWidth / 2);
  dialogOKTex.mY = 300;

  SDL_Texture *choiceBackground = GetSDLTexture(renderer, window, "res/dialogUI/choiceBackground.png");
  Texture choiceBackgroundTexA = Texture(choiceBackground, "res/dialogUI/choiceBackgroundA.png");
  Texture choiceBackgroundTexB = Texture(choiceBackground, "res/dialogUI/choiceBackgroundB.png");

  Texture responseBackgroundTexA = Texture(choiceBackground, "responseBackgroundTexA");
  responseBackgroundTexA.mRender = false;
  Texture responseBackgroundTexB = Texture(choiceBackground, "responseBackgroundTexB");
  responseBackgroundTexB.mRender = false;

  //Create ship char
  SDL_Texture *shipTex = GetSDLTexture(renderer, window, "res/ship/ship.png");
  RemoveTextureWhiteSpace(window, shipTex);
  Ship mainShip(shipTex);

  //Load textures for mining progress
  SDL_Texture *miningBarEmpty = GetSDLTexture(renderer, window, "res/dialogUI/mineBarEmpty.png");
  Texture miningBarEmptyTex = Texture(miningBarEmpty, "res/dialogUI/mineBarEmpty.png");

  SDL_Texture *miningBarColor = GetSDLTexture(renderer, window, "res/dialogUI/mineBarColor.png");
  Texture miningBarColorTex = Texture(miningBarColor, "res/dialogUI/mineBarColor.png");

  //Create bounds for player
  SDL_Texture *backgroundTex = GetSDLTexture(renderer, window, "res/background/level1.png");
  Texture gameBackground = Texture(backgroundTex, "res/background/level1.png");

  int curLevelBoundX = gameBackground.mWidth;
  int curLevelBoundY = gameBackground.mHeight;

  mainShip.mPosition.x = curLevelBoundX / 2;
  mainShip.mPosition.y = curLevelBoundY / 2;

  CenterCamOnPlayer(&camX, &camY, camW, camH,
                    mainShip.mPosition.x,
                    mainShip.mPosition.y,
                    mainShip.mWidth,
                    mainShip.mHeight);

  //Initialize random seed for generating debris and name
  srand (time(NULL));
  
  //Create texture handling
  SDL_Texture *fontTex = GetSDLTexture(renderer, window, "res/text/mainText.png");
  RemoveTextureWhiteSpace(window, fontTex);
  TextObj textArray[NUM_TEXT];

  TextObj textArrayIntro[NUM_TEXT_INTRO];

  SetIntroText(textArrayIntro);

  TextObj choiceA;
  SetTextString(&choiceA, "");
  choiceA.mDelay = 20;
  textArray[0] = choiceA;
  textArray[1] = choiceA;

  TextObj choiceB;
  SetTextString(&choiceB, "");
  choiceB.mDelay = 20;
  textArray[1] = choiceB;

  //Create debris Array
  Texture debrisArray[NUM_DEBRIS];

  SDL_Texture *debrisTex = GetSDLTexture(renderer, window, DEBRIS_IMG);
  RemoveTextureWhiteSpace(window, debrisTex);



  //Generate debris
  GenerateDebris(debrisTex, debrisArray, 0, 10, camX, camY);

  //RemoveDebris(&vDebris, camX, camY);
  bool runGame = true;

  int debrisIndex = -1;

  unsigned int gameTime = 0;

  unsigned int numDebris = 0;

  //Variables
  bool isMining = false;
  unsigned int holdDownTime = 0;

  bool showDialog = true;

  string gameState = STATE_INTRO;

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

    if (eventType == "EXIT")
    {
      runGame = false;
    }

    if (gameState == STATE_GAME)
    {      
      if (eventType != "NONE")
      {
        cout << eventType << "\n";
        if (eventType == "MOUSE_DOWN")
        {
          string texCol = TextureMouseCollision(uiSpaceArray, NUM_SPACE_UI, xMouse, yMouse);

          if (texCol == BTN_LEFTCURSOR)
          {
            mainShip.curState = Ship::ShipStates::ROTATELEFT;
          }
          else if (texCol == BTN_RIGHTCURSOR)
          {
            mainShip.curState = Ship::ShipStates::ROTATERIGHT;
          }
          else if (texCol == BTN_STOPROT)
          {
            mainShip.curState = Ship::ShipStates::IDLE;
          }
          else if (texCol == BTN_INCREASESPEED)
          {
            mainShip.changeSpeed(1);
          }
          else if (texCol == BTN_DECREASESPEED)
          {
            mainShip.changeSpeed(-1);
          }
          else if (texCol == BTN_HARVESTDEBRIS_ACTIVE)
          {
            if (debrisIndex != -1)
            {
              isMining = true;
            }
          }
          cout << texCol << "\n";
        }
        else if (eventType == "MOUSE_UP")
        {
          isMining = false;
          holdDownTime = 0;

          Mix_PlayChannel( -1, gScratch, 0 );

          cout << "going upo";

          if (TextureMouseCollisionSingle(dialogOKTex, xMouse, yMouse))
          {
            if (showDialog)
            {
              showDialog = false;

              SHADE = 0;

              cout << "Shade val:" << SHADE << '\n';
            }
          }
        }
      }

      //Check if mining and increment
      if (isMining)
      {
        holdDownTime += 1;

        //If player held down for entire width of bar, destroy debris
        if (holdDownTime == (unsigned int)miningBarEmptyTex.mWidth)
        {
          debrisArray[debrisIndex].mRender = false;
          numDebris += 1;
        }
      }

      int worldMouseX = camX + xMouse;
      int worldMouseY = camY + yMouse;

      bool debrisCol = false;

      for (int i = 0; i < NUM_DEBRIS; ++i)
      {
        Texture dObj = debrisArray[i];

        if (TextureCollide(mainShip.mPosition.x, mainShip.mPosition.y, mainShip.mWidth, mainShip.mHeight, dObj))
        {
          debrisIndex = i;
          debrisCol = true;
        }
      }
      if (debrisCol)
      {
        uiSpaceArray[6].mRender = false;
        uiSpaceArray[7].mRender = true;
      }
      else
      {
        uiSpaceArray[6].mRender = true;
        uiSpaceArray[7].mRender = false;
        debrisIndex = -1;
        isMining = false;
        holdDownTime = 0;
      }

      //Different num of debris which causes scene transitions
      if (numDebris == 1)
      {
        gameState = STATE_PAUSE;


        Mix_FadeOutMusic(1000);
        //SDL_Delay(1);

        Mix_PlayMusic( interLevelMus, 1 );

        SetInterLevelChoices(textArray,
                             "Are you acquainted with your ship?",
                             "Yes",
                             "No",
                             "That's good",
                             "Too Bad");
      }
      else if (numDebris == 3)
      {
        gameState = STATE_PAUSE;

        SetInterLevelChoices(textArray,
                             "How are you doing so far?",
                             "This is really boring",
                             "I really like this",
                             "I appreciate your honesty",
                             "I'm glad");
      }

      //Update game state
      mainShip.updateBasedOnState(curLevelBoundX, curLevelBoundY);

      //Update camera position
      MoveCameraBaseOnShip(renderer, &camX, &camY, camW, camH,
                           mainShip.mPosition.x, mainShip.mPosition.y, mainShip.mWidth, mainShip.mHeight,
                           mainShip.mSpeed);

      //Set num of debris gathered
      textArray[5].mString = to_string(numDebris);

      ////////////////////
      //Render to screen//
      ////////////////////

      //Render Background
      RenderTextureByCam(camX, camY, renderer, gameBackground);

      //Render Debris
      RenderDebris(renderer, debrisArray, camX, camY);

      //Render Ship
      RenderShip(renderer, camX, camY, mainShip);

      //Render Miningbar
      if (isMining)
      {
        //Convert player world position to screen position
        int playCamX = mainShip.mPosition.x - camX;
        int playCamY = mainShip.mPosition.y - camY;
        miningBarEmptyTex.mX = playCamX;
        miningBarEmptyTex.mY = playCamY + mainShip.mHeight;

        RenderTexture(renderer, miningBarEmptyTex);

        //Now render the progress based on
        miningBarColorTex.mX = playCamX;
        miningBarColorTex.mY = playCamY + mainShip.mHeight;
        miningBarColorTex.mWidth = holdDownTime;

        RenderTexture(renderer, miningBarColorTex);

        //Draws the debis render trail
        int debrisCamX = debrisArray[debrisIndex].mX - camX;
        int debrisCamY = debrisArray[debrisIndex].mY - camY;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

        SDL_RenderDrawLine(renderer,
                           playCamX + mainShip.mWidth / 2,
                           playCamY + mainShip.mHeight / 2,
                           debrisCamX + debrisArray[debrisIndex].mWidth / 2,
                           debrisCamY + debrisArray[debrisIndex].mHeight / 2);
      }

      //Render UI
      RenderUI(renderer, uiSpaceArray, NUM_SPACE_UI);

      //Popup dialogs rendering

      if (showDialog)
      {
        RenderTexture(renderer, tutorialDiagTex);
        RenderTexture(renderer, dialogOKTex);
      }

      //Render DEBUG items if turned on
      if (DEBUG == 1)
      {
        textArray[6].mY = 30;
        textArray[6].mString = "x:" + to_string(xMouse) + " y:" + to_string(yMouse);

        textArray[7].mY = 60;
        textArray[7].mString = "x:" + to_string(worldMouseX) + " y:" + to_string(worldMouseY);

        textArray[8].mY = 90;
        textArray[8].mString = "GameTime:" + to_string(gameTime);

        textArray[9].mY = 120;
        textArray[9].mString = "HoldDown:" + to_string(holdDownTime);

        SDL_SetRenderDrawColor(renderer, 100, 255, 255, SDL_ALPHA_OPAQUE);

        SDL_RenderDrawLine(renderer,
                           mainShip.mPosition.x,
                           mainShip.mPosition.y,
                           mainShip.mPosition.x + mainShip.mDirection.x * 10,
                           mainShip.mPosition.y + mainShip.mDirection.y * 10);
      }
    }
    else if (gameState == STATE_PAUSE) //Interlevel UI
    {
      //Render UI
      RenderUI(renderer, uiInterLevelArray, NUM_INTERUI);

      //Get position of text to render a background for it
      choiceBackgroundTexA.mX = textArray[1].mX;
      choiceBackgroundTexA.mY = textArray[1].mY - 15;
      choiceBackgroundTexA.mWidth = textArray[1].mWidth;

      choiceBackgroundTexB.mX = textArray[2].mX;
      choiceBackgroundTexB.mY = textArray[2].mY - 15;
      choiceBackgroundTexB.mWidth = textArray[2].mWidth;

      responseBackgroundTexA.mX = textArray[3].mX;
      responseBackgroundTexA.mY = textArray[3].mY - 15;
      responseBackgroundTexA.mWidth = textArray[3].mWidth;

      responseBackgroundTexB.mX = textArray[4].mX;
      responseBackgroundTexB.mY = textArray[4].mY - 15;
      responseBackgroundTexB.mWidth = textArray[4].mWidth;

      RenderTexture(renderer, choiceBackgroundTexA);
      RenderTexture(renderer, choiceBackgroundTexB);
      RenderTexture(renderer, responseBackgroundTexA);
      RenderTexture(renderer, responseBackgroundTexB);

      if (eventType == "MOUSE_UP")
      {
        string texCol = TextureMouseCollision(uiInterLevelArray, NUM_INTERUI, xMouse, yMouse);

        if (texCol == BTN_STARTGAME)
        {
          //Reset text
          textArray[0].mString = "";
          textArray[0].mLetters = 0;

          textArray[1].mString = "";
          textArray[1].mLetters = 0;

          textArray[2].mString = "";
          textArray[2].mLetters = 0;

          textArray[3].mString = "";
          textArray[3].mLetters = 0;

          textArray[4].mString = "";
          textArray[4].mLetters = 0;

          //Reset renders
          choiceBackgroundTexA.mRender = true;
          choiceBackgroundTexB.mRender = true;
          responseBackgroundTexA.mRender = false;
          responseBackgroundTexB.mRender = false;

          textArray[1].enabled = true;
          textArray[2].enabled = true;
          textArray[3].enabled = false;

          gameState = STATE_GAME;
          numDebris += 1;
        }

        if (TextureMouseCollisionSingle(choiceBackgroundTexA, xMouse, yMouse))
        {
          cout << "selected A";
          choiceBackgroundTexA.mRender = false;
          choiceBackgroundTexB.mRender = false;

          textArray[1].enabled = false;
          textArray[2].enabled = false;

          responseBackgroundTexA.mRender = true;
          textArray[3].enabled = true;
        }

        if (TextureMouseCollisionSingle(choiceBackgroundTexB, xMouse, yMouse))
        {
          cout << "selected B";
          choiceBackgroundTexA.mRender = false;
          choiceBackgroundTexB.mRender = false;
          textArray[1].enabled = false;
          textArray[2].enabled = false;

          responseBackgroundTexB.mRender = true;
          textArray[4].enabled = true;

          cout << textArray[4].mString << "\n";
        }
      }
    }
    else if (gameState == STATE_INTRO)
    {
      if (eventType == "MOUSE_DOWN")
      {
        string texCol = TextureMouseCollision(uiIntroArray, NUM_INTRO_UI, xMouse, yMouse);

        Mix_FadeOutMusic(1000);
        //SDL_Delay(1);

        Mix_PlayMusic( levelOneMus, 1 );

        if (texCol == BTN_STARTGAME)
        {
          textArray[0].mString = "";
          textArray[0].mLetters = 0;
          gameState = STATE_GAME;
          numDebris = 0;
        }
      }

      //Render UI
      RenderUI(renderer, uiIntroArray, NUM_INTRO_UI);

      RenderTextWithDelays(renderer, fontTex, textArrayIntro, NUM_TEXT_INTRO);
    }

    //Render text
    RenderText(renderer, fontTex, textArray);

    

    //Swap buffers to present backbuffer to screen
    SDL_RenderPresent(renderer);

    ////////////////////////////////////////////////////////////////////////
    //End of main game code
    ////////////////////////////////////////////////////////////////////////

    frameTime = SDL_GetTicks() - frameStart;

    if (frameDelay > frameTime)
    {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  //Cleanup code

  //Free the sound effects
  Mix_FreeChunk( gScratch );
  Mix_FreeMusic( introMus );

  Mix_Quit();

  SDL_Quit();

  return 0;
}
