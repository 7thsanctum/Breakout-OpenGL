#include "Brick.h"

// COLOURS - TODO: shift these to their own helper class
glm::vec4 Black = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 Orange = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
glm::vec4 Yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);


Brick::Brick()
{ 
	health = 1;
}


Brick::~Brick() { }

int Brick::GetHealth()
{
	return health;
}

void Brick::UpdateColour()
{
	if(health == 2)
		renderObj.colour = Orange;
	else renderObj.colour = Yellow;
}

void Brick::ChangeColour(glm::vec4 newColour)
{	
	renderObj.colour = newColour;
}


void Brick::SetHealth(int health)
{
	Brick::health = health;
}

void Brick::DeductHealth(int loss)
{
	health -= loss;
	UpdateColour();
}