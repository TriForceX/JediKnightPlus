# Make sure the user is not executing this script directly
if(NOT InMVSDK)
	message(FATAL_ERROR "Use the top-level cmake script!")
endif(NOT InMVSDK)

set(CPPFiles
	"cpp.h"

	"cpp.c"
	"eval.c"
	"getopt.c"
	"hideset.c"
	"include.c"
	"lex.c"
	"macro.c"
	"nlist.c"
	"tokens.c"
	"unix.c"
)

add_executable(${CPP} ${CPPFiles})
set_target_properties(${CPP} PROPERTIES PROJECT_LABEL "q3cpp")
set_property(TARGET ${CPP} PROPERTY FOLDER "Tools")
