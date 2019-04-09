#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#include <string>

#include "obj_texture.h"

using namespace std;

//Be carefull to always set a size divisible cleanly by 3

#define GAMEWIDTH  640
#define GAMEHEIGHT 480

#define PI 3.14159265

#define DEBUG 1


//SPACE UI
const int NUM_UI_ELEMENTS = 6;
const string GRAYBACKGROUND     = "res/spaceUI/uiGray.png";

const string BTN_LEFTCURSOR     = "res/spaceUI/leftCursor.png";
const string BTN_RIGHTCURSOR    = "res/spaceUI/rightCursor.png";
const string BTN_STOPROT        = "res/spaceUI/stopRotate.png";

const string BTN_INCREASESPEED  = "res/spaceUI/increaseSpeed.png";
const string BTN_DECREASESPEED  = "res/spaceUI/decreaseSpeed.png";

const string BTN_HARVESTDEBRIS  = "res/spaceUI/harvestDebris.png";

struct vect2
{
  float x;
  float y;
};



#endif // GAMEDEFS_H
 
