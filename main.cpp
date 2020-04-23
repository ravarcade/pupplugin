
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define DRAWDIB_INCLUDE_STRETCHDIB
// Windows Header Files:
#include <windows.h>
#include <Vfw.h>	// BSTR

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
//#include <crtdbg.h>
#define _USE_MATH_DEFINES
#include <vector>
#include <regex> 

#define GLEW_STATIC
#include <GL/glew.h>
#include <gl/GL.h>
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "opengl32.lib")


#include "BAM.h"
#include "pbo.h"
#include "PuPVideo.h"

// secuirity level
enum {
	PARANOIC = 0,
	VERY_SAFE,
	SAFE,
	DEVELOPER
};

// configuration structure
struct Ccfg {
	int SecuitiyLevel;
} cfg = { SAFE }; // defaults


// menu "switch" values
const char *SecuitiyLevel_txt[] = { "Paranoic (no extension)", "Very Safe (only whitelisted)", "Safe (disable blacklisted)", "Same as VPX (don't care)" };

// cfg must be defined before PuPCOM.h is included
#include "PuPCOM.h"
#include "PuPCOM_i.c"

PuPCOM *sc = nullptr;

HMODULE dll_hModule;

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	dll_hModule = hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" {
	// called by FP load (before any hooks to opengl is made)
	// It must exist
	// If not exist => dll is not BAM plugin and will be unloaded from RAM
	BAMEXPORT int BAM_load(HMODULE bam_module)
	{
		// BAM plugin ID - 0x4101 - COM Extension for scripts plugin ID
		// ID > 0x4000 - No Tracking Plugin
		// - No Tracking Plugin can't be selected as "mode"
		// - Is accesible in "Config"->"Plugins"
		BAM::Init( 0x4201, bam_module ); 

		// Creation of menu
		BAM::menu::create("PuP Plugin Extensions", "Here you can control PuP Plugin");  
		BAM::menu::info("#c777##-Options" DEFIW);

		BAM::menu::paramSwtich("#-Security Level:" DEFPW "#+ #cfff#%s", &cfg.SecuitiyLevel, SecuitiyLevel_txt, ARRAY_ENTRIES(SecuitiyLevel_txt), "");


		BAM::helpers::load("PuPPlugin", &cfg, sizeof(cfg));

		// in this place only menu can be created and config files can be loaded
		// there is not opengl context ready

		return 0;
	}

	// For "Tracking" plugins, it is called when user select plugin (for example: create working thread and take resurces like cams)
	// For "Non-Tracking" plugins it is called on game start
	BAMEXPORT void BAM_PluginStart()
	{
		// do init here
		// create OpenGL resources...
		GLenum nGlewError = glewInit();
		if (nGlewError)
			BAM::dbg::hudDebugLong("Panic! glewInit failed.");
		
		PuPVideo::OnPluginStart();		
	}

	/**
	 * For "Treacking" plugins, it is called then plugin is "unselected" as mode. 
	 * It is signal to release resources (like cams, one cam can be used be two or more plugins, but not in same time)
	 *
	 * Forr "Non-Tracking" plugins, it is called at end of game (exit to FP editor)
	 */ 
	BAMEXPORT void BAM_PluginStop()
	{
		BAM::helpers::save("PuPPlugin", &cfg, sizeof(cfg));
		
		// release reasources
		if (sc) {
			delete sc;
			sc = nullptr;
		}

		PuPVideo::OnPluginStop();
	}

	BAMEXPORT void BAM_swapBuffers(HDC hDC)
	{
		PuPVideo::OnSwapBuffer();
	}

	// Called from script with:
	// Set icom = xBAM.Get("PuPPlugin")
	BAMEXPORT IDispatch *BAM_GetCOM()
	{
		if (sc == nullptr)
			sc = new PuPCOM(dll_hModule);

		return sc;
	}

}