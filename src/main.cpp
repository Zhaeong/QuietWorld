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

  Mix_Music *levelOneMus = Mix_LoadMUS("res/wavs/spaceambient.wav");
  if (levelOneMus == NULL)
  {
    printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Music *interLevelMus = Mix_LoadMUS("res/wavs/girlfrombar.wav");
  if (interLevelMus == NULL)
  {
    printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Music *endMus = Mix_LoadMUS("res/wavs/heyyabye.wav");
  if (endMus == NULL)
  {
    printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
  }

  //Load sound effects

  Mix_Chunk *gScratch = NULL;

  gScratch = Mix_LoadWAV("res/wavs/mouseclickdown.wav");
  if (gScratch == NULL)
  {
    printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Chunk *humOne = NULL;
  humOne = Mix_LoadWAV("res/wavs/humOne.wav");
  if (humOne == NULL)
  {
    printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Chunk *humTwo = NULL;
  humTwo = Mix_LoadWAV("res/wavs/humTwo.wav");
  if (humTwo == NULL)
  {
    printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Chunk *humThree = NULL;
  humThree = Mix_LoadWAV("res/wavs/humThree.wav");
  if (humThree == NULL)
  {
    printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Chunk *mining = NULL;
  mining = Mix_LoadWAV("res/wavs/mining.wav");
  if (mining == NULL)
  {
    printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Chunk *crumble = NULL;
  crumble = Mix_LoadWAV("res/wavs/crumble.wav");
  if (crumble == NULL)
  {
    printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_Chunk *rotateSound = NULL;
  rotateSound = Mix_LoadWAV("res/wavs/triangleAsharp.wav");
  if (rotateSound == NULL)
  {
    printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
  }

  //Initiate Camera coords
  int camX = 0;
  int camY = 0;
  int camW = GAMEWIDTH;
  int camH = 366;

  //Create UI arrays
  Texture uiSpaceArray[NUM_SPACE_UI];
  InitSpaceUI(renderer, window, uiSpaceArray);

  Texture uiIntroArray[NUM_INTRO_UI];
  InitIntroUI(renderer, window, uiIntroArray);

  Texture uiInterLevelArray[NUM_INTERUI];
  InitInterLevelUI(renderer, window, uiInterLevelArray);

  //Commendation textures
  SDL_Texture *commendation1SDLTex = GetSDLTexture(renderer, window, COMMENDATION_1);
  RemoveTextureWhiteSpace(window, commendation1SDLTex);
  Texture commendation1(commendation1SDLTex, COMMENDATION_1);
  commendation1.mX = 565;
  commendation1.mY = 380;
  commendation1.mRender = false;

  SDL_Texture *commendation2SDLTex = GetSDLTexture(renderer, window, CITATION);
  RemoveTextureWhiteSpace(window, commendation2SDLTex);
  Texture commendation2(commendation2SDLTex, CITATION);
  commendation2.mX = 565;
  commendation2.mY = 380;
  commendation2.mRender = false;
  

  //Init Dialog textures
  SDL_Texture *tutorialDiag = GetSDLTexture(renderer, window, "res/dialogUI/tutorial.png");
  RemoveTextureWhiteSpace(window, tutorialDiag);
  Texture tutorialDiagTex = Texture(tutorialDiag, "res/dialogUI/tutorial.png");

  SDL_Texture *dialogOK = GetSDLTexture(renderer, window, "res/dialogUI/dialogOK.png");
  Texture dialogOKTex = Texture(dialogOK, "res/dialogUI/dialogOK.png");
  dialogOKTex.mX = 524;
  dialogOKTex.mY = 271;

  SDL_Texture *choiceBackground = GetSDLTexture(renderer, window, "res/dialogUI/buttonBackground.png");
  Texture choiceBackgroundTexA = Texture(choiceBackground, "res/dialogUI/choiceBackgroundA.png");
  Texture choiceBackgroundTexB = Texture(choiceBackground, "res/dialogUI/choiceBackgroundB.png");

  
  SDL_Texture *responseBackground = GetSDLTexture(renderer, window, "res/dialogUI/responseBackground.png");

  Texture questionBackgroundTex = Texture(responseBackground, "questionBackground");
  questionBackgroundTex.mRender = false;
  
  Texture responseBackgroundTexA = Texture(responseBackground, "responseBackgroundTexA");
  responseBackgroundTexA.mRender = false;
  Texture responseBackgroundTexB = Texture(responseBackground, "responseBackgroundTexB");
  responseBackgroundTexB.mRender = false;

  
  //Button textures
  SDL_Texture *buttonTopLeft     = GetSDLTexture(renderer, window, "res/dialogUI/buttonTopLeft.png");
  Texture buttonTopLeftTex = Texture(buttonTopLeft, "buttonTopLeft");  
  SDL_Texture *buttonTop         = GetSDLTexture(renderer, window, "res/dialogUI/buttonTop.png");
  Texture buttonTopTex = Texture(buttonTop, "buttonTop");
  SDL_Texture *buttonTopRight    = GetSDLTexture(renderer, window, "res/dialogUI/buttonTopRight.png");
  Texture buttonTopRightTex = Texture(buttonTopRight, "buttonTopRight");  
  SDL_Texture *buttonRight       = GetSDLTexture(renderer, window, "res/dialogUI/buttonRight.png");
  Texture buttonRightTex = Texture(buttonRight, "buttonRight");
  SDL_Texture *buttonBottomRight = GetSDLTexture(renderer, window, "res/dialogUI/buttonBottomRight.png");
  Texture buttonBottomRightTex = Texture(buttonBottomRight, "buttonBottomRight");
  SDL_Texture *buttonBottom      = GetSDLTexture(renderer, window, "res/dialogUI/buttonBottom.png");
  Texture buttonBottomTex = Texture(buttonBottom, "buttonBottom");
  SDL_Texture *buttonBottomLeft  = GetSDLTexture(renderer, window, "res/dialogUI/buttonBottomLeft.png");
  Texture buttonBottomLeftTex = Texture(buttonBottomLeft, "buttonBottomLeft");
  SDL_Texture *buttonLeft        = GetSDLTexture(renderer, window, "res/dialogUI/buttonLeft.png");
  Texture buttonLeftTex = Texture(buttonLeft, "buttonLeft");

  //response textures
  SDL_Texture *responseTopLeft     = GetSDLTexture(renderer, window, "res/dialogUI/responseTopLeft.png");
  Texture responseTopLeftTex       = Texture(responseTopLeft, "responseTopLeft");  
  SDL_Texture *responseTop         = GetSDLTexture(renderer, window, "res/dialogUI/responseTop.png");
  Texture responseTopTex           = Texture(responseTop, "responseTop");
  SDL_Texture *responseTopRight    = GetSDLTexture(renderer, window, "res/dialogUI/responseTopRight.png");
  Texture responseTopRightTex      = Texture(responseTopRight, "responseTopRight");  
  SDL_Texture *responseRight       = GetSDLTexture(renderer, window, "res/dialogUI/responseRight.png");
  Texture responseRightTex         = Texture(responseRight, "responseRight");
  SDL_Texture *responseBottomRight = GetSDLTexture(renderer, window, "res/dialogUI/responseBottomRight.png");
  Texture responseBottomRightTex   = Texture(responseBottomRight, "responseBottomRight");
  SDL_Texture *responseBottom      = GetSDLTexture(renderer, window, "res/dialogUI/responseBottom.png");
  Texture responseBottomTex        = Texture(responseBottom, "responseBottom");
  SDL_Texture *responseBottomLeft  = GetSDLTexture(renderer, window, "res/dialogUI/responseBottomLeft.png");
  Texture responseBottomLeftTex    = Texture(responseBottomLeft, "responseBottomLeft");
  SDL_Texture *responseLeft        = GetSDLTexture(renderer, window, "res/dialogUI/responseLeft.png");
  Texture responseLeftTex          = Texture(responseLeft, "responseLeft");

  //Create ship char
  SDL_Texture *shipTex = GetSDLTexture(renderer, window, "res/ship/ship.png");
  RemoveTextureWhiteSpace(window, shipTex);
  Ship mainShip(shipTex);

  //Ship thrust textures
  SDL_Texture *leftThrustTex = GetSDLTexture(renderer, window, "res/ship/shipLeftThrust.png");
  RemoveTextureWhiteSpace(window, leftThrustTex);
  mainShip.mLeftThrust = leftThrustTex;

  SDL_Texture *rightThrustTex = GetSDLTexture(renderer, window, "res/ship/shipRightThrust.png");
  RemoveTextureWhiteSpace(window, rightThrustTex);
  mainShip.mRightThrust = rightThrustTex;

  SDL_Texture *bottomThrust1Tex = GetSDLTexture(renderer, window, "res/ship/bottomThrust1.png");
  RemoveTextureWhiteSpace(window, bottomThrust1Tex);
  mainShip.mBottomThrust1 = bottomThrust1Tex;

  SDL_Texture *bottomThrust2Tex = GetSDLTexture(renderer, window, "res/ship/bottomThrust2.png");
  RemoveTextureWhiteSpace(window, bottomThrust2Tex);
  mainShip.mBottomThrust2 = bottomThrust2Tex;

  SDL_Texture *bottomThrust3Tex = GetSDLTexture(renderer, window, "res/ship/bottomThrust3.png");
  RemoveTextureWhiteSpace(window, bottomThrust3Tex);
  mainShip.mBottomThrust3 = bottomThrust3Tex;
  
  //Load textures for mining progress
  SDL_Texture *miningBarEmpty = GetSDLTexture(renderer, window, "res/dialogUI/mineBarEmpty.png");
  Texture miningBarEmptyTex = Texture(miningBarEmpty, "res/dialogUI/mineBarEmpty.png");

  SDL_Texture *miningBarColor = GetSDLTexture(renderer, window, "res/dialogUI/mineBarColor.png");
  Texture miningBarColorTex = Texture(miningBarColor, "res/dialogUI/mineBarColor.png");

  //Create bounds for player
  SDL_Texture *backgroundTex = GetSDLTexture(renderer, window, "res/background/level1.png");
  Texture gameBackground = Texture(backgroundTex, "res/background/level1.png");

  SDL_Texture *backgroundOfflimitTex = GetSDLTexture(renderer, window, "res/background/backgroundOfflimit.png");
  Texture backGroundOfflimit = Texture(backgroundOfflimitTex, "res/background/backgroundOfflimit.png");

  backGroundOfflimit.mX = -350;
  backGroundOfflimit.mY = -350;

  int curLevelBoundX = gameBackground.mWidth;
  int curLevelBoundY = gameBackground.mHeight;

  mainShip.mPosition.x = curLevelBoundX / 2;
  mainShip.mPosition.y = curLevelBoundY / 2 - (mainShip.mHeight/2);

  CenterCamOnPlayer(&camX, &camY, camW, camH,
                    mainShip.mPosition.x,
                    mainShip.mPosition.y,
                    mainShip.mWidth,
                    mainShip.mHeight);

  //Initialize random seed for generating debris and name
  srand(time(NULL));

  //star texture
  //star texture
  SDL_Texture *starSDLTex = GetSDLTexture(renderer, window, "res/background/star.png");
  
  Texture starArray[NUM_STARS];

  InitStarArray(starArray, starSDLTex, gameBackground.mWidth, gameBackground.mHeight);


  //Fonts
  SDL_Texture *fontTex = GetSDLTexture(renderer, window, "res/text/mainText.png");
  RemoveTextureWhiteSpace(window, fontTex);

  //Fadeout image
  SDL_Texture *fadeOutTex = GetSDLTexture(renderer, window, "res/background/blackOverlay.png");
  Texture blackFade(fadeOutTex, "fadeOut");
  blackFade.mAlpha = 0;

  SDL_Texture *endfadeOutTex = GetSDLTexture(renderer, window, "res/background/endOverlay.png");
  Texture endFade(endfadeOutTex, "fadeOut");
  endFade.mAlpha = 0;

  SDL_Texture *startOverlayTex = GetSDLTexture(renderer, window, "res/background/introOverlay.png");
  Texture startFade(startOverlayTex, "fadeOut");
  startFade.mAlpha = 255;
  
  TextObj textArray[NUM_TEXT];

  TextObj textArrayIntro[NUM_TEXT_INTRO];

  TextObj textArraySurvey[NUM_TEXT_SURVEY];

  int playerID = SetIntroText(textArrayIntro);

  
  textArray[1].mX = 16;
  textArray[1].mY = 10;
  textArray[1].mString = "DMaRu" + to_string(playerID);
  textArray[1].enabled = false;

  //set location of debris num ui
  textArray[0].mX = 320;
  textArray[0].mY = 448;
  textArray[0].mString = "";

  //Create debris Array
  Texture debrisArray[NUM_DEBRIS];

  //Create debrisTexture

  SDL_Texture* debrisTexArray[NUM_DEBRIS_TEX];
  
  SDL_Texture *debrisTex = GetSDLTexture(renderer, window, DEBRIS_IMG);
  RemoveTextureWhiteSpace(window, debrisTex);
  debrisTexArray[0] = debrisTex;

  SDL_Texture *debrisTex2 = GetSDLTexture(renderer, window, DEBRIS_IMG2);
  RemoveTextureWhiteSpace(window, debrisTex2);
  debrisTexArray[1] = debrisTex2;

  SDL_Texture *debrisTex3 = GetSDLTexture(renderer, window, DEBRIS_IMG3);
  RemoveTextureWhiteSpace(window, debrisTex3);
  debrisTexArray[2] = debrisTex3;

  //Initize it all to none
  GenerateDebris(debrisTexArray, debrisArray, 0, gameBackground.mWidth, gameBackground.mHeight, 0);

  //Initial debris generation hardcoded to single area
  Texture initdObj(debrisTex, DEBRIS_IMG);
  initdObj.mRender = true;
  initdObj.mX = curLevelBoundX / 2 - (mainShip.mWidth/2) + 100;
  initdObj.mY = curLevelBoundY / 2 - (initdObj.mHeight/2);
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

  bool allowControl = false;

  bool fadeUI = false;

  int delayTime = 0;

  unsigned int modulusValue = 0;

  //255/x = 2
  //We want it to cleaning divid into 255 because we add a certain alpha value per frame
  int delayPeriod = 127;

  int curPlay = 0;
  int nextPlay = 0;
  int curRot = 0;
  int nextRot = 0;

  while (runGame)
  {
    frameStart = SDL_GetTicks();

    //The color at which the screen will be if alpha = 0 on all textures
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);

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
                            debrisTexArray, 
                            debrisArray, 
                            gameLevel, 
                            gameBackground.mWidth, 
                            gameBackground.mHeight);
          if(newState == STATE_GAME)
          {
            if (DEBUG == 0)
            {
              if(fadeUI)
              {
                //Mix_PlayMusic(endMus, 1);
               
              }
              else
              {
                Mix_PlayMusic(levelOneMus, -1);
              }
            }
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
          else if(newState == STATE_PAUSE)
          {
            Mix_FadeInMusic(interLevelMus, 1, 1000);
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
      textArray[1].enabled = true;

      //Update stars
      UpdateStars(starArray, gameBackground.mWidth, gameBackground.mHeight);


      if (eventType != "NONE")
      {
        cout << eventType << "\n";

        string texCol = TextureMouseCollision(uiSpaceArray, NUM_SPACE_UI, xMouse, yMouse);

        if (eventType == "MOUSE_DOWN")
        {
          
          if(allowControl)
          {
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

		Mix_PlayChannel(2, mining, -1);
                isMining = true;
              }
            }
            cout << texCol << "\n";
          }
        }
        else if (eventType == "MOUSE_UP")
        {
          isMining = false;

	  Mix_HaltChannel(2); 
          holdDownTime = 0;
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS_ACTIVE, false);
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_INCREASESPEED, true);
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_INCREASESPEED_ACTIVE, false);
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_DECREASESPEED, true);
          SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_DECREASESPEED_ACTIVE, false);

          //Only play click sounds when clicked a button
          if(texCol == BTN_LEFTCURSOR_ACTIVE  ||
             texCol == BTN_RIGHTCURSOR_ACTIVE ||
             texCol == BTN_STOPROT_ACTIVE ||
             texCol == BTN_INCREASESPEED_ACTIVE ||
             texCol == BTN_INCREASESPEED_ACTIVE ||
             texCol == BTN_DECREASESPEED_ACTIVE)
          {
            Mix_PlayChannel(-1, gScratch, 0);
          }




          if (TextureMouseCollisionSingle(dialogOKTex, xMouse, yMouse))
          {
            if (showDialog)
            {
              showDialog = false;
              allowControl = true;

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
        nextPlay = 0;

      }
      else if(mainShip.mSpeed == 1)
      {
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_0, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_1, true);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_2, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_3, false);        
        nextPlay = 1;
      }
      else if(mainShip.mSpeed == 2)
      {
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_0, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_1, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_2, true);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_3, false);
        nextPlay = 2;
      }
      else if(mainShip.mSpeed == 3)
      {
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_0, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_1, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_2, false);
        SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, SPEED_3, true);
        nextPlay = 3;
      }



      if(nextPlay == 0) 
      {
        nextPlay = 0;
      }

      if(curPlay != nextPlay)
      {
        Mix_HaltChannel(1); 
        if(nextPlay == 0)
        {
          Mix_HaltChannel(1); 
        }
        else if(nextPlay == 1)
        {
          Mix_PlayChannel(1, humOne, -1);
          cout << "play humone\n";
        }
        else if(nextPlay == 2)
        {
          Mix_PlayChannel(1, humTwo, -1);
        }
        else if(nextPlay == 3)
        {
          Mix_PlayChannel(1, humThree, -1);
        }
        curPlay = nextPlay;
      }


      if(mainShip.curState == Ship::ShipStates::ROTATELEFT ||
          mainShip.curState == Ship::ShipStates::ROTATERIGHT)
      {
        nextRot = 1;
      }
      else
      {
        nextRot = 0;
      }
      if(curRot != nextRot)
      {
        if(nextRot == 0)
        {
          Mix_HaltChannel(4);
        }
        else if(nextRot == 1)
        {
          Mix_PlayChannel(4, rotateSound, -1);
          cout <<"rotating\n";
        }
        curRot = nextRot;
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
          Mix_PlayChannel(3, crumble, 0);

          //Check remaining debris, if 0 then add to game level and set the survey choice
          if (GetActiveDebrisNum(debrisArray) == 0)
          {
            Mix_HaltChannel(2);
            gameLevel += 1;
            newState = STATE_PAUSE;
            isMining = false;
            holdDownTime = 0;

            //reset ui
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS_ACTIVE, false);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS_ENABLE, false);
            SetRenderUIElement(uiSpaceArray, NUM_SPACE_UI, BTN_HARVESTDEBRIS, true);   

            if (DEBUG == 0)
            {
              Mix_FadeOutMusic(1000);

              //Mix_PlayMusic(interLevelMus, 1);
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
      //textArray[1].mString = to_string(numDebris);
      textArray[0].mString = to_string(GetActiveDebrisNum(debrisArray));      
      
    }
    else if (gameState == STATE_PAUSE) //Interlevel UI
    {
      //Set the num debris remaining to off
      textArray[0].enabled = false;



      questionBackgroundTex.mRender = true;
      questionBackgroundTex.mX = textArraySurvey[0].mX - 10;
      questionBackgroundTex.mY = textArraySurvey[0].mY - 10;

      //Fix width on question
      questionBackgroundTex.mWidth = 565;

      //Divides the width of the text by total allowed with and gets the ceiling
      //This way we can increment the height from the width
      //int numHeight = ceil((float)textArraySurvey[0].mWidth / (float)520);        
      //questionBackgroundTex.mHeight = numHeight * 40;

      //fuck it just fix the height
      questionBackgroundTex.mHeight = 120;
        
      //cout << "w: " << textArraySurvey[0].mWidth << " calc: " << numHeight << "\n";


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

      responseBackgroundTexA.mX = textArraySurvey[3].mX - 10;
      responseBackgroundTexA.mY = textArraySurvey[3].mY - 10;
      responseBackgroundTexA.mWidth = textArraySurvey[3].mWidth + 18;

      int numHeight = ceil((float)textArraySurvey[3].mWidth / (float)520);        
      responseBackgroundTexA.mHeight = numHeight * 40;

      if (textArraySurvey[3].mWidth > 520)
      {
        responseBackgroundTexA.mWidth = 520;
      }

      responseBackgroundTexB.mX = textArraySurvey[4].mX - 10;
      responseBackgroundTexB.mY = textArraySurvey[4].mY - 10;
      responseBackgroundTexB.mWidth = textArraySurvey[4].mWidth + 18;

      numHeight = ceil((float)textArraySurvey[4].mWidth / (float)520);        
      responseBackgroundTexB.mHeight = numHeight * 40;

      if (textArraySurvey[4].mWidth > 520)
      {
        responseBackgroundTexB.mWidth = 520;
      }     

      //search for inputs only after the second choice finished rendering
      if (eventType == "MOUSE_UP" && textArraySurvey[2].finished )
      {
        string texCol = TextureMouseCollision(uiInterLevelArray, NUM_INTERUI, xMouse, yMouse);

        if (TextureMouseCollisionSingle(choiceBackgroundTexA, xMouse, yMouse))
        {
          cout << "selected A: " << textArraySurvey[1].mString << "\n";
          choiceBackgroundTexA.mRender = false;
          choiceBackgroundTexB.mRender = false;

          textArraySurvey[1].enabled = false;
          textArraySurvey[2].enabled = false;

          responseBackgroundTexA.mRender = true;
          textArraySurvey[3].enabled = true;

          //I really like this
          //Nope, not my concern
            
          if(textArraySurvey[1].mString == "Nope, not my concern")
          {
            cout << "commeneded\n";
            commendation1.mRender = true;
          }          
        }

        if (TextureMouseCollisionSingle(choiceBackgroundTexB, xMouse, yMouse))
        {
          cout << "selected B: " << textArraySurvey[2].mString << "\n";
          choiceBackgroundTexA.mRender = false;
          choiceBackgroundTexB.mRender = false;
          textArraySurvey[1].enabled = false;
          textArraySurvey[2].enabled = false;

          responseBackgroundTexB.mRender = true;
          textArraySurvey[4].enabled = true;

          if(textArraySurvey[2].mString == "I'd guess they probably offed themselves")
          {
            cout << "citated\n";
            commendation2.mRender = true;
          }          

          cout << textArraySurvey[4].mString << "\n";
        }

        if (texCol == BTN_CONTINUE)
        {
          //Last level, play the ending
          if(gameLevel == 13)
          {
            fadeUI = true;
            mainShip.mPosition.x = curLevelBoundX / 2 - 20;
            mainShip.mPosition.y = curLevelBoundY / 2 - (mainShip.mHeight/2);

            mainShip.mSpeed = 0;

            CenterCamOnPlayer(&camX, &camY, camW, camH,
                    mainShip.mPosition.x,
                              mainShip.mPosition.y,
                              mainShip.mWidth,
                              mainShip.mHeight);            
      
            allowControl = false;
          }
          newState = STATE_GAME;
          numDebris += 1;
          Mix_FadeOutMusic(2000);
        }
      }
    }
    else if (gameState == STATE_INTRO)
    {
      if(startFade.mAlpha == 1)
      {
        Mix_PlayMusic(introMus, 1);
      }
      if (eventType == "MOUSE_DOWN")
      {
        string texCol = TextureMouseCollision(uiIntroArray, NUM_INTRO_UI, xMouse, yMouse);        
        
        if (texCol == BTN_STARTGAME)
        {
          Mix_FadeOutMusic(1000);
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
      //Render offlimit background
      RenderTextureByCam(camX, camY, renderer, backGroundOfflimit);
      
      //Render Background
      RenderTextureByCam(camX, camY, renderer, gameBackground);

      //Render Stars
      RenderStars(renderer, starArray, camX, camY);

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

        int miningBarY = playCamY + mainShip.mHeight + 2;
        miningBarEmptyTex.mX = playCamX;
        miningBarEmptyTex.mY = miningBarY;

        RenderTexture(renderer, miningBarEmptyTex);

        //Now render the progress based on
        miningBarColorTex.mX = playCamX;
        miningBarColorTex.mY = miningBarY;
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

        SDL_RenderDrawLine(renderer,
                           playCamX + mainShip.mWidth / 2,
                           playCamY + mainShip.mHeight / 2 + 1,
                           debrisCamX + debrisArray[debrisIndex].mWidth / 2,
                           debrisCamY + debrisArray[debrisIndex].mHeight / 2);

        SDL_RenderDrawLine(renderer,
                           playCamX + mainShip.mWidth / 2,
                           playCamY + mainShip.mHeight / 2 - 1,
                           debrisCamX + debrisArray[debrisIndex].mWidth / 2,
                           debrisCamY + debrisArray[debrisIndex].mHeight / 2);

        SDL_RenderDrawLine(renderer,
                           playCamX + mainShip.mWidth / 2 + 1,
                           playCamY + mainShip.mHeight / 2,
                           debrisCamX + debrisArray[debrisIndex].mWidth / 2,
                           debrisCamY + debrisArray[debrisIndex].mHeight / 2);

        SDL_RenderDrawLine(renderer,
                           playCamX + mainShip.mWidth / 2 - 1,
                           playCamY + mainShip.mHeight / 2,
                           debrisCamX + debrisArray[debrisIndex].mWidth / 2,
                           debrisCamY + debrisArray[debrisIndex].mHeight / 2);
        
      }

      if(fadeUI)
      {
        //Reduce the speed of fadeout by factor of 2
        if(modulusValue % 3 == 0)
        {
          ReduceAlphaArray(uiSpaceArray, NUM_SPACE_UI, 1);
          commendation1.mAlpha -= 1;
          commendation2.mAlpha -= 1;
        }
        modulusValue += 1;
        cout << "modval:" <<modulusValue << "\n";

        if(modulusValue >= 700)
        {
          //remove rendering of remaining debris and text
          textArray[0].enabled = false;
          textArray[1].enabled = false;
        }
        
        if(modulusValue == 500)
        {
          
          Mix_FadeInMusic(endMus, 1, 3000);
        }
        //Start fading to black after ui fades
        if(modulusValue >= 1700)
        {
          
            
          if(endFade.mAlpha <= 254)
          {
            endFade.mAlpha += 1;
          }
        }
        //Fade out song
        if(modulusValue > 2600)
        {
          Mix_FadeOutMusic(1000);
        }
        //Exit game
        if(modulusValue > 2800)
        {
          runGame = false;
        }
      }
      //Render UI
      RenderUI(renderer, uiSpaceArray, NUM_SPACE_UI);

      //Render commendation
      RenderTexture(renderer, commendation1);
      RenderTexture(renderer, commendation2);
      

      //Popup dialogs rendering
      if (showDialog)
      {
        RenderTexture(renderer, tutorialDiagTex);
        RenderTexture(renderer, dialogOKTex);
      }

      //Render DEBUG items if turned on
      if (DEBUG == 1)
      {

        //Screen cam coords to world cam coords
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

        //convert ship world pos to screen coord
        int shipWorldPosX = mainShip.mPosition.x - camX;
        int shipWorldPosY = mainShip.mPosition.y - camY;

        SDL_RenderDrawLine(renderer,
                           shipWorldPosX,
                           shipWorldPosY,
                           shipWorldPosX + mainShip.mDirection.x * 10,
                           shipWorldPosY + mainShip.mDirection.y * 10);
      }

    }
    else if (gameState == STATE_PAUSE)
    {
      //Render UI
      RenderUI(renderer, uiInterLevelArray, NUM_INTERUI);

      RenderTexture(renderer, questionBackgroundTex);

      if(questionBackgroundTex.mRender)
      {
        RenderTextBoxBorders(renderer,
                             questionBackgroundTex.mX,
                             questionBackgroundTex.mY,
                             questionBackgroundTex.mWidth,
                             questionBackgroundTex.mHeight,
                             responseTopLeftTex.mWidth,
                             responseTopLeftTex,
                             responseTopTex,
                             responseTopRightTex,
                             responseRightTex,
                             responseBottomRightTex,
                             responseBottomTex,
                             responseBottomLeftTex,
                             responseLeftTex);
      }
      

      RenderTexture(renderer, choiceBackgroundTexA);

      if(choiceBackgroundTexA.mRender)
      {
        RenderTextBoxBorders(renderer,
                             choiceBackgroundTexA.mX,
                             choiceBackgroundTexA.mY,
                             choiceBackgroundTexA.mWidth,
                             choiceBackgroundTexA.mHeight,
                             buttonTopLeftTex.mWidth,
                             buttonTopLeftTex,
                             buttonTopTex,
                             buttonTopRightTex,
                             buttonRightTex,
                             buttonBottomRightTex,
                             buttonBottomTex,
                             buttonBottomLeftTex,
                             buttonLeftTex);
      }
      
      RenderTexture(renderer, choiceBackgroundTexB);

      if(choiceBackgroundTexB.mRender)
      {
        RenderTextBoxBorders(renderer,
                             choiceBackgroundTexB.mX,
                             choiceBackgroundTexB.mY,
                             choiceBackgroundTexB.mWidth,
                             choiceBackgroundTexB.mHeight,
                             buttonTopLeftTex.mWidth,
                             buttonTopLeftTex,
                             buttonTopTex,
                             buttonTopRightTex,
                             buttonRightTex,
                             buttonBottomRightTex,
                             buttonBottomTex,
                             buttonBottomLeftTex,
                             buttonLeftTex);
      }
      
      RenderTexture(renderer, responseBackgroundTexA);

      if(responseBackgroundTexA.mRender)
      {
        RenderTextBoxBorders(renderer,
                             responseBackgroundTexA.mX,
                             responseBackgroundTexA.mY,
                             responseBackgroundTexA.mWidth,
                             responseBackgroundTexA.mHeight,
                             responseTopLeftTex.mWidth,
                             responseTopLeftTex,
                             responseTopTex,
                             responseTopRightTex,
                             responseRightTex,
                             responseBottomRightTex,
                             responseBottomTex,
                             responseBottomLeftTex,
                             responseLeftTex);
      }
      
      RenderTexture(renderer, responseBackgroundTexB);

      if(responseBackgroundTexB.mRender)
      {
        RenderTextBoxBorders(renderer,
                             responseBackgroundTexB.mX,
                             responseBackgroundTexB.mY,
                             responseBackgroundTexB.mWidth,
                             responseBackgroundTexB.mHeight,
                             responseTopLeftTex.mWidth,
                             responseTopLeftTex,
                             responseTopTex,
                             responseTopRightTex,
                             responseRightTex,
                             responseBottomRightTex,
                             responseBottomTex,
                             responseBottomLeftTex,
                             responseLeftTex);
      }

       //Render button when player made a choice
      if (RenderSurveyText(renderer, fontTex, textArraySurvey, NUM_TEXT_SURVEY))
      {
        //Continue button
        uiInterLevelArray[1].mRender = true;
      }
    }
    else if (gameState == STATE_INTRO)
    {
      RenderTexture(renderer, startFade);

      if(startFade.mAlpha > 0)
      {
        startFade.mAlpha -= 1;
      }

     

      if(startFade.mAlpha == 0)
      {        
        //Render UI
        RenderUI(renderer, uiIntroArray, NUM_INTRO_UI);
        //Render each text
        RenderTextWithDelays(renderer, fontTex, textArrayIntro, NUM_TEXT_INTRO);
      }
    }
    
    //Render text
    RenderText(renderer, fontTex, textArray);    

    //Render the fadeoutTexture
    RenderTexture(renderer, blackFade);

    RenderTexture(renderer, endFade);

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
