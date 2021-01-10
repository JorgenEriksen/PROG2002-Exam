#pragma once

#include <time.h>
#include <random>
#include <chrono>
#include "Model.h"
#include "Material.h"
#include "Camera.h"



class Duck {
private:
	glm::vec3 position;
	glm::vec3 velocity;

	std::vector<glm::vec3> validPositions;
	std::vector<std::vector<float>> allHeights;

	std::unique_ptr<Model> duckModel;
	std::unique_ptr<Material> duckMat;
	std::unique_ptr<Shader> duckShader;

	int mapWidth;
	int mapHeight;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

public:
	Duck(int width, int height, std::vector<glm::vec3> waterPositions, std::vector<std::vector<float>> heights);
	~Duck();
	int calculateRandomNumber(int highestRandomNumber);
	void move(float dt);
	void setRandomVelocity(float dt);
	void draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection);
};