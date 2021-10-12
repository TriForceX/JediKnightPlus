/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Main client module, cvar definitions, etc...
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header

// Extern stuff
extern float CG_Cvar_Get(const char *cvar);

/*
=====================================================================
Cvar table list
=====================================================================
*/

typedef struct 
{
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;

} cvarTable_t;

vmCvar_t	jkcvar_cg_duelGlow;
vmCvar_t	jkcvar_cg_duelEnd;
vmCvar_t	jkcvar_cg_duelEndOrbit;
vmCvar_t	jkcvar_cg_duelEndDelay;
vmCvar_t	jkcvar_cg_drawClock;
vmCvar_t	jkcvar_cg_drawHitBox;
vmCvar_t	jkcvar_cg_drawForcePoints;
vmCvar_t	jkcvar_cg_drawInventory;
vmCvar_t	jkcvar_cg_drawRaceTimer;
vmCvar_t	jkcvar_cg_drawBactaModel;
vmCvar_t	jkcvar_cg_drawPlayerNames;
vmCvar_t	jkcvar_cg_drawMovementKeys;
vmCvar_t	jkcvar_cg_scoreboardIcons;
vmCvar_t	jkcvar_cg_scoreboardExtras;
vmCvar_t	jkcvar_cg_chatIcon;
vmCvar_t	jkcvar_cg_saberTrailSpeed;
vmCvar_t	jkcvar_cg_damageBlend;
vmCvar_t	jkcvar_cg_flagOpacity;
vmCvar_t	jkcvar_cg_flagAlignment;
vmCvar_t	jkcvar_cg_chatPlayerOpacity;
vmCvar_t	jkcvar_cg_specialMoveCamera;

vmCvar_t	jkcvar_cg_chatBox;
vmCvar_t	jkcvar_cg_chatBoxTime;
vmCvar_t	jkcvar_cg_chatBoxHeight;
vmCvar_t	jkcvar_cg_chatBoxHistory;

vmCvar_t	jkcvar_cg_customHats;
vmCvar_t	jkcvar_cg_customAnims;
vmCvar_t	jkcvar_cg_jetPackIdle;

vmCvar_t	jkcvar_cg_test1;
vmCvar_t	jkcvar_cg_test2;
vmCvar_t	cg_developer;

