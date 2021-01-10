
#pragma once

#include "Shader.h"
#include "Material.h"
#include "Renderer.h"
#include "Model.h"

#include <time.h>
#include <random>
#include <chrono>


class Tree {
private:
	int numOfTrees;

	std::vector<glm::vec3> validPositions;

	std::unique_ptr<Model> treeModel;
	std::unique_ptr<Material> treeSpec;

	std::shared_ptr<VertexArray> instancedVAO;
	std::shared_ptr<VertexBuffer> instancedVBO;
	std::shared_ptr<VertexBufferLayout> instancedVBLayout;


	std::shared_ptr<Shader> treeShader;

	std::vector<glm::vec3> treePositions;

	glm::mat4 model;

	glm::mat4* modelMatrices;

	GLuint uniformStoneTex = 0;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

public:
	Tree(std::vector<glm::vec3> intermediateHeightPositions, int numberOfTrees);
	void generateTrees();
	int calculateRandomNumber(int highestRandomNumber);
	std::vector<glm::vec3> getTreePositions();
	void draw(std::shared_ptr<Shader>& shader);
};