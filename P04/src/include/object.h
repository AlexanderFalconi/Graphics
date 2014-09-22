class Object
{
	private:
		std::string name;
		double mass;
		double density;
		double rotation;
		double speed;
		Object *orbits;
		List *orbiters;

	public:
		Object (std::string obName, double obMass, double obDensity, double obRotation, double obSpeed);
		double getMass();
		double getDensity();
		double getRotation();
		double getSpeed();
		Object *environment();
		List *inventory();
};