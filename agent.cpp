#include "agent.h"

Agent::Agent(b2World& world)
{
   b2BodyDef myBodyDef;
   myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
   myBodyDef.position.Set (100, 200); //set the starting position
   myBodyDef.angle = 0; //set the starting angle
   myBodyDef.angularDamping = 1;
   myBodyDef.linearDamping = 1;
   body = world.CreateBody (&myBodyDef);

   b2PolygonShape boxShape;
   boxShape.SetAsBox (8, 4);

   b2FixtureDef boxFixtureDef;
   boxFixtureDef.shape = &boxShape;
   boxFixtureDef.density = 1;
   body->CreateFixture (&boxFixtureDef);
   body->SetUserData ( (void *) 1);
   body->SetFixedRotation (true);
}

Agent::~Agent()
{
}
void Agent::moveForward()
{
   body->SetTransform (body->GetPosition (), -angle);

   b2Vec2 force = b2Vec2 (sinf (angle + 1.57) * 30000,
                          cosf (angle + 1.57 ) * 30000);
   if (isCollision != 1 && reverseMove == false)
   { body->ApplyForce (force, body->GetPosition () ); }
   else 
   {
      b2Vec2 force = b2Vec2 (sinf (angle + 1.57) * -5000,
                             cosf (angle + 1.57 ) * -5000);
      body->ApplyForce (force,
                               body->GetPosition () );
   }
}

