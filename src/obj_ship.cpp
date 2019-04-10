#include "obj_ship.h"

Ship::Ship(SDL_Texture *shipTexture)
{

  mShipTexture = shipTexture;
  mOrigDirection.x = 0;
  mOrigDirection.y = -1;
  
  mDirection.x = 0;
  mDirection.y = -1;
  
  mPosition.x = 0;
  mPosition.y = 0;

  SDL_QueryTexture(shipTexture, NULL, NULL, &mWidth, &mHeight);
}

void Ship::rotateByDegree(double degrees)
{
  //mRotation = (mRotation + degrees) % 360.0;
  mRotation = std::fmod((mRotation + degrees), 360.0);

  //Convert degrees to radians
  double rad = mRotation * PI / (double)180.0;

  //Normalize the vector
  float vecLength = sqrt(
                         (mOrigDirection.x * mOrigDirection.x) +
                         (mOrigDirection.y * mOrigDirection.y)
                         );
  float dirX = mOrigDirection.x / vecLength;
  float dirY = mOrigDirection.y / vecLength;

  //apply transformation
  float newX = dirX * cos(rad) - dirY * sin(rad);
  float newY = dirX * sin(rad) + dirY * cos(rad);
  
  //round to 2 decimal points
  mDirection.x = roundf(newX * 100) / 100;
  mDirection.y = roundf(newY * 100) / 100;

}

void Ship::changeRotation(bool left)
{
  double rotationVal = mRotationValue;

  if(!left)
  {
    rotationVal = -mRotationValue;
  }
  
  rotateByDegree(rotationVal);

}

void Ship::updateBasedOnState()
{
  switch(curState)
  {
  case IDLE:
    //Do stuff
    break;
  case ROTATELEFT:
    changeRotation(true);
    break;
  case ROTATERIGHT:
    changeRotation(false);
    break; 
  default:
    cout << "ERROR: ShipState not Recognized";
    break;
  }

  changePosition();
}

void Ship::changePosition()
{  
  mPosition.x += mDirection.x * mSpeed;
  mPosition.y += mDirection.y * mSpeed; 
}

void Ship::changeSpeed(int speed)
{
  int newSpeed = mSpeed + speed;
  if(newSpeed < 0)
  {
    newSpeed = 0;
  }
  mSpeed = newSpeed;
}
