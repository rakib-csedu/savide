#ifndef AGENT_H
#define AGENT_H
#include "box2d/Box2D.h"
class Agent
{
public:
   Agent()
   {
      angle = 0;
      foundNearestAngle = 0;
      reverseMove = 0 ;
      isCollision = 0;
      frontObstacle = 0;
      nearestAngle[0] = nearestAngle[1] = nearestAngle[2] = nearestAngle[3] = 1000;
   }
   Agent(b2World& world);
   ~Agent();

   b2Body * body;
   float angle;
   bool foundNearestAngle;
   float reverseMove  ;
   int isCollision;
   int frontObstacle ;
   float nearestAngle[4];
   void moveForward();
};

#endif // AGENT_H
