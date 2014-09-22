Universe::Universe(int windowWidth, int windowHeight)
{
	window.width = windowWidth;
	window.height = windowHeight;
	leftClick = 0;
	// Initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window.width, window.height);
	// Name and create the Window
	glutCreateWindow("CS480 Graphics");
	// Initialize GLEW to prepare work with shaders
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "[F] GLEW NOT INITIALIZED: ";
		std::cerr << glewGetErrorString(status) << std::endl;
		return -1;
	}
	// Set all of the callbacks to GLUT that we need
	glutDisplayFunc(render);// Called when its time to display
	glutReshapeFunc(reshape);// Called if the window is resized
	glutIdleFunc(update);// Called if there is nothing else to do
	glutKeyboardFunc(keyboard);// Called if there is keyboard input
	glutMouseFunc(mouse);// Called if there is mouse input
	glutSpecialFunc(specialInput);
	// Set up menu
	glutCreateMenu(mainMenu);
	glutAddMenuEntry("Start Rotation", 1);
	glutAddMenuEntry("Stop Rotation", 2);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// Initialize all of our resources(shaders, geometry)
	bool init = initialize();
	if (init)
	{
		t1 = std::chrono::high_resolution_clock::now();
		glutMainLoop();
	}
	cleanUp();
}

bool Universe::initialize()
{
	// Initialize basic geometry and shaders for this example
	Vertex geometry[] = { { { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, -1.0, 1.0 }, { 0.0, 0.0, 1.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 0.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { 1.0, -1.0, -1.0 }, { 1.0, 0.0, 0.0 } },
	{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 0.0 } },
	{ { 1.0, -1.0, -1.0 }, { 1.0, 0.0, 0.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { -1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } },
	{ { -1.0, -1.0, 1.0 }, { 0.0, 0.0, 1.0 } },
	{ { -1.0, -1.0, -1.0 }, { 0.0, 0.0, 0.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { -1.0, -1.0, 1.0 }, { 0.0, 0.0, 1.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { 1.0, -1.0, -1.0 }, { 1.0, 0.0, 0.0 } },
	{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 0.0 } },
	{ { 1.0, -1.0, -1.0 }, { 1.0, 0.0, 0.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 0.0 } },
	{ { -1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { -1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } },
	{ { -1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } },
	{ { 1.0, -1.0, 1.0 }, { 1.0, 0.0, 1.0 } }
	};
	// Create a Vertex Buffer object to store this vertex info on the GPU
	glGenBuffers(1, &vbo_geometry);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
	glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);
	//--Geometry done
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER); 
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	//Shader Sources
	std::ifstream file("shader.vert");
	std::string vsContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	const char *vs = vsContent.c_str();
	file.close();
	file.open("shader.frag");
	std::string fsContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	const char *fs = fsContent.c_str();
	//compile the shaders
	GLint shader_status;
	// Vertex shader first
	glShaderSource(vertex_shader, 1, &vs, NULL);
	glCompileShader(vertex_shader);
	//check the compile status
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_status);
	if (!shader_status)
	{
		std::cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << std::endl;
		return false;
	}
	// Now the Fragment shader
	glShaderSource(fragment_shader, 1, &fs, NULL);
	glCompileShader(fragment_shader);
	//check the compile status
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_status);
	if (!shader_status)
	{
		std::cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << std::endl;
		return false;
	}
	//Now we link the 2 shader objects into a program
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	//check if everything linked ok
	glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
	if (!shader_status)
	{
		std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
		return false;
	}
	//Start loading objects
	center = new Object(this, "Planet", 0.0, 0.0);
	Object moon(this, "Moon", 0.0, 0.0);
	center->receive(moon);
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//and its done
	return true;
}

void Universe::cleanUp()
{
	// Clean up, Clean up
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_geometry);
}

Object *Universe::getCenter()
{
	return center;
}

float Universe::getDT()
{
	float ret;
	t2 = std::chrono::high_resolution_clock::now();
	ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2 - t1).count();
	t1 = std::chrono::high_resolution_clock::now();
	return ret;
}

void Universe::reshape(int n_w, int n_h)
{
	window.width = n_w;
	window.height = n_h;
	//Change the viewport to be correct
	glViewport(0, 0, w, h);
	//Update the projection matrix as well
	projection = glm::perspective(45.0f, float(window.width) / float(window.height), 0.01f, 100.0f);
}

void Universe::keyboard(unsigned char key, int x_pos, int y_pos)
{
	// Handle keyboard input
	if (key == 27)//ESC
		exit(0);
	else if (key == 97)//A
		center->rotation.inverse = !center->rotation.inverse;//reverse rotation
}

void Universe::specialInput(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			center->rotation.inverse = true;
			break;
		case GLUT_KEY_RIGHT:
			center->rotation.inverse = false;
			break;
	}
}

void Universe::mouse(int button, int state, int x_pos, int y_pos)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		leftClick = GLUT_LEFT_BUTTON;
	if (state == GLUT_UP && button == leftClick)
	{
		leftClick = 0;
		center->rotation.inverse = !center->rotation.inverse;//reverses direction
	}
}

void Universe::mainMenu(int value)
{
	switch (value)
	{
		case 1://Start rotating
			center->orbit.active = true;
			break;
		case 2://Pause rotating
			center->orbit.active = false;
			break;
		case 3://Exit
			exit(0);
			break;
	}
}

void Universe::render()
{
	//clear the screen
	glClearColor(0.0, 0.0, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	center->render();
	//swap the buffers
	glutSwapBuffers();
}

void Universe::update()
{
	center->update();
	glutPostRedisplay();//call the display callback
}