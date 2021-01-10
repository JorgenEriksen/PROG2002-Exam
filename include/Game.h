#pragma once

#include <iostream>
#include <set>
#include <cmath>
#include <vector>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <chrono>
#include <thread>

#include "GLWindow.h"
#include "Camera.h"
#include "FrameBuffer.h"

#include "Material.h"
#include "Renderer.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Water.h"
#include "Map.h"
#include "Tree.h"
#include "Sun.h"
#include "Aerial.h"
#include "GroundAnimal.h"
#include "Duck.h"

class Game 
{

private:

	GLfloat deltaTime;
	GLfloat lastTime;
	GLfloat now;

	int numberOfAerials;
	int numberOfGroundAnimals;
	int numberOfTrees;

	std::vector<std::unique_ptr<Aerial>> aerials;
	std::vector<std::unique_ptr<GroundAnimal>> groundAnimals;


	std::unique_ptr<FrameBuffer> frameBuffer;


	std::shared_ptr<Renderer> renderer;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> mapShader;
	std::shared_ptr<Shader> treeShader;
	std::shared_ptr<Shader> eagleShader;
	std::shared_ptr<Shader> sunShader;




	std::shared_ptr<Camera> camera;
	//std::unique_ptr<Aerial> aerial;
	std::unique_ptr<Map> map;
	std::unique_ptr<Sun> sun;
	std::unique_ptr<Duck> duck;
	std::unique_ptr<Tree> tree;
	std::unique_ptr<Water> water;

	std::shared_ptr<GLWindow> mainWindow;


	std::shared_ptr<DirectionalLight> mapLight;
	std::shared_ptr<DirectionalLight> treeLight;

	PointLight pointLights[15];
	SpotLight spotLights[3];

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;
	GLuint uniformEyePosition;
	GLuint uniformAmbientIntensity;
	GLuint uniformDiffuseIntensity;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	GLuint uView;
	GLuint uProj;
	GLuint uMod;


	glm::mat4 model;
	glm::mat4 projection;


	glm::vec3 lowerLight;

public:

	Game();
	~Game();

	void generateGame(std::shared_ptr<GLWindow>& mainWindow);
	void updateGame(std::shared_ptr<GLWindow>& mainWindow);

	void generateShaders();
	void generateLights();

	void updateSunLight();

	void updateLights(std::shared_ptr<Shader>& argShader);
	void updateMVP(std::shared_ptr<Shader>& argShader);
	void updateTime();

};
