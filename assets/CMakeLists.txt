# Make sure the user is not executing this script directly
if(NOT InMVSDK)
	message(FATAL_ERROR "Use the top-level cmake script!")
endif(NOT InMVSDK)

# Generate PK3 file
set(PK3FILE jkmod_plugin_0.0.0.pk3)
set(PK3PATH ${CMAKE_SOURCE_DIR}/assets/client)
set(TMPFILE ${CMAKE_CURRENT_BINARY_DIR}/${PK3FILE})
set(OUTFILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/${PK3FILE})

file(GLOB PK3ASSETS "${PK3PATH}/*")

include(InstallZIP)
add_zip_command(${TMPFILE} ${OUTFILE} DIR "${PK3PATH}" FILES "${PK3ASSETS}" )

# Finish and cleanup
add_custom_target(assets DEPENDS "${TMPFILE}")
add_custom_command(TARGET assets POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E remove -f "${TMPFILE}"
)
