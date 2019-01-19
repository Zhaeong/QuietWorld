#ifndef OBJ_SHIP_H
#define OBJ_SHIP_H

#include <vector>
#include "obj_texture.h"

class Ship
{

 public:
  int mX = 0;
  int mY = 0;
  double mRotation = 0;
  vector<Texture> vShipTextures;

  Ship();

  void renderShip();

};

#endif // OBJ_SHIP_H
