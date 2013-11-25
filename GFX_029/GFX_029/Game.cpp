#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>

vector< vector<Brick> > brick;
Ball ball;				// Declare render_object for geometry
Paddle paddle;			// Declare render_object for geometry

float hLimit = 16.0f;
float vLimit = 14.0f;
float paddleSpeed = 12.0f;
int rows = 20;
int columns = 26;
int offset = 0;

vector<vector<int>> grid(1);

Game::Game() 
{
	Initialise();
}

Game::~Game() { }

void Game::Initialise()
{
	geometry* geom = createQuad();			// Create a quad to add to all the objects in the scene
	render_object temp;	

	ifstream fin("map1.csv", ifstream::in);
    char singleCharacter;
    //Get a character from the input file
    fin.get(singleCharacter);

	int row = 0;
	int column = 0;

	while (fin.good())
    {
		if(singleCharacter - '0' >= 0 && singleCharacter - '0' < 10)
		{
			grid[row].push_back(singleCharacter - '0');
			++column;
			if(column >= columns)
			{
				++row;
				vector<int> temp(0);
				grid.push_back(temp);
				column = 0;
			}
		}
        //std::cout << singleCharacter;
        fin.get(singleCharacter);
    }

	for(int i = 0; i < rows; ++i)			// Each row of bricks
	{
		vector<Brick> tempB;				// Place holder so we can push rows back
		for(int j = 0; j < columns; ++j)	// Each individual brick
		{
			Brick b;
			temp.geometry = geom;
			temp.transform.position = vec3(-15.0f + (j*1.2f), 10.0f - i*0.8f, 0.0f);
			temp.transform.scale = vec3(0.6f, 0.30f, 0.0f);
			temp.colour = vec4(i - 2, i - 1, i + 1 , 1.0f);
			b.SetRenderObject(temp);
			b.SetHealth(grid[i][j]);
			//if(i % 2 == 0) b.SetHealth(1);
			//else b.SetHealth(2);
			b.UpdateColour();
			tempB.push_back(b);
		}
		brick.push_back(tempB);
	}

	paddle.renderObj.geometry = geom;
	paddle.renderObj.transform.position -= vec3(0.0f, 10.0f, 0.0f);
	paddle.renderObj.transform.scale.y = 0.1f;
	paddle.renderObj.transform.scale.x = 1.0f;
	paddle.renderObj.colour = vec4(0.0f, 1.0f, 0.0f, 1.0f);  // Set colour 
	paddle.SetStatus(true);
	geom = createSphere(20,20);
	
	temp.geometry = geom;
	temp.transform.scale = vec3(0.1f, 0.1f, 0.1f);
	temp.transform.position -= vec3(10.0f, 2.5f, 0.0f);
	temp.colour = vec4(1.0f, 1.0f, 0.4f, 1.0f);  // Set colour 
	ball.SetRenderObject(temp);
}

void Game::UpdateObjectList()
{
	objectsInScene.clear();
	for(vector<Brick> r : brick)
		for(Brick b : r)
		{
			objectsInScene.push_back(b);
		}
	objectsInScene.push_back((GameObject)paddle);
	objectsInScene.push_back((GameObject)ball);
	
}

bool Game::Update(float deltaTime)
{
	bool running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	paddle.SetVelocity(vec3(0.0f, 0.0f, 0.0f));

	if (glfwGetKey(GLFW_KEY_UP))
	{
		ball.SetSpeed(ball.GetSpeed() + (1.0f * deltaTime));
		//paddle.renderObj.transform.position.x += paddleSpeed * deltaTime;
	}
	if (glfwGetKey(GLFW_KEY_DOWN))
	{
		ball.SetSpeed(ball.GetSpeed() - (1.0f * deltaTime));
		//paddle.renderObj.transform.position.x -= paddleSpeed * deltaTime;
	}

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
	ball.Update(deltaTime);
	// Check paddle limits
	//if(glm::abs(paddle.transform.position.x) > 4.0f) paddle.transform.position.x = 4.0f; // Fix this to use ABS so the check happens in one line
	if(paddle.renderObj.transform.position.x > hLimit) 
		paddle.renderObj.transform.position.x = hLimit;
	else if(paddle.renderObj.transform.position.x < -hLimit) 
		paddle.renderObj.transform.position.x = -hLimit;
	
	if(ball.renderObj.transform.position.x > hLimit) 
		ball.Bounce();
	else if(ball.renderObj.transform.position.x < -hLimit) 
		ball.Bounce();
	if(ball.renderObj.transform.position.y > vLimit - 2.0f)
	{
		glm::vec3 temp = glm::vec3(ball.GetVelocity());
		temp.y *= -1.0f;
		ball.SetVelocity(temp);
	}
	else if(ball.renderObj.transform.position.y < -vLimit + 1.0f) 
		ball.ResetBall();

	// TODO: move this collision and bounding box stuff into a better place
	// Check collisions with bricks
	bool collision = false;

	for(int i = 0; i < brick.size(); ++i)
	{
		for(int j = 0; j < brick[i].size(); ++j)
		{
			if(brick[i][j].GetStatus())
			{
				//brick[i][j].SetStatus(false);
				//brick[i][j].DeductHealth(1);
				if((ball.GetPosition().y - ball.GetYBound() <= brick[i][j].GetPosition().y + brick[i][j].GetYBound()
					&& ball.GetPosition().y + ball.GetYBound() >= brick[i][j].GetPosition().y - brick[i][j].GetYBound()) 
					&& (ball.GetPosition().x + ball.GetXBound() >= brick[i][j].GetPosition().x - brick[i][j].GetXBound()
					&& ball.GetPosition().x - ball.GetXBound() <= brick[i][j].GetPosition().x + brick[i][j].GetXBound()))
				{
					// There was a collision with this brick
					// TODO: Deduct life from brick, change its colour and possibly destroy it?
					if(brick[i][j].GetHealth() == 2)
						ball.RandomColour();
					brick[i][j].DeductHealth(1);
					//if(brick[i][j].GetHealth() == 2)
					//	brick[i][j].renderObj.colour = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
					//else brick[i][j].renderObj.colour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
					//ball.renderObj.transform.position += ball.GetVelocity() * glm::normalize(glm::vec3(ball.GetPosition() - paddle.GetPosition()));
					// Calculate which way to deflect the ball
					glm::vec3 temp = glm::vec3(ball.GetVelocity());
					if((ball.GetPosition().x + ball.GetXBound() < brick[i][j].GetPosition().x + brick[i][j].GetXBound())
						&& (ball.GetPosition().x - ball.GetXBound() > brick[i][j].GetPosition().x- brick[i][j].GetXBound()))
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
					collision = true;	
				}
				if(collision) break;
			}
			if(collision) break;
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
			ball.SetVelocity(glm::normalize (glm::vec3(ball.GetVelocity() + paddle.GetVelocity())));
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

	UpdateObjectList();
	return running;
}

vector<GameObject> Game::GetAllObjectsInScene()
{
	return objectsInScene;
}

vector<GameObject> Game::GetActiveObjectsInScene()
{
	vector<GameObject> activeObjs;
	for(int i = 0; i < objectsInScene.size(); ++i) 
		if(objectsInScene[i].GetStatus() == true) activeObjs.push_back(objectsInScene[i]);
	return activeObjs;
}