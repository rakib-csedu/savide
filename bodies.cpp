#include "bodies.h"

b2Body* createBody (b2World& world, b2BodyType type, int posX,int posY,int sizeX,int sizeY, bool isSensor)
{
   b2BodyDef myBodyDef1;
   myBodyDef1.type = type; //this will be a dynamic body
   myBodyDef1.position.Set (posX, posY); //set the starting position
   myBodyDef1.angle = 0; //set the starting angle
   
   b2Body* dynamicBody2;
   dynamicBody2 = world.CreateBody (&myBodyDef1);
   b2PolygonShape boxShape1;
   boxShape1.SetAsBox (sizeX, sizeY);

   b2FixtureDef boxFixtureDef1;
   boxFixtureDef1.shape = &boxShape1;
   boxFixtureDef1.density = 1000;
   boxFixtureDef1.isSensor = isSensor;
   dynamicBody2->CreateFixture (&boxFixtureDef1);

   dynamicBody2->SetFixedRotation (true);
   return dynamicBody2;
//   if(type == b2_dynamicBody)
//      dynamicBody2->ApplyLinearImpulse(b2Vec2(0,1000000000),b2Vec2(0,0));
}