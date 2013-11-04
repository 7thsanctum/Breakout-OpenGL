#include "Paddle.h"

Paddle::Paddle() { }
Paddle::~Paddle() { }

void Paddle::Update(float deltaTime)
{
	renderObj.transform.position += GetVelocity() * glm::vec3(deltaTime, 0.0f, 0.0f);
}
