#ifndef OBJ_SHIP_H
#define OBJ_SHIP_H

#include <vector>
#include "obj_texture.h"

class Ship
{

 public:
  int mX = 0;
  int mY = 0;
  double mRotationValue = 1;
  vector<Texture> vShipTextures;

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

  void changeRotation(int rotationVal);

};

#endif // OBJ_SHIP_H
