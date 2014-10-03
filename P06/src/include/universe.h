#ifndef __UNIVERSE_H__
#define __UNIVERSE_H__

class Object;

class Universe
{
	private: 
		Object* center;

	public:
		Universe ();
		Object* getCenter();
		void setCenter(Object* c);
};
#endif