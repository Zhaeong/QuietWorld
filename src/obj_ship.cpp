#include "obj_ship.h"

Ship::Ship()
{
  
}

void Ship::changeRotation(int rotationVal)
{
    for (unsigned i = 0; i < vShipTextures.size(); ++i)
    {
      vShipTextures.at(i).mRotation += rotationVal;
    }
}

void Ship::updateBasedOnState()
{
  switch(curState)
    {
    case IDLE:
      //Do stuff
      break;
    case ROTATELEFT:
      changeRotation(mRotationValue);
      break;
    case ROTATERIGHT:
      changeRotation(-mRotationValue);
      break; 
    default:
      cout << "ERROR: ShipState not Recognized";
      break;
    }
}

void Ship::renderShip()
{
  for (unsigned i = 0; i < vShipTextures.size(); ++i)
    {
      vShipTextures.at(i).renderTexture();
    }
}

