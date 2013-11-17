#include "GameObject.h"
#include "Utilities.h"
class Brick: public GameObject
{
public:
	Brick();
	~Brick();
	int GetHealth();
	void UpdateColour();	
	void SetHealth(int health);
	void DeductHealth(int loss);
private:
	int health;
};