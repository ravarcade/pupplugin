#include "stdafx.h"
#include <regex>

PuPVideo *   PuPVideo::_defaultPuPVideo = nullptr;
bool         PuPVideo::_stopCapture = false;
std::thread  PuPVideo::_threadCap;
std::mutex   PuPVideo::_captureThreadSync;

// ============================================================== Ctor/Dtor ===
PuPVideo::PuPVideo(std::string sPupWindowName, bool useRegEx) :
	_sPupWindowName(sPupWindowName),
	_ecPUPStage(ecUninitialized),
	_useRegEx(useRegEx),
	_hPupWindow(NULL)
{
	_ecPUPStage = ecSearching;
}

PuPVideo::~PuPVideo()
{
	_pboTextures.Cleanup();
}

// ================================================================ Factory ===
PuPVideo * PuPVideo::_Create(std::string sPupWindowName, bool useRegEx)
{
	std::lock_guard<std::mutex> lock(_captureThreadSync);
	return new PuPVideo(sPupWindowName, useRegEx);
}

void PuPVideo::_Destroy(PuPVideo *pv)
{
	std::lock_guard<std::mutex> lock(_captureThreadSync);
	delete pv;
}

// ============================================================================
/// Helper to use lamba with EnumWindows
template<typename T>
void EnumWindows(T &cb)
{
	EnumWindows([](HWND hwnd, LPARAM lparam) {
		auto cb = reinterpret_cast<T*>(lparam);
		return (*cb)(hwnd);
	}, reinterpret_cast<LPARAM>(&cb));
}

HWND PuPVideo::_FindWindowRegex(std::string windowName)
{
	HWND ret = NULL;
	static char title[1000];

	EnumWindows([&](HWND hwnd) {	
		GetWindowTextA(hwnd, title, sizeof(title));
		if (std::regex_search(title, std::regex(windowName, std::regex::icase | std::regex::optimize)))
		{
			ret = hwnd;
			return FALSE;
		}
		return TRUE;
	});

	return ret;
}

HWND PuPVideo::_FindWindow(std::string windowName, bool useRegEx)
{
	return useRegEx ? _FindWindowRegex(windowName.c_str()) : FindWindowA(NULL, windowName.c_str());
}

void PuPVideo::_CaptureFindPUP()
{
	if (_hPupWindow == NULL)
		_hPupWindow = _FindWindow(_sPupWindowName, _useRegEx);

	if (_hPupWindow != NULL)
	{
		RECT r;
		GetWindowRect(_hPupWindow, &r);
		if (_ecPUP.SetupCapture(r))
		{
			_ecPUPStage = ecTexture;
		}
		else
		{
			_ecPUPStage = ecFailure;
		}
	}
}

bool PuPVideo::_CheckTexture()
{
	if (_ecPUPStage == ecTexture)
	{
		_pboTextures.Init(_ecPUP.m_Width, _ecPUP.m_Height, GL_BGRA, 1);
		_ecPUP.m_pData = _pboTextures.GetBuffer();
		_ecPUPStage = ecCapturing;
	}
	return _ecPUPStage == ecCapturing;
}

void PuPVideo::_CaptureThread()
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

	while (!_stopCapture)
	{
		{
			// in main thread we can insert/delete new PuPVideo objects
			// this mutex is used to make insert/delete synced
			std::lock_guard<std::mutex> lock(_captureThreadSync);
			for (auto &pv : List())
			{
				if (pv._ecPUPStage == ecSearching) 
				{
					pv._CaptureFindPUP();
				}

				if (pv._ecPUPStage == ecCapturing && pv._targetTextureIds.size())
				{
					pv._ecPUP.GetFrame();
				}
			}
		}
		SleepEx(5, false);
	}
}

// ============================================================================

void PuPVideo::OnPluginStart()
{
	// start worker
	_stopCapture = false;
	std::thread t(_CaptureThread);
	_threadCap = move(t);
}

