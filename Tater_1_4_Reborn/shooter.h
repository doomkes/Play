#include "WPILib.h"

class Shooter {
	Solenoid pow;
public:
	Shooter();
	~Shooter();
	void bump();
	void truss();
	void high();

};
