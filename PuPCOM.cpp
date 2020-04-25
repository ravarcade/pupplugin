#include <windows.h>
// TODO: reference additional headers your program requires here
#include <Vfw.h>

// TODO: reference additional headers your program requires here
// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <crtdbg.h>
#define _USE_MATH_DEFINES
#include <vector>
#include <regex>


#include "PuPCOM.h"
#include "BAM.h"
#include "PuPVideo.h"

HRESULT STDMETHODCALLTYPE PuPCOM::Msg(
	/* [in] */ BSTR txt)
{
	MessageBoxW(0, txt, L"SimpleCOM", 0);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE PuPCOM::GetTextureId(
	/* [in] */ BSTR textureName,
	/* [retval][out] */ int *retval)
{
	*retval = BAM::render::GetTextureId(textureName);
	return S_OK;
}


HRESULT STDMETHODCALLTYPE PuPCOM::ReplaceTexture(
	/* [in] */ int srcTextureId,
	/* [in] */ int dstTextureId)
{
	BAM::render::ReplaceTexture(srcTextureId, dstTextureId);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE PuPCOM::ReplaceTextureWithVideo(int targetTextureId, int videoStreamId)
{
	PuPVideo::ReplaceTextureWithVideo(targetTextureId, videoStreamId);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE PuPCOM::SearchFor(BSTR windowName)
{
   char szWindowName[MAX_PATH];
   WideCharToMultiByte(CP_ACP, 0, windowName, -1, szWindowName, MAX_PATH, NULL, NULL);
 
   PuPVideo::SearchFor(szWindowName);
}
