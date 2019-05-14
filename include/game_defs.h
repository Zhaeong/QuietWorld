#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#include <string>


using namespace std;

//Be carefull to always set a size divisible cleanly by 3

#define GAMEWIDTH  640
#define GAMEHEIGHT 480


#define DEBRISWIDTH 700
#define DEBRISHEIGHT 540

#define PI 3.14159265

#define DEBUG 1

struct vect2
{
  float x;
  float y;
};

struct TextObj
{
  string mString = "";
  int mX = 0;
  int mY = 0;
  unsigned int mDelay = 0;
  unsigned int mLastTime = 0;
  unsigned int mLetters = 0;
};

//GAME STATES

const string STATE_GAME = "GAME";
const string STATE_PAUSE = "PAUSE";

//SPACE UI

const int NUM_SPACE_UI = 8;

const string GRAYBACKGROUND     = "res/spaceUI/uiGray.png";

const string BTN_LEFTCURSOR     = "res/spaceUI/leftCursor.png";
const string BTN_RIGHTCURSOR    = "res/spaceUI/rightCursor.png";
const string BTN_STOPROT        = "res/spaceUI/stopRotate.png";

const string BTN_INCREASESPEED  = "res/spaceUI/increaseSpeed.png";
const string BTN_DECREASESPEED  = "res/spaceUI/decreaseSpeed.png";

const string BTN_HARVESTDEBRIS  = "res/spaceUI/harvestDebrisGray.png";
const string BTN_HARVESTDEBRIS_ACTIVE  = "res/spaceUI/harvestDebrisActive.png";


//INTRO UI

const int NUM_INTRO_UI = 1;
const string BTN_STARTGAME     = "res/dialogUI/startGame.png";

//DEBRIS
const int NUM_DEBRIS = 90;
const string DEBRIS_IMG  = "res/debris/debris1.png";


//TEXT DISPLAY
const int NUM_TEXT = 4;

#endif // GAMEDEFS_H
 
