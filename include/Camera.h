#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <vector>

#include "Renderer.h"
#include "Material.h"
#include "Shader.h"

class Camera
{

private:

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	bool firstPerson;


	std::shared_ptr<Shader> shader;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

	enum class direction {
		UP, DOWN, LEFT, RIGHT
	};

	void update();

public:

	Camera(glm::vec3 startPosition, glm::vec3 startUp, 
		      GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

	void keyControls(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat changeX, GLfloat changeY);
	void thirdPersonCalculation(glm::vec3 objectPosition, glm::vec3 objectVelocity);


	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();

	glm::mat4 calculateViewMatrix();




};

