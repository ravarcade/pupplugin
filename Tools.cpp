// Windows Header Files:
#include "stdafx.h"

namespace Tools 
{
	// ============================================================================ Tools ===

	LPVOID GetResource(const char *resData, const char *resType = "TEXT", DWORD *outLen = NULL)
	{
		static HMODULE dll_hModule = NULL;

		if (!dll_hModule)
			GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&GetResource, &dll_hModule);

		HRSRC hResource = FindResourceA(dll_hModule, resData, resType);
		if (!hResource)
			return NULL;

		HGLOBAL hLoadedResource = LoadResource(dll_hModule, hResource);
		if (!hLoadedResource)
			return NULL;

		LPVOID pLockedResource = LockResource(hLoadedResource);
		if (!pLockedResource)
			return NULL;

		if (outLen != NULL)
			*outLen = SizeofResource(dll_hModule, hResource);

		return pLockedResource;
	}

	const char *GetResourceText(const char *resData)
	{
		DWORD olen;

		// we have to add '\0' and end of string, so we create tmp with size+1 and add '\0' at end
		const char * vs = (const char *)GetResource(resData, "TEXT", &olen);
		char *tmp = new char[olen + 1];
		memcpy_s(tmp, olen + 1, vs, olen);
		tmp[olen] = 0;

		return tmp;
	}
}