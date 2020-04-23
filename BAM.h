#pragma once

/**
 * New BAM.h v.2.0
 * 
 * Info about how to use it is at end of this file
 */

#define DEFPW "#r160#"
#define DEFPW2 DEFPW"#+ #cfff#"
#define DEFIW "#m350#"

#define BAMEXPORT __declspec(dllexport)
#define BAMIMPORT __declspec(dllimport)
#define ARRAY_ENTRIES(array)	(sizeof(array)/sizeof(array[0]))

namespace BAM {

	namespace _functions_in_bam_dll 
	{
		void BAM_MessageBox(const char *fmt, ...);
		void BAM_hudDebug(const char *fmt, ...);
		void BAM_hudDebugLong(const char *fmt, ...);

		void BAM_MessageBox2(const char *txt);
		void BAM_hudDebug2(const char *txt);
		void BAM_hudDebugLong2(const char *txt);


		void BAM_menu_create(int pluginID, const char *pname, const char *phelp);
		void BAM_menu_add_info(int pluginID, const char *text);
		void BAM_menu_add_info3D(int pluginID, const char *txt, double *x, double *y, double *z, const char *help);
		void BAM_menu_add_param(int PluginID, const char *txt, double *value, double smallStep, double bigStep, const char *help);
		void BAM_menu_add_switch(int PluginID, const char *txt, int *value, const char *options[], int count, const char *help);
		void BAM_menu_add_button(int PluginID, const char *txt, void (*callback)(int));
		void BAM_menu_add_cam(int pluginID);
		void BAM_menu_add_3Points(int pluginID);
		void BAM_menu_add_TL(int pluginID);
		void BAM_menu_add_Reality(int pluginID);
		void BAM_menu_add_StartCalibration(int pluginID);
		void BAM_menu_add_CalibrationBoard(int pluginID);
		int  BAM_create_submenu(int PluginID);
		void BAM_menu_add_submenu(int PluginID, const char *txt, int SubMenuID, const char *help);
		void BAM_menu_add_back_button(int PluginID);
		void BAM_get_menu_position(int *menuId, int *menuLine);
		void BAM_set_submenu_cam_info(int submenuId, int camCount);


		void BAM_printf(int x, int y, double size, bool shadow, const char *fmt, ...);
		void BAM_printf2(int x, int y, double size, bool shadow, const char *txt);
		BYTE *BAM_get_cam_buffer(int cam);
		void BAM_get_cam_update_completed(int cam);

		void BAM_push_raw_XYZ(int pluginID, double x, double y, double z);
		void BAM_push_raw_XYZ_2Eyes(int PluginID, double x1, double y1, double z1, double x2, double y2, double z2);
		void BAM_push_raw_cam(double *leftPoints, int leftCount, double *rightPoints, int rightCount);
		void BAM_testing_SetCamXYZ(double x, double y, double z);

		void BAM_calibrate(double *leftPoints, double *rightPoints, int CornersX, int CornersY, int frames);
		void BAM_calibrateCams(double *leftPoints, double *rightPoints, int CornersX, int CornersY, int frames);
		void BAM_set_CamsParams(double(*M1)[3][3], double(*D1)[5], double(*M2)[3][3], double(*D2)[5]);

		double * BAM_get_CalbrationBoard();
		void BAM_showCheckerboard(int status);
		double *BAM_get_checerboard_points();
		void BAM_set_checkerboard_3points(double *points);

		void BAM_AttachDetour(void **ppPointer, void *pDetour);
		void BAM_DetachDetour(void **ppPointer, void *pDetour);

		unsigned int BAM_compileShaders(const char *vertexShaderSrc, const char *fragmentShaderSrc);

		unsigned int BAM_GetTextureId(BSTR textureName);
		void BAM_ReplaceTexture(unsigned int src, unsigned int dst);
	}
}
	
#define BAM_FUNC(x) decltype(_functions_in_bam_dll::BAM_ ## x) * BAM_ ## x
#define SET_FUNC(x) *((void **)&Internal().BAM_ ## x) = (void *)GetProcAddress(bam_module, "BAM_" # x);

