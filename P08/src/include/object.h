#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <glm/glm.hpp>
#include <vector>
using std::vector;

class Mesh;
class Texture;
class Shader;
class Universe;

class Object
{
	private:
		std::string name;
		float mass;
		float density;
		Universe* daemon;//points to universe
		Object* environment;//points to parent
		vector<Object*> inventory;//stores a list of children
		glm::mat4 model;//obj->world each object should have its own model matrix
		glm::mat4 view;//world->eye
		glm::mat4 projection;//eye->clip
		glm::mat4 mvp;//premultiplied modelviewprojection
		Mesh* mesh;//mesh handler
		Texture* texture;//texture handler
		Shader* shader;//texture handler
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

	public:
		Momentum rotation;
		Momentum orbit;
		Object (Universe* d, Shader *shade, int width, int height);
		Object (Universe* d, Shader *shade, std::string obName, float obMass, float obDensity, int width, int height);
		float getMass();
		float getDensity();
		glm::mat4 getModel();
		Object* getEnvironment();
		vector<Object*> getInventory();
		void render();
		void update(float dt);
		void reshape(int width, int height);
		bool initialize(int width, int height);
		void receive(Object *ob);
		void release();
		void eventMove(Object *ob);
};
#endif