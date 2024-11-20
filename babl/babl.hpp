// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the BABL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// BABL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef BABL_EXPORTS
#define BABL_API __declspec(dllexport)
#else
#define BABL_API __declspec(dllimport)
#endif

// This class is exported from the dll
class BABL_API Cbabl {
public:
	Cbabl(void);
	// TODO: add your methods here.
};

extern BABL_API int nbabl;

BABL_API int fnbabl(void);