namespace BAM {
	typedef void BAM_OnButtonPress_func(int keyCode);
	struct SInternal {
		int _pluginID;

		BAM_FUNC(MessageBox);
		BAM_FUNC(MessageBox2);
		BAM_FUNC(hudDebug);
		BAM_FUNC(hudDebugLong);
		BAM_FUNC(hudDebug2);
		BAM_FUNC(hudDebugLong2);

		// ::menu::
		BAM_FUNC(menu_create);
		BAM_FUNC(menu_add_info);
		BAM_FUNC(menu_add_info3D);
		BAM_FUNC(menu_add_cam);
		BAM_FUNC(menu_add_param);
		BAM_FUNC(menu_add_switch);
		BAM_FUNC(menu_add_button);
		BAM_FUNC(create_submenu);
		BAM_FUNC(menu_add_submenu);
		BAM_FUNC(menu_add_back_button);


		BAM_FUNC(menu_add_3Points);
		BAM_FUNC(menu_add_StartCalibration);
		BAM_FUNC(menu_add_TL);
		BAM_FUNC(menu_add_Reality);
		BAM_FUNC(menu_add_CalibrationBoard);

		BAM_FUNC(get_menu_position);
		BAM_FUNC(set_submenu_cam_info);
		BAM_FUNC(printf);
		BAM_FUNC(printf2);

		// cams
		BAM_FUNC(get_cam_buffer);
		BAM_FUNC(get_cam_update_completed);


		// data to track
		BAM_FUNC(push_raw_XYZ);
		BAM_FUNC(push_raw_XYZ_2Eyes);
		BAM_FUNC(push_raw_cam);
		BAM_FUNC(testing_SetCamXYZ);


		BAM_FUNC(calibrate);
		BAM_FUNC(calibrateCams);
		BAM_FUNC(get_CalbrationBoard);
		BAM_FUNC(set_CamsParams);
		BAM_FUNC(showCheckerboard);
		BAM_FUNC(get_checerboard_points);
		BAM_FUNC(set_checkerboard_3points);

		// detours
		BAM_FUNC(AttachDetour);
		BAM_FUNC(DetachDetour);
		BAM_FUNC(compileShaders);
		BAM_FUNC(GetTextureId);
		BAM_FUNC(ReplaceTexture);
	};

	inline SInternal & Internal() {
		static SInternal data;
		return data;
	}


	inline void Init (int pluginID, HMODULE bam_module) { 
		Internal()._pluginID = pluginID;

		// ========= init BAM functions ==================
		// ::dbg::
		SET_FUNC(MessageBox);
		SET_FUNC(hudDebug);
		SET_FUNC(hudDebugLong);
		SET_FUNC(MessageBox2);
		SET_FUNC(hudDebug2);
		SET_FUNC(hudDebugLong2);


		// ::menu::
		SET_FUNC(menu_create);
		SET_FUNC(menu_add_info);
		SET_FUNC(menu_add_info3D);
		SET_FUNC(menu_add_cam);
		SET_FUNC(menu_add_param);
		SET_FUNC(menu_add_switch);
		SET_FUNC(menu_add_button);
		SET_FUNC(menu_add_submenu);
		SET_FUNC(menu_add_back_button);

		SET_FUNC(menu_add_3Points);
		SET_FUNC(menu_add_StartCalibration);
		SET_FUNC(menu_add_TL);
		SET_FUNC(menu_add_Reality);
		SET_FUNC(menu_add_CalibrationBoard);

		SET_FUNC(get_menu_position);
		SET_FUNC(set_submenu_cam_info);
		SET_FUNC(printf);
		SET_FUNC(printf2);

		SET_FUNC(get_cam_buffer);
		SET_FUNC(get_cam_update_completed);
		
		
		// data to track
		SET_FUNC(push_raw_XYZ);
		SET_FUNC(push_raw_XYZ_2Eyes);
		SET_FUNC(push_raw_cam);
		SET_FUNC(testing_SetCamXYZ);


		SET_FUNC(calibrate);
		SET_FUNC(calibrateCams);
		SET_FUNC(get_CalbrationBoard);
		SET_FUNC(set_CamsParams);
		SET_FUNC(showCheckerboard);
		SET_FUNC(get_checerboard_points);
		SET_FUNC(set_checkerboard_3points);

		SET_FUNC(AttachDetour);
		SET_FUNC(DetachDetour);
		SET_FUNC(compileShaders);
		SET_FUNC(GetTextureId);
		SET_FUNC(ReplaceTexture);
	};

