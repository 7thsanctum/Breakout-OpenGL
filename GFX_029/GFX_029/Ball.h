#include "GameObject.h"
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

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
private:
	float speed;
};
