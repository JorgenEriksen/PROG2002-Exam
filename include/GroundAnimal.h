#pragma once

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <random>
#include <chrono>

#include "Model.h"
#include "Material.h"
#include "Shader.h"
#include "Camera.h"



class GroundAnimal {
private:
	glm::vec3 position;
	glm::vec3 velocity;
	std::vector<glm::vec3> validPositions;
	std::vector<std::vector<float>> allHeights;

	int mapWidth;
	int mapHeight;

	bool deer;

	float moveCounter;
	int groundAnimalIndex;

	std::unique_ptr<Model> groundModel;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

public:
	GroundAnimal(int width, int height, std::vector<glm::vec3> groundPositions, std::vector<std::vector<float>> heights, int index);
	~GroundAnimal();
	int calculateRandomNumber(int highestRandomNumber);
	glm::vec3 getPosition();
	glm::vec3 getVelocity();
	void move(float dt, std::vector<std::unique_ptr<GroundAnimal>>& allAnimals, std::vector<glm::vec3> treePositions);
	void setRandomVelocity(float dt);
	bool collision(std::vector<std::unique_ptr<GroundAnimal>>& allAnimals, glm::vec3 projectedPosition, std::vector<glm::vec3> treePositions);
	float distance(int pos1, int pos2);
	void draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection);
};