void PuPVideo::OnPluginStop()
{
	// stop workder
	_stopCapture = true;
	if (_threadCap.joinable())
		_threadCap.join();

	ExtCapture::Dispose();

	// we don't need to use mutex here.... worker is finished.
	PuPVideo::clear(); // clear is method in cfList, it destroy all PuPVideo objects

	_defaultPuPVideo = nullptr;
}

void PuPVideo::SearchFor(const char *windowName, bool useRegEx)
{
	std::vector<GLuint> targetTextureIds; // we will preserve target textres
	if (_defaultPuPVideo != nullptr) 
	{
		targetTextureIds = _defaultPuPVideo->_targetTextureIds;
		_Destroy( _defaultPuPVideo);
	}

	_defaultPuPVideo = _Create(windowName, useRegEx);
	_defaultPuPVideo->_targetTextureIds = targetTextureIds;

	// first try to find window
	_defaultPuPVideo->_hPupWindow = _FindWindow(windowName, useRegEx);
}

void PuPVideo::ReplaceTextureWithVideo(int textureId, int videoId)
{
	if (_defaultPuPVideo == nullptr)
		_defaultPuPVideo = _Create("PUPSCREEN2", false);

	_defaultPuPVideo->_targetTextureIds.clear();
	_defaultPuPVideo->_targetTextureIds.push_back(textureId);
}

void PuPVideo::OnSwapBuffer()
{
	if (cfList::empty())
		return;

	for (auto &pv : List())
	{
		if (pv._CheckTexture() && pv._targetTextureIds.size())
		{
			// push data to texture
			pv._pboTextures.Swap();

			// get texture for video
			auto texId = pv._pboTextures.GetTextureId();

			for (GLuint targetId : pv._targetTextureIds)
			{
				BAM::render::ReplaceTexture(targetId, texId);
			}
		}
	}
}

void PuPVideo::StopVideo(int textureId)
{
	for (auto &pv : List())
	{
		auto ptid = std::find(pv._targetTextureIds.begin(), pv._targetTextureIds.end(), textureId);
		if (ptid != pv._targetTextureIds.end())
		{
			std::lock_guard<std::mutex> lock(_captureThreadSync);
			pv._targetTextureIds.erase(ptid);
			if (pv._targetTextureIds.size() == 0)
			{
				delete &pv;
			}
			BAM::render::ReplaceTexture(textureId, 0);
			break;
		}
	}
}

PuPVideo * PuPVideo::_FindExisting(const char *windowName, bool useRegEx)
{
	PuPVideo *ret = nullptr;

	HWND wnd = _FindWindow(windowName, useRegEx);
	if (wnd != NULL)
	{	// search by HWND
		for (auto &pv : List())
		{
			if (pv._hPupWindow == NULL)
			{
				// try again to find window
				pv._hPupWindow = _FindWindow(pv._sPupWindowName, pv._useRegEx);
			}
			if (wnd == pv._hPupWindow)
			{
				ret = &pv;
				break;
			}
		}
	}
	else {
		// search by name
		for (auto &pv : List())
		{
			if (pv._hPupWindow == NULL)
			{
				// try again to find window
				pv._hPupWindow = _FindWindow(pv._sPupWindowName, pv._useRegEx);
			}

			if (pv._hPupWindow == NULL)
			{
				if (_strcmpi(pv._sPupWindowName.c_str(), windowName) == 0 && useRegEx == pv._useRegEx)
				{
					ret = &pv;
					break;
				}
			}
		}
	}
	return ret;
}

void PuPVideo::ShowVideo(int textureId, const char *windowName, bool useRegEx)
{
	if (textureId == 0)
		return; // nothing to show
	
	auto pv = _FindExisting(windowName, useRegEx);
	if (pv == nullptr)
	{
		// insert new
		pv = _Create(windowName, useRegEx);		
	}

	// check if textureId is already present in _targetTextureIds	
	if (std::find(pv->_targetTextureIds.begin(), pv->_targetTextureIds.end(), textureId) == pv->_targetTextureIds.end())
	{
		StopVideo(textureId);
		{
			std::lock_guard<std::mutex> lock(_captureThreadSync);
			pv->_targetTextureIds.push_back(textureId);
		}
	}
}