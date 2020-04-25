#include <windows.h>
#include <vector>
#include <string>

#include "PuPVideo.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <gl/GL.h>
#include "pbo.h"
#include "BAM.h"
#include "captureExt.h"


PboTextures texs;
GLuint targetTextureId = 0;
std::string sWindowName;

void PuPVideo::OnPluginStart()
{
	targetTextureId = 0;
   sWindowName = "PUPSCREEN2";
   captureStartup();
}

void PuPVideo::OnPluginStop()
{
   captureStop();
	texs.Cleanup();
}

void PuPVideo::SearchFor(const char *windowName)
{
   sWindowName = windowName;
}

void PuPVideo::ReplaceTextureWithVideo(int textureId, int videoId)
{
	targetTextureId = textureId;
}

char *PuPVideo::CreateTexture(int width, int height)
{
   texs.Init(width, height, GL_RGBA, 1);
   return texs.GetBuffer();
}

extern ecStage ecDMDStage;

void PuPVideo::OnSwapBuffer()
{
   captureCheckTextures();

   if (ecDMDStage == ecCapturing && (targetTextureId != 0))
   {
      // push data to texture
      texs.Swap();

      // get texture for video
      auto texId = texs.GetTexture();

      BAM::dbg::hudDebug("target: %d\n", targetTextureId);
      BAM::render::ReplaceTexture(targetTextureId, texId);
   }
}
