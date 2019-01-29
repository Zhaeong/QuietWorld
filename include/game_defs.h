#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#include <string>
using namespace std;


#define GAMEWIDTH  640
#define GAMEHEIGHT 480

#define PI 3.14159265


//SPACE UI
const string GRAYBACKGROUND     = "res/spaceUI/uiGray.png";

const string BTN_LEFTCURSOR     = "res/spaceUI/leftCursor.png";
const string BTN_RIGHTCURSOR    = "res/spaceUI/rightCursor.png";
const string BTN_STOPROT        = "res/spaceUI/stopRotate.png";

const string BTN_INCREASESPEED  = "res/spaceUI/increaseSpeed.png";
const string BTN_DECREASESPEED  = "res/spaceUI/decreaseSpeed.png";

struct vect2
{
  float x;
  float y;
};



#endif // GAMEDEFS_H
 
