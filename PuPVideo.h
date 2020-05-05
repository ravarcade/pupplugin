#pragma once

#include "cfList.h"
#include "pbo.h"

class PuPVideo : protected cfList<PuPVideo>
{
private:
	PuPVideo(std::string sPupWindowName, bool useRegEx);
	~PuPVideo();
	
	// Factory
	static PuPVideo *_Create(std::string sPupWindowName, bool useRegEx);
	static void _Destroy(PuPVideo *);

	static HWND _FindWindowRegex(std::string windowName);
	static HWND _FindWindow(std::string windowName, bool useRegEx);

	static PuPVideo * _FindExisting(const char * windowName, bool useRegEx);
	void _CaptureFindPUP();
	bool _CheckTexture();

	std::string _sPupWindowName;
	PboTextures _pboTextures;
	std::vector<GLuint> _targetTextureIds;
	ExtCapture _ecPUP;
	ecStage _ecPUPStage = ecUninitialized;	
	bool _useRegEx;
	HWND _hPupWindow;

	static PuPVideo *_defaultPuPVideo;
	static bool _stopCapture;
	static std::thread _threadCap;
	static std::mutex _captureThreadSync;

	static void _CaptureThread(); // Worker

	friend cfList<PuPVideo>;		// destructor must be accessible to cfList
public:
	// no standard constructors, copy/move operators
	PuPVideo() = delete;
	PuPVideo(const PuPVideo &) = delete;
	PuPVideo(PuPVideo &&) = delete;
	PuPVideo operator = (const PuPVideo &s) = delete;
	PuPVideo & operator = (PuPVideo &&s) = delete;


	static void OnPluginStart();
	static void OnPluginStop();
	static void ReplaceTextureWithVideo(int textureId, int videoId);
	static void OnSwapBuffer();
	static void SearchFor(const char *windowName, bool useRegEx = false);
	static void ShowVideo(int textureId, const char *windowName, bool useRegEx = false);
	static void StopVideo(int textureId);
	
};