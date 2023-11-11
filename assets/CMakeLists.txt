# Make sure the user is not executing this script directly
if(NOT InMVSDK)
	message(FATAL_ERROR "Use the top-level cmake script!")
endif(NOT InMVSDK)

# Get mod version
file(READ "${CMAKE_SOURCE_DIR}/jkmod/game/jk_version.h" VER)
string(REGEX MATCH "JK_MAJOR 			([0-9]*)" _ ${VER})
set(VER_MAJOR ${CMAKE_MATCH_1})
string(REGEX MATCH "JK_MINOR 			([0-9]*)" _ ${VER})
set(VER_MINOR ${CMAKE_MATCH_1})
string(REGEX MATCH "JK_PATCH 			([0-9]*)" _ ${VER})
set(VER_PATCH ${CMAKE_MATCH_1})

# Set main variables
set(PK3FILE jkmod_plugin_${VER_MAJOR}.${VER_MINOR}.${VER_PATCH}.pk3)
set(SRVPATH ${CMAKE_SOURCE_DIR}/assets/server)
set(PK3PATH ${CMAKE_SOURCE_DIR}/assets/client)
set(QVMPATH ${CMAKE_BINARY_DIR}/code/vm)
set(TMPPATH ${CMAKE_CURRENT_BINARY_DIR}/client)
set(TMPFILE ${CMAKE_CURRENT_BINARY_DIR}/${PK3FILE})
set(OUTFILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MVSDK}/${PK3FILE})

# Add custom target
add_custom_target(assets DEPENDS ${TMPFILE} ${TMPPATH})
set_property(TARGET assets PROPERTY PROJECT_LABEL "Build Assets")

# Check QVM files
if(EXISTS ${QVMPATH}/jk2mpgame.qvm AND EXISTS ${QVMPATH}/cgame.qvm AND EXISTS ${QVMPATH}/ui.qvm)
	# Server stuff
	message(STATUS "Configure server stuff...")
	file(COPY "${SRVPATH}/" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MVSDK}/)
	file(COPY "${QVMPATH}/jk2mpgame.qvm" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MVSDK}/vm)
	configure_file(${SRVPATH}/ref_whitelist.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MVSDK}/ref_whitelist.txt)
	
	# Client stuff
	message(STATUS "Configure client stuff...")
	file(GLOB PK3QVMS ${QVMPATH}/cgame.qvm ${QVMPATH}/ui.qvm)
	file(COPY "${PK3PATH}/" DESTINATION ${TMPPATH})
	file(COPY ${PK3QVMS} DESTINATION ${TMPPATH}/vm/)
	file(GLOB PK3ASSETS "${TMPPATH}/*")
	
	# Compressed stuff
	message(STATUS "Configure PK3 file...")
	include(InstallZIP)
	add_zip_command(${TMPFILE} ${OUTFILE} DIR "${TMPPATH}" FILES "${PK3ASSETS}" DEPENDS ${TMPFILE})
	
	# Finish and cleanup
	add_custom_command(TARGET assets POST_BUILD
		COMMENT "Cleaning temporary files..."
		COMMAND ${CMAKE_COMMAND} -E remove_directory -f ${TMPPATH}
		COMMAND ${CMAKE_COMMAND} -E remove -f ${TMPFILE}
		DEPENDS ${TMPFILE} ${TMPPATH}
	)
endif()

# Clean previous cache
add_custom_command(TARGET assets PRE_BUILD
	COMMENT "Cleaning any previous cache..."
	COMMAND ${CMAKE_COMMAND} -E remove_directory -f ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MVSDK}
	COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/generate.stamp.depend
)