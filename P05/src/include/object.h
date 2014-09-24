#ifndef __OBJECT_H__
#define __OBJECT_H__

class Object
{
	private:
		std::string name;
		float mass;
		float density;
		Universe* daemon;
		Object* environment;
		List* inventory;
		GLint loc_position;
		GLint loc_color;
		GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader
		glm::mat4 model;//obj->world each object should have its own model matrix
		glm::mat4 view;//world->eye
		glm::mat4 projection;//eye->clip
		glm::mat4 mvp;//premultiplied modelviewprojection

		struct Vertex
		{
			GLfloat position[3];
			GLfloat color[3];
		};

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
		Object (Universe* d, GLint program, int width, int height);
		Object (Universe* d, std::string obName, float obMass, float obDensity, GLint program, int width, int height);
		float getMass();
		float getDensity();
		glm::mat4 getModel();
		Object* getEnvironment();
		List* getInventory();
		void render(GLuint program, GLuint vbo_geometry, int width, int height);
		void update(float dt);
		bool initialize(GLuint program, int width, int height);
		void receive(Object *ob);
		void release(Object *ob);
		void eventMove(Object *ob);
};
#endif