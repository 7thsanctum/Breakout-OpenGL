#pragma comment(lib, "GLFWDLL")  //Now using GLFWDLL.lib instead of GLFW.lib (compatibilty)
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "glew32")  //GLEW library (Allows us to use modern openGL functions)

#define GLFW_DLL

#include <GL\glew.h>
#include <GL\glfw.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_precision.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\constants.hpp>
#include <cstdlib>
#include <vector>
#include <random>
#include <iostream>
#include "geometry.h"
#include "shader.h"

#include "Game.h"


using namespace std;
using namespace glm;

GLuint shaders[2];  // Declare 2 shaders
GLuint program;		// Declare one program paddle
GLint colourUniform;

Game breakout = Game();;

bool running = true;

void initialise()
{
	// TODO: add in resolution selection code from other project
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	mat4 projection = perspective(degrees(quarter_pi<float>()), 
		                                                 800.0f/600.0f, 
		                                                 0.1f, 
		                                                 10000.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(projection));
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	shaders[0] = loadShader("colour.vert", GL_VERTEX_SHADER);	// Load vertex shader
	shaders[1] = loadShader("colour.frag", GL_FRAGMENT_SHADER); // Load fragment shader
	if (shaders[0] && shaders[1])  // Make sure both shader programs have been created
	{
		program = createProgram(shaders, 2);  // If both shaders created, create the program
		if (!program)  // If it fails, exit
			exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_FAILURE);

	colourUniform = glGetUniformLocation(program, "colour");
}

void update (double deltaTime)  
{
	running = breakout.Update(deltaTime);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen
	// TODO: set this up so it is inside the game class 
	// and can be changed dependent on the size of the game
	mat4 view = lookAt(vec3(0.0f, 0.0f, 30.0f),		// Cameras position
						vec3(0.0f, 0.0f, 0.0f),	
						vec3(0.0f, 1.0f, 0.0f));	// Camers up vector
	glMatrixMode(GL_MODELVIEW);
	glUseProgram(program);			// Use program we created
	for(GameObject obj : breakout.GetActiveObjectsInScene())
	{
		glUniform4fv(colourUniform, 1, value_ptr(obj.renderObj.colour));
		obj.renderObj.render(view);
	}

	glUseProgram(0);		// Tell OpenGL to stop using the program
	glfwSwapBuffers();
}

void cleanup()  // Allow us to clean up used resources in our application
{
	// Check if objects are created (not equal to 0) then delete accordingly
	//if (paddle.renderObj.geometry) delete paddle.renderObj.geometry;
	breakout.~Game();
	if (program) glDeleteProgram(program);
	if(shaders[0]) glDeleteShader(shaders[0]);  
	if(shaders[1]) glDeleteShader(shaders[1]);
}

int main()
{
	if(!glfwInit())  //initialise GLFW by calling gltwInit
		exit(EXIT_FAILURE);  //If cant find it then close window

	if (!glfwOpenWindow(800, 600, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))  //Setting window size
	{
		glfwTerminate();		// If can open window this size Terminate GLFW
		exit(EXIT_FAILURE);		// And exit
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)  // Check for errors in glewInit
	{
		std::cout<< "Error: " << glewGetErrorString(error) << std::endl;
		exit(EXIT_FAILURE);
	}

	initialise();  // Intialiase everything so far then enter main render loop

	double prevTimeStamp = glfwGetTime();
	double currentTimeStamp;	// Place to store the current time
	while (running)				// The Loop
	{
		currentTimeStamp = glfwGetTime();			// Store the current time in "currentTimeStamp"
		update(currentTimeStamp - prevTimeStamp);	// Subtract prev time from current time to give us time passed since last frame
		render();									// Then render
		prevTimeStamp = currentTimeStamp;			// Then set previous frame time to current frame time
	}												// Loop continues until running is set to false

	cleanup();

	glfwTerminate();

	exit(EXIT_SUCCESS);
}