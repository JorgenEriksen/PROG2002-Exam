#pragma once

#include <iostream>
#include <vector>
#include "Shader.h"
#include "Camera.h"

class Map {
private:
	int width; 
	int height;

	std::vector<glm::vec3> intermediateHeightPositions;
	std::vector<glm::vec3> groundPositions;
	std::vector<glm::vec3> waterPositions;
	std::vector<std::vector<float>> allHeights;

	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;
	std::unique_ptr<Shader> shader;

	std::shared_ptr<VertexArray>		VAO;
	std::unique_ptr<VertexBuffer>		VBO;
	std::unique_ptr<VertexBufferLayout> VBLayout;
	std::shared_ptr<IndexBuffer>		IBO;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

	std::shared_ptr<Renderer> renderer;

	std::unique_ptr <Material> mat;

public:
	Map();
	~Map();
	bool load_image(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y);
	void draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection);
	void generateMap();
	int getMapWidth();
	int getMapHeight();
	std::vector<glm::vec3> getWaterPositions();
	std::vector<glm::vec3> getGroundPositions();
	std::vector<glm::vec3> getIntermediateHeightPositions();
	std::vector<std::vector<float>> getAllHeights();
};