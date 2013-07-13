/**
 *
 * @file easyglut.h
 *
 **/



#ifndef __easyglut_h__
#define __easyglut_h__

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

float eglut_fps;
float eglut_frame = 0, eglut_timebase = 0;
float eglut_time;
long cur_fps = 1, old_fps = 0;
float eglut_FPS;
int originalWindow = 0, currentWindow;
bool isGlobalColor = false ;
int win_cursorX, win_cursorY;
float Set_time, Load_time = 0;

void NormalKeyInput (unsigned char key, int x, int y );
void ReleaseNormalKeyInput (unsigned char key, int x, int y);
void SpecialKeyInput (int key, int x, int y);
void ReleaseSpecialKeyInput (int key, int x, int y);
void ProcessMouse (int button, int state, int x, int y);
void CursorPassiveMotion (int x, int y);
void CursorActiveMotion (int x, int y);
void RenderScene (void);
void LoadImages ();
void WinProcessMouse ();

void Render ();
float GetFps ();

void LoadFPS (double set)
{
   Set_time = glutGet (GLUT_ELAPSED_TIME);
   while (1)
   {
      Set_time = glutGet (GLUT_ELAPSED_TIME);
      if (Set_time - Load_time > 950 / set)
      {
         Load_time = Set_time;
         break;
      }

   }
}
void CalculateFps ()
{
   eglut_frame++;
   eglut_time = glutGet (GLUT_ELAPSED_TIME);
   if (eglut_time - eglut_timebase > 1000)
   {
      eglut_fps = eglut_frame * 1000.0 / (eglut_time - eglut_timebase);
      eglut_timebase = eglut_time ;
      eglut_frame = 0;
   }
   eglut_FPS = eglut_fps;
}

float GetFps ()
{
   return eglut_FPS;
}


// GLUT_BITMAP_9_BY_15
// GLUT_BITMAP_8_BY_13
// GLUT_BITMAP_TIMES_ROMAN_10
// GLUT_BITMAP_TIMES_ROMAN_24
// GLUT_BITMAP_HELVETICA_10
// GLUT_BITMAP_HELVETICA_12
// GLUT_BITMAP_HELVETICA_18
void RenderBitmapText (
   float x,
   float y,
   void * font,
   char * string)
{
   char * c;


   glPushMatrix ();
   glDisable (GL_TEXTURE_2D);
   glRasterPos2d (x, y);
   for (c = string; *c != '\0'; c++)
   {
      glutBitmapCharacter (font, *c);
   }
   glPopMatrix ();
   glEnable (GL_TEXTURE_2D);
   if (!isGlobalColor)
   { glColor4ub (255, 255, 255, 255); }
}


//GLUT_STROKE_ROMAN
//GLUT_STROKE_MONO_ROMAN
void RenderStrokeText (
   float x,
   float y,
   void * font,
   char * string,
   float sizeX,
   float sizeY,
   float width)
{
   char * c;

   glPushMatrix ();
   glTranslatef (x, y, 0);
   glScalef (sizeX / 100, sizeY / 100, 0.00);
   glLineWidth (width);
   glEnable (GL_LINE_SMOOTH);
   for (c = string; *c != '\0'; c++)
   {
      glTranslatef (12, 0, 0);
      glutStrokeCharacter (font, *c);
   }
   glDisable (GL_LINE_SMOOTH);

   glLineWidth (1);
   glPopMatrix ();

   if (!isGlobalColor)
   { glColor4ub (255, 255, 255, 255); }
}


void RenderScene (void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
   Render ();
   glPopMatrix ();
   glutSwapBuffers ();

}

