#pragma once

#include <GL\glfw.h>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>  //For mat4_cast
#include <glm\gtc\matrix_transform.hpp>

struct geometry
{
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
};

struct transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	transform()
	{
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void move(const glm::vec3& movement)
	{
		position += movement; 
	}

	void rotate(float angle, const glm::vec3& axis)
	{
		rotation = glm::rotate(rotation, glm::degrees(angle), axis);
	}

	glm::mat4 getTransformationMatrix()
	{
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
		matrix = glm::scale(matrix, scale);
		glm::mat4 rotMatrix = glm::mat4_cast(rotation);
		matrix *= rotMatrix;
		return matrix;
	}
};

struct render_object
{
	geometry* geometry;
	transform transform;
	glm::vec4 colour;

	void render(const glm::mat4& view)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(view * transform.getTransformationMatrix()));
		glVertexPointer(3, GL_FLOAT, 0, &geometry ->vertices[0]);
		glColor3fv(glm::value_ptr(colour));
		if (geometry->indices.size() != 0)
			glDrawElements(GL_TRIANGLES, geometry->indices.size(), GL_UNSIGNED_INT, &geometry->indices[0]);
		else
			glDrawArrays(GL_TRIANGLES, 0, geometry->vertices.size());
	}
};

geometry* createQuad();
geometry* createBox();
geometry* createTetrahedron();
geometry* createPyramid();
geometry* createDisk(int slices);
geometry* createCylinder(int stacks, int slices);
geometry* createSphere(int stacks, int slices);
geometry* createTorus(float radius, int stacks, int slices);
geometry* createPlane(int width, int depth);
geometry* createSierpinski(int divisions);