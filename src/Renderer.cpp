#include <iostream>
#include "Renderer.h"


void Renderer::drawArrays(std::shared_ptr<VertexArray>& va)
{
	va->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Renderer::drawElements(std::shared_ptr<VertexArray>& va, std::shared_ptr<IndexBuffer>& ib)
{
	va->bind();
	ib->bind();
	glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);
}


void Renderer::drawInstanced(std::shared_ptr<VertexArray>& va, std::shared_ptr<IndexBuffer>& ib, int numInstanced)
{
	va->bind();
	ib->bind();
	glDrawElementsInstanced(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr, numInstanced);
}


void Renderer::enableDepth()
{
	glEnable(GL_DEPTH_TEST);
}

void Renderer::disableDepth()
{
	glDisable(GL_DEPTH_TEST);
}


void Renderer::clearBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::clear(float f0, float f1, float f2, float f3) const
{
	glClearColor(f0, f1, f2, f3);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
