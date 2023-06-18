# Make sure the user is not executing this script directly
if(NOT InMVSDK)
	message(FATAL_ERROR "Use the top-level cmake script!")
endif(NOT InMVSDK)

# Get current version
file(READ "${CMAKE_SOURCE_DIR}/jkmod/game/jk_version.h" VER)
string(REGEX MATCH "JK_MAJOR 			([0-9]*)" _ ${VER})
set(VER_MAJOR ${CMAKE_MATCH_1})
string(REGEX MATCH "JK_MINOR 			([0-9]*)" _ ${VER})
set(VER_MINOR ${CMAKE_MATCH_1})
string(REGEX MATCH "JK_PATCH 			([0-9]*)" _ ${VER})
set(VER_PATCH ${CMAKE_MATCH_1})

# Generate PK3 file
set(PK3FILE jkmod_plugin_${VER_MAJOR}.${VER_MINOR}.${VER_PATCH}.pk3)
set(SRVPATH ${CMAKE_SOURCE_DIR}/assets/server)
set(PK3PATH ${CMAKE_SOURCE_DIR}/assets/client)
set(TMPFILE ${CMAKE_CURRENT_BINARY_DIR}/${PK3FILE})
set(OUTFILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/${PK3FILE})

configure_file(${SRVPATH}/ref_forcelist.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/ref_forcelist.txt)
configure_file(${SRVPATH}/ref_whitelist.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/ref_whitelist.txt)

file(MAKE_DIRECTORY "${SRVPATH}/vm")
file(MAKE_DIRECTORY "${PK3PATH}/vm")
file(GLOB PK3ASSETS "${PK3PATH}/*")

include(InstallZIP)
add_zip_command(${TMPFILE} ${OUTFILE} DIR "${PK3PATH}" FILES "${PK3ASSETS}" )

# Finish and cleanup
add_custom_target(assets DEPENDS "${TMPFILE}")
add_custom_command(TARGET assets POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E remove -f "${TMPFILE}"
	COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/generate.stamp.depend
)
