#include "Aerial.h"

#include <math.h> 
#define PI 3.14159265

Aerial::Aerial(int width, int height) {

	mapWidth = width;
	mapHeight = height;
	moveCounter = 0;
	float posX = 100 + calculateRandomNumber(800);
	float posZ = 100 + calculateRandomNumber(800);
	position = glm::vec3(posX, 150.0f, posZ);
	
	setRandomVelocity();

	aerialModel = std::make_unique<Model>();
	aerialModel->loadModel("assets/Objects/Eagle/Eagle02.obj");
}

Aerial::~Aerial() {};

int Aerial::calculateRandomNumber(int highestRandomNumber) {
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine(seed); // mersenne twister engine for good PRNG.
	std::uniform_int_distribution<> dist(0, highestRandomNumber);

	auto randomNum = dist(engine);
	return randomNum;
}

void Aerial::setRandomVelocity() {
	float velX = calculateRandomNumber(500) - 250;
	float velZ = calculateRandomNumber(500) - 250;

	velocity = glm::normalize(glm::vec3(velX, 0.0f, velZ));
}

glm::vec3 Aerial::getPosition() {
	return position;
}

glm::vec3 Aerial::getVelocity() {
	return velocity;
}

void Aerial::move(float dt) {
	position += velocity * dt * 10.0f;
	
	moveCounter += dt;

	if (moveCounter > 8) {
		setRandomVelocity();
		moveCounter = 0;
	}

	if (position.x < 2) position.x = mapWidth - 2;
	if (position.x > mapWidth - 2) position.x = 2;

	if (position.z < 2) position.z = mapHeight - 2;
	if (position.z > mapHeight - 2) position.z = 2;
}



void Aerial::draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection)
{
	
	shader->useShader();
	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();

	float angle = 90 - atan2(velocity.z, velocity.x) * 180 / PI; // calculates the angle from velocity

	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	aerialModel->renderElements();
}