// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the GEGL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// GEGL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GEGL_EXPORTS
#define GEGL_API __declspec(dllexport)
#else
#define GEGL_API __declspec(dllimport)
#endif

// This class is exported from the dll
class GEGL_API Cgegl {
public:
	Cgegl(void);
	// TODO: add your methods here.
};

extern GEGL_API int ngegl;

GEGL_API int fngegl(void);
