#include <cstdlib>
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

class Universe;
class List;

class Object
{
	private:
		std::string name;
		double mass;
		double density;
		Universe *daemon;
		Object *environment;
		List* inventory;
		GLint loc_position;
		GLint loc_color;
		GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader
		glm::mat4 model;//obj->world each object should have its own model matrix
		glm::mat4 view;//world->eye
		glm::mat4 projection;//eye->clip
		glm::mat4 mvp;//premultiplied modelviewprojection

		struct Momentum
		{
			bool active;
			bool inverse;
			float angle;
			float velocity;
			float tilt;
			Momentum() 
			{
				active = true;
				inverse = false;
				angle = 0.0;
			};
		};

		Momentum rotation;
		Momentum orbit;

	public:
		Object (Universe *d);
		Object (Universe *d, std::string obName, double obMass, double obDensity);
		double getMass();
		double getDensity();
		Object* getEnvironment();
		List* getInventory();
		void initiative();
		void render();
		void update();
};