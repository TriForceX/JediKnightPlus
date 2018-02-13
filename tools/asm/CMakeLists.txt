# Make sure the user is not executing this script directly
if(NOT InMVSDK)
	message(FATAL_ERROR "Use the top-level cmake script!")
endif(NOT InMVSDK)

set(ASMFiles
	"cmdlib.h"
	"mathlib.h"
	"opstrings.h"

	"cmdlib.c"
	"q3asm.c"
)

add_executable(${ASM} ${ASMFiles})
set_target_properties(${ASM} PROPERTIES PROJECT_LABEL "q3asm")
set_property(TARGET ${ASM} PROPERTY FOLDER "Tools")