	struct dbg {
		
		// Shows MessageBox
		static void MessageBox(const char *fmt, ...)
		{
			va_list	ap;
			static char txt[2048];

			va_start(ap, fmt);
			vsprintf_s(txt,fmt, ap);
			va_end(ap);

			BAM::Internal().BAM_MessageBox2( txt );
		}

		// Shows debug line (visible in single frame). Use it in hudDisplay
		static void hudDebug(const char *fmt, ...)
		{
			va_list	ap;
			static char txt[2048];

			va_start(ap, fmt);
			vsprintf_s(txt,fmt, ap);
			va_end(ap);

			BAM::Internal().BAM_hudDebug2( txt );
		}

		// Shows debug line in on screen log (last 30 lines)
		static void hudDebugLong(const char *fmt, ...)
		{
			va_list	ap;
			static char txt[2048];

			va_start(ap, fmt);
			vsprintf_s(txt,fmt, ap);
			va_end(ap);

			BAM::Internal().BAM_hudDebugLong2( txt );
		}
	};

	struct menu {
		// add plugin to BAM (menu entry)
		static void create(const char *pluginName, const char *help) {
			BAM::Internal().BAM_menu_create(BAM::Internal()._pluginID, pluginName, help);
		}

		// default menu entry
		struct defaults {
			static void camPosition() {
				BAM::Internal().BAM_menu_add_cam(BAM::Internal()._pluginID);
			}

			static void set3Points() {
				BAM::Internal().BAM_menu_add_3Points(BAM::Internal()._pluginID);
			}

			static void startCalibration() {
				BAM::Internal().BAM_menu_add_StartCalibration(BAM::Internal()._pluginID);
			}

			static void translateAndLighting() {
				BAM::Internal().BAM_menu_add_TL(BAM::Internal()._pluginID);
			}

			static void reality() {
				BAM::Internal().BAM_menu_add_Reality(BAM::Internal()._pluginID);
			}

			static void setCalibrationBoard() {
				BAM::Internal().BAM_menu_add_CalibrationBoard(BAM::Internal()._pluginID);
			}
		};

		// line of text in menu
		static void info(const char *txt) {
			BAM::Internal().BAM_menu_add_info(BAM::Internal()._pluginID, txt);
		}

		// line of text with read only 3 params (like 3D position)
		static void info3D(const char *txt, double *x, double *y, double *z) {
			BAM::Internal().BAM_menu_add_info3D(BAM::Internal()._pluginID, txt, x, y, z, "");
		}

		// line with param. User can change param value
		static void paramValue(const char *txt, double *value, double smallStep, double bigStep, const char *help) {
			BAM::Internal().BAM_menu_add_param(BAM::Internal()._pluginID, txt, value, smallStep, bigStep, help);
		}

		// line with param as switch
		static void paramSwtich(const char *txt, int *value, const char *options[], int count, const char *help) {
			BAM::Internal().BAM_menu_add_switch(BAM::Internal()._pluginID, txt, value, options, count, help);
		}

		// menu option/command
		static void command(const char *txt, BAM_OnButtonPress_func *f) {
			BAM::Internal().BAM_menu_add_button(BAM::Internal()._pluginID, txt, f);
		}

		// display text, info... Use it in hudDisplay
		static void printf(int x, int y, double size, bool shadow, const char *fmt, ...) {
			va_list	ap;
			static char txt[2048];

			va_start(ap, fmt);
			vsprintf_s(txt,fmt, ap);
			va_end(ap);

			BAM::Internal().BAM_printf2(x, y, size, shadow, txt);
		}

