#pragma once

#include <GL\glfw.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>  //For mat4_cast
#include <glm\gtc\matrix_transform.hpp>
#include "geometry.h"

class GameObject 
{
public:
	GameObject();
	GameObject(render_object setObj, bool alive);
	void Update(float deltaTime);
	bool GetStatus();
	glm::vec3 GetVelocity();
	void SetVelocity(glm::vec3 vel);	
	void SetStatus(bool status);
	void SetRenderObject(render_object setObj);
	~GameObject();
	render_object renderObj;
private:
	bool alive;
	glm::vec3 velocity;
};
