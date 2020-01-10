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
  Mix_Music *introMus = Mix_LoadMUS("res/wavs/intro2.wav");
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
  if (DEBUG == 0)
  {
    Mix_PlayMusic(introMus, 1);
  }

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

  mainShip.mPosition.x = 200;
  mainShip.mPosition.y = curLevelBoundY / 2;

  CenterCamOnPlayer(&camX, &camY, camW, camH,
                    mainShip.mPosition.x,
                    mainShip.mPosition.y,
                    mainShip.mWidth,
                    mainShip.mHeight);

  //Initialize random seed for generating debris and name
  srand(time(NULL));

  //Create texture handling

  //Fonts
  SDL_Texture *fontTex = GetSDLTexture(renderer, window, "res/text/mainText.png");
  RemoveTextureWhiteSpace(window, fontTex);

  //Fadeout image
  SDL_Texture *fadeOutTex = GetSDLTexture(renderer, window, "res/background/blackOverlay.png");
  Texture blackFade(fadeOutTex, "fadeOut");
  blackFade.mAlpha = 0;
  
  TextObj textArray[NUM_TEXT];

  TextObj textArrayIntro[NUM_TEXT_INTRO];

  TextObj textArraySurvey[NUM_TEXT_SURVEY];

  SetIntroText(textArrayIntro);

  //set location of debris num ui
  textArray[0].mX = 320;
  textArray[0].mY = 444;
  textArray[0].mString = "";

  //Create debris Array
  Texture debrisArray[NUM_DEBRIS];

  //Create debrisTexture
  SDL_Texture *debrisTex = GetSDLTexture(renderer, window, DEBRIS_IMG);
  RemoveTextureWhiteSpace(window, debrisTex);

  //Initize it all to none
  GenerateDebris(debrisTex, debrisArray, 0, gameBackground.mWidth, gameBackground.mHeight, 0);

  //Initial debris generation hardcoded to single area
  Texture initdObj(debrisTex, DEBRIS_IMG);
  initdObj.mRender = true;
  initdObj.mX = 300;
  initdObj.mY = 250;
  debrisArray[0] = initdObj;

  bool runGame = true;

  int gameLevel = 0;

  int debrisIndex = -1;

  unsigned int gameTime = 0;

  unsigned int numDebris = 0;

  bool isMining = false;

  unsigned int holdDownTime = 0;

  bool showDialog = true;

  string gameState = STATE_INTRO;
  string newState = STATE_INTRO;

  bool inTransition = false;

  int delayTime = 0;

  //255/x = 2
  //We want it to cleaning divid into 255 because we add a certain alpha value per frame
  int delayPeriod = 127;

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
    
    //Catch when state transition occurs
    if(newState != gameState)
    {
      inTransition = true;
      if(delayTime < delayPeriod)
      {
        blackFade.mAlpha += 2;
        delayTime++;
      }
      else
      {
        cout << "New State: " << newState << "\n";
        gameState = newState;
        delayTime = 0;
        blackFade.mAlpha = 0;
        inTransition = false;
        if(newState == STATE_GAME || newState == STATE_PAUSE)
        {
          //Helper func to set the inter level texts
          SetInterLevelText(textArraySurvey, 
                            debrisTex, 
                            debrisArray, 
                            gameLevel, 
                            gameBackground.mWidth, 
                            gameBackground.mHeight);
          if(newState == STATE_GAME)
          {
            //Reset inter level text
            textArraySurvey[0].mString = "";
            textArraySurvey[0].mLetters = 0;

            textArraySurvey[1].mString = "";
            textArraySurvey[1].mLetters = 0;

            textArraySurvey[2].mString = "";
            textArraySurvey[2].mLetters = 0;

            textArraySurvey[3].mString = "";
            textArraySurvey[3].mLetters = 0;

            textArraySurvey[4].mString = "";
            textArraySurvey[4].mLetters = 0;

            //Reset renders
            choiceBackgroundTexA.mRender = true;
            choiceBackgroundTexB.mRender = true;
            responseBackgroundTexA.mRender = false;
            responseBackgroundTexB.mRender = false;

            textArraySurvey[1].enabled = true;
            textArraySurvey[2].enabled = true;
            textArraySurvey[3].enabled = false;

            uiInterLevelArray[1].mRender = false;
          }
        }
      }
    }

    if(inTransition)
    {
      //In transition means that we don't want any game states to update
      //But we still want it to render as it fades to black
    }
    else if (gameState == STATE_GAME)
    {
      //Set the num debris remaining to on
      textArray[0].enabled = true;

      if (eventType != "NONE")
      {
        cout << eventType << "\n";
        if (eventType == "MOUSE_DOWN")
        {
          string texCol = TextureMouseCollision(uiSpaceArray, NUM_SPACE_UI, xMouse, yMouse);

          if (texCol == BTN_LEFTCURSOR)
          {
            mainShip.curState = Ship::ShipStates::ROTATELEFT;
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_LEFTCURSOR_ACTIVE, true);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_LEFTCURSOR, false);

            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_RIGHTCURSOR_ACTIVE, false);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_RIGHTCURSOR, true);

            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_STOPROT_ACTIVE, false);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_STOPROT, true);
          }
          else if (texCol == BTN_RIGHTCURSOR)
          {
            mainShip.curState = Ship::ShipStates::ROTATERIGHT;
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_RIGHTCURSOR_ACTIVE, true);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_RIGHTCURSOR, false);

            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_LEFTCURSOR_ACTIVE, false);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_LEFTCURSOR, true);

            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_STOPROT_ACTIVE, false);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_STOPROT, true);
          }
          else if (texCol == BTN_STOPROT)
          {
            mainShip.curState = Ship::ShipStates::IDLE;
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_STOPROT_ACTIVE, true);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_STOPROT, false);

            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_LEFTCURSOR, true);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_RIGHTCURSOR, true);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_LEFTCURSOR_ACTIVE, false);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_RIGHTCURSOR_ACTIVE, false);
          }
          else if (texCol == BTN_INCREASESPEED)
          {
            mainShip.changeSpeed(1);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_INCREASESPEED_ACTIVE, true);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_INCREASESPEED, false);
          }
          else if (texCol == BTN_DECREASESPEED)
          {
            mainShip.changeSpeed(-1);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_DECREASESPEED_ACTIVE, true);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_DECREASESPEED, false);
          }
          else if (texCol == BTN_HARVESTDEBRIS_ENABLE)
          {
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS_ACTIVE, true);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS_ENABLE, false);
            
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
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS_ACTIVE, false);
          
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_INCREASESPEED, true);
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_INCREASESPEED_ACTIVE, false);
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_DECREASESPEED, true);
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_DECREASESPEED_ACTIVE, false);
          

          if (DEBUG == 0)
          {
            Mix_PlayChannel(-1, gScratch, 0);
          }

          if (TextureMouseCollisionSingle(dialogOKTex, xMouse, yMouse))
          {
            if (showDialog)
            {
              showDialog = false;

              SHADE = 0;
            }
          }
        }
      }

      //change the speed display
      if(mainShip.mSpeed == 0)
      {
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_0, true);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_1, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_2, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_3, false);
      }
      else if(mainShip.mSpeed == 1)
      {
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_0, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_1, true);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_2, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_3, false);
      }
      else if(mainShip.mSpeed == 2)
      {
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_0, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_1, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_2, true);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_3, false);
      }
      else if(mainShip.mSpeed == 3)
      {
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_0, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_1, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_2, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_3, true);
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

          //Check remaining debris, if 0 then add to game level and set the survey choice
          if (GetActiveDebrisNum(debrisArray) == 0)
          {
            gameLevel += 1;
            newState = STATE_PAUSE;
            isMining = false;
            holdDownTime = 0;

            if (DEBUG == 0)
            {
              //Mix_FadeOutMusic(1000);
              Mix_PlayMusic(interLevelMus, 1);
            }
          }
        }
      }

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
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS_ENABLE, true);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS, false);        
      }
      else
      {
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS_ENABLE, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS, true);
        debrisIndex = -1;
        isMining = false;
        holdDownTime = 0;
      }

      //Update game state
      mainShip.updateBasedOnState(curLevelBoundX, curLevelBoundY);

      //Update debris position
      UpdateDebris(debrisArray, curLevelBoundX, curLevelBoundY);

      //Update camera position
      MoveCameraBaseOnShip(renderer, &camX, &camY, camW, camH,
                           mainShip.mPosition.x, mainShip.mPosition.y, mainShip.mWidth, mainShip.mHeight,
                           mainShip.mSpeed);

      //Set num of debris gathered
      textArray[1].mString = to_string(numDebris);
      textArray[0].mString = to_string(GetActiveDebrisNum(debrisArray));      
      
    }
    else if (gameState == STATE_PAUSE) //Interlevel UI
    {
      //Set the num debris remaining to off
      textArray[0].enabled = false;

      //Get position of text to render a background for it
      choiceBackgroundTexA.mX = textArraySurvey[1].mX;
      choiceBackgroundTexA.mY = textArraySurvey[1].mY - 10;
      choiceBackgroundTexA.mWidth = textArraySurvey[1].mWidth;

      //If the text exceeds the width of screen it will flow to next line
      //Need to expand the height to accound for this
      // 520 = 640 - 60 * 2
      //Gamewidth minus the two margins
      if (textArraySurvey[1].mWidth > 520)
      {
        choiceBackgroundTexA.mHeight = 60;
        choiceBackgroundTexA.mWidth = 520;
      }

      choiceBackgroundTexB.mX = textArraySurvey[2].mX;
      choiceBackgroundTexB.mY = textArraySurvey[2].mY - 10;
      choiceBackgroundTexB.mWidth = textArraySurvey[2].mWidth;

      if (textArraySurvey[2].mWidth > 520)
      {
        choiceBackgroundTexB.mHeight = 60;
        choiceBackgroundTexB.mWidth = 520;
      }

      responseBackgroundTexA.mX = textArraySurvey[3].mX;
      responseBackgroundTexA.mY = textArraySurvey[3].mY - 10;
      responseBackgroundTexA.mWidth = textArraySurvey[3].mWidth;

      if (textArraySurvey[3].mWidth > 520)
      {
        responseBackgroundTexA.mHeight = 60;
        responseBackgroundTexA.mWidth = 520;
      }

      responseBackgroundTexB.mX = textArraySurvey[4].mX;
      responseBackgroundTexB.mY = textArraySurvey[4].mY - 10;
      responseBackgroundTexB.mWidth = textArraySurvey[4].mWidth;

      if (textArraySurvey[4].mWidth > 520)
      {
        responseBackgroundTexB.mHeight = 60;
        responseBackgroundTexB.mWidth = 520;
      }     

      if (eventType == "MOUSE_UP")
      {
        string texCol = TextureMouseCollision(uiInterLevelArray, NUM_INTERUI, xMouse, yMouse);

        if (TextureMouseCollisionSingle(choiceBackgroundTexA, xMouse, yMouse))
        {
          cout << "selected A";
          choiceBackgroundTexA.mRender = false;
          choiceBackgroundTexB.mRender = false;

          textArraySurvey[1].enabled = false;
          textArraySurvey[2].enabled = false;

          responseBackgroundTexA.mRender = true;
          textArraySurvey[3].enabled = true;
        }

        if (TextureMouseCollisionSingle(choiceBackgroundTexB, xMouse, yMouse))
        {
          cout << "selected B";
          choiceBackgroundTexA.mRender = false;
          choiceBackgroundTexB.mRender = false;
          textArraySurvey[1].enabled = false;
          textArraySurvey[2].enabled = false;

          responseBackgroundTexB.mRender = true;
          textArraySurvey[4].enabled = true;

          cout << textArraySurvey[4].mString << "\n";
        }

        if (texCol == BTN_CONTINUE)
        {        
          newState = STATE_GAME;
          numDebris += 1;
        }
      }
    }
    else if (gameState == STATE_INTRO)
    {
      if (eventType == "MOUSE_DOWN")
      {
        string texCol = TextureMouseCollision(uiIntroArray, NUM_INTRO_UI, xMouse, yMouse);

        if (DEBUG == 0)
        {
          //Mix_FadeOutMusic(1000);
          //SDL_Delay(1);

          Mix_PlayMusic(levelOneMus, 1);
        }
        
        if (texCol == BTN_STARTGAME)
        {
          newState = STATE_GAME;
          numDebris = 0;
        }
      }
    }

    ////////////////////////////
    //Render based on game State
    ////////////////////////////
    
    if (gameState == STATE_GAME)
    {
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

        int worldMouseX = camX + xMouse;
        int worldMouseY = camY + yMouse;
      
        textArray[2].mY = 30;
        textArray[2].mString = "x:" + to_string(xMouse) + " y:" + to_string(yMouse);

        textArray[3].mY = 60;
        textArray[3].mString = "x:" + to_string(worldMouseX) + " y:" + to_string(worldMouseY);

        textArray[4].mY = 90;
        textArray[4].mString = "GameTime:" + to_string(gameTime);

        textArray[5].mY = 120;
        textArray[5].mString = "HoldDown:" + to_string(holdDownTime);

        SDL_SetRenderDrawColor(renderer, 100, 255, 255, SDL_ALPHA_OPAQUE);

        SDL_RenderDrawLine(renderer,
                           mainShip.mPosition.x,
                           mainShip.mPosition.y,
                           mainShip.mPosition.x + mainShip.mDirection.x * 10,
                           mainShip.mPosition.y + mainShip.mDirection.y * 10);
      }

    }
    else if (gameState == STATE_PAUSE)
    {
      //Render UI
      RenderUI(renderer, uiInterLevelArray, NUM_INTERUI);

      RenderTexture(renderer, choiceBackgroundTexA);
      RenderTexture(renderer, choiceBackgroundTexB);
      RenderTexture(renderer, responseBackgroundTexA);
      RenderTexture(renderer, responseBackgroundTexB);

       //Render button when player made a choice
      if (RenderSurveyText(renderer, fontTex, textArraySurvey, NUM_TEXT_SURVEY))
      {
        //Continue button
        uiInterLevelArray[1].mRender = true;
      }
    }
    else if (gameState == STATE_INTRO)
    {
      //Render UI
      RenderUI(renderer, uiIntroArray, NUM_INTRO_UI);
      //Render each text
      RenderTextWithDelays(renderer, fontTex, textArrayIntro, NUM_TEXT_INTRO);
    }
    
    //Render text
    RenderText(renderer, fontTex, textArray);

    //Render the fadeoutTexture
    RenderTexture(renderer, blackFade);

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
  Mix_FreeChunk(gScratch);
  Mix_FreeMusic(introMus);

  Mix_Quit();

  SDL_Quit();

  return 0;
}
