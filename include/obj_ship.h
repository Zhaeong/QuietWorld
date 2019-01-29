#ifndef OBJ_SHIP_H
#define OBJ_SHIP_H

#include <vector>
#include <cmath>

#include "obj_texture.h"
#include "game_defs.h"

class Ship
{

 public:

  int mRotation = 0;
  int mRotationValue = 1;

  int mSpeed = 0;
  
  vector<Texture> vShipTextures;

  vect2 mPosition;
  vect2 mOrigDirection;
  vect2 mDirection;

  enum ShipStates
    {
     IDLE,
     ROTATELEFT,
     ROTATERIGHT
    };

  ShipStates curState = ShipStates::IDLE;

  Ship();

  void rotateByDegree(int degrees);

  void changePosition();

  void changeSpeed(int speed);
  
  void renderShip();

  void updateBasedOnState();

  void changeRotation(bool left);


};

#endif // OBJ_SHIP_H
