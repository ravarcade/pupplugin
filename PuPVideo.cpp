#include <windows.h>
#include <vector>

#include "PuPVideo.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <gl/GL.h>
#include "pbo.h"
#include "BAM.h"


PboTextures texs;
GLuint targetTextureId = 0;

void PuPVideo::OnPluginStart()
{
	targetTextureId = 0;
	texs.Init(1024, 1024, GL_RGBA, 2);
}

void PuPVideo::OnPluginStop()
{
	texs.Cleanup();
}

void PuPVideo::ReplaceTextureWithVideo(int textureId, int videoId)
{
	targetTextureId = textureId;
}

void PuPVideo::OnSwapBuffer()
{
	bool we_have_new_data_to_display = targetTextureId != 0;

	char *outBuf = texs.GetBuffer(); // we get next empty buffer
	int w = texs.GetWidth();
	int h = texs.GetHeight();
	size_t bufSize = texs.GetBufSize();


	// =================== fill buffer
	static uint32_t cols[] = {
		0xff000000, 0xff0000ff, 0xff00ff00, 0xffff0000,
		0xffffffff, 0xffffff00, 0xffff00ff, 0xff00ffff };

	static int cnt = 0;
	uint32_t col = cols[++cnt % ARRAY_ENTRIES(cols)];
	auto buf = reinterpret_cast<uint8_t *>(outBuf);
	static int start = 0;
	start = (start + 1) % h;
	int dy = start;
	int dx = start;
	for (int y =0; y<h; ++y)
	{

		for (int x = 0; x < w; ++x)
		{
			*buf++ = (uint8_t)dy;             // R
			*buf++ = (uint8_t)dx;             // G
			*buf++ = (uint8_t)(dx+dx);        // B
			*buf++ = 0xff;                    // A
			++dx;
		}
		++dy;
		++start;
	}

	// push data to texture
	texs.Swap();

	// get texture for video
	auto texId = texs.GetTexture();

	BAM::dbg::hudDebug("target: %d\n", targetTextureId);
	if (targetTextureId != 0)
	{
		BAM::render::ReplaceTexture(targetTextureId, texId);
	}
}
