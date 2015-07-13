// libiphoneDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "libiphoneDll.h"
#include "idevicebackup2.h"
#include "ideviceinfo.h"
#include "ideviceinstaller.h"

LIBIPHONEDLL_API int libiphoneDll_info(int argc, char *argv[])
{	
	return libiphone_info(argc, argv);
}

LIBIPHONEDLL_API int libiphoneDll_installer(int argc, char *argv[])
{
	return libiphone_installer(argc, argv);
}

LIBIPHONEDLL_API int libiphoneDll_backup2(int argc, char *argv[])
{
	return libiphone_backup2(argc, argv);
}

