#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"

class Water {
private:

	std::unique_ptr<Shader> shader;

	float width;
	float height;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

	std::shared_ptr<VertexArray>		VAO;
	std::unique_ptr<VertexBuffer>		VBO;
	std::unique_ptr<VertexBufferLayout> VBLayout;
	std::shared_ptr<IndexBuffer>		IBO;

	std::shared_ptr<Renderer> renderer;
	std::unique_ptr <Material> mat;

public:
	Water(int w, int h);
	void generate();
	void draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection);
};