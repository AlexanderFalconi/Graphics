Universe::Universe(std::string centerName)
{
	Object * center;
	center = new Object(centerName, 0.0, 0.0, 0.0, 0.0);
}

Universe::Object getCenter()
{
	return center;
}