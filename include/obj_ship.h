#ifndef OBJ_SHIP_H
#define OBJ_SHIP_H

#include <cmath>
#include <iostream>
#include <SDL.h>
#include "game_defs.h"


class Ship
{

public:
  
  double mRotation = 0;
  
  double mRotationValue = 0.5;
  int mSpeed = 0;
  
  SDL_Texture *mShipTexture;

  vect2 mPosition;
  vect2 mOrigDirection;
  vect2 mDirection;

  int mWidth = 20;
  int mHeight = 20;

  SDL_Point *mCenter = NULL;
  SDL_RendererFlip mFlip = SDL_FLIP_NONE;

  enum ShipStates
    {
     IDLE,
     ROTATELEFT,
     ROTATERIGHT
    };

  ShipStates curState = ShipStates::IDLE;

  Ship(SDL_Texture *shipTexture);

  void rotateByDegree(double degrees);

  void changePosition(int curLevelBoundX, int curLevelBoundY);

  void changeSpeed(int speed);

  void updateBasedOnState(int curLevelBoundX, int curLevelBoundY);

  void changeRotation(bool left);
  


};

#endif // OBJ_SHIP_H