		static void showCheckerboard(int status) {
			BAM::Internal().BAM_showCheckerboard(status);
		}

		static double *getChecerboardPoints() {
			return BAM::Internal().BAM_get_checerboard_points();
		}

	};

	struct  cam {
		static BYTE *get_buffer(int cam) {
			return BAM::Internal().BAM_get_cam_buffer(cam);
		}

		static void update_completed(int cam) {
			BAM::Internal().BAM_get_cam_update_completed(cam);
		}
	};

	struct push {
		static void rawXYZ(double x, double y, double z) {
			BAM::Internal().BAM_push_raw_XYZ(BAM::Internal()._pluginID, x, y, z);
		}

		static void rawXYZ_2Eyes(double x1, double y1, double z1, double x2, double y2, double z2) {
			BAM::Internal().BAM_push_raw_XYZ_2Eyes(BAM::Internal()._pluginID, x1, y1, z1, x2, y2, z2);
		}

		static void camsPoints(double *leftPoints, int leftCount, double *rightPoints, int rightCount) {
			BAM::Internal().BAM_push_raw_cam(leftPoints, leftCount, rightPoints, rightCount);
		}

		static void test(double x, double y, double z) {
			BAM::Internal().BAM_testing_SetCamXYZ(x, y, z);
		}

		static void cherkerboard_3points(double *points) {
			BAM::Internal().BAM_set_checkerboard_3points(points);
		}
	};

	struct helpers {

		static const char *buildFilename(const char *fname = NULL, const char *ext = NULL) {
			static char dir[_MAX_PATH+1] = "";
			static char ret[_MAX_PATH+1];
			if (dir[0] == 0) {
				GetCurrentDirectoryA(sizeof(dir), dir);
				strcat_s(dir, _MAX_PATH, "\\");
			}
			strcpy_s(ret, dir);
			if (fname != NULL) {
				strcat_s(ret, _MAX_PATH, fname);
				strcat_s(ret, _MAX_PATH, ext ? ext : ".cfg");
			}
			return ret;
		}

		static void load(const char *fname, void *cfg, size_t len) {
			// load settings
			const char *strPathName = buildFilename(fname, ".cfg");

			FILE *fp;
			fopen_s(&fp, strPathName, "rb");
			if(fp != NULL) {
				fread(cfg, len, 1, fp);
				fclose(fp);
			}
		}

		static void save(const char *fname, void *cfg, size_t len) {
			const char *strPathName = buildFilename(fname, ".cfg");

			FILE *fp;
			fopen_s(&fp, strPathName, "wb");
			if(fp != NULL) {
				fwrite(cfg, len, 1, fp);
				fclose(fp);
			}
		}
	};

	struct detours {
		typedef void *PVOID;
		static void Attach(PVOID *ppPointer, PVOID pDetour) { BAM::Internal().BAM_AttachDetour(ppPointer, pDetour); }
		static void Detach(PVOID *ppPointer, PVOID pDetour) { BAM::Internal().BAM_DetachDetour(ppPointer, pDetour); }
	};

	struct render {
		static unsigned int CompileShader(const char *vertexSrc, const char *fragmentSrc) {
			return BAM::Internal().BAM_compileShaders(vertexSrc, fragmentSrc);
		}

		static unsigned int GetTextureId(BSTR textureName) {
			return BAM::Internal().BAM_GetTextureId(textureName);
		}

