#pragma once

#define GLFW_DLL

#include <GL\glfw.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>  //For mat4_cast
#include <glm\gtc\matrix_transform.hpp>
#include <random>
#include <chrono>

#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"

using namespace std;
using namespace std::chrono;
using namespace glm;

// TODO: Use singleton pattern so only one instance of this game exists, also 
//		 enable game state saving.

class Game 
{
public:
	// ### Constructors/Destructors ###
	Game();	// Generic constructor and destructor
	~Game();	
	// ### Game Logic ###
	void Initialise();
	bool Update(float deltaTime);	// Update function
	vector<GameObject> GetAllObjectsInScene();
	vector<GameObject> GetActiveObjectsInScene();
private:
	void UpdateObjectList();
	vector<GameObject> objectsInScene;
};
