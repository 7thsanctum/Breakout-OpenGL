#include "GameObject.h"

class Ball: public GameObject 
{
public:
	using GameObject::GameObject;
	Ball();
	~Ball();
	void StartBall(glm::vec3 paddlePos);
	void ResetBall();
	void Update(float deltaTime);
	void MoveBall(float deltaTime);
	void Bounce();
	glm::vec3 RandomDirection();
	void checkCollision();
	float GetSpeed();
	void SetSpeed(float value);
private:
	float speed;
};
