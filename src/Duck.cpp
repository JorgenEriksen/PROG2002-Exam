#include "Duck.h"

#include <math.h> 
#define PI 3.14159265

Duck::Duck(int width, int height, std::vector<glm::vec3> waterPositions, std::vector<std::vector<float>> heights){

	mapWidth = width;
	mapHeight = height;
	validPositions = waterPositions;
	allHeights = heights;


	int random = calculateRandomNumber(validPositions.size() - 1);
	position = validPositions[random];
	position.y = 10.0f;

	float velX = calculateRandomNumber(10) - 5;
	float velZ = calculateRandomNumber(10) - 5;

	velocity = glm::normalize(glm::vec3(velX, 0.0f, velZ));
	duckModel = std::make_unique<Model>();
	duckModel->loadModel("assets/Objects/Duck/12248_Bird_v1_L2.obj");

}

Duck::~Duck() {};

int Duck::calculateRandomNumber(int highestRandomNumber) {
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine(seed); // mersenne twister engine for good PRNG.
	std::uniform_int_distribution<> dist(0, highestRandomNumber);

	auto randomNum = dist(engine);
	return randomNum;
}

void Duck::move(float dt) {
	
	glm::vec3 projectedPosition = position + velocity * dt * 4.0f;
	
	bool forceNewVelocity = false;
	if (projectedPosition.x < 2) {
		velocity.x = 1.0f;
		forceNewVelocity = true;
	} 
	if (projectedPosition.x > mapWidth-2) {
		velocity.x = -1.0f;
		forceNewVelocity = true;
	}
	if (projectedPosition.z < 2) {
		velocity.z = 1.0f;
		forceNewVelocity = true;
	}
	if (projectedPosition.x > mapHeight- 2) {
		velocity.z = -1.0f;
		forceNewVelocity = true;
	}
	if(!forceNewVelocity){ // to prevent vector error.
		int counter = 0; // incase it gets stuck in the loop for some reason.
		while(allHeights[projectedPosition.z][projectedPosition.x] > 10.0f && counter < 10) {
			setRandomVelocity(dt);
			glm::vec3 projectedPosition = position + velocity * dt * 4.0f;
			counter++;
			//std::cout << allHeights[projectedPosition.z][projectedPosition.x] << ": " << projectedPosition.z << ", " << projectedPosition.x << "\n";
		}
	}
	position += velocity * dt * 4.0f;
}

void Duck::setRandomVelocity(float dt) {
	glm::vec3 newVelocity;
	float velX, velZ;
	if (position.x < 50.0f) {
		velX = calculateRandomNumber(500); // random number from 0 to 500 (to avoid edge of map)
	}
	else if (position.x > mapWidth - 50.0f) {
		velX = 500 - calculateRandomNumber(500); // random number from -500 to 0 (to avoid edge of map)
	}
	else {
		velX = 250 - calculateRandomNumber(500); // random number from 250 to 500 
	}

	if (position.z < 50.0f) {
		velZ = calculateRandomNumber(500); // random number from 0 to 500 (to avoid edge of map)
	}
	else if (position.z > mapHeight - 50.0f) {
		velZ = 500 - calculateRandomNumber(500); // random number from -500 to 0 (to avoid edge of map)
	}
	else {
		velZ = 250 - calculateRandomNumber(500); // random number from 250 to 500
	}

	newVelocity = glm::normalize(glm::vec3(velX, 0.0f, velZ)); // new velocity with 1 in length

	velocity = newVelocity;
}

void Duck::draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection) {
	shader->useShader();
	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();

	//float angle = 90 - atan2(velocity.z, velocity.x) * 180 / PI; // calculates the angle from velocity



	model = glm::translate(model, position);
	//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	float angle = -90.0f;
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

	angle = 90 - atan2(velocity.z, velocity.x) * 180 / PI; // calculates the angle from velocity
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, glm::vec3(0.2f)); // makes the duck smaler

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	duckModel->renderElements();
}