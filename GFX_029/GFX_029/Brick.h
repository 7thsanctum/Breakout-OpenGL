#include "GameObject.h"

class Brick: public GameObject
{
public:
	Brick();
	~Brick();
	int GetHealth();
	void UpdateColour();
	void ChangeColour(glm::vec4 newColour);
	void SetHealth(int health);
	void DeductHealth(int loss);
private:
	int health;
};