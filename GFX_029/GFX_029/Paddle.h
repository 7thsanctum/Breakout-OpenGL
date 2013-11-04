#include "GameObject.h"

class Paddle: public GameObject
{
public:
	Paddle();
	~Paddle();
	void Update(float deltaTime);
private:
	int lives;
	float size;
	float speed;
};