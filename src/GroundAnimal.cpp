#include "GroundAnimal.h"

#include <math.h> 
#define PI 3.14159265

GroundAnimal::GroundAnimal(int width, int height, std::vector<glm::vec3> groundPositions, std::vector<std::vector<float>> heights, int index) {
	mapWidth = width;
	mapHeight = height;
	validPositions = groundPositions;
	allHeights = heights;
	moveCounter = 0.0f;
	groundAnimalIndex = index;
	
	int random = calculateRandomNumber(groundPositions.size() - 1);
	position = groundPositions[random];
	//position = glm::vec3(150.0f, 15.0f, 500.0f);
	setRandomVelocity(0.01f);


	groundModel = std::make_unique<Model>();

	if (index % 2 == 0) {
		deer = true;
		groundModel->loadModel("assets/Objects/Deer1/12961_White-Tailed_Deer_v1_l2.obj");
	}
	else {
		deer = false;
		groundModel->loadModel("assets/Objects/MooseFemale/12959_Moose_Female_v1_l3.obj");
	}
}

GroundAnimal::~GroundAnimal() {};


int GroundAnimal::calculateRandomNumber(int highestRandomNumber) {
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine(seed); // mersenne twister engine for good PRNG.
	std::uniform_int_distribution<> dist(0, highestRandomNumber);

	auto randomNum = dist(engine);
	return randomNum;
}

glm::vec3 GroundAnimal::getPosition() {
	return position;
}

glm::vec3 GroundAnimal::getVelocity() {
	return velocity;
}

// allAnimals and treePositions will only be used in collision()
void GroundAnimal::move(float dt, std::vector<std::unique_ptr<GroundAnimal>>& allAnimals, std::vector<glm::vec3> treePositions) {
	moveCounter += dt;
	glm::vec3 projectedPosition = position + velocity * dt * 4.0f; // the projected position

	if (moveCounter > 20.0f) { // time to calculate new direction
		moveCounter = 0;
		setRandomVelocity(dt); 
	} else if (projectedPosition.z > 5 && projectedPosition.z < mapHeight - 5 && projectedPosition.x > 5 && projectedPosition.x < mapWidth - 5) { // to avoid vector error
		if (allHeights[projectedPosition.z][projectedPosition.x] <= 10 || collision(allAnimals, projectedPosition, treePositions))  // if water or collision
			velocity = -velocity; // turn around
	}
	
	position += velocity * dt * 4.0f;

	if (position.z >= 0 && position.z < mapHeight-1 && position.x >= 0 && position.x < mapWidth-1) { // to avoid vector error if they go outside of map (which they in theory would not do)
		position.y = allHeights[position.z][position.x]; // correct height to animal
	}
	

}

// checks collision with deer or tree
bool GroundAnimal::collision(std::vector<std::unique_ptr<GroundAnimal>> & allAnimals, glm::vec3 projectedPosition, std::vector<glm::vec3> treePositions) {
	
	// check animal collision
	for (int i = 0; i < allAnimals.size(); i++) {
		glm::vec3 animalPosition = allAnimals[i]->getPosition();
		if ( i != groundAnimalIndex && (distance(animalPosition.x, projectedPosition.x) < 1.0f || distance(animalPosition.z, projectedPosition.z) < 1.0f)) { // checks if projected position is close to another animal
			return true;
		}
	}
	
	// check tree collision
	for (int i = 0; i < treePositions.size(); i++) {
		if (distance(treePositions[i].x, projectedPosition.x) < 1.0f || distance(treePositions[i].z, projectedPosition.z) < 1.0f) { // checks if projected position is close to tree
			return true;
		}
	}

	return false; // no collision
}

float GroundAnimal::distance(int pos1, int pos2)
{
	return sqrt(pow(pos1 - pos2, 2));
}


void GroundAnimal::setRandomVelocity(float dt) {
	glm::vec3 newVelocity;
	float velX, velZ;


	if (position.x < 50.0f) { 
		velX = calculateRandomNumber(500); // random number from 0 to 500 (to avoid edge of map)
	}
	else if (position.x > mapWidth - 50.0f) {
		velX = 500-calculateRandomNumber(500); // random number from -500 to 0 (to avoid edge of map)
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



void GroundAnimal::draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection) {
	shader->useShader();
	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();

	//float angle = 90 - atan2(velocity.z, velocity.x) * 180 / PI; // calculates the angle from velocity

	model = glm::translate(model, position);
	float angle = -90.0f;
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f)); // flips the deer on the right side

	angle = 360 - atan2(velocity.z, velocity.x) * 180 / PI; // calculates the angle from velocity
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	
	if (deer) {
		model = glm::scale(model, glm::vec3(0.3f)); // makes the deer smaler
	}
	else {
		model = glm::scale(model, glm::vec3(0.1f)); // makes the moose smaler
	}

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	groundModel->renderElements();
}