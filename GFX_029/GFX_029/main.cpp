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

#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"

using namespace std;
using namespace glm;

GLuint shaders[2];  // Declare 2 shaders
GLuint program;		// Declare one program paddle
GLint colourUniform;

vector< vector<Brick> > brick;
Ball ball;				// Declare render_object for geometry
Paddle paddle;	// Declare render_object for geometry

float horizontalLimit = 5.0f;
float paddleSpeed = 8.0f;
int rows = 4;
int columns = 8;

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
	geometry* geom = createQuad();			// Create a quad to add to all the objects in the scene
	render_object temp;	

	for(int i = 0; i < rows; ++i)	// Each line
	{
		vector<Brick> tempB;
		for(int j = 0; j < columns; ++j)			// Each individual brick
		{
			Brick b;
			temp.geometry = geom;
			temp.transform.position = vec3(-4.0f + (j*1.2f), 3.0f - i*0.8f, 0.0f);
			temp.transform.scale = vec3(0.6f, 0.30f, 0.0f);
			temp.colour = vec4(i - 2, i - 1, i + 1 , 1.0f);
			b.SetRenderObject(temp);
			if(i % 2 == 0) b.SetHealth(1);
			else b.SetHealth(2);
			b.UpdateColour();
			tempB.push_back(b);
		}
		brick.push_back(tempB);
	}

	paddle.renderObj.geometry = geom;
	paddle.renderObj.transform.position -= vec3(0.0f, 3.0f, 0.0f);
	paddle.renderObj.transform.scale.y = 0.1f;
	paddle.renderObj.transform.scale.x = 1.0f;
	paddle.renderObj.colour = vec4(0.0f, 1.0f, 0.0f, 1.0f);  // Set colour 

	geom = createSphere(20,20);
	
	temp.geometry = geom;
	temp.transform.scale = vec3(0.1f, 0.1f, 0.1f);
	temp.transform.position -= vec3(0.0f, 2.5f, 0.0f);
	temp.colour = vec4(1.0f, 1.0f, 0.4f, 1.0f);  // Set colour 
	ball.SetRenderObject(temp);
}