static cvarTable_t	JKModCGcvarTable[] = {

	{ &jkcvar_cg_duelGlow,			"jk_cg_duelGlow",			"1",	CVAR_ARCHIVE },
	{ &jkcvar_cg_duelEnd,			"jk_cg_duelEnd",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_duelEndOrbit,		"jk_cg_duelEndOrbit",		"1.5",	CVAR_ARCHIVE },
	{ &jkcvar_cg_duelEndDelay,		"jk_cg_duelEndDelay",		"1",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawClock,			"jk_cg_drawClock",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawHitBox,		"jk_cg_drawHitBox",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawForcePoints,	"jk_cg_drawForcePoints",	"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawInventory,		"jk_cg_drawInventory",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawRaceTimer,		"jk_cg_drawRaceTimer",		"1",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawBactaModel,	"jk_cg_drawBactaModel",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawPlayerNames,	"jk_cg_drawPlayerNames",	"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawMovementKeys,	"jk_cg_drawMovementKeys",	"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_scoreboardIcons,	"jk_cg_scoreboardIcons",	"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_scoreboardExtras,	"jk_cg_scoreboardExtras",	"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatIcon,			"jk_cg_chatIcon",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_saberTrailSpeed,	"jk_cg_saberTrailSpeed",	"40",	CVAR_ARCHIVE },
	{ &jkcvar_cg_damageBlend,		"jk_cg_damageBlend",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_flagOpacity,		"jk_cg_flagOpacity",		"255",	CVAR_ARCHIVE },
	{ &jkcvar_cg_flagAlignment,		"jk_cg_flagAlignment",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatPlayerOpacity,	"jk_cg_chatPlayerOpacity",	"1",	CVAR_ARCHIVE },
	{ &jkcvar_cg_specialMoveCamera,	"jk_cg_specialMoveCamera",	"0",	CVAR_ARCHIVE },

	{ &jkcvar_cg_chatBox,			"jk_cg_chatBox",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatBoxTime,		"jk_cg_chatBoxTime",		"10",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatBoxHeight,		"jk_cg_chatBoxHeight",		"360",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatBoxHistory,	"jk_cg_chatBoxHistory",		"1",	CVAR_ARCHIVE },

	{ &jkcvar_cg_customHats,		"jk_cg_customHats",			"0",	CVAR_ARCHIVE | CVAR_USERINFO },
	{ &jkcvar_cg_customAnims,		"jk_cg_customAnims",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_jetPackIdle,		"jk_cg_jetPackIdle",		"0",	CVAR_ARCHIVE },

	{ &jkcvar_cg_test1,				"jk_cg_test1",				"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_test2,				"jk_cg_test2",				"0",	CVAR_ARCHIVE },
	{ &cg_developer,				"developer",				"0",	CVAR_NONE },

};

static int JKModCGcvarTableSize = ARRAY_LEN(JKModCGcvarTable);

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/
void JKMod_CG_RegisterCvars(void)
{
	int			i;
	cvarTable_t	*cv;
	char		var[MAX_TOKEN_CHARS];

	// Register all the cvars
	for (i = 0, cv = JKModCGcvarTable; i < JKModCGcvarTableSize; i++, cv++) {
		trap_Cvar_Register(cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags);
	}

	// Launch original register cvars function
	BaseJK2_CG_RegisterCvars();

	// Set the client plugin version
	trap_Cvar_Register(NULL, "jkmod_client", "", CVAR_USERINFO | CVAR_ROM);
	trap_Cvar_Set("jkmod_client", JK_VERSION);

	// Check console notify time
	cg.jkmodCG.consoleNotifyTime = CG_Cvar_Get("con_notifytime");
}

void JKMod_CG_UpdateCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Update all the cvars
	for (i = 0, cv = JKModCGcvarTable; i < JKModCGcvarTableSize; i++, cv++) {
		trap_Cvar_Update(cv->vmCvar);
	}

	// Launch original update cvars function
	BaseJK2_CG_UpdateCvars();

	// Check console notify time
	cg.jkmodCG.consoleNotifyTime = CG_Cvar_Get("con_notifytime");
}

/*
=====================================================================
Only print when "developer 1"
=====================================================================
*/
void QDECL JKMod_CG_Printf(const char *msg, ...) 
{
	if (cg_developer.integer)
	{
		va_list		argptr;
		char		text[1024];

		va_start (argptr, msg);
		Q_vsnprintf (text, sizeof(text), msg, argptr);
		va_end (argptr);

		cg.jkmodCG.consolePrint++;
		cg.jkmodCG.consolePrintTime = cg.time;
		trap_Print(text);
	}
}

/*
=====================================================================
Register client plugin media
=====================================================================
*/
void JKMod_CG_RegisterMedia(void) 
{
	cgs.jkmodMedia.saberModel			= trap_R_RegisterModel("models/weapons2/saber/saber_w.md3");
	cgs.jkmodMedia.bactaModel			= trap_R_RegisterModel("models/items/bacta.md3");
	cgs.jkmodMedia.jetpackModel			= trap_R_RegisterModel("models/items/jkmod_jetpack.md3");
	cgs.jkmodMedia.hatSanta				= trap_R_RegisterModel("models/items/jkmod_hats/santa.md3");
	cgs.jkmodMedia.hatPumpkin			= trap_R_RegisterModel("models/items/jkmod_hats/pumpkin.md3");
	cgs.jkmodMedia.hatCap				= trap_R_RegisterModel("models/items/jkmod_hats/cap.md3");
	cgs.jkmodMedia.hatCowboy			= trap_R_RegisterModel("models/items/jkmod_hats/cowboy.md3");
	cgs.jkmodMedia.hatCringe			= trap_R_RegisterModel("models/items/jkmod_hats/cringe.md3");
	cgs.jkmodMedia.hatSombrero			= trap_R_RegisterModel("models/items/jkmod_hats/sombrero.md3");
	cgs.jkmodMedia.hatGentleman			= trap_R_RegisterModel("models/items/jkmod_hats/gentleman.md3");
	cgs.jkmodMedia.hatPirate			= trap_R_RegisterModel("models/items/jkmod_hats/pirate.md3");
	cgs.jkmodMedia.hatProbe				= trap_R_RegisterModel("models/players/droids/probe_droid_head.md3");
	cgs.jkmodMedia.hatDroid				= trap_R_RegisterModel("models/players/droids/r5d2_head.md3");
	cgs.jkmodMedia.hatYsalamiri			= trap_R_RegisterModel("models/map_objects/mp/ysalimari.md3");

	cgs.jkmodMedia.hitBox				= trap_R_RegisterShader("jkmod_hitbox");
	cgs.jkmodMedia.hitBoxNoCull			= trap_R_RegisterShader("jkmod_hitbox_nocull");
	cgs.jkmodMedia.dotRed				= trap_R_RegisterShader("gfx/misc/borgeyeflare");
	cgs.jkmodMedia.dotGreen				= trap_R_RegisterShader("gfx/mp/dmgshader_shields");
	cgs.jkmodMedia.dotYellow			= trap_R_RegisterShader("gfx/mp/dmgshader_shieldsandhealth");
	cgs.jkmodMedia.clockBg				= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_clock_bg");
	cgs.jkmodMedia.jetpackIcon			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_jetpack_icon");

	
	cgs.jkmodMedia.jetpackActive		= trap_FX_RegisterEffect("jkmod_jetpack/idle");
	cgs.jkmodMedia.jetpackIdle			= trap_FX_RegisterEffect("jkmod_jetpack/ground");

	cgs.jkmodMedia.jetpackActiveSound	= trap_S_RegisterSound("sound/effects/fire_lp.wav");
	cgs.jkmodMedia.jetpackIdleSound		= trap_S_RegisterSound("sound/effects/torch_burning_lp.wav");

	cgs.jkmodMedia.keyCrouchOff			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/crouch_off");
	cgs.jkmodMedia.keyCrouchOn			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/crouch_on");
	cgs.jkmodMedia.keyJumpOff			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/jump_off");
	cgs.jkmodMedia.keyJumpOn			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/jump_on");
	cgs.jkmodMedia.keyBackOff			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/back_off");
	cgs.jkmodMedia.keyBackOn			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/back_on");
	cgs.jkmodMedia.keyForwardOff		= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/forward_off");
	cgs.jkmodMedia.keyForwardOn			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/forward_on");
	cgs.jkmodMedia.keyLeftOff			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/left_off");
	cgs.jkmodMedia.keyLeftOn			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/left_on");
	cgs.jkmodMedia.keyRightOff			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/right_off");
	cgs.jkmodMedia.keyRightOn			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/right_on");
	cgs.jkmodMedia.keyRightOn			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/right_on");
	cgs.jkmodMedia.keyAttackOn			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/attack_on");
	cgs.jkmodMedia.keyAttackOff			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/attack_off");
	cgs.jkmodMedia.keyAltOn				= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/alt_on");
	cgs.jkmodMedia.keyAltOff			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_keys/alt_off");

	
	Com_Printf( S_COLOR_CYAN "------------ JK+ Client Complete ------------\n" );
}