GLvoid changeSize (GLsizei w, GLsizei h)
{
   glViewport (0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (0.0 , w , 0.0 , h , -1.0 , 1.0) ;
//   gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.0,10000.0f);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
//   glTranslatef(-400,-400,-700);

}


void idle (void)
{
   WinProcessMouse ();

//   glutPostRedisplay ();
}

void visible (int vis)
{
   if (vis == GLUT_VISIBLE)
   {
      glutIdleFunc (idle);
   }
   else
   {
      glutIdleFunc (NULL);
   }
}
void InitScene ()
{
   LoadImages ();
   eglut_time = glutGet (GLUT_ELAPSED_TIME);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);     // Really Nice

}
void InitWindow ()
{
   InitScene ();
   glutIdleFunc (idle);
   glutKeyboardFunc (NormalKeyInput);
   glutKeyboardUpFunc (ReleaseNormalKeyInput);
   glutSpecialFunc (SpecialKeyInput);
   glutSpecialUpFunc (ReleaseSpecialKeyInput);
   glutDisplayFunc (RenderScene);
   glutMouseFunc (ProcessMouse);
   glutMotionFunc (CursorActiveMotion);
   glutPassiveMotionFunc (CursorPassiveMotion);
   glutVisibilityFunc (visible);
   glutReshapeFunc (changeSize);
}
void SetGlutWindow (int posX, int posY, int width, int height, char * title)
{

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (width, height);
   glutInitWindowPosition (posX, posY);

   originalWindow = glutCreateWindow (title);
   currentWindow = glutGetWindow ();
   InitWindow ();
   glutMainLoop ();

}


void SetRenderColor (unsigned char r, unsigned char g, unsigned char b,
                     unsigned char alpha )
{
   glColor4ub (r, g, b, alpha);
}


void CameraStart (int posX, int posY, int width, int height)
{

   glPushMatrix ();
   glEnable (GL_SCISSOR_TEST);
   glScissor (posX, posY, width, height);
   glTranslatef (posX, posY, 0);

}


void CameraZoom (float zoomX, float zoomY, float zoomPointX, float zoomPointY)
{

   glTranslatef (zoomPointX, zoomPointY, 0);
   glScalef (zoomX, zoomY, 0);
   glTranslatef (-zoomPointX, -zoomPointY, 0);

}

void CameraRotate (float angle, float pivotX, float pivotY)
{

   glTranslatef (pivotX, pivotY, 0);
   glRotatef (-angle, 0, 0, 1);
   glTranslatef (-pivotX, -pivotY, 0);

}

void CameraMove (float moveX, float moveY)
{
   glTranslatef (-moveX, -moveY, 0);
}

void CameraStop ()
{
   glPopMatrix ();
   glDisable (GL_SCISSOR_TEST);

}

void WinProcessMouse ()
{
   POINT point;
   GetCursorPos (&point);
   win_cursorX = (point.x - glutGet (GLUT_WINDOW_X) );
   win_cursorY = glutGet (GLUT_WINDOW_HEIGHT) - point.y + glutGet (GLUT_WINDOW_Y);
}

void SetBackGroundColor (unsigned char r, unsigned char g, unsigned char b,
                         unsigned char a)
{
   glClearColor (r / 255, g / 255, b / 255, a / 255);

}

bool DoIExist ( LPCTSTR szMutexName)
{
   HANDLE				g_hMutexAppIsRunning;
   g_hMutexAppIsRunning = CreateMutex ( NULL, FALSE,
                                        szMutexName );   // Return TRUE if existing semaphore opened

   if (g_hMutexAppIsRunning != NULL && GetLastError () == ERROR_ALREADY_EXISTS)
   {
      CloseHandle (g_hMutexAppIsRunning);
      return TRUE;
   }
   return (g_hMutexAppIsRunning == NULL);
}




void GetPixelColor (int cursorX, int cursorY, int rgb[3])
{
   GLubyte pixel[3];
   glReadPixels (cursorX, cursorY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *) pixel);
   rgb[0] = pixel[0];
   rgb[1] = pixel[1];
   rgb[2] = pixel[2];
}

void GlobalColor (bool isGlobal)
{
   isGlobalColor = isGlobal;
}
int GetWinCursorX ()
{
   return win_cursorX ;
}
int GetWinCursorY ()
{
   return win_cursorY ;
}

void glutLeaveFullScreen (int posX, int posY, int width, int height)
{

   glutReshapeWindow (width, height);
   glutPositionWindow (posX, posY);

}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __easyglut_h__
