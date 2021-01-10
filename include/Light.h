#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



class Light
{

//These are protected so that the sub classes can access them
protected:

	glm::vec3 colour;

	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;


public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
	void setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);
	~Light();

};