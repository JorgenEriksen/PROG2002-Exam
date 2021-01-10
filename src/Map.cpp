#include "Map.h"




Map::Map() {

    mat = std::make_unique<Material>();
    //mat->getTexture("assets/textures/grass.jpg");
    mat->getTexture("assets/textures/plain.png");
    mat->loadTexture();

    generateMap();
   
}

Map::~Map() {};

void Map::generateMap() {
    std::string filename = "assets/HeightMaps/Mosjoen_Height Map.png";

    int n;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &n, 0);

    allHeights.resize(height, std::vector<float>(width));

    int counter = 0;
    for (GLfloat z = 0; z < height; z++) {
        for (GLfloat x = 0; x < width; x++) {
            GLfloat uv, uv2, y = static_cast<int>(data[counter*n]) / 4;
            counter++;

            // first row of vertex has two different uv values, and the row under has another two different values (every square will have 4 different uv values)
            if (static_cast<int>(z) % 2 == 0) uv = 0.0f;
            else uv = 1.0f;
            if (static_cast<int>(z) % 2 == 0) uv2 = 0.0f;
            else uv2 = 1.0f;

            std::vector<GLfloat> verticePlaceHolder = {
                 x,	y,	z,     uv,  uv2,	    x,   -y,  z,  
            };
            vertices.insert(end(vertices), verticePlaceHolder.begin(), verticePlaceHolder.end());

            allHeights[z][x] = y;
            if (y <= 10.0f) { // if under water height
                waterPositions.push_back(glm::vec3(x, y, z));
            } else { // if above water 
                groundPositions.push_back(glm::vec3(x, y, z));
                if (y < 25.0f) // if intermediate height
                    intermediateHeightPositions.push_back(glm::vec3(x, y, z));
            }
        }
    }

    for (GLuint z = 0; z < height - 1; z++) {
        for (GLuint x = 0; x < width - 1; x++) {
            std::vector<GLuint> indicesPlaceHolder = { // square
             width * z + x, width * z + x + 1, (z + 1) * width + x, 
             width * z + x + 1, (z + 1) * width + x, (z + 1) * width + x + 1 
            };
            indices.insert(end(indices), indicesPlaceHolder.begin(), indicesPlaceHolder.end());
        }
    }


    stbi_image_free(data);

    shader->calculateAverageNormals(indices, indices.size(), vertices, vertices.size(), 8, 5);

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

int Map::getMapWidth() {
    return width;
}

int Map::getMapHeight() {
    return height;
}

std::vector<glm::vec3> Map::getWaterPositions() {
    return waterPositions;
}

std::vector<glm::vec3> Map::getGroundPositions() {
    return groundPositions;
}

std::vector<glm::vec3> Map::getIntermediateHeightPositions() {
    return intermediateHeightPositions;
}

std::vector<std::vector<float>> Map::getAllHeights() {
    return allHeights;
}



void Map::draw(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader, glm::mat4 model, glm::mat4 projection) {
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