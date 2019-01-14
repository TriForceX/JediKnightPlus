/*
===================== Jedi Knight Plus Launcher =====================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2019
=====================================================================
[Description]: Version information for the main application
=====================================================================
*/

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define VERSION_MAJOR               1
#define VERSION_MINOR               0
#define VERSION_REVISION            0
#define VERSION_BUILD               0

#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
                                    "." STRINGIZE(VERSION_MINOR)    \
                                    "." STRINGIZE(VERSION_REVISION) \
                                    "." STRINGIZE(VERSION_BUILD)    \

#define VER_PRODUCT_VERSION         VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR     VER_FILE_VERSION_STR

#define VER_FILE_DESCRIPTION_STR    "Jedi Knight Plus Launcher"
#define VER_PRODUCTNAME_STR         "Jedi Knight Plus Launcher"
#define VER_COPYRIGHT_STR           "Tr!Force"
#define VER_INTERNAL_NAME_STR       "jkplus_launcher"
#define VER_ORIGINAL_FILENAME_STR   VER_INTERNAL_NAME_STR ".exe"

#ifdef _DEBUG
#define VER_VER_DEBUG				0x1L 
#else
#define VER_VER_DEBUG				0x0L 
#endif

#define VER_FILEFLAGS               VER_VER_DEBUG
#define VER_FILEFLAGSMASK           0x17L 
#define VER_FILEOS                  0x4L
#define VER_FILETYPE                0x1L
#define VER_FILESUBTYPE             0x0L 

#define VER_TRANSLATIONLANG			0x0409
#define VER_TRANSLATIONCHARSET		1200
