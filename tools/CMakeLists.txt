# Make sure the user is not executing this script directly
if(NOT InMVSDK)
	message(FATAL_ERROR "Use the top-level cmake script!")
endif(NOT InMVSDK)

if(${CMAKE_C_COMPILER_ID} MATCHES GNU|Clang|AppleClang)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-sign-compare")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tools)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tools)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tools)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/tools)

add_subdirectory("lcc")
add_subdirectory("cpp")
add_subdirectory("rcc")
add_subdirectory("asm")
