#pragma comment(lib, "GLFWDLL")  //Now using GLFWDLL.lib instead of GLFW.lib (compatibilty)
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "glew32")  //GLEW library (Allows us to use modern openGL functions)

#define GLFW_DLL

#include <GL\glew.h>
#include <GL\glfw.h>
#include <IL\ilut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_precision.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\constants.hpp>
#include <cstdlib>
#include <vector>
#include <random>
#include <iostream>
#include <conio.h>
#include <Box2D/Box2D.h>

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

int resX = 800; 
int resY = 600;

void initialise()
{
	// TODO: add in resolution selection code from other project
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	mat4 projection = perspective(degrees(quarter_pi<float>()), 
		                                                 (float)resX/(float)resY, 
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

bool resolutionSelect()	//Sets the resolution of the application
{
	int val = 0;
	char SELEC; 

	GLFWvidmode return_struct;				

	glfwGetDesktopMode( &return_struct );	// Gets the current desktop resolution

	int width = return_struct.Width;
	int height = return_struct.Height;
	resX = width;
	resY = height;
	do{
		system("CLS");
		std::cout << "Please select your resolution\n1) 800x600\n2) 1024x768\n3) 1280x720\n4) 1920x1080\n5) Native : " << width << "x" << height;	
		std::cout << "\n\nYour native resolution is " << width << "x" << height << " and has been set as the default\n";
		std::cout<<"\nPlease press either 1,2,3,4 or 5.\n";
		SELEC = _getche(); //Takes in the users choice instantly without having to press enter
    }while (SELEC < '1' || SELEC > '5');

	val = SELEC - '0';	//Converts from ASCII to the actual value
	if(val == 1) { resX = 800; resY = 600; }
	else if(val == 2) { resX = 1024; resY = 768; }
	else if(val == 3) { resX = 1280; resY = 720; }
	else if(val == 4) { resX = 1920; resY = 1080; }
	else if(val == 5) { }
	else return(false); 

	system("CLS");	//Clear screen so it isn't too crowded

	return(true);	
}

int main()
{
	int mode = GLFW_WINDOW;
	if(!glfwInit())  //initialise GLFW by calling gltwInit
		exit(EXIT_FAILURE);  //If cant find it then close window

	int check = MessageBox(NULL,L"Welcome! \nWould you like to run this application in full screen mode?", L"Welcome to Mark's Program!",MB_YESNO|MB_ICONINFORMATION);
	if(check == IDYES)		mode = GLFW_FULLSCREEN;		 // Set to fullscreen mode
	else if (check == IDNO) std::cout << "OK, your loss ...\n"; 
	//ShowCursor(FALSE);

	//glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);	//Anti Aliasing effect only seems to work at 16

	if(!resolutionSelect())	//Select resolution
		std::cout << "\nResolution selection failed, you have been set to the default resolution\n";
	if (!glfwOpenWindow(int(resX), int(resY), 0, 0, 0, 0, 0, 0, mode))  //Setting window size
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