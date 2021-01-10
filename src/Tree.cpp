#include "Tree.h"


Tree::Tree(std::vector<glm::vec3> intermediateHeightPositions, int numberOfTrees) {
	std::unique_ptr<Model> treeModel;
	validPositions = intermediateHeightPositions;
	//std::unique_ptr<Material> treeSpec;

	numOfTrees = numberOfTrees;
	generateTrees();
	treeSpec = std::make_unique<Material>();
	treeSpec->getSpecular(2.0f, 128);
	

	
}

void Tree::generateTrees() {
	treeModel = std::make_unique<Model>();
	//treeModel->loadModel("assets/Objects/PineTree2/10447_Pine_Tree_v1_L3b.obj");
	treeModel->loadModel("assets/Objects/Tree/Tree.obj");
	for (int i = 0; i < numOfTrees; i++) {
		int random = calculateRandomNumber(validPositions.size());
		treePositions.push_back(validPositions[random]);
		//treePositions.push_back(glm::vec3(150.0f + i, 150.0f, 150.0f + i));
	}

	modelMatrices = new glm::mat4[numOfTrees];


	for (unsigned int i = 0; i < numOfTrees; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, treePositions[i]);
		modelMatrices[i] = model;
	}

	instancedVAO = std::make_shared<VertexArray>();
	instancedVAO = treeModel->getVertexArray();
	instancedVAO->bind();

	instancedVBO = std::make_shared<VertexBuffer>(&modelMatrices[0], numOfTrees * sizeof(glm::mat4));
	instancedVBO->bind();

	instancedVAO->addBufferDivisor();

	instancedVAO->unbind();


}

int Tree::calculateRandomNumber(int highestRandomNumber) {
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine(seed); // mersenne twister engine for good PRNG.
	std::uniform_int_distribution<> dist(0, highestRandomNumber);

	auto randomNum = dist(engine);
	return randomNum;
}


std::vector<glm::vec3> Tree::getTreePositions() {
	return treePositions;
}

void Tree::draw(std::shared_ptr<Shader>& shader) {

	uniformSpecularIntensity = shader->getSpecularIntensityLocation();
	uniformShininess = shader->getShininessLocation();

	treeSpec->useMaterial(uniformSpecularIntensity, uniformShininess);
	treeModel->renderInstanced(numOfTrees);
}