void update (double deltaTime)  
{
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	paddle.SetVelocity(vec3(0.0f, 0.0f, 0.0f));
	if (glfwGetKey(GLFW_KEY_RIGHT) || glfwGetKey('D') )
	{
		paddle.SetVelocity(vec3(paddleSpeed, 0.0f, 0.0f));
		//paddle.renderObj.transform.position.x += paddleSpeed * deltaTime;
	}
	if (glfwGetKey(GLFW_KEY_LEFT) || glfwGetKey('A') )
	{
		paddle.SetVelocity(vec3(-paddleSpeed, 0.0f, 0.0f));
		//paddle.renderObj.transform.position.x -= paddleSpeed * deltaTime;
	}
	if (glfwGetKey(GLFW_KEY_SPACE) && !ball.GetStatus())
	{
		ball.SetStatus(true);
		ball.StartBall(paddle.renderObj.transform.position);
	}

	paddle.Update(deltaTime);

	// Check paddle limits
	//if(glm::abs(paddle.transform.position.x) > 4.0f) paddle.transform.position.x = 4.0f; // Fix this to use ABS so the check happens in one line
	if(paddle.renderObj.transform.position.x > horizontalLimit) 
		paddle.renderObj.transform.position.x = horizontalLimit;
	else if(paddle.renderObj.transform.position.x < -horizontalLimit) 
		paddle.renderObj.transform.position.x = -horizontalLimit;
	
	if(ball.renderObj.transform.position.x > horizontalLimit) 
		ball.Bounce();
	else if(ball.renderObj.transform.position.x < -horizontalLimit) 
		ball.Bounce();
	if(ball.renderObj.transform.position.y > horizontalLimit)
	{
		glm::vec3 temp = glm::vec3(ball.GetVelocity());
		temp.y *= -1.0f;
		ball.SetVelocity(temp);
	}
	else if(ball.renderObj.transform.position.y < -horizontalLimit + 1.0f) 
		ball.ResetBall();

	// TODO: move this collision and bounding box stuff into a better place
	// Check collisions with bricks
	for(vector<Brick> r : brick)	// Each line	
		for(Brick b : r)			// Each individual brick
		{
			if(b.GetStatus())
			{
				//b.SetStatus(false);
				b.DeductHealth(1);
				if((ball.renderObj.transform.position.y - ball.renderObj.transform.scale.y <= b.renderObj.transform.position.y + b.renderObj.transform.scale.y 
					&& ball.renderObj.transform.position.y + ball.renderObj.transform.scale.y >= b.renderObj.transform.position.y - b.renderObj.transform.scale.y) 
				&& (ball.renderObj.transform.position.x + ball.renderObj.transform.scale.x >= b.renderObj.transform.position.x - (b.renderObj.transform.scale.x * 2.0f) 
					&& ball.renderObj.transform.position.x - ball.renderObj.transform.scale.x <= b.renderObj.transform.position.x + (b.renderObj.transform.scale.x * 2.0f)))
				{
					// There was a collision with this brick
					// TODO: Deduct life from brick, change its colour and possibly destroy it?
					/*
					b.DeductHealth(1);
					if(b.GetHealth() == 2)
						b.renderObj.colour = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
					else b.renderObj.colour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
					*/
					// calculate which way to deflect the ball
					glm::vec3 temp = glm::vec3(ball.GetVelocity());
					if((ball.renderObj.transform.position.x + ball.renderObj.transform.scale.x < b.renderObj.transform.position.x + (b.renderObj.transform.scale.x) 
						&& ball.renderObj.transform.position.x - ball.renderObj.transform.scale.x > b.renderObj.transform.position.x - b.renderObj.transform.scale.x))
					{					
						temp.y *= -1.0f;
						ball.SetVelocity(temp);
					}
					else
					{
						temp.x *= -1.0f;
						ball.SetVelocity(temp);
					}
					//ball.direction.x *= -1.0f;
					//ball.Update(deltaTime);
					break;
				}

			}
		}
	// Check collision with paddle
	if((ball.renderObj.transform.position.y - ball.renderObj.transform.scale.y <= paddle.renderObj.transform.position.y + paddle.renderObj.transform.scale.y 
		&& ball.renderObj.transform.position.y + ball.renderObj.transform.scale.y >= paddle.renderObj.transform.position.y - paddle.renderObj.transform.scale.y) 
	&& (ball.renderObj.transform.position.x + ball.renderObj.transform.scale.x >= paddle.renderObj.transform.position.x - paddle.renderObj.transform.scale.x 
		&& ball.renderObj.transform.position.x - ball.renderObj.transform.scale.x <= paddle.renderObj.transform.position.x + paddle.renderObj.transform.scale.x))
	{
		glm::vec3 temp = glm::vec3(ball.GetVelocity());
		if(abs(paddle.GetVelocity().x) > 0)
		{
			ball.SetVelocity(glm::normalize
			(glm::vec3(ball.GetVelocity() + paddle.GetVelocity())));
		}
		if(ball.renderObj.transform.position.x + ball.renderObj.transform.scale.x < paddle.renderObj.transform.position.x + paddle.renderObj.transform.scale.x 
			&& ball.renderObj.transform.position.x - ball.renderObj.transform.scale.x < paddle.renderObj.transform.position.x - paddle.renderObj.transform.scale.x)
		{					
			temp.x *= -1.0f;
			ball.SetVelocity(temp);
		}
		else
		{
			temp.y *= -1.0f;
			ball.SetVelocity(temp);
		}
		
		//ball.SetVelocity(ball.GetVelocity() + (paddle.GetVelocity()));
		//ball.SetDirection(glm::vec3(ball.GetDirection().y) *= -1.0f);
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
	glUseProgram(program);			//Use program we created
	for(vector<Brick> r : brick)	// Each line	
		for(Brick b : r)			// Each individual brick
		{
			if(b.GetStatus())
			{
				glUniform4fv(colourUniform, 1, value_ptr(b.renderObj.colour));
				b.renderObj.render(view);
			}
		}
		glUniform4fv(colourUniform, 1, value_ptr(ball.renderObj.colour));	
	ball.renderObj.render(view);	// Call render as before
	glUniform4fv(colourUniform, 1, value_ptr(paddle.renderObj.colour));	
	paddle.renderObj.render(view);	// Call render as before
	glUseProgram(0);		// Tell OpenGL to stop using the program
	glfwSwapBuffers();
}

void cleanup()  //Allow us to clean up used resources in our application
{
	if (paddle.renderObj.geometry) delete paddle.renderObj.geometry;
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