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
	return GameObject::alive;
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
	xBounds = (renderObj.transform.scale.x * 2.0f);
	yBounds = (renderObj.transform.scale.y);
}