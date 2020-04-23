#pragma once


class PboTextures
{

public:
	PboTextures() {}

	void Init(int width, int height, GLenum pixelFormat, int num = 2);
	void Cleanup();

	char *GetBuffer();
	GLuint GetTexture();
	void Swap();

	int GetWidth() { return _width; }
	int GetHeight() { return _height; }
	int GetPixelSize() { return _GetPixelSize(_pixelformat); }
	size_t GetBufSize() { return GetWidth() * GetHeight() * GetPixelSize(); }

private:
	std::vector<GLuint> textures;
	std::vector<GLuint> pbos;
	std::vector<char *> buffers;
	void _GenPboTextures(GLsizei width, GLsizei height, GLenum pixelformat, GLsizei num);
	size_t _GetPixelSize(GLenum pixelFormat);
	int _bufferIdx;
	int _textureIdx;
	int _width;
	int _height;
	GLenum _pixelformat;
};
