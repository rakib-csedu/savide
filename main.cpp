
#include "easyglut.h"
#include "bodies.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include "agent.h"
#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif
#include "Box2d/Box2D.h"
#include <string.h>
#include "Render.h"
#include "fstream"

char * readFile (char * fileName);
b2World myWorld (b2Vec2 (0, 0) );
DebugDraw draw;
b2Vec2 destination = b2Vec2 (0, 0);
float rayLength = 65; //long enough to hit the walls
Agent * agent;


enum
{
   TIRE = 100,
   LEFT_SIDE = 0,
   RIGHT_SIDE = 1
};
// LKP = Left arrow Key Press
// RKP = Right arrow Key Press
// UKP = Up arrow Key Press
// DKP = Down arrow Key Press
// LMB = Left Mouse Button
// RMB = Right Mouse Button
// MMB = Middle Mouse Button
bool LKP, RKP, UKP, DKP, LMB, RMB, MMB;

bool fullscreen;
char map[30][50];
char fps[10];

char carCollisionMap[41][41] = {0};

bool emptyRayCast[3];

void NormalKeyInput (unsigned char key, int x, int y)
{
   switch (key)
   {
      case 27:
         exit (0);

      case 'p':
         break;
      case 61:
         if (rayLength < 100)
         { rayLength = rayLength + 1; }
         break;
      case '-':
         if (rayLength > 1)
         { rayLength = rayLength - 1; }
         break;
      default:
         break;
   }
}
void ReleaseNormalKeyInput (unsigned char key, int x, int y)
{
   printf (" input %d\n", key );
   switch (key)
   {
      case 'w':
         break;
      case 'f':

         break;
      case 'a':

         break;
      case 'r':
         for ( b2Body * b = myWorld.GetBodyList (); b; b = b->GetNext () )
         {
            myWorld.DestroyBody (b);
         }
         delete agent;

         agent = new Agent (myWorld);
         readFile ("map.txt");
         break;

      case '1':
         break;
      default:
         break;
   }
}
void SpecialKeyInput (int key, int x, int y)
{
   switch (key)
   {
      case GLUT_KEY_LEFT:
         LKP = 1;
         break;
      case GLUT_KEY_RIGHT:
         RKP = 1;
         break;
      case GLUT_KEY_UP:
         UKP = 1;
         break;
      case GLUT_KEY_DOWN:
         DKP = 1;
         break;
      default:
         break;
   }
}
void ReleaseSpecialKeyInput (int key, int x, int y)
{

   switch (key)
   {

// pressing F1 key makes the prpgram fullscreen, press F1 again to get back in windowed mode
      case GLUT_KEY_F1:
         fullscreen = !fullscreen;
         if (fullscreen)
         {
            glutFullScreen ();

         }
         else
         {
            glutLeaveFullScreen (glutGet (GLUT_INIT_WINDOW_X),
                                 glutGet (GLUT_INIT_WINDOW_Y),
                                 glutGet (GLUT_INIT_WINDOW_WIDTH),
                                 glutGet (GLUT_INIT_WINDOW_HEIGHT) );
         }
         break;

      case GLUT_KEY_F2:

         break;
      case GLUT_KEY_F3:

         break;
      case GLUT_KEY_RIGHT:

         RKP = false;

         break;
      case GLUT_KEY_LEFT:
         LKP = false;

         break;
      case GLUT_KEY_UP:
         UKP = false;
         break;
      case GLUT_KEY_DOWN:

         DKP = false;
         break;
      default:
         break;
   }
}
void ProcessMouse (int button, int state, int x, int y)
{
   if (state == GLUT_DOWN)
   {
      printf ("%d %d\n", x, y);
      if (button == GLUT_LEFT_BUTTON)
      {
         //hides the cursor
         if (glutGetModifiers () == GLUT_ACTIVE_CTRL)
         {
            createBody (myWorld, b2_staticBody, (x / 16) * 16 , ( (600 - y) / 10) * 10 , 10, 10, false);
         }
         if (glutGetModifiers () == GLUT_ACTIVE_ALT)
         {
            destination = b2Vec2 (x, 600 - y);
         }
//         glutSetCursor (GLUT_CURSOR_NONE);
         LMB = true;
      }
      else if (button == GLUT_MIDDLE_BUTTON)
      {
         MMB = true;
      }
      else if (button == GLUT_RIGHT_BUTTON)
      {
         RMB = true;
      }
   }
   else if ( state == GLUT_UP )
   {
      if ( button == GLUT_LEFT_BUTTON )
      {
         LMB = false;
      }
      else if ( button == GLUT_MIDDLE_BUTTON )
      {
         MMB = false;
         if (glutGetModifiers () == GLUT_ACTIVE_CTRL)
         {
            b2Body * temp;
            temp = createBody (myWorld, b2_dynamicBody, (x / 16) * 16 , ( (600 - y) / 10) * 10 , 10, 10, true);
            temp->ApplyLinearImpulse (b2Vec2 (-50000000, 0), b2Vec2 () );
         }
      }
      else if ( button == GLUT_RIGHT_BUTTON )
      {
         if (glutGetModifiers () == GLUT_ACTIVE_CTRL)
         {

            b2Body * temp;
            temp = createBody (myWorld, b2_dynamicBody, (x / 16) * 16 , ( (600 - y) / 10) * 10 , 10, 10, true);
            temp->ApplyLinearImpulse (b2Vec2 (0, 50000000), b2Vec2 () );
         }
         RMB = false;
      }
   }

}
void CursorActiveMotion (int x, int y)
{
// This makes the cursor (x,y) relative to the lower left corner of the glut
// window as in opengl
   y = glutGet (GLUT_INIT_WINDOW_HEIGHT) - y;
}
void CursorPassiveMotion (int x, int y)
{
   y = glutGet (GLUT_INIT_WINDOW_HEIGHT) - y;
}

