#pragma once


class PuPVideo
{
public:
	static std::string sPupWindowName;
	static void OnPluginStart();
	static void OnPluginStop();
	static void ReplaceTextureWithVideo(int textureId, int videoId);
	static void OnSwapBuffer();
	static char *CreateTexture(int width, int height);
	static void SearchFor(const char *WindowName);
};