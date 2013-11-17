#include "Brick.h"

// COLOURS - TODO: shift these to their own helper class
glm::vec4 Black = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 Orange = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
glm::vec4 Yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);


Brick::Brick()
{ 
	health = 1;
	SetStatus(true);
}

Brick::~Brick() { }

int Brick::GetHealth()
{
	return health;
}

void Brick::UpdateColour()
{
	if(health == 0) ChangeColour(Colors::Black);
	else if(health > 4) ChangeColour(Colors::Red);
	else if(health == 4) ChangeColour(Colors::Blue);
	else if(health == 3) ChangeColour(Colors::LightSteelBlue);
	else if(health == 2) ChangeColour(Colors::Orange);
	else if(health == 1) ChangeColour(Colors::Yellow);
}

void Brick::SetHealth(int health)
{
	Brick::health = health;
	UpdateColour();
}

void Brick::DeductHealth(int loss)
{
	health -= loss;
	if(health == 0) SetStatus(false);
	UpdateColour();
}