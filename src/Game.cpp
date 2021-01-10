#include "Game.h"


Game::Game()
	:projection(0), deltaTime(0), lastTime(0), now(0), 
	uniformModel(0), uniformView(0), uniformProjection(0),model(1.0f)
{
	numberOfAerials = 10;
	numberOfGroundAnimals = 10;
	numberOfTrees = 100;
}


Game::~Game()
{
	
}


void Game::generateShaders()
{
	static const char* vShader = "assets/shaders/lights.vert";
	static const char* fShader = "assets/shaders/lights.frag";

	shader = std::make_shared<Shader>(); // general shader
	shader->createShaderFromFile(vShader, fShader); 

	static const char* map_vShader = "assets/shaders/map.vert";
	static const char* map_fShader = "assets/shaders/map.frag";

	mapShader = std::make_shared<Shader>();
	mapShader->createShaderFromFile(map_vShader, map_fShader);


	static const char* tree_vShader = "assets/shaders/tree.vert";
	static const char* tree_fShader = "assets/shaders/tree.frag";

	treeShader = std::make_shared<Shader>();
	treeShader->createShaderFromFile(tree_vShader, tree_fShader);

	static const char* eagle_vShader = "assets/shaders/eagle.vert";
	static const char* eagle_fShader = "assets/shaders/eagle.frag";

	eagleShader = std::make_shared<Shader>();
	eagleShader->createShaderFromFile(eagle_vShader, eagle_fShader);

	static const char* sun_vShader = "assets/shaders/sun.vert";
	static const char* sun_fShader = "assets/shaders/sun.frag";

	sunShader = std::make_shared<Shader>();
	sunShader->createShaderFromFile(sun_vShader, sun_fShader);
}

void Game::generateLights()
{


	mapLight = std::make_shared<DirectionalLight>(
		1.0f, 1.0f, 1.0f,	 // Color
		0.2f, 0.0f,			 // Ambient, diffuse 
		5.0f, -25.0f, 6.0f); // Direction

	pointLights[0] = PointLight(
		10.0f, 0.0f, 0.0f,    // Color
		800.0f, 1.0f, 		 // Ambient, diffuse 
		150.0f, 150.0f, 150.0f,  // Position 
		0.1f, 0.1f, 0.1f); 	 // Constant, linear, exponent



	treeLight = std::make_shared<DirectionalLight>(
		1.0f, 1.0f, 1.0f,	 // Color
		0.5f, 0.4f,			 // Ambient, diffuse 
		-5.0f, -25.0f, -6.0f); // Direction



	pointLights[1] = PointLight(
		0.9f, 0.0f, 1.0f,    // Color
		2.5f, 0.5f, 		 // Ambient, diffuse 
		13.0f, -0.5f, 10.0f,  // Position 
		0.1f, 0.1f, 0.1f); 	 // Constant, linear, exponent

	pointLights[2] = PointLight(
		1.0f, 1.0f, 0.4f,    // Color
		2.5f, 0.5f, 		 // Ambient, diffuse 
		3.0f, -0.5f, 65.0f,  // Position 
		0.1f, 0.1f, 0.1f); 	 // Constant, linear, exponent

	pointLights[3] = PointLight(
		1.0f, 0.0f, 0.9f,    // Color
		2.5f, 0.5f, 		 // Ambient, diffuse 
		53.0f, -0.5f, 63.0f,  // Position 
		0.1f, 0.1f, 0.1f); 	 // Constant, linear, exponent

	pointLights[4] = PointLight(
		0.1f, 1.0f, 0.3f,	 // Color
		2.5f, 0.5f, 		 // Ambient, diffuse 
		7.0f, -0.5f, 35.0f, // Position
		0.1f, 0.1f, 0.1f);   // Constant, linear, exponent

	/* --- The Flash light in the scene --- */

	spotLights[0] = SpotLight(
		1.0f, 1.0f, 1.0f,	 // Color
		1.0f, 1.0f, 		 // Ambient, diffuse 
		0.0f, 0.0f, 0.0f,	 // Position 
		0.0f, 0.0f, 0.0f,	 // Direction
		1.0f, 0.0f, 0.0f,	 // Constant, linear, exponent
		13.0f);				 // Edge


}



void Game::generateGame(std::shared_ptr<GLWindow>& mainWindow)
{
	float offset = mainWindow->getBufferWidth() - mainWindow->getBufferHeight() + 40.0f;
	
	generateShaders();
	generateLights();
	 
	map = std::make_unique<Map>();
	tree = std::make_unique<Tree>(map->getIntermediateHeightPositions(), numberOfTrees);
	sun = std::make_unique<Sun>();
	duck = std::make_unique<Duck>(map->getMapWidth(), map->getMapHeight(), map->getWaterPositions(), map->getAllHeights());

	glm::vec3 startingPos(glm::vec3(300.0f, 70.0f, 500.0f));
	camera = std::make_shared<Camera>(startingPos, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 4.0f, 0.03f);
	
	

	water = std::make_unique<Water>(map->getMapWidth(), map->getMapHeight());
	for (int i = 0; i < numberOfAerials; i++) {
		auto aerial = std::make_unique<Aerial>(map->getMapWidth(), map->getMapHeight());
		aerials.push_back(std::move(aerial));
	}
	for (int i = 0; i < numberOfGroundAnimals; i++) {
		auto ground = std::make_unique<GroundAnimal>(map->getMapWidth(), map->getMapHeight(), map->getGroundPositions(), map->getAllHeights(), i);
		groundAnimals.push_back(std::move(ground));
	}

	projection = glm::perspective(glm::radians(45.0f), ((GLfloat)mainWindow->getBufferWidth() / mainWindow->getBufferHeight()), 0.1f, 1200.0f);
}


void Game::updateLights(std::shared_ptr<Shader>& argShader)
{
	argShader->setDirectionalLight(mapLight);
	argShader->setPointLights(pointLights, 1);
	argShader->setSpotLights(spotLights, 1);
	lowerLight = camera->getCameraPosition();
	lowerLight.y -= 0.1f; // Offsets the flashlight
	spotLights[0].setFlash(lowerLight, camera->getCameraDirection());
}


void Game::updateMVP(std::shared_ptr<Shader>& argShader)
{
	uniformModel = argShader->getModelLocation();
	uniformProjection = argShader->getProjectionLocation();
	uniformView = argShader->getViewLocation();
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}


void Game::updateTime()
{
	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;
	if (deltaTime > 0.1f) {
		deltaTime = 0.1;
	}
}

void Game::updateSunLight() {


	float sunPos = sun->getPosition().y;
	float p = (sun->getPosition().y)/2; // 0 - 100
	//std::cout << sunPos << "\n";

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;

	if (p <= 20.0f) { // night
		blue = (20 - p) / 20;
		red = (p - 15) / 10;
		green = 0;
	}
	else if (p <= 50.0f) { // dawn
		blue = 0;
		red = 0.5f;
		green = ((p*2) - 50) / 100;
	}
	else if(p <= 100.0f){ // day
		blue = (p - 50) / 50;
		red = p / 100;
		green = p / 100;
	}

	if (blue < 0) blue = 0;
	if (green < 0) green = 0;
	if (red < 0) red = 0;


	mapLight->setColor(
		red, green, blue,
		0.3f, 0.0f);


	pointLights[0].setColor(
		red, green, blue,   
		1600.0f, 50.0f);

	pointLights[0].setPosition(sun->getPosition());

	renderer->clear(red, green, blue, 0.1f); // sky
}


void Game::updateGame(std::shared_ptr<GLWindow>& mainWindow)
{
	renderer->clear(0.1f, 0.1f, 0.1f, 1.0f);
	renderer->enableDepth();
	updateTime();
	updateSunLight();
	if (aerials[0]) {
		camera->thirdPersonCalculation(aerials[0]->getPosition(), aerials[0]->getVelocity());
	}
	camera->keyControls(mainWindow->retrieveKeys(), deltaTime);
	camera->mouseControl(mainWindow->getChangeX(), mainWindow->getChangeY());
	

	mapShader->useShader();
	//updateMVP(mapShader);
	updateLights(mapShader);

	// Toggle the Flash light on and off with the L key.
	if (mainWindow->retrieveKeys()[GLFW_KEY_F])
	{
		spotLights[0].toggleFlashLight();

		// Set it to false to only catch the button pressed, and not while it's pressed.
		mainWindow->retrieveKeys()[GLFW_KEY_F] = false;
	}

	map->draw(camera, mapShader, model, projection);

	eagleShader->useShader();
	updateMVP(eagleShader);
	updateLights(eagleShader);

	for (int i = 0; i < numberOfAerials; i++) {
		aerials[i]->move(deltaTime);
		aerials[i]->draw(camera, eagleShader, model, projection);
	}

	shader->useShader(); // general shader
	updateMVP(shader);
	updateLights(shader);
	water->draw(camera, shader, model, projection);
	duck->move(deltaTime);
	duck->draw(camera, shader, model, projection);

	sunShader->useShader();
	updateMVP(sunShader);
	updateLights(sunShader);
	sun->move(deltaTime);
	sun->draw(camera, sunShader, model, projection);



	shader->useShader();
	updateMVP(shader);
	updateLights(shader);
	for (int i = 0; i < numberOfGroundAnimals; i++) {
		groundAnimals[i]->draw(camera, shader, model, projection);
		groundAnimals[i]->move(deltaTime, groundAnimals, tree->getTreePositions());
	}


	treeShader->useShader();

	uView = treeShader->getViewLocation();
	uProj = treeShader->getProjectionLocation();

	glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

	
	//treeShader->setDirectionalLight(mapLight);
	treeShader->setDirectionalLight(treeLight);
	
	treeShader->setSpotLights(spotLights, 1);

	tree->draw(treeShader);
	



	glUseProgram(0);
}

