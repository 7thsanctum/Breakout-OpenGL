#include "GameObject.h"

GameObject::GameObject() { }

GameObject::GameObject(render_object setObj, bool alive) 
{
	GameObject::renderObj = setObj;
	GameObject::alive = alive;
}

void Update() { }

GameObject::~GameObject() { }

void GameObject::SetRenderObject(render_object setObj)
{
	renderObj = setObj;
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
	GameObject::alive = status;
}