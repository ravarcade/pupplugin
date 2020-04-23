#pragma once


class PuPVideo
{
public:
	static void OnPluginStart();
	static void OnPluginStop();
	static void ReplaceTextureWithVideo(int textureId, int videoId);
	static void OnSwapBuffer();
	
};