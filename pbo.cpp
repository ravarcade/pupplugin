#include "stdafx.h"


void PboTextures::Init(int width, int height, GLenum pixelFormat, int num)
{
	_GenPboTextures(width, height, pixelFormat, num);
}

void PboTextures::Cleanup()
{
	for (auto &buf : buffers)
		delete[] buf;

	if (pbos.size())
		glDeleteBuffersARB(pbos.size(), pbos.data());
	if (textures.size())
		glDeleteTextures(textures.size(), textures.data());

	buffers.clear();
	pbos.clear();
	textures.clear();
}

char * PboTextures::GetBuffer()
{
	int nidx = (_bufferIdx + 1) % buffers.size();
	if (nidx != _textureIdx)
		_bufferIdx = nidx;
	return buffers[_bufferIdx];
}

GLuint PboTextures::GetTextureId()
{
	if (_textureIdx != _bufferIdx)
		_textureIdx = (_textureIdx + 1) % textures.size();

	return textures[_textureIdx];
}

void PboTextures::Swap()
{
	glBindTexture(GL_TEXTURE_2D, textures[_bufferIdx]);
	if (pbos.size() == 0) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _pixelformat, GL_UNSIGNED_BYTE, (GLvoid*)buffers[_bufferIdx]);
	}
	else {
		size_t bufSize = GetBufSize();
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbos[_bufferIdx]);

		// copy pixels from PBO to texture object
		// Use offset instead of ponter.
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _pixelformat, GL_UNSIGNED_BYTE, 0);

		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, bufSize, 0, GL_STREAM_DRAW_ARB);
		uint8_t *buf = (uint8_t *)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);

		memcpy_s(buf, bufSize, buffers[_bufferIdx], bufSize);
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer 
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

/// <summary>
/// Gens textures and pixel buffer objects.
/// </summary>
/// <param name="outTextureIds">The out texture ids.</param>
/// <param name="outPBOs">The out PBO ids.</param>
/// <param name="num">The number of textures and PBOs.</param>
/// <param name="width">The width.</param>
/// <param name="height">The height.</param>
/// <param name="pixelformat">The pixelformat.</param>
/// <returns>TRUE if PBO is supported.</returns>
void PboTextures::_GenPboTextures(GLsizei width, GLsizei height, GLenum pixelformat, GLsizei num)
{
	auto pixelSize = _GetPixelSize(pixelformat);

	if (pixelSize == 0) // not supported pixel format
		return;

	textures.resize(num);
	GLuint *outTextureIds = textures.data();

	glGenTextures(num, outTextureIds);
	for (int i = 0; i < num; i++)
	{
		glBindTexture(GL_TEXTURE_2D, outTextureIds[i]);

		// filtrowanie
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// textura
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixelformat, GL_UNSIGNED_BYTE, nullptr);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	bool pboSupported = false;
	size_t bufSize = width * height * pixelSize; 
	// check once again PBO extension
	if (glGenBuffersARB && glBindBufferARB && glBufferDataARB && glBufferSubDataARB &&
		glMapBufferARB && glUnmapBufferARB && glDeleteBuffersARB && glGetBufferParameterivARB)
	{
		pbos.resize(num);
		GLuint *outPBOs = pbos.data();
		glGenBuffersARB(num, outPBOs);

		for (int i = 0; i < num; ++i) 
		{
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, outPBOs[i]);
			glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, bufSize, 0, GL_STREAM_DRAW_ARB);
		}
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	}

	buffers.resize(num);
	for (int i = 0; i < num; ++i)
	{
		buffers[i] = new char[bufSize];
	}

	_width = width;
	_height = height;
	_pixelformat = pixelformat;
	_bufferIdx = 0;	
	_textureIdx = 1;
}

size_t PboTextures::_GetPixelSize(GLenum pixelFormat)
{
	size_t pixelSize = 0;

	switch (pixelFormat)
	{
	case GL_BGR: pixelSize = 3; break;
	case GL_BGRA: pixelSize = 4; break;
	case GL_RGB: pixelSize = 3; break;
	case GL_RGBA: pixelSize = 4; break;
	case GL_LUMINANCE: pixelSize = 1; break;
	}

	return pixelSize;
}
