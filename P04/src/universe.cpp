Universe::Universe(int windowWidth, int windowHeight)
{
	window.width = windowWidth;
	window.height = windowHeight;
	leftClick = 0;
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