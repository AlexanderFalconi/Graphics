Object::Object (std::string obName, double obMass, double obDensity, double obRotation, double obSpeed)
{
	name = obName;
	mass = obMass;
	density = obDensity;
	rotation = obRotation;
	speed = obSpeed;
}

double Object::getMass()
{
	return mass;
}

double Object::getDensity()
{
	return density;
}

double Object::getRotation()
{
	return rotation;
}

double Object::getSpeed()
{
	return speed;
}

Object *Object::environment()
{
	return orbits;
}

List *Object::orbiters()
{
	return inventory;
}