		static void ReplaceTexture(unsigned int src, unsigned int dst) {
			BAM::Internal().BAM_ReplaceTexture(src, dst);
		}
	};
};
/*
//#ifndef BAM_PLUGIN_C_FUNCTIONS
//#define BAM_PLUGIN_C_FUNCTIONS
//	BAMEXPORT int BAM_load(HMODULE bam_module) {
//		BAM::onLoad(bam_module);
//	}
//#endif

typedef BAMIMPORT void BAM_MessageBox_func(const char *fmt, ...);
typedef	BAMIMPORT void BAM_menu_create_func(int pluginID, const char *pname, const char *phelp);
typedef	BAMIMPORT void BAM_menu_add_3Points_func(int pluginID);
typedef	BAMIMPORT void BAM_menu_add_info_func(int pluginID, const char *text);
typedef	BAMIMPORT void BAM_push_raw_XYZ_func(int pluginID, double x, double y, double z);
typedef BAMIMPORT void BAM_menu_add_info3D_func(int pluginID, const char *txt, double *x, double *y, double *z, const char *help );

typedef	BAMIMPORT void BAM_menu_add_cam_func(int pluginID);
typedef	BAMIMPORT void BAM_menu_add_TL_func(int pluginID);
typedef	BAMIMPORT void BAM_menu_add_Reality_func(int pluginID);
typedef	BAMIMPORT void BAM_menu_add_param_func(int PluginID, const char *txt, double *value, double smallStep, double bigStep, const char *help );
typedef BAMIMPORT BYTE *BAM_get_cam_buffer_func(int cam);

typedef BAMIMPORT void BAM_menu_add_switch_func(int PluginID, const char *txt, int *value, const char *options[], int count, const char *help );

#define ARRAY_ENTRIES(array)	(sizeof(array)/sizeof(array[0]))
#define PLUGIN_ID_NUM 0x0088 // RaV plugins 0x80 - 0xa0, 0x0101 - Kinect, other - free
*/
extern "C" {
	BAMEXPORT int BAM_load(HMODULE bam_module);
	BAMEXPORT void BAM_unload();
	BAMEXPORT void BAM_hudDisplay();
	BAMEXPORT void BAM_PluginStart();
	BAMEXPORT void BAM_PluginStop();
	BAMEXPORT void BAM_AttachDetours();
	BAMEXPORT void BAM_DetachDetours();
	BAMEXPORT void BAM_swapBuffers(HDC hDC);
};


// -----------------------------------------------------------------
/********************************************************
dbg::hudDebug(const char *fmt, ...);
dbg::hudDebugLong(const char *fmt, ...);

- x, y, position on BAM hud. left/lower corner [0,0] , top/right corner [1079, 639] 
  it is "virtual resolution" 

- size (in menu i use 2.2f)

- shadow - (then true is drawed "shadow" to letters)


Now about MY#MOD.

In all text printed in hud (in menu, help, or from BAM_printf) i use # as special char to modify displayed text.
To draw # You have to add ##

Now how to use '#'

#cxyz# - change color to (RGB) xxyyzz (x/y/z is hex [0-9a-f] )
#cxxyyzz# - full rgb color

#+ - increase font "25%"

#- - decrease font "25%"

example:   "text#-smalltext#+sameSizeLikeAtBegin#c00f#bluetext"

#l0123# - (small L) - draw text before # and  move text pointer given number "pixels" left (text have constant width)

example "PARAM#l300#VALUE" - will draw "PARAM" as text with Width = 300 and after this VALUE

#m123# - center text, text can be wider than given "value".
example 
BAM_printf(500, 500, 3.0, true, "centeredtext#m0"); 
BAM_printf(500, 400, 3.0, true, "centeredlongtext#m0"); 
BAM_printf(500, 300, 3.0, true, "smal#m0"); 
will draw all text centered

#r123# - move text to right of given "space"

#. - is joust marker to divide text
example
"text#.to right#r300#" ... will draw "text                                  to right"


i added macros:
#define DEFPW "#r160#"
#define DEFIW "#m350#"
 
DEFIW will center "buttons" / "submenus"
DEFPW will move "params names to right"

example:
"#-Processing res:"DEFPW"#+ #cfff#%s", .
is like:
"Processing res: %s"

but text "Processing res:" is moved to right and whole text is 160 "virtual pixels" wide and is drawed 25% smaller font

text value %s is drawed "normal size" and color is set to "white".


**********************************************************************************************/

