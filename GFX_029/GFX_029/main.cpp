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
#include "Ball.h"
#include "geometry.h"
#include "shader.h"

using namespace std;
using namespace glm;

GLuint shaders[2];  // Declare 2 shaders
GLuint program;		// Declare one program paddle
GLint colourUniform;

vector< vector<render_object> > brick;
Ball ball;				// Declare render_object for geometry
render_object paddle;	// Declare render_object for geometry

float horizontalLimit = 5.0f;
float paddleSpeed = 8.0f;
int rows = 4;
int columns = 8;

bool running = true;

void initialise()
{
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
	geometry* geom = createQuad();			// Create a quad to add to all the objects in the scene
	for(int i = 0; i < rows; ++i)	// Each line
	{
		vector<render_object> temp;
		for(int j = 0; j < columns; ++j)			// Each individual brick
		{
			render_object b;
			b.geometry = geom;
			b.transform.position = vec3(-4.0f + (j*1.1f), 3.0f - i*0.8f, 0.0f);
			b.transform.scale = vec3(0.5f, 0.30f, 0.0f);
			b.colour = vec4(i - 2, i - 1, i + 1 , 1.0f);
			temp.push_back(b);
		}
		brick.push_back(temp);
	}

	paddle.geometry = geom;
	paddle.transform.position -= vec3(0.0f, 3.0f, 0.0f);
	paddle.transform.scale.y = 0.1f;
	paddle.transform.scale.x = 1.0f;
	paddle.colour = vec4(0.0f, 1.0f, 0.0f, 1.0f);  // Set colour 

	geom = createSphere(20,20);
	render_object temp;	
	temp.geometry = geom;
	temp.transform.scale = vec3(0.1f, 0.1f, 0.1f);
	temp.transform.position -= vec3(0.0f, 2.5f, 0.0f);
	temp.colour = vec4(1.0f, 1.0f, 0.4f, 1.0f);  // Set colour 
	ball.SetRenderObject(temp);
}

void update (double deltaTime)  
{
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	
	if (glfwGetKey(GLFW_KEY_RIGHT) || glfwGetKey('D') )
		paddle.transform.position.x += paddleSpeed * deltaTime;
	if (glfwGetKey(GLFW_KEY_LEFT) || glfwGetKey('A') )
		paddle.transform.position.x -= paddleSpeed * deltaTime;
	if (glfwGetKey(GLFW_KEY_SPACE) && !ball.GetStatus())
	{
		ball.SetStatus(true);
		ball.StartBall(paddle.transform.position);
	}

	//if(glm::abs(paddle.transform.position.x) > 4.0f) paddle.transform.position.x = 4.0f; // Fix this to use ABS so the check happens in one line
	if(paddle.transform.position.x > horizontalLimit) 
		paddle.transform.position.x = horizontalLimit;
	else if(paddle.transform.position.x < -horizontalLimit) 
		paddle.transform.position.x = -horizontalLimit;
	
	if(ball.renderObj.transform.position.x > horizontalLimit) 
		ball.Bounce();
	else if(ball.renderObj.transform.position.x < -horizontalLimit) 
		ball.Bounce();
	if(ball.renderObj.transform.position.y > horizontalLimit)
		ball.SetDirection(glm::vec3(ball.GetDirection().y) *= -1.0f);
	else if(ball.renderObj.transform.position.y < -horizontalLimit + 1.0f) 
		ball.ResetBall();

	if((ball.renderObj.transform.position.y <= paddle.transform.position.y + 0.1f && ball.renderObj.transform.position.y >= paddle.transform.position.y - 0.1f) && 
		(ball.renderObj.transform.position.x >= paddle.transform.position.x - 0.2f && ball.renderObj.transform.position.x <= paddle.transform.position.x + 0.2f))
	{
		ball.SetDirection(glm::vec3(ball.GetDirection().y) *= -1.0f);
		//ball.direction.x *= -1.0f;
	}

	ball.Update(deltaTime);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 view = lookAt(vec3(0.0f, 0.0f, 10.0f),  //Clear screen
								 vec3(0.0f, 0.0f, 0.0f),
								 vec3(0.0f, 1.0f, 0.0f));
	glMatrixMode(GL_MODELVIEW);
	glUseProgram(program);  //Use program we created
	for(vector<render_object> r : brick)	// Each line	
		for(render_object b : r)			// Each individual brick
		{
			glUniform4fv(colourUniform, 1, value_ptr(b.colour));
			b.render(view);
		}
		glUniform4fv(colourUniform, 1, value_ptr(ball.renderObj.colour));	
	ball.renderObj.render(view);	// Call render as before
	glUniform4fv(colourUniform, 1, value_ptr(paddle.colour));	
	paddle.render(view);	// Call render as before
	glUseProgram(0);		// Tell OpenGL to stop using the program
	glfwSwapBuffers();
}

void cleanup()  //Allow us to clean up used resources in our application
{
	if (paddle.geometry) delete paddle.geometry;
	if (program) glDeleteProgram(program);
	if(shaders[0]) glDeleteShader(shaders[0]);  //Check if objects are created (not equal to 0) then delete accordingly
	if(shaders[1]) glDeleteShader(shaders[1]);
}

int main()
{
	if(!glfwInit())  //initialise GLFW by calling gltwInit
		exit(EXIT_FAILURE);  //If cant find it then close window

	if (!glfwOpenWindow(800, 600, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))  //Setting window size
	{
		glfwTerminate();  //If can open window this size Terminate GLFW
		exit(EXIT_FAILURE);  //And exit
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)  //Check for errors in glewInit
	{
		std::cout<< "Error: " << glewGetErrorString(error) << std::endl;
		exit(EXIT_FAILURE);
	}

	initialise();  //Intialiase everything so far then enter main render loop

	double prevTimeStamp = glfwGetTime();
	double currentTimeStamp;	//Place to store the current time
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