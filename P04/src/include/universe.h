class Universe
{
	private: 
		Object *center;
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

	public:
		Window window;
		Universe(int windowWidth, int windowHeight);
		Object *getCenter();
		float getDT();
		//--GLUT Callbacks
		void render();
		void update();
		Object *getCenter();
		void reshape(int n_w, int n_h);
		void keyboard(unsigned char key, int x_pos, int y_pos);
		void mouse(int button, int state, int x_pos, int y_pos);
		void mainMenu(int value);
		void specialInput(int key, int x, int y);
};