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
	bool GetStatus();
	void SetStatus(bool status);
	void SetRenderObject(render_object setObj);
	~GameObject();
	render_object renderObj;
private:
	bool alive;
};