void Update (int timer)
{
   float32 timeStep = 1 /
                      60.0f;    //the length of time passed to simulate (seconds)
   int32 velocityIterations = 6;   //how strongly to correct velocity
   int32 positionIterations = 3;   //how strongly to correct position

   myWorld.Step ( timeStep, velocityIterations, positionIterations);
   for ( b2Body * b = myWorld.GetBodyList (); b; b = b->GetNext () )
   {
      //do something with the body 'b'
      if (b->GetUserData () == 0)
      { break; }
   }
   glutPostRedisplay ();
   CalculateFps ();
   glutTimerFunc (1000.0f / 60.0f , Update, 1);
}
void Clear (int count)
{
   memset (carCollisionMap, 0, sizeof (carCollisionMap) );
   agent->nearestAngle[0] = 1000.0f;
   agent->nearestAngle[1] = 1000.0f;
   agent->reverseMove = false;
   if (agent->frontObstacle == true)
      if (agent->body->GetLinearVelocity ().x < 0.5 && agent->body->GetLinearVelocity ().y < 0.5)
      {
         agent->reverseMove = true;
      }
   agent->frontObstacle = false ;
   agent->foundNearestAngle  = true;
   glutTimerFunc (1000  , Clear, 1);
}
// Load your images here
void LoadImages ()
{
   myWorld.SetDebugDraw (&draw);
   draw.SetFlags ( DebugDraw::e_shapeBit | DebugDraw::e_aabbBit |
                   DebugDraw::e_jointBit | DebugDraw::e_centerOfMassBit | DebugDraw::e_pairBit);

   glutTimerFunc (1000 / 100.0, Update, 1);
   glutTimerFunc (2000 / 1000, Clear, 1);

}
// Your rendering functions all kinds of rendering functions needed to be called here
// custom opengl rendering functions should be called here
float converToDegree (float radian)
{
   return (int) (fabs (radian + 1.57)  / DEGTORAD) % 360;
}
void turnToDestination ()
{
//   if (agent->frontObstacle == 0)
   if (destination.x != 0)
   {
      if (agent->foundNearestAngle)
      {
         static float temp = glutGet (GLUT_ELAPSED_TIME);
         if (glutGet (GLUT_ELAPSED_TIME) - temp > 1000)
         {
            float deltaX = destination.x - agent->body->GetPosition ().x;
            float deltaY = destination.y - agent->body->GetPosition ().y;

            float tempAngle = atan2 (deltaX, deltaY);
            agent->angle = tempAngle - 1.57;

            temp = glutGet (GLUT_ELAPSED_TIME);
            agent->foundNearestAngle = false;
         }
      }
   }
}
void turnToNearestAngle ()
{

//  if there is any obstalce in front of the vehicle then, look for the nearest safe angle
//  and change vehicle angle to that angle

   if (agent->frontObstacle)
   {
      static float timerF = glutGet (GLUT_ELAPSED_TIME) ;
      if (glutGet (GLUT_ELAPSED_TIME) - timerF > 500)
         if (agent->nearestAngle[0] != 1000 || agent->nearestAngle[1] != 1000)
         {
            if (agent->nearestAngle[LEFT_SIDE] < agent->nearestAngle[RIGHT_SIDE])
            {
               printf ("Left side wins %f\n", agent->nearestAngle[LEFT_SIDE]);
               agent->angle = agent->angle - .5;
            }
            else
            {
               printf ("Right side wins %f\n", agent->nearestAngle[RIGHT_SIDE]);

               agent->angle = agent->angle + .5;
            }
            timerF = glutGet (GLUT_ELAPSED_TIME);
         }
   }
}
void Render ()
{
   LoadFPS (60);
   glClearColor (1, 1, 1, 1);
   srand (time (NULL) );
   //at global scope
   static float currentRayAngle = 0;
   currentRayAngle += 6;
   //calculate points of ray


   b2Vec2 p1 ( b2Vec2 (agent->body->GetPosition ().x,
                       agent->body->GetPosition ().y) ); //center of scene
   b2Vec2 p2 = p1 + rayLength * b2Vec2 (sinf (DEGTORAD * (currentRayAngle  ) ),
                                        cosf ( DEGTORAD * (currentRayAngle ) ) );

   b2Vec2 p3 ( b2Vec2 (agent->body->GetPosition ().x,
                       agent->body->GetPosition ().y) );
   b2Vec2 p4 = p3 + rayLength * b2Vec2 (sinf (DEGTORAD * (currentRayAngle + 90) ),
                                        cosf ( DEGTORAD * (currentRayAngle + 90) ) );

   b2Vec2 p5 ( b2Vec2 (agent->body->GetPosition ().x,
                       agent->body->GetPosition ().y) ); //center of scene
   b2Vec2 p6 = p5 + rayLength * b2Vec2 (sinf (DEGTORAD * (currentRayAngle + 180) ),
                                        cosf ( DEGTORAD * (currentRayAngle + 180) ) );

   b2Vec2 p7 ( b2Vec2 (agent->body->GetPosition ().x,
                       agent->body->GetPosition ().y) );
   b2Vec2 p8 = p7 + rayLength * b2Vec2 (sinf (DEGTORAD * (currentRayAngle + 270 ) ),
                                        cosf ( DEGTORAD * (currentRayAngle + 270 ) ) );
   b2Vec2 p9 =  b2Vec2 (agent->body->GetPosition ().x ,
                        agent->body->GetPosition ().y  - 5 ) ;
   b2Vec2 p10 = b2Vec2 (agent->body->GetPosition ().x ,
                        agent->body->GetPosition ().y - 5 )  + rayLength / 2.0 * b2Vec2 (sinf ( (
                                 agent->angle + 1.57) ),
                              cosf ( ( agent->angle + 1.57  ) ) );

   b2Vec2 p11 =  b2Vec2 (agent->body->GetPosition ().x ,
                         agent->body->GetPosition ().y + 5  ) ;
   b2Vec2 p12 = b2Vec2 (agent->body->GetPosition ().x,
                        agent->body->GetPosition ().y + 5   )  + rayLength / 2.0  * b2Vec2 (sinf ( (
                                 agent->angle + 1.57) ),
                              cosf ( ( agent->angle + 1.57 ) ) );

   b2RayCastInput input;
   input.p1 = p1;
   input.p2 = p2;
   input.maxFraction = rayLength / 50.0;

   b2RayCastInput input2;
   input2.p1 = p3;
   input2.p2 = p4;
   input2.maxFraction = rayLength / 50.0;

   b2RayCastInput input3;
   input3.p1 = p5;
   input3.p2 = p6;
   input3.maxFraction = rayLength / 50.0 ;

   b2RayCastInput input4;
   input4.p1 = p7;
   input4.p2 = p8;
   input4.maxFraction = rayLength / 50.0;

   b2RayCastInput input5;
   input5.p1 = p9;
   input5.p2 = p10;
   input5.maxFraction = rayLength / 50.0;

   b2RayCastInput input6;
   input6.p1 = p11;
   input6.p2 = p12;
   input6.maxFraction = rayLength / 50.0;

   //check every fixture of every body to find closest
   float closestFraction = rayLength / 50.0; //start with end of line as p2
   float closestFraction2 = rayLength / 50.0 ;
   float closestFraction3 =  rayLength / 50.0;
   float closestFraction4 =  rayLength / 50.0;
   float closestFraction5 = rayLength / 50.0;
   float closestFraction6 = rayLength / 50.0;
   static int timer = 0;
   timer++;
   b2Vec2 intersectionNormal (0, 0);

   bool isObstacle[5] = {0} ;
   static int freeAngle[5] = {0};
   freeAngle[1] = agent->angle;
   static int count = 0 ;
   /*
    * Raycasting is done here, there are 6 ray here
   */
   for (b2Body * b = myWorld.GetBodyList (); b; b = b->GetNext () )
   {
      if (100 == (int) b->GetUserData () )
      { break; }
      for (b2Fixture * f = b->GetFixtureList (); f; f = f->GetNext () )
      {
         b2RayCastOutput output;

         if ( f->RayCast ( &output, input, 0 ) )
         {
            if ( output.fraction < closestFraction )
            {
               isObstacle[1] = 1;

               count = 0 ;
               closestFraction = output.fraction;
               intersectionNormal = output.normal;
               b2Vec2 intersectionPoint = p1 + closestFraction * (p2 - p1);
               b2Vec2 diff1  =  agent->body->GetPosition () - intersectionPoint;

               int x = diff1.x / (800 / 80.0) ;
               int y = diff1.y / (600 / 60.0) ;
               carCollisionMap[20 - y][20 - x] = 1;
            }
         }

         b2RayCastOutput output2;
         if (f->RayCast (&output2, input2, 0) )
         {
            if (output2.fraction < closestFraction)
            {
               isObstacle[2] = 1;
               closestFraction2 = output2.fraction;
               b2Vec2 intersectionPoint2 = p3 + closestFraction2 * (p4 - p3);

               b2Vec2 diff2 =  agent->body->GetPosition () - intersectionPoint2;
               int x = diff2.x / (800 / 80.0) ;
               int y = diff2.y / (600 / 60.0) ;
               carCollisionMap[20 - y][20 - x] = 1;
            }

         }
         b2RayCastOutput output3;
         if (f->RayCast (&output3, input3, 0) )
         {
            if (output3.fraction < closestFraction3)
            {
               isObstacle[3] = 1;
               closestFraction3 = output3.fraction;
               b2Vec2 intersectionPoint3 = p5 + closestFraction3 * (p6 - p5);

               b2Vec2 diff2 =  agent->body->GetPosition () - intersectionPoint3;
               int x = diff2.x / (800 / 80.0) ;
               int y = diff2.y / (600 / 60.0) ;
               carCollisionMap[20 - y][20 - x] = 1;
            }
         }

         b2RayCastOutput output4;
         if (f->RayCast (&output4, input4, 0) )
         {
            if (output4.fraction < closestFraction4)
            {
               isObstacle[4] = 1;
               closestFraction4 = output4.fraction;
               b2Vec2 intersectionPoint4 = p7 + closestFraction4 * (p8 - p7);

               b2Vec2 diff2 =  agent->body->GetPosition () - intersectionPoint4;
               int x = diff2.x / (800 / 80.0) ;
               int y = diff2.y / (600 / 60.0) ;
               carCollisionMap[20 - y][20 - x] = 1;
            }
         }

         b2RayCastOutput output5;
         if (f->RayCast (&output5, input5, 0) )
         {
            if (output5.fraction < closestFraction5)
            {
               closestFraction5 = output5.fraction;
               b2Vec2 intersectionPoint5 = p9 + closestFraction5 * (p10 - p9);

               b2Vec2 diff2 =  agent->body->GetPosition () - intersectionPoint5;
               int x = diff2.x / (800 / 80.0) ;
               int y = diff2.y / (600 / 60.0) ;
               carCollisionMap[20 - y][20 - x] = 1;
               agent->frontObstacle = 1;
            }
         }
         b2RayCastOutput output6;
         if (f->RayCast (&output6, input6, 0) )
         {
            if (output6.fraction < closestFraction6)
            {
               closestFraction6 = output6.fraction;
               b2Vec2 intersectionPoint6 = p11 + closestFraction6 * (p12 - p11);

               b2Vec2 diff2 =  agent->body->GetPosition () - intersectionPoint6;
               int x = diff2.x / (800 / 80.0) ;
               int y = diff2.y / (600 / 60.0) ;
               carCollisionMap[20 - y][20 - x] = 1;
               agent->frontObstacle = 1;
            }
         }
      }
   }

   static float timetem= glutGet(GLUT_ELAPSED_TIME);
   if(glutGet(GLUT_ELAPSED_TIME)-timetem > 1000)
   for (int i = 1 ; i <= 4; ++i)
   {
      if (isObstacle[i] == false)
      {
         float tempAngle;
         tempAngle = (int) (currentRayAngle + 90 * (i - 1) ) % 360;

         if (tempAngle >= converToDegree (agent->angle) && tempAngle <= (int) (converToDegree (agent->angle) + 180) % 360)
         {
            if (agent->nearestAngle[RIGHT_SIDE] > tempAngle - converToDegree (agent->angle) )
            {
               agent->nearestAngle[RIGHT_SIDE] = tempAngle - converToDegree (agent->angle) ;

            }
            else
            { isObstacle[i] = true; }
         }
         else
         {
            if (agent->nearestAngle[LEFT_SIDE] > (360 - tempAngle) - converToDegree (agent->angle) )
            { agent->nearestAngle[LEFT_SIDE] = fabs ( (360 - tempAngle) - converToDegree (agent->angle) ); }
            else
            { isObstacle[i] = true; }
         }

      }
      timetem= glutGet(GLUT_ELAPSED_TIME);
   }


// if no obstacle is infront of behicle and destination is set, then head for the destination
// this is done by changing vehicle angle to face the destination



   b2Vec2 intersectionPoint  = p1  + closestFraction  * (p2 - p1);
   b2Vec2 intersectionPoint2 = p3  + closestFraction2 * (p4 - p3);
   b2Vec2 intersectionPoint3 = p5  + closestFraction3 * (p6 - p5);
   b2Vec2 intersectionPoint4 = p7  + closestFraction4 * (p8 - p7);
   b2Vec2 intersectionPoint5 = p9  + closestFraction4 * (p10 - p9);
   b2Vec2 intersectionPoint6 = p11 + closestFraction4 * (p12 - p11);

   //draw a line
   glColor3f (255, 0, 0); //white
   glBegin (GL_LINES);
   glVertex2f ( p1.x, p1.y );
   glVertex2f ( intersectionPoint.x, intersectionPoint.y );
   glEnd ();

   SetRenderColor (0, 0, 0, 255);
   if (isObstacle[1] == false)
   { SetRenderColor (255, 0, 255, 255); }
   //draw a point at the intersection point
   glPointSize (10);

   glBegin (GL_POINTS);
   glVertex2f ( intersectionPoint.x, intersectionPoint.y );
   glEnd ();

   SetRenderColor (255, 34, 25, 255);
   glBegin (GL_POINTS);
   glVertex2f ( destination.x, destination.y );
   glEnd ();

   glBegin (GL_LINES);
   glVertex2f ( p3.x, p3.y );
   glVertex2f ( intersectionPoint2.x, intersectionPoint2.y );
   glEnd ();

   glBegin (GL_LINES);
   glVertex2f ( p5.x, p5.y );
   glVertex2f ( intersectionPoint3.x, intersectionPoint3.y );
   glEnd ();

   glBegin (GL_LINES);
   glVertex2f ( p7.x, p7.y );
   glVertex2f ( intersectionPoint4.x, intersectionPoint4.y );
   glEnd ();

   glBegin (GL_LINES);
   glVertex2f ( p9.x, p9.y );
   glVertex2f ( intersectionPoint5.x, intersectionPoint5.y );
   glEnd ();

   glBegin (GL_LINES);
   glVertex2f ( p11.x, p11.y );
   glVertex2f ( intersectionPoint6.x, intersectionPoint6.y );
   glEnd ();

   SetRenderColor (0, 0, 0, 255);
   if (isObstacle[2] == false)
   { SetRenderColor (255, 0, 255, 255); }
   glPointSize (10);
   glBegin (GL_POINTS);
   glVertex2f ( intersectionPoint2.x, intersectionPoint2.y );
   glEnd ();


   SetRenderColor (0, 0, 0, 255);
   if (isObstacle[3] == false)
   { SetRenderColor (255, 0, 255, 255); }
   glBegin (GL_POINTS);
   glVertex2f ( intersectionPoint3.x, intersectionPoint3.y );
   glEnd ();


   SetRenderColor (0, 0, 0, 255);
   if (isObstacle[4] == false)
   { SetRenderColor (255, 0, 255, 255); }

   glBegin (GL_POINTS);
   glVertex2f ( intersectionPoint4.x, intersectionPoint4.y );
   glEnd ();

   glBegin (GL_LINES);
   glVertex2f ( agent->body->GetPosition ().x, agent->body->GetPosition ().y );
   glVertex2f ( agent->body->GetPosition ().x + sin (agent->angle + 1.57) * 100,
                agent->body->GetPosition ().y + cos (agent->angle + 1.57) * 100);
   glEnd ();

   static float posx = 0;
   posx = posx + .1;
// we are drawing collision array here
   for (int i = 0; i < 40; ++i)
      for (int j = 0; j < 40; ++j)
      {
         if (carCollisionMap[i][j] == 1)
         {
            glPointSize (5);
            glBegin (GL_POINTS);
            glVertex2f (agent->body->GetPosition ().x -  (20 - j) * 10 ,
                        agent->body->GetPosition ().y -  (20 - i) * 10 );

            glEnd ();
         }
      }

   turnToDestination();
   turnToNearestAngle ();
   agent->isCollision = 0;

//  checking collision here, not so important now
   for (int i = 1; i < 25; ++i)
   {
      float x = 20  + (i * 1) * sinf (agent->angle + 1.57) ;
      float y = 20  +  (i * 1) * cosf (agent->angle + 1.57);
      if (x < 40 && y < 40)
         if (carCollisionMap[ (int) y][ (int) x] == 1)
         {
//               printf ("obstacle found\n");
            agent->isCollision = 1;
            static float timerT = glutGet (GLUT_ELAPSED_TIME);
            bool safe = 1;
            safe = rand () % 2;
            if (!agent->frontObstacle)
               if (agent->nearestAngle[LEFT_SIDE] != 1000 || agent->nearestAngle[RIGHT_SIDE] != 1000)
               {
//                  if (glutGet (GLUT_ELAPSED_TIME) - timerT > 1000)
                  {
                     if (agent->nearestAngle[LEFT_SIDE] < agent->nearestAngle[RIGHT_SIDE])
                     {
                        printf ("Left side wins %f\n", agent->nearestAngle[LEFT_SIDE]);
                        agent->angle = agent->angle - .2;
                        agent->foundNearestAngle = true;
//                  agent->nearestAngle[RIGHT_SIDE] = 1000;
                     }
                     else
                     {
                        printf ("Right side wins %f\n", agent->nearestAngle[RIGHT_SIDE]);
                        agent->foundNearestAngle = true;
//                  agent->nearestAngle[LEFT_SIDE] = 1000;
                        agent->angle = agent->angle + .2;
                     }
//                  timerT = glutGet (GLUT_ELAPSED_TIME);
                  }
               }
//            if (glutGet (GLUT_ELAPSED_TIME) - timerT > 1000)
//            {
//
//               if (safe == 1)
//                  for (int j = 0; j < 10; ++j)
//                  {
//                     float x = 20 + ( (j ) * sinf (agent->angle + 1.57 + .3) ) ;
//                     float y = 20 + ( (j ) * cosf (agent->angle + 1.57 + .3) ) ;
//
//                     if (carCollisionMap[ (int) y][ (int) x] == 1)
//                     {
//                        safe = 0;
//                        break;
//                     }
//                  }
//               if (safe == 1)
//               { agent->angle = agent->angle + .3; }
//               {
//                  memset (carCollisionMap, 0, sizeof (carCollisionMap) );
//               }
//               if (safe == 0)
//               {
//                  safe = 1;
//                  for (int j = 0; j < 10; ++j)
//                  {
//                     float x = 20 + ( (j ) * sinf (agent->angle + 1.57 - .3) ) ;
//                     float y = 20 + ( (j ) * cosf (agent->angle + 1.57 - .3) ) ;
//                     if (carCollisionMap[ (int) y][ (int) x] == 1)
//                     {
//                        safe = 0;
//                        break;
//                     }
//                  }
//                  if (safe == 1)
//                  {
//                     agent->angle = agent->angle - .3;
//                     memset (carCollisionMap, 0, sizeof (carCollisionMap) );
//                  }
//               }
//               timerT = glutGet (GLUT_ELAPSED_TIME);
//            }

         }
   }

   agent->moveForward ();
   SetRenderColor (0, 0, 0, 255);

   RenderBitmapText (60, 550, GLUT_BITMAP_9_BY_15, "ctrl + left click to add obstacle, alt + left click to add a destination");

   sprintf (fps, "%f", GetFps () );
   SetRenderColor (255, 0, 255, 255);
   RenderBitmapText (10, 590, GLUT_BITMAP_9_BY_15, fps);

   myWorld.DrawDebugData ();

}
char * readFile (char * fileName)
{
   std::string sLine = "";
   std::ifstream infile;
   int row = 0;
   infile.open (fileName);
   if (!infile)
   {
      std::cout << std::endl << "Failed to open file";
      return NULL;
   }
   while (!infile.eof () )
   {
      getline (infile, sLine);
      map[row][0] = sLine.length ();
      for (int i = 0; i < map[row][0]; i++)
      { map[row][i + 1] = sLine[i]; }
      ++row;
      std::cout << sLine << std::endl;
   }

   for (int j = 0; j < 30; j++)
      for (int i = 1 ; i <= map[j][0]; ++i)
         if (map[j][i] == '1')
         {
            createBody (myWorld, b2_staticBody, i * 800 / 40, (30 - j) * 600 / 30, 10, 10, false);
         }
         else if (map[j][i] == '2')
         {
            agent->body->SetTransform (b2Vec2 (i * 800 / 40, (30 - j) * 600 / 30), 0);
         }
   infile.close ();

   return NULL;
}
int main ()
{

   agent = new Agent (myWorld);
   readFile ("map.txt");

   SetGlutWindow (100, 100, 800, 600, "Sample");

   return 0 ;
}
