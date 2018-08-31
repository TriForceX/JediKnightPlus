# Make sure the user is not executing this script directly
if(NOT InMVSDK)
	message(FATAL_ERROR "Use the top-level cmake script!")
endif(NOT InMVSDK)

# mv_setup.h preparation
set(MVGLOBALIncludeDirs ${MVGLOBALIncludeDirs} ${CMAKE_BINARY_DIR})
configure_file("mvsdk_setup.h.in" "${CMAKE_BINARY_DIR}/mvsdk_setup.h" @ONLY)

add_subdirectory("game")
add_subdirectory("cgame")
add_subdirectory("ui")

if(BuildQVMs)
	# Custom target for building qvm's
	file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/vm_build")
	file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/vm")

	foreach(FILE ${MVGAMEVMSOURCES})
		file(TO_NATIVE_PATH "${FILE}" FILE_NATIVE)
		set(MVGAMEVMSOURCES_NATIVE ${MVGAMEVMSOURCES_NATIVE} ${FILE_NATIVE})

		get_filename_component(FILE ${FILE} NAME_WE)
		set(MVGAMEASMFILES ${MVGAMEASMFILES} "${FILE}.asm")
	endforeach()

	foreach(FILE ${MVCGAMEVMSOURCES})
		file(TO_NATIVE_PATH "${FILE}" FILE_NATIVE)
		set(MVCGAMEVMSOURCES_NATIVE ${MVCGAMEVMSOURCES_NATIVE} ${FILE_NATIVE})

		get_filename_component(FILE ${FILE} NAME_WE)
		set(MVCGAMEASMFILES ${MVCGAMEASMFILES} "${FILE}.asm")
	endforeach()

	foreach(FILE ${MVUIVMSOURCES})
		file(TO_NATIVE_PATH "${FILE}" FILE_NATIVE)
		set(MVUIVMSOURCES_NATIVE ${MVUIVMSOURCES_NATIVE} ${FILE_NATIVE})

		get_filename_component(FILE ${FILE} NAME_WE)
		set(MVUIASMFILES ${MVUIASMFILES} "${FILE}.asm")
	endforeach()

	file(RELATIVE_PATH MVSDK_CGAME_PATH "${CMAKE_CURRENT_BINARY_DIR}/vm_build" "${SRCDir}/cgame")
	file(TO_NATIVE_PATH "${MVSDK_CGAME_PATH}" MVSDK_CGAME_PATH)
	
	file(RELATIVE_PATH MVSDK_GAME_PATH "${CMAKE_CURRENT_BINARY_DIR}/vm_build" "${SRCDir}/game")
	file(TO_NATIVE_PATH "${MVSDK_GAME_PATH}" MVSDK_GAME_PATH)
	
	file(RELATIVE_PATH MVSDK_UI_PATH "${CMAKE_CURRENT_BINARY_DIR}/vm_build" "${SRCDir}/ui")
	file(TO_NATIVE_PATH "${MVSDK_UI_PATH}" MVSDK_UI_PATH)
	
	file(RELATIVE_PATH MVSDK_BIN_PATH "${CMAKE_CURRENT_BINARY_DIR}/vm_build" "${CMAKE_BINARY_DIR}")
	file(TO_NATIVE_PATH "${MVSDK_BIN_PATH}" MVSDK_BIN_PATH)
	
	add_custom_target(qvms
		COMMAND $<TARGET_FILE:${LCC}>
			"-DQ3_VM" "-DJK2_GAME" "-DMISSIONPACK" "-S" "-Wf-target=bytecode" "-Wf-g" "-I${MVSDK_GAME_PATH}" "-I${MVSDK_UI_PATH}" "-I${MVSDK_BIN_PATH}" ${MVGAMEVMSOURCES_NATIVE}

		COMMAND $<TARGET_FILE:${ASM}>
			"-vq3" "-m" "-v" "-o" "${CMAKE_CURRENT_BINARY_DIR}/vm/jk2mpgame.qvm" ${MVGAMEASMFILES} ${MVGAMEASM}

		COMMAND $<TARGET_FILE:${LCC}>
			"-DQ3_VM" "-DJK2_CGAME" "-DMISSIONPACK" "-S" "-Wf-target=bytecode" "-Wf-g" "-I${MVSDK_CGAME_PATH}" "-I${MVSDK_GAME_PATH}" "-I${MVSDK_UI_PATH}" "-I${MVSDK_BIN_PATH}" ${MVCGAMEVMSOURCES_NATIVE}

		COMMAND $<TARGET_FILE:${ASM}>
			"-vq3" "-m" "-v" "-o" "${CMAKE_CURRENT_BINARY_DIR}/vm/cgame.qvm" ${MVCGAMEASMFILES} ${MVCGAMEASM}

		COMMAND $<TARGET_FILE:${LCC}>
			"-DQ3_VM" "-DJK2_UI" "-S" "-Wf-target=bytecode" "-Wf-g" "-I${SRCDir}/cgame" "-I${MVSDK_GAME_PATH}" "-I${MVSDK_UI_PATH}" "-I${MVSDK_BIN_PATH}" ${MVUIVMSOURCES_NATIVE}

		COMMAND $<TARGET_FILE:${ASM}>
			"-vq3" "-m" "-v" "-o" "${CMAKE_CURRENT_BINARY_DIR}/vm/ui.qvm" ${MVUIASMFILES} ${MVUIASM}

		WORKING_DIRECTORY
			"${CMAKE_CURRENT_BINARY_DIR}/vm_build"

		DEPENDS
			${LCC} ${CPP} ${RCC} ${ASM}
	)

	set_property(TARGET qvms PROPERTY PROJECT_LABEL "Build QVMs")
	if(InMV)
		set_property(TARGET qvms PROPERTY FOLDER "Modules")
	endif()

	if(InMV)
		configure_file("mv.info" "mv.info" COPYONLY)
		set(AssetsFiles
			"vm/jk2mpgame.qvm"
            "vm/jk2mpgame.map"
			"vm/cgame.qvm"
            "vm/cgame.map"
			"vm/ui.qvm"
            "vm/ui.map"
			"mv.info"
		)

		source_group("assetsmv2" FILES ${AssetsFiles})

		# generate assetsmv2.pk3
		if(MultiConfigGenerator)
			set(OUTFILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/base/assetsmv2.pk3)
		else()
			set(OUTFILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/base/assetsmv2.pk3)
		endif()

		include(InstallZIP)
		add_zip_command(${CMAKE_CURRENT_BINARY_DIR}/assetsmv2.pk3 ${OUTFILE}
			DIR "${CMAKE_CURRENT_BINARY_DIR}"
			FILES "${AssetsFiles}"
			DEPENDS qvms
		)

		add_custom_target(assetsmv2 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/assetsmv2.pk3")
		set_property(TARGET assetsmv2 PROPERTY FOLDER "Modules")

		if(WIN32 OR BuildPortableVersion)
			install(FILES ${CMAKE_CURRENT_BINARY_DIR}/assetsmv2.pk3 DESTINATION "base")
		elseif(APPLE)
			install(FILES ${CMAKE_CURRENT_BINARY_DIR}/assetsmv2.pk3 DESTINATION "jk2mvmp.app/Contents/MacOS/base")
		else()
			install(FILES ${CMAKE_CURRENT_BINARY_DIR}/assetsmv2.pk3 DESTINATION "share/jk2mv/base")
		endif()
	endif()
endif()
