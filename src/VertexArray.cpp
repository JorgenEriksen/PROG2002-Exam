#include "VertexArray.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &rendererID); //Generating a vertex array for the array object
	bind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::addBuffer(VertexBuffer& vb, VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized,
			layout.getStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::addBufferDivisor()
{
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glVertexAttribDivisor(3, 1);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glVertexAttribDivisor(4, 1);

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribDivisor(5, 1);

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(6, 1);
}

void VertexArray::bind()
{
	glBindVertexArray(rendererID);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::changeData(std::shared_ptr<VertexBuffer>& VBO, void* data, unsigned int size)
{
	bind();
	VBO->bind();
	VBO->updateBuffer(data, size);
}
