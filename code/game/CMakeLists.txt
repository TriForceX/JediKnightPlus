# Make sure the user is not executing this script directly
if(NOT InMVSDK)
	message(FATAL_ERROR "Use the top-level cmake script!")
endif(NOT InMVSDK)

set(MVGAMEHEADERS
	"${SRCDir}/game/ai_main.h"
	"${SRCDir}/game/anims.h"
	"${SRCDir}/game/be_aas.h"
	"${SRCDir}/game/be_ai_char.h"
	"${SRCDir}/game/be_ai_chat.h"
	"${SRCDir}/game/be_ai_gen.h"
	"${SRCDir}/game/be_ai_goal.h"
	"${SRCDir}/game/be_ai_move.h"
	"${SRCDir}/game/be_ai_weap.h"
	"${SRCDir}/game/be_ea.h"
	"${SRCDir}/game/bg_local.h"
	"${SRCDir}/game/bg_public.h"
	"${SRCDir}/game/bg_saga.h"
	"${SRCDir}/game/bg_weapons.h"
	"${SRCDir}/game/botlib.h"
	"${SRCDir}/game/chars.h"
	"${SRCDir}/game/g_local.h"
	"${SRCDir}/game/g_public.h"
	"${SRCDir}/game/g_team.h"
	"${SRCDir}/game/inv.h"
	"${SRCDir}/game/match.h"
	"${SRCDir}/game/q_shared.h"
	"${SRCDir}/game/surfaceflags.h"
	"${SRCDir}/game/syn.h"
	"${SRCDir}/game/w_saber.h"
	"${SRCDir}/game/g_multiversion.h"
	"${SRCDir}/game/g_multiversion_syscalls.h"
	"${SRCDir}/game/bg_multiversion.h"

	"${SRCDir}/cgame/animtable.h"
	"${SRCDir}/cgame/cg_local.h"
	"${SRCDir}/cgame/cg_public.h"
	"${SRCDir}/cgame/tr_types.h"

	"${SRCDir}/qcommon/tags.h"

	"${SRCDir}/ghoul2/g2.h"

	"${SRCDir}/api/mvapi.h"
)

set(MVGAMESOURCES
	"${SRCDir}/game/g_main.c" # keep this on top (qvm compiler needs VM_Main as the first compiled function)
	"${SRCDir}/game/ai_main.c"
	"${SRCDir}/game/ai_util.c"
	"${SRCDir}/game/ai_wpnav.c"
	"${SRCDir}/game/bg_misc.c"
	"${SRCDir}/game/bg_panimate.c"
	"${SRCDir}/game/bg_pmove.c" 
	"${SRCDir}/game/bg_saber.c"
	"${SRCDir}/game/bg_slidemove.c"
	"${SRCDir}/game/bg_weapons.c"
	"${SRCDir}/game/g_active.c"
	"${SRCDir}/game/g_arenas.c"
	"${SRCDir}/game/g_bot.c"
	"${SRCDir}/game/g_client.c"
	"${SRCDir}/game/g_cmds.c"
	"${SRCDir}/game/g_combat.c"
	"${SRCDir}/game/g_items.c"
	"${SRCDir}/game/g_log.c"
	"${SRCDir}/game/g_mem.c"
	"${SRCDir}/game/g_misc.c"
	"${SRCDir}/game/g_missile.c"
	"${SRCDir}/game/g_mover.c"
	"${SRCDir}/game/g_object.c"
	"${SRCDir}/game/g_saga.c"
	"${SRCDir}/game/g_session.c"
	"${SRCDir}/game/g_spawn.c"
	"${SRCDir}/game/g_svcmds.c"
	"${SRCDir}/game/g_target.c"
	"${SRCDir}/game/g_team.c"
	"${SRCDir}/game/g_trigger.c"
	"${SRCDir}/game/g_utils.c"
	"${SRCDir}/game/g_weapon.c"
	"${SRCDir}/game/q_math.c"
	"${SRCDir}/game/q_shared.c"
	"${SRCDir}/game/w_force.c"
	"${SRCDir}/game/w_saber.c"
	"${SRCDir}/game/g_multiversion.c"
	"${SRCDir}/game/g_multiversion_syscalls.c"
	"${SRCDir}/game/bg_multiversion.c"
	"${SRCDir}/game/animMappingTable.c"
)

set(MVGAMEASM
	"${SRCDir}/game/g_syscalls.asm"
	PARENT_SCOPE
)

set(MVGAMEFILES
	${MVGAMEHEADERS}
	${MVGAMESOURCES}
	"${SRCDir}/game/g_syscalls.c"
)

set(MVGAMEVMSOURCES ${MVGAMESOURCES}
	"${SRCDir}/game/bg_lib.c"
	PARENT_SCOPE
)

add_library(${MVGAME} SHARED ${MVGAMEFILES})

# Defines
set(MVGAMEDefines ${MVGAMEDefines} "QAGAME")
set(MVGAMEDefines ${MVGAMEDefines} "JK2_GAME")
set(MVGAMEIncludeDirs ${MVGLOBALIncludeDirs})

set_target_properties(${MVGAME} PROPERTIES PREFIX "") # remove "lib" prefix for .so/.dylib files

set_property(TARGET ${MVGAME} APPEND PROPERTY COMPILE_DEFINITIONS
  $<$<CONFIG:Debug>:${GlobalDefines} ${MVGAMEDefines} ${DebugDefines}>
  $<$<CONFIG:Release>:${GlobalDefines} ${MVGAMEDefines} ${ReleaseDefines}>
  $<$<CONFIG:RelWithDebInfo>:${GlobalDefines} ${MVGAMEDefines} ${ReleaseDefines}>
)

set_target_properties(${MVGAME} PROPERTIES INCLUDE_DIRECTORIES "${MVGAMEIncludeDirs}")
set_target_properties(${MVGAME} PROPERTIES PROJECT_LABEL "Game Module")
if(InMV)
	set_property(TARGET ${MVGAME} PROPERTY FOLDER "Modules")
endif()
