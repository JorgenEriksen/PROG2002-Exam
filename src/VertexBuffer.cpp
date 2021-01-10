#include <GL/glew.h>
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &renderer_ID); //generating a buffer for the vertex buffer object
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &renderer_ID);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, renderer_ID);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::updateBuffer(const void* data, unsigned int size)
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}