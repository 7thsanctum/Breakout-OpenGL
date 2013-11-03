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
}

bool GameObject::GetStatus()
{
	return GameObject::alive;
}

void GameObject::SetStatus(bool status)
{
	GameObject::alive = status;
}