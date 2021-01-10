#include "Camera.h"


Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition; 
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f); 

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;


	firstPerson = true;


	update();

    std::vector<GLuint> indices = 
   {
        0, 1, 2, 
        1, 3, 2,
   };


   std::vector<GLfloat> vertices = 
    {
         //x        y       z          u     v       
         0.0f,    0.0f,    2.0f,      0.0f,  0.0f,	  // 0
         2.0f,    0.0f,    2.0f,      1.0f,  0.0f,	  // 1
		 0.0f,    0.0f,    0.0f,      0.0f,  1.0f,	  // 2
		 2.0f,    0.0f,    0.0f,      1.0f,  1.0f     // 3
   };


	static const char* vShader = "assets/shaders/texture.vert";
	static const char* fShader = "assets/shaders/texture.frag";

	shader = std::make_shared<Shader>();
	shader->createShaderFromFile(vShader, fShader);

	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();
}


Camera::~Camera() {}


void Camera::keyControls(bool* keys, GLfloat deltaTime) {

	if (firstPerson) {
		GLfloat velocity = 10 * deltaTime;
		if (keys[GLFW_KEY_W])
		{
			glm::vec3 direction = front;
			position += front * velocity * moveSpeed;
		}

		if (keys[GLFW_KEY_S])
		{
			glm::vec3 direction = -front;

			position -= front * velocity * moveSpeed;
		}

		if (keys[GLFW_KEY_A])
		{
			glm::vec3 direction = -right;
			position -= right * velocity * moveSpeed;
		}

		if (keys[GLFW_KEY_D])
		{
			glm::vec3 direction = right;
			position += right * velocity * moveSpeed;
		}
	}

	if (keys[GLFW_KEY_C])
	{
		firstPerson = !firstPerson;
	}
}


void Camera::thirdPersonCalculation(glm::vec3 objectPosition, glm::vec3 objectVelocity) {
	if (!firstPerson) { // if thirdpersion
		position = objectPosition - (objectVelocity * 40.0f);
		position.y += 20.0f;
		front = objectVelocity;
		
	}
}



void Camera::mouseControl(GLfloat changeX, GLfloat changeY) 
{
	if (firstPerson) {
		changeX *= turnSpeed;
		changeY *= turnSpeed;

		yaw += changeX;
		pitch += changeY;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		} // lock the up-and down scroll of the mouse.

		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
	}

	update();
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}


glm::mat4 Camera::calculateViewMatrix() 
{
	//can place an object to lookAt where "position + front" is. Third-person view
	return glm::lookAt(position, position + front, up);
}




void Camera::update() 
{
	if (firstPerson) {
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch)); // if you want to fly, use this instead of 0.0f
		//front.y = 0.0f;
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
}
