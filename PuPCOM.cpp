#include"stdafx.h"

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
	if (dstTextureId == 0)
		PuPVideo::StopVideo(srcTextureId);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE PuPCOM::ReplaceTextureWithVideo(int targetTextureId, int videoStreamId)
{
	PuPVideo::ReplaceTextureWithVideo(targetTextureId, videoStreamId);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE PuPCOM::SearchFor(BSTR windowName, VARIANT_BOOL useRegEx)
{
   char szWindowName[MAX_PATH];
   WideCharToMultiByte(CP_ACP, 0, windowName, -1, szWindowName, MAX_PATH, NULL, NULL);
 
   PuPVideo::SearchFor(szWindowName, useRegEx != VARIANT_FALSE);
   return S_OK;
}

HRESULT STDMETHODCALLTYPE PuPCOM::ShowVideo(BSTR targetTextureName, BSTR windowName, VARIANT_BOOL useRegEx)
{
	int tid = 0;
	GetTextureId(targetTextureName, &tid);
	if (tid)
	{
		char szWindowName[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, windowName, -1, szWindowName, MAX_PATH, NULL, NULL);

		PuPVideo::ShowVideo(tid, szWindowName, useRegEx != VARIANT_FALSE);
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE PuPCOM::StopVideo(
	/* [in] */ BSTR targetTextureName)
{
	int tid = 0;
	GetTextureId(targetTextureName, &tid);
	if (tid)
	{
		PuPVideo::StopVideo(tid);
	}

	return S_OK;
}
