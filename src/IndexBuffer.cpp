#include <iostream>
#include <GL/glew.h>

#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(void* data, unsigned int count)
	: m_count(count)
{
	glGenBuffers(1, &renderer_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}


IndexBuffer::~IndexBuffer()
{
	deleteBuffer();
}


void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_ID);
}


void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void IndexBuffer::selectIndices(unsigned int* data, unsigned int count)
{
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::deleteBuffer()
{
	glDeleteBuffers(1, &renderer_ID);
}
