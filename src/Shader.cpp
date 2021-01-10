#include <cstring>

#include "Shader.h"


Shader::Shader() 
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::createShaderFromFile(const char* vertexLocation, const char* fragmentLocation) 
{
	std::string vertexString = readFile(vertexLocation);
	std::string fragmentString = readFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char* fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		std::cout << "Failed to read: " << fileLocation << " File doesn't exist." << "\n";
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}


void Shader::compileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) {
		std:: cout << "Error creating shader program!\n";
		return;
	}

	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);


	// ERROR CHECKS
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error LINKING program: " << eLog;
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error VALIDATING program: " << eLog;
		return;
	}

	//All of the uniform variables are set to the values from the shader

	uniformView = glGetUniformLocation(shaderID, "view");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");



	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (unsigned int i = 0; i < 15; i++)
	{
		char localBuffer[100] = { '\0' };

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].base.colour", i);
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, localBuffer);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (unsigned int i = 0; i < 3; i++)
	{
		char localBuffer[100] = { '\0' };

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.base.colour", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, localBuffer);
	}
}



void Shader::setDirectionalLight(std::shared_ptr<DirectionalLight>& dLight)
{

	dLight->useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}


void Shader::setPointLights(PointLight* pLight, unsigned int lightCount)
{
	if (lightCount > 15) lightCount = 15;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}


void Shader::setSpotLights(SpotLight* sLight, unsigned int lightCount)
{
	if (lightCount > 3) lightCount = 3;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);
	}
}


void Shader::useShader() 
{
	glUseProgram(shaderID);
}

void Shader::clearShader() 
{
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}


void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) 
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the: " << shaderType << " error is: "<<  eLog;
		return;
	}

	glAttachShader(theProgram, theShader);
}


Shader::~Shader() 
{
	clearShader();
}


void Shader::calculateAverageNormals(std::vector<GLuint> indices, unsigned int indiceCount, std::vector<GLfloat> vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	//We want to go through every triangle we have in the mesh and increment 3 at a time.
	//This is to jump to each triangle since 3 indices make up a triangle.
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		//We need to multiply each indices element by vLength, It moves you down the "table" by 8.
		unsigned long int in0 = double(indices[i]) * vLength;
		unsigned long int in1 = double(indices[i + 1]) * vLength;
		unsigned long int in2 = double(indices[i + 2]) * vLength;

		//We create two vector and connect them, that we will use later to create a line.
		glm::vec3 v1(vertices[in1] - vertices[in0], 
					 vertices[in1 + 1] - vertices[in0 + 1], 
					 vertices[in1 + 2] - vertices[in0 + 2]);

		glm::vec3 v2(vertices[in2] - vertices[in0], 
					 vertices[in2 + 1] - vertices[in0 + 1], 
					 vertices[in2 + 2] - vertices[in0 + 2]);

		//Now we want to start adding these values to our normal. We add the normalOffset to work 
		//on the normal in the vertex data.
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		//Now we want to start adding these values to our normal. 
		//We add the normalOffset to work on the normal in the vertex data.
		in0 += normalOffset; 
		in1 += normalOffset; 
		in2 += normalOffset;

		vertices[in0] += normal.x; 
		vertices[in0 + 1] += normal.y; 
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x; 
		vertices[in1 + 1] += normal.y; 
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x; 
		vertices[in2 + 1] += normal.y; 
		vertices[in2 + 2] += normal.z;
	}

	//Then we want to normalize these values, to unit vector, much like above. nOffset will be the normal 
	//offset for each individual one. Every time we go through its counting up each of these 0, 1, 2 etc. 
	//and grabbing relevant one from the list of vertex and going to the offset section of it. Then we 
	//normalize this vector. Then we assign them to each vertex axis (x, y, z).
	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned long int nOffset = i * vLength + normalOffset;

		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);

		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}