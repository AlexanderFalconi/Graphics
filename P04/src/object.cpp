Object::Object(Universe *d)
{
	daemon = d;
	initialize();
}

Object::Object (Universe *d, std::string obName, double obMass, double obDensity)
{
	daemon = d;
	name = obName;
	mass = obMass;
	density = obDensity;
	initialize();
}

void initialize()
{
	//Now we set the locations of the attributes and uniforms
	loc_position = glGetAttribLocation(daemon->program,
		const_cast<const char*>("v_position"));
	if (loc_position == -1)
	{
		std::cerr << "[F] POSITION NOT FOUND" << std::endl;
		return false;
	}
	loc_color = glGetAttribLocation(daemon->program,
		const_cast<const char*>("v_color"));
	if (loc_color == -1)
	{
		std::cerr << "[F] V_COLOR NOT FOUND" << std::endl;
		return false;
	}
	loc_mvpmat = glGetUniformLocation(daemon->program,
		const_cast<const char*>("mvpMatrix"));
	if (loc_mvpmat == -1)
	{
		std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
		return false;
	}
	//--Init the view and projection matrices
	//  if you will be having a moving camera the view matrix will need to more dynamic
	//  ...Like you should update it before you render more dynamic 
	//  for this project having them static will be fine
	view = glm::lookAt(glm::vec3(0.0, 8.0, -16.0), //Eye Position
		glm::vec3(0.0, 0.0, 0.0), //Focus point
		glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

	projection = glm::perspective(45.0f, //the FoV typically 90 degrees is good which is what this is set to
		float(daemon->width) / float(daemon->height), //Aspect Ratio, so Circles stay Circular
		0.01f, //Distance to the near plane, normally a small value like this
		100.0f); //Distance to the far plane, 
}

double Object::getMass()
{
	return mass;
}

double Object::getDensity()
{
	return density;
}

Object *Object::getEnvironment()
{
	return environment;
}

List *Object::getInventory()
{
	return inventory;
}

void Object::render()
{
	//premultiply the matrix for this example
	mvp = projection * view * model;
	//enable the shader program
	glUseProgram(daemon->program);
	//upload the matrix to the shader
	glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
	//set up the Vertex Buffer Object so it can be drawn
	glEnableVertexAttribArray(loc_position);	
	glEnableVertexAttribArray(loc_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
	//set pointers into the vbo for each of the attributes(position and color)
	glVertexAttribPointer(loc_position,//location of attribute
		3,//number of elements
		GL_FLOAT,//type
		GL_FALSE,//normalized?
		sizeof(Vertex),//stride
		0);//offset
	glVertexAttribPointer(loc_color,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, color));
	glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, count
	//clean up
	glDisableVertexAttribArray(loc_position);
	glDisableVertexAttribArray(loc_color);
	//Iterate through inventory
	for (iterator i = iterator(&inventory.head.next); i.node_ptr->next != nullptr; i = i++)
	{
		i.node_ptr->object.render();
		inventory->iter++;
	}
}

void Object::update()
{
	float dt = daemon->getDT();// if you have anything moving, use dt.
	orbit.position += dt * M_PI / 2; //move through 90 degrees a second
	rotation.position += dt * M_PI / 2;//spin 90 degrees a second
	model = glm::translate(glm::mat4(0.1f), glm::vec3(8.0 * sin(orbit.position), 0.0, 8.0 * cos(orbit.position))) * glm::rotate(glm::mat4(1.0f), rotation.position, glm::vec3(0, 1, 0));
	//Iterate through inventory
	for (iterator i = iterator(&inventory.head.next); i.node_ptr->next != nullptr; i = i++)
	{
		i.node_ptr->object.update();
		inventory->iter++;
	}
}