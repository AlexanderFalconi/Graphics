#include "include/universe.h"

Universe::Universe() 
{ 
}

Object* Universe::getCenter()
{
	return center;
}

void Universe::setCenter(Object *c)
{
	center = c;
}