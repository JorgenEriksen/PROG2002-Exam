#include "Model.h"
#include "Camera.h"

class Sun {
private:
	glm::vec3 position;
	glm::vec3 velocity;
	std::unique_ptr<Model> sunModel;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

public:
	Sun();
	glm::vec3 getPosition();
	void move(float dt);
	void draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection);
};