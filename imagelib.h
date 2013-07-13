/**
 *
 * @file imagelib.h
 *
 **/

#ifndef __imagelib_h__
#define __imagelib_h__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#define bool BOOL
#define false 0
#define true 1
#endif
#include <windows.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include "gl/glut.h"

#include "SOIL.h"
#pragma comment(lib,"SOIL.lib")

#define FILE_NOT_FOUND 0
#define IMAGE_LOADING_DONE 1
#define DUPLICATE_IMAGE_ID 2

#define BITMAP_ID   0x4D42
#define MAX_IMAGE_NUM 100

bool flipX = false, flipY = false ;
float RotPivotX = 0, RotPivotY = 0;
float eglut_Angle = 0 ;
float textureX = 100, textureY = 100;
extern bool isGlobalColor;
extern int win_cursorX, win_cursorY;
float eglut_x1, eglut_x2, eglut_y1, eglut_y2;
unsigned int texture[MAX_IMAGE_NUM];

unsigned char * LoadBitmapFile (char * filename,
                                BITMAPINFOHEADER * bitmapInfoHeader)
{
   FILE * filePtr;                       // the file pointer
   BITMAPFILEHEADER  bitmapFileHeader;   // bitmap file header
   unsigned char  *  bitmapImage;        // bitmap image data
   unsigned int      imageIdx = 0;       // image index counter
   unsigned char     tempRGB;            // swap variable

   // open filename in "read binary" mode
   filePtr = fopen (filename, "rb");
   if (filePtr == NULL)
   {
      printf ("Error : Unable to Open File %s\n", filename);
      return FILE_NOT_FOUND;
   }

   // read the bitmap file header
   fread (&bitmapFileHeader, sizeof (BITMAPFILEHEADER), 1, filePtr);

   // verify that this is a bitmap by checking for the universal bitmap id
   if (bitmapFileHeader.bfType != BITMAP_ID)
   {
      fclose (filePtr);
      return NULL;
   }

   // read the bitmap information header
   fread (bitmapInfoHeader, sizeof (BITMAPINFOHEADER), 1, filePtr);

   // move file pointer to beginning of bitmap data
   fseek (filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

   // allocate enough memory for the bitmap image data
   bitmapImage = (unsigned char *)malloc (bitmapInfoHeader->biSizeImage);

   // verify memory allocation
   if (!bitmapImage)
   {
      free (bitmapImage);
      fclose (filePtr);
      return NULL;
   }

   // read in the bitmap image data
   fread (bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

   // make sure bitmap image data was read
   if (bitmapImage == NULL)
   {
      fclose (filePtr);
      return NULL;
   }

   // swap the R and B values to get RGB since the bitmap color format is in BGR
   for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
   {
      tempRGB = bitmapImage[imageIdx];
      bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
      bitmapImage[imageIdx + 2] = tempRGB;
   }
   fclose (filePtr);
   // fclose ( fp ) ;
   return bitmapImage;
} // end LoadBitmapFile()


/*****************************************************************************
 LoadBitmapFileWithAlpha

 Loads a bitmap file normally, and then adds an alpha component to use for
 blending
*****************************************************************************/
unsigned char * LoadBitmapFileWithAlpha (
   char * filename,
   BITMAPINFOHEADER * bitmapInfoHeader,
   unsigned char r,
   unsigned char g,
   unsigned char b)
{
   unsigned char * bitmapImage = LoadBitmapFile (filename, bitmapInfoHeader);
   unsigned char * bitmapWithAlpha = (unsigned char *)malloc (
                                        bitmapInfoHeader->biSizeImage * 4 / 3);
   unsigned int src = 0, dst = 0 ;
   if (bitmapImage == NULL || bitmapWithAlpha == NULL)
   {
      return NULL;
   }

   // loop through the bitmap data
   for (; src < bitmapInfoHeader->biSizeImage; src += 3, dst += 4)
   {
      // if the pixel is black, set the alpha to 0. Otherwise, set it to 255.

      if (bitmapImage[src] == r &&
          bitmapImage[src + 1] == g
          && bitmapImage[src + 2] == b)
      {
         bitmapWithAlpha[dst + 3] = 0 ;
      }
      else
      {
         bitmapWithAlpha[dst + 3] = 0xFF;
      }

      // copy pixel data over
      bitmapWithAlpha[dst] = bitmapImage[src];
      bitmapWithAlpha[dst + 1] = bitmapImage[src + 1];
      bitmapWithAlpha[dst + 2] = bitmapImage[src + 2];
   }

   free (bitmapImage);

   return bitmapWithAlpha;
}
// Function to load bmp image into memory with one exception, user can provide
// a pixel color and alpha value of that colored pixel will be set to 0, thus
// a purticuler color of pixels can be removed while rendering
// 1st parameter is image id, it can be any integer number less than 100
//     becasue the number of image can be loaded is 100, if more than that is needed
//     just change the MAX_IMAGE_NUM in imagelib.h file,
// 2nd parameter is the path to your image file
// 3rd parameter is a bool value, if it is false then any image with a duplicate
//     image_id wont be loaded, if true, previously loaded image will be overridden
// 4th, 5th and 6th parameter defined the r,g,b values of transparent pixel, these values should be
//      between 0 to 255
bool LoadBmpWithAlpha (int image_id,
                       char * name,
                       bool replace_duplicate,
                       unsigned char r,
                       unsigned char g,
                       unsigned char b)
{
   unsigned char * buffer;
   BITMAPINFOHEADER bitmapInfoHeader;
   if ( 0 != texture[image_id])
   {
      printf ("Another image is already loaded in this image_id, thus not loading the image \n");
      return false ;
   }

   buffer = LoadBitmapFileWithAlpha (name, &bitmapInfoHeader, r, g, b);
   if (buffer == NULL)
   {
      return false;
   }
   glGenTextures (1,
                  &texture[image_id]);         // Typical Texture Generation Using Data From The Bitmap
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   glBindTexture (GL_TEXTURE_2D, texture[image_id]);

   glTexImage2D (GL_TEXTURE_2D, 0, 4, bitmapInfoHeader.biWidth,
                 bitmapInfoHeader.biHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, buffer);
   // set up the texture
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   free (buffer);

   return true;

}

// Function to load images into memory, supported image formats are bmp,jpg,tga,png and dds
// 1st parameter is image id, it can be any integer number less than 100. this number
//     should be unique for each image and this number will later be used to render that image
//     the highest number of image can be loaded is 100, if more than that is needed
//     just change the MAX_IMAGE_NUM in imagelib.h file,
// 2nd parameter is the  name and relative path to your image file
// 3rd parameter is a bool value, if it is false then any image with a duplicate
//     image_id wont be loaded, and an error message will be shown
//     if true, previously loaded image will be overridden
int LoadTexture (int image_id,
                 char * name,
                 bool replace_duplicate)
{
   if ( 0 != texture[image_id] )
   {
      if (!replace_duplicate)
      {
         printf ("Error : Another image is already loaded in this image_id,not loading %s \n",
                 name);
         return DUPLICATE_IMAGE_ID;
      }
      else
      {
         printf ("Warning : Another image is already loaded in this image_id, overriding  %s \n",
                 name);
      }
   }
   texture[image_id] = SOIL_load_OGL_texture
                       (
                          name,
                          4,
                          SOIL_CREATE_NEW_ID,
                          SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y);
   if ( 0 == texture[image_id] )
   {
      printf ( "Error : %s %s\n", SOIL_last_result (), name);
      return false;
   }
   else
   {
      return true;
   }
}

void BindTexture (int image_id)
{
   glBindTexture (GL_TEXTURE_2D, texture[image_id]);
}


void SetImageSmooth (int image_id, bool isSmooth)
{
   glBindTexture (GL_TEXTURE_2D, texture[image_id]);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   if (isSmooth)
   {
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   }

}

// Image manipulating functions
void FxRotPivot (float pivotX, float pivotY)
{
   RotPivotX = pivotX;
   RotPivotY = pivotY;
}
void FxAngle (float angle)
{
   eglut_Angle = angle;
}
void FxImagePercent (float x, float y)
{
   textureX = x;
   textureY = y;
}

void FxImageFlip (bool X, bool Y)
{
   flipX = X;
   flipY = Y;
}
// End of Image manipulating functions


void RenderImage (int   image_id,
                  float posX,
                  float posY,
                  float sizeX,
                  float sizeY)
{
   glEnable (GL_TEXTURE_2D);
   BindTexture (image_id);
   glEnable(GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin (GL_QUADS);           // Draw A Quad
   glTexCoord2f (0, 0);
   glVertex2f (posX, posY);
   glTexCoord2f (1, 0);
   glVertex2f (posX + sizeX, posY);
   glTexCoord2f (1, 1);
   glVertex2f (sizeX + posX, sizeY + posY);
   glTexCoord2f (0, 1);
   glVertex2f (posX, sizeY + posY);
   glEnd ();
   if (!isGlobalColor)
   {
      glColor4ub (255, 255, 255, 255);
   }
   glDisable (GL_TEXTURE_2D);
}

void FxRenderImage (int   image_id,
                    float posX,
                    float posY,
                    float sizeX,
                    float sizeY)
{
   glEnable (GL_TEXTURE_2D);
   glPushMatrix ();
   glEnable(GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glTranslatef (posX, posY, 0);

   glRotatef (180, flipX, 0, 0);
   glRotatef (180, 0, flipY, 0);
   glTranslatef (-sizeX * flipX, -sizeY * flipY, 0);
   glTranslatef (RotPivotX, RotPivotY, 0);
   glRotatef (eglut_Angle, 0, 0, 1);
   glTranslatef (-RotPivotX, -RotPivotY, 0);

   BindTexture (image_id);

   glBegin (GL_QUADS);           // Draw A Quad
   glTexCoord2f (0, 0);
   glVertex2f (0, 0);
   glTexCoord2f (textureX / 100, 0);
   glVertex2f (0 + sizeX, 0);
   glTexCoord2f (textureX / 100, textureY / 100);
   glVertex2f (sizeX + 0, sizeY + 0);
   glTexCoord2f (0, textureY / 100);
   glVertex2f (0, sizeY + 0);
   glEnd ();

   glPopMatrix ();

   textureX = textureY = 100;
   flipX = flipY = 0;

   eglut_Angle = RotPivotX = RotPivotY = 0;
   if (!isGlobalColor)
   {
      glColor4ub (255, 255, 255, 255);
   }

   glDisable (GL_TEXTURE_2D);
}
void RenderSprite (int   image_id,
                   float posX,
                   float posY,
                   float sizeX,
                   float sizeY,
                   float rowNum,
                   float columnNum,
                   float frameNumX,
                   float frameNumY)

{

   glEnable (GL_TEXTURE_2D);

   frameNumX = rowNum + 1 - frameNumX;

   eglut_x1 = ((frameNumY - 1) * ((1 / columnNum)));
   eglut_y1 = ((frameNumX - 1) * ((1 / rowNum)));
   eglut_x2 = ((frameNumY) * ((1 / columnNum)));
   eglut_y2 = ((frameNumX) * ((1 / rowNum)));

   BindTexture (image_id);
   glBegin (GL_QUADS);           // Draw A Quad
   glTexCoord2f (eglut_x1, eglut_y1);
   glVertex2f (posX, posY);
   glTexCoord2f (eglut_x2, eglut_y1);
   glVertex2f (posX + sizeX, +posY);
   glTexCoord2f (eglut_x2, eglut_y2);
   glVertex2f (sizeX + posX, sizeY + posY);
   glTexCoord2f (eglut_x1, eglut_y2);
   glVertex2f (posX, sizeY + posY);
   glEnd ();

   if (!isGlobalColor)
   {
      glColor4ub (255, 255, 255, 255);
   }

   glDisable (GL_TEXTURE_2D);
}

void FxRenderSprite (int   image_id,
                     float posX,
                     float posY,
                     float sizeX,
                     float sizeY,
                     float rowNum,
                     float columnNum,
                     float frameNumX,
                     float frameNumY)
{
   glEnable (GL_TEXTURE_2D);
   frameNumX = rowNum + 1 - frameNumX;

   eglut_x1 = ((frameNumY - 1) * ((1 / columnNum)));
   eglut_y1 = ((frameNumX - 1) * ((1 / rowNum)));
   eglut_x2 = ((frameNumY) * ((1 / columnNum)));
   eglut_y2 = ((frameNumX) * ((1 / rowNum)));

   glPushMatrix ();

   glTranslatef (posX, posY, 0);

   glRotatef (180, flipX, 0, 0);
   glRotatef (180, 0, flipY, 0);
   glTranslatef (-sizeX * flipX, -sizeY * flipY, 0);
   glTranslatef (RotPivotX, RotPivotY, 0);
   glRotatef (eglut_Angle, 0, 0, 1);

   glTranslatef (-RotPivotX, -RotPivotY, 0);



   BindTexture (image_id);
   glBegin (GL_QUADS);           // Draw A Quad
   glTexCoord2f (eglut_x1 * (textureX / 100), eglut_y1 * (textureY / 100));
   glVertex2f (0, 0);
   glTexCoord2f (eglut_x2 * (textureX / 100), eglut_y1 * (textureY / 100));
   glVertex2f (0 + sizeX, 0);
   glTexCoord2f (eglut_x2 * (textureX / 100), eglut_y2 * (textureY / 100));
   glVertex2f (sizeX, sizeY);
   glTexCoord2f (eglut_x1 * (textureX / 100), eglut_y2 * (textureY / 100));
   glVertex2f (0, sizeY);
   glEnd ();

   glPopMatrix ();

   flipX = flipY = 0;

   textureX = textureY = 100;
   eglut_Angle = RotPivotX = RotPivotY = 0;

   if (!isGlobalColor)
   {
      glColor4ub (255, 255, 255, 255);
   }

   glDisable (GL_TEXTURE_2D);

}

void RenderCursor (int image_id,
                   float sizeX,
                   float sizeY,
                   float hotSpotX,
                   float hotSpotY)
{


   RenderImage (image_id, win_cursorX - hotSpotX, win_cursorY - hotSpotY, sizeX,
                sizeY);
}


void FxRenderCursor (int image_id,
                     float sizeX,
                     float sizeY,
                     float hotSpotX,
                     float hotSpotY)
{
   FxRenderImage (image_id, win_cursorX - hotSpotX, win_cursorY - hotSpotY, sizeX,
                  sizeY);
}


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __imagelib_h__
