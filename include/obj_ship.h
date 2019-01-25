#ifndef OBJ_SHIP_H
#define OBJ_SHIP_H

#include <vector>
#include <cmath>

#include "obj_texture.h"
#include "game_defs.h"
class Ship
{

 public:

  double mRotationValue = 180;
  
  vector<Texture> vShipTextures;

  vect2 mPosition;
  vect2 mDirection;

  enum ShipStates
    {
     IDLE,
     ROTATELEFT,
     ROTATERIGHT
    };

  ShipStates curState = ShipStates::IDLE;

  Ship();

  void renderShip();

  void updateBasedOnState();

  void changeRotation(bool left);

  void changeDirection();

};

#endif // OBJ_SHIP_H
