#include "GameObject.h"

GameObject::GameObject() { }

GameObject::GameObject(render_object setObj, bool alive) 
{
	GameObject::renderObj = setObj;
	GameObject::alive = alive;
}

GameObject::~GameObject() { }

void GameObject::SetRenderObject(render_object setObj)
{
	renderObj = setObj;
	CalculateBounds();
}

// Change object colour to whatever is defined
void GameObject::ChangeColour(glm::vec4 newColour)
{	
	renderObj.colour = newColour;
}

void GameObject::RandomColour()
{	
	// Create a random engine
	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	default_random_engine e(millis.count());
	uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	float r = distribution(e);
	float g = distribution(e);
	float b = distribution(e);
	// TODO: add check to make sure it doesn't go black, 
	// possibly add a list of colours and instead select a random colour?

	renderObj.colour = glm::vec4(r, g, b, 1.0f);
}

// Return the objects current position
glm::vec3 GameObject::GetPosition()
{
	return renderObj.transform.position;
}

float GameObject::GetXBound()
{
	return xBounds;
}

float GameObject::GetYBound()
{
	return yBounds;
}

bool GameObject::GetStatus()
{
	return alive;
}

glm::vec3 GameObject::GetVelocity()
{
	return velocity;
}

void GameObject::SetVelocity(glm::vec3 vel)
{
	velocity = vel;
}

void GameObject::SetStatus(bool status)
{
	alive = status;
}

void GameObject::CalculateBounds()
{
	xBounds = (renderObj.transform.scale.x);
	yBounds = (renderObj.transform.scale.y);
}