#include "obj_ship.h"

Ship::Ship()
{
  
}

void Ship::renderShip()
{
  for (unsigned i = 0; i < vShipTextures.size(); ++i)
    {
      vShipTextures.at(i).renderTexture();
    }
}
