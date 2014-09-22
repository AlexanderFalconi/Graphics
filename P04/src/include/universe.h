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

class Object;

class Universe
{
	private: 
		Object* center;
		GLuint program;// The GLSL program handle
		GLuint vbo_geometry;// VBO handle for our geometry
		std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

		struct Window
		{
			int width;
			int height;
			int leftClick;
			Window() {};
		};

		Window window;

	public:
		Universe (int windowWidth, int windowHeight);
		Object *getCenter();
		float getDT();
		//--GLUT Callbacks
		void render();
		void update();
		void cleanUp();
		bool initialize();
		void reshape(int n_w, int n_h);
		void keyboard(unsigned char key, int x_pos, int y_pos);
		void mouse(int button, int state, int x_pos, int y_pos);
		void mainMenu(int value);
		void specialInput(int key, int x, int y);
};