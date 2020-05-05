#pragma once

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
#include <thread>
#include <mutex>

#include "BAM.h"
#include "pbo.h"
#include "PuPCOM.h"

#include "captureExt.h"
#include "PuPVideo.h"

#include "Tools.h"


