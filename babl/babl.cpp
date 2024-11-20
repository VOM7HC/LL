// babl.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "babl.hpp"


// This is an example of an exported variable
BABL_API int nbabl=0;

// This is an example of an exported function.
BABL_API int fnbabl(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cbabl::Cbabl()
{
    return;
}
