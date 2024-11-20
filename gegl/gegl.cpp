// gegl.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "gegl.h"


// This is an example of an exported variable
GEGL_API int ngegl=0;

// This is an example of an exported function.
GEGL_API int fngegl(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cgegl::Cgegl()
{
    return;
}
