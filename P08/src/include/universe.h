#ifndef __UNIVERSE_H__
#define __UNIVERSE_H__

class Object;

class Universe
{
	private: 
		Object* center;
		//bullet globals
		btDispatcher* dispatcher;
		btCollisionConfiguration* collisionConfig;
		btBroadPhaseInterface* broadphase;//collision algorithm between object
		btConstraintSolver* solver;//calculates everything (force, etc.)
	public:
		btDynamicsWorld* world;
		Universe ();
		Object* getCenter();
		void setCenter(Object* c);
};
#endif