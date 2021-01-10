#include "Model.h"


Model::Model()
{

}


void Model::loadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		std::cout << "Model (" << fileName << ") failed to load: " << importer.GetErrorString() << std::endl;
		return;
	}

	loadNode(scene->mRootNode, scene);

	loadMaterials(scene);
}


void Model::loadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		loadNode(node->mChildren[i], scene);
	}

}


void Model::loadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				unsigned long int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("assets/textures/") + filename;

				textureList[i] = std::make_unique<Material>();
				textureList[i]->getTexture(texPath.c_str());

				if (!textureList[i]->loadTexture())
				{
					std::cout << "Failed to load texture at: " << texPath << std::endl;
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i])
		{
			textureList[i] = std::make_unique<Material>();
			textureList[i]->getTexture("assets/textures/plain.png"); // In case no texture is found
			textureList[i]->loadTextureA();				// we set a standard texture.
		}
	}
}


void Model::loadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}

		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	modelVAO = std::make_shared<VertexArray>();
	modelVAO->bind();
	
	modelVBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(GLfloat));
	modelVBO->bind();
	
	modelVBLayout = std::make_unique<VertexBufferLayout>();
	modelVBLayout->Push<float>(3);
	modelVBLayout->Push<float>(2);
	modelVBLayout->Push<float>(3);
	
	modelVAO->addBuffer(*modelVBO, *modelVBLayout);
	modelIBO = std::make_shared<IndexBuffer>(&indices[0], indices.size());

	rendererList.push_back(modelRenderer);

	meshToTex.push_back(mesh->mMaterialIndex);
}




void Model::renderElements()
{
	for (size_t i = 0; i < rendererList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->useTexture();
		}

		rendererList[i]->drawElements(modelVAO, modelIBO);
	}
}


void Model::renderInstanced(int numInstanced)
{
	for (size_t i = 0; i < rendererList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->useTexture();
		}

		rendererList[i]->drawInstanced(modelVAO, modelIBO, numInstanced);
	}
}


Model::~Model()
{
	
}
