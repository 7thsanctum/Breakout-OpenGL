#include "Ball.h"

Ball::Ball() 
{
	speed = 8.0f;
}

Ball::~Ball() { }


void Ball::StartBall(glm::vec3 paddlePos)
{
	SetVelocity(glm::normalize(glm::vec3(renderObj.transform.position - paddlePos)));
}

void Ball::ResetBall()
{
	renderObj.transform.position = glm::vec3(0.0f, -2.4f, 0.0f);
	SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	SetStatus(false);
}

void Ball::Update(float deltaTime)
{
	MoveBall(deltaTime);
}

void Ball::MoveBall(float deltaTime)
{
	renderObj.transform.position += GetVelocity() * glm::vec3(-speed * deltaTime, -speed * deltaTime, 0.0f);
}

glm::vec3 Ball::RandomDirection()
{
	// Create a random engine
	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	default_random_engine e(millis.count());
	uniform_real_distribution<float> distribution(-1.0f, 1.0f);

 	return glm::vec3(distribution(e), 1.0f, 0.0f);
}

void Ball::Bounce()
{
	// TODO: Currently only reverses direction on X-axis
	glm::vec3 temp = GetVelocity();
	temp.x *= -1.0f;
	SetVelocity(temp);
}