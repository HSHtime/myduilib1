// dllTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../libiphoneDll/libiphoneDll.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char *temp[] = { "ideviceinstaller"
	};
	libiphoneDll_info(1, temp);
	getchar();
	return 0;
}

