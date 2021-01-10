#include "FrameBuffer.h"


FrameBuffer::FrameBuffer()
	: frameBuffer(0), texColorBuffer(0), renderObjectBuffer(0)
{

}

FrameBuffer::~FrameBuffer()
{

}

void FrameBuffer::generateFB()
{
	glGenFramebuffers(1, &frameBuffer);
	bind();
}


void FrameBuffer::generateTBO(int width, int height)
{
	glGenTextures(1, &texColorBuffer);
	bindTBO();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
}


void FrameBuffer::generateRBO(int width, int height)
{
	glGenRenderbuffers(1, &renderObjectBuffer);
	bindRBO();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single render buffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderObjectBuffer); // now actually attach it

}


void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void FrameBuffer::bindTBO()
{
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
}


void FrameBuffer::bindRBO()
{
	glBindRenderbuffer(GL_RENDERBUFFER, renderObjectBuffer);
}


void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::checkStatus()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Error:  Frame buffer is not completed, missing components!" << std::endl;
}
