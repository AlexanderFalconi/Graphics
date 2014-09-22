#include <GL/glew.h> // glew must be included before the main gl libs
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include "include/dllist.h"
#include "include/object.h"
#include "include/universe.h"
using namespace std;

//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
	GLfloat position[3];
	GLfloat color[3];
};

//--Resource management
bool initialize();
void cleanUp();

int main(int argc, char **argv)
{
	new Universe(640, 480);
	return 0;
}