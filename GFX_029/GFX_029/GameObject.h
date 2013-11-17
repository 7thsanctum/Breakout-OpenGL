#pragma once

#include <GL\glfw.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>  //For mat4_cast
#include <glm\gtc\matrix_transform.hpp>
#include <random>
#include <chrono>

#include "geometry.h"
#include "Utilities.h"

using namespace std;
using namespace std::chrono;

class GameObject 
{
public:
	// ### Constructors/Destructors ###
	GameObject();	// Generic constructor and destructor
	GameObject(render_object setObj, bool alive);	// Constructor for initialising and setting status of object when it's created
	~GameObject();	
	// ### Game Logic ###
	void Update(float deltaTime);	// Update function
	bool GetStatus();
	void SetStatus(bool status);
	// ### Collision/Positional Information ###
	glm::vec3 GetVelocity();
	void SetVelocity(glm::vec3 vel);	
	glm::vec3 GetPosition();
	float GetXBound();
	float GetYBound();	
	// ### Rendering ###
	void SetRenderObject(render_object setObj);	
	void ChangeColour(glm::vec4 newColour);
	void RandomColour();
	render_object renderObj;	
private:
	bool alive;
	glm::vec3 velocity;
	float xBounds;
	float yBounds;

	void CalculateBounds();
};
