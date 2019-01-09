#include "obj_texture.h"

Texture::Texture(SDL_Renderer *SRen, string imgLocation)
{

  mImgLocation = imgLocation;

  mRenderer = SRen;
  
  loadTexture();
}


void Texture::loadTexture()
{
  cout << "Loading: " << mImgLocation << "\n";
  
  //Make sure to initialize texture to null or else SDL_DestroyTexture will crash program
  mTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load( mImgLocation.c_str() );

  if( loadedSurface == NULL )
    {
      printf( "Unable to load image %s! SDL_image Error: %s\n",
              mImgLocation.c_str(),
              IMG_GetError() );
    }
  else
    {
      //Convert surface to display format
      SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface,
                                                                SDL_PIXELFORMAT_ARGB8888,
                                                                0 );
      if( formattedSurface == NULL )
        {
          printf( "Unable to convert loaded surface to display format! SDL Error: %s\n",
                  SDL_GetError() );
        }
      else
        {
          //Create blank streamable texture
          mTexture = SDL_CreateTexture( mRenderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        formattedSurface->w,
                                        formattedSurface->h );
          if( mTexture == NULL )
            {
              printf( "Unable to create blank texture! SDL Error: %s\n",
                      SDL_GetError() );
            }
          else
            {

              //Lock texture for manipulation
              SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch );

              //Copy loaded/formatted surface pixels
              memcpy( mPixels, formattedSurface->pixels,
                      formattedSurface->pitch * formattedSurface->h );

              //Unlock texture to update
              SDL_UnlockTexture( mTexture );
              mPixels = NULL;

              //Get image dimensions
              mWidth = formattedSurface->w;
              mHeight = formattedSurface->h;

              cout << "image is w:" << mWidth << " h:" << mHeight << "\n";
            }

          //Get rid of old formatted surface
          SDL_FreeSurface( formattedSurface );
        }

      //Get rid of old loaded surface

      //This block of code saves the surface as a png to location
      //const char * location = "res/png/out.png";
      //IMG_SavePNG(loadedSurface, location);

      SDL_FreeSurface( loadedSurface );

      cout << "Finish Loading: " << mImgLocation << "\n";
    }
}
