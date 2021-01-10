#include "Sun.h"

Sun::Sun() {
	position = glm::vec3(1130.0f, 190.0f, 500.0f);
	velocity = glm::vec3(0.0f, -1.0f, 0.0f);
	sunModel = std::make_unique<Model>();
	sunModel->loadModel("assets/Objects/Sun/Sun.obj");
}

glm::vec3 Sun::getPosition() {
	return position;
}

void Sun::move(float dt) {
	position += velocity * dt * 7.0f;
	if (position.y < - 2) velocity.y = 1.0f;
	if (position.y > 200) velocity.y = -1.0f;

	
}


void Sun::draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection)
{
	shader->useShader();
	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();

	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(300.0f));

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	sunModel->renderElements();
}
