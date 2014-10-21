#include <bullet/btBulletDynamicsCommon.h>	//you may need to change this
#include "include/universe.h"

Universe::Universe() 
{ 
	collisionConfig = new btDefaultCollisionConfiguration();//initialize bullet
	dispatcher = new btCollisionDispatcher(collisionConfig);
	broadphase = new btDbvtBroadPhase();
	solver = new btSequentialImpulseConstraintSolver();
	world=new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	world->setGravity(btVector3(0, -10, 0));//10 meters per second downwards
}

Object* Universe::getCenter()
{
	return center;
}

void Universe::setCenter(Object *c)
{
	center = c;
}