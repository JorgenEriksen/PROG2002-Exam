#include "Water.h"



Water::Water(int w, int h) {

	mat = std::make_unique<Material>();
	mat->getTexture("assets/NormalMapping/NormalMapping_Water.png");
	mat->loadTexture();

	width = w;
	height = h;

	generate();
}

void Water::generate() {
	std::vector<GLuint> indices =
	{
		0, 1, 2,
		1, 3, 2
	};

	std::vector<GLfloat> vertices =
	{
		// x        y       z          u     v        nx     ny    nz
		 0.0f,    10.0f,    height,     0.0f,  0.0f,	 0.0f, -1.0f, 0.0f, // 0
		 width,   10.0f,    height,     25.0f, 0.0f,	 0.0f, -1.0f, 0.0f, // 1
		 0.0f,    10.0f,    0.0f,       0.0f,  25.0f,	 0.0f, -1.0f, 0.0f, // 2
		 width,   10.0f,    0.0f,       25.0f, 25.0f,    0.0f, -1.0f, 0.0f  // 3
	};

	shader->calculateAverageNormals(indices, 6, vertices, 32, 8, 5);

	VAO = std::make_shared<VertexArray>();
	VAO->bind();

	VBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(GLfloat));
	VBO->bind();

	VBLayout = std::make_unique<VertexBufferLayout>();
	VBLayout->Push<float>(3);
	VBLayout->Push<float>(2);
	VBLayout->Push<float>(3);

	VAO->addBuffer(*VBO, *VBLayout);
	IBO = std::make_shared<IndexBuffer>(&indices[0], indices.size());
}

void Water::draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection) {

	shader->useShader();

	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


	mat->useTexture();
	renderer->drawElements(VAO, IBO);

}