#pragma once

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <random>
#include <chrono>

#include "Model.h"
#include "Material.h"
#include "Camera.h"

class Aerial {

private:
	glm::vec3 position;
	glm::vec3 velocity;

	int mapWidth;
	int mapHeight;

	float moveCounter;

	std::unique_ptr<Model> aerialModel;
	std::unique_ptr<Material> aerialMat;
	std::unique_ptr<Shader> aerialShader;

	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;
public:

	Aerial(int width, int height);
	~Aerial();
	int calculateRandomNumber(int highestRandomNumber);
	void setRandomVelocity();
	void move(float dt);
	glm::vec3 getPosition();
	glm::vec3 getVelocity();
	void draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection);
};