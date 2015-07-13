#pragma once
#ifdef LIBIPHONEDLL_EXPORTS
#define LIBIPHONEDLL_API __declspec(dllexport)
#else
#define LIBIPHONEDLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
LIBIPHONEDLL_API int libiphoneDll_info(int argc, char *argv[]);

LIBIPHONEDLL_API int libiphoneDll_installer(int argc, char *argv[]);

LIBIPHONEDLL_API int libiphoneDll_backup2(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif