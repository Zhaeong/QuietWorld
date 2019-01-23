#include "obj_ship.h"

Ship::Ship()
{
  mDirection.x = 1;
  mDirection.y = 1;
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

void Ship::changeDirection()
{
  //Convert degrees to radians
  float rad = 10 * PI / 180.0;

  //Normalize the vector
  float vecLength = sqrt((mDirection.x * mDirection.x) + (mDirection.y * mDirection.y));
  float dirX = mDirection.x / vecLength;
  float dirY = mDirection.y / vecLength;

  //Perform rotation by calling the rotation matrix
  mDirection.x = dirX * cos(rad) - dirY * sin(rad);
  mDirection.y = dirX * sin(rad) + dirY * cos(rad);
  
}

void Ship::renderShip()
{
  for (unsigned i = 0; i < vShipTextures.size(); ++i)
    {
      vShipTextures.at(i).renderTexture();
    }
}

