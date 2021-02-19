/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main client module, cvar definitions, etc...
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header

/*
=====================================================================
Cvar table list
=====================================================================
*/

static qboolean jkmod_duel_warning = qfalse;

typedef struct { // Cvar table struct
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
} cvarTable_t;

vmCvar_t	jkcvar_cg_privateDuel;
vmCvar_t	jkcvar_cg_duelGlow;
vmCvar_t	jkcvar_cg_drawClock;
vmCvar_t	jkcvar_cg_drawHitBox;
vmCvar_t	jkcvar_cg_drawForcePoints;
vmCvar_t	jkcvar_cg_drawInventory;
vmCvar_t	jkcvar_cg_drawBactaModel;
vmCvar_t	jkcvar_cg_scoreboardIcons;
vmCvar_t	jkcvar_cg_chatIcon;
vmCvar_t	jkcvar_cg_saberTrailSpeed;
vmCvar_t	jkcvar_cg_damageBlend;
vmCvar_t	jkcvar_cg_flagOpacity;
vmCvar_t	jkcvar_cg_flagAlignment;
vmCvar_t	jkcvar_cg_chatPlayerOpacity;

vmCvar_t	jkcvar_cg_chatBox;
vmCvar_t	jkcvar_cg_chatBoxTime;
vmCvar_t	jkcvar_cg_chatBoxHeight;
vmCvar_t	jkcvar_cg_chatBoxHistory;

vmCvar_t	jkcvar_cg_customHats;
vmCvar_t	jkcvar_cg_customAnims;
vmCvar_t	jkcvar_cg_jetPackIdle;

vmCvar_t	jkcvar_cg_test1;
vmCvar_t	jkcvar_cg_test2;

static cvarTable_t	JKModCGCvarTable[] = {

	{ &jkcvar_cg_privateDuel,		"jk_cg_privateDuel",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_duelGlow,			"jk_cg_duelGlow",			"1",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawClock,			"jk_cg_drawClock",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawHitBox,		"jk_cg_drawHitBox",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawForcePoints,	"jk_cg_drawForcePoints",	"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawInventory,		"jk_cg_drawInventory",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_drawBactaModel,	"jk_cg_drawBactaModel",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_scoreboardIcons,	"jk_cg_scoreboardIcons",	"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatIcon,			"jk_cg_chatIcon",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_saberTrailSpeed,	"jk_cg_saberTrailSpeed",	"40",	CVAR_ARCHIVE },
	{ &jkcvar_cg_damageBlend,		"jk_cg_damageBlend",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_flagOpacity,		"jk_cg_flagOpacity",		"255",	CVAR_ARCHIVE },
	{ &jkcvar_cg_flagAlignment,		"jk_cg_flagAlignment",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatPlayerOpacity,	"jk_cg_chatPlayerOpacity",	"1",	CVAR_ARCHIVE },

	{ &jkcvar_cg_chatBox,			"jk_cg_chatBox",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatBoxTime,		"jk_cg_chatBoxTime",		"10",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatBoxHeight,		"jk_cg_chatBoxHeight",		"360",	CVAR_ARCHIVE },
	{ &jkcvar_cg_chatBoxHistory,	"jk_cg_chatBoxHistory",		"1",	CVAR_ARCHIVE },

	{ &jkcvar_cg_customHats,		"jk_cg_customHats",			"0",	CVAR_ARCHIVE | CVAR_USERINFO },
	{ &jkcvar_cg_customAnims,		"jk_cg_customAnims",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_jetPackIdle,		"jk_cg_jetPackIdle",		"0",	CVAR_ARCHIVE },

	{ &jkcvar_cg_test1,				"jk_cg_test1",				"0",	CVAR_ARCHIVE },
	{ &jkcvar_cg_test2,				"jk_cg_test2",				"0",	CVAR_ARCHIVE },

};

static int JKModCGCvarTableSize = sizeof(JKModCGCvarTable) / sizeof(JKModCGCvarTable[0]);

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/
void JKMod_CG_RegisterCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Register all the cvars
	for (i = 0, cv = JKModCGCvarTable; i < JKModCGCvarTableSize; i++, cv++) {
		trap_Cvar_Register(cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags);
	}

	// Launch original register cvars function
	BaseJK2_CG_RegisterCvars();

	// Set the client plugin version
	trap_Cvar_Register(NULL, "jkmod_clientversion", "", CVAR_USERINFO | CVAR_ROM);
	trap_Cvar_Set("jkmod_clientversion", JK_VERSION);
}

void JKMod_CG_UpdateCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Update all the cvars
	for (i = 0, cv = JKModCGCvarTable; i < JKModCGCvarTableSize; i++, cv++) {
		trap_Cvar_Update(cv->vmCvar);
	}

	// Launch original update cvars function
	BaseJK2_CG_UpdateCvars();
}

void JKMod_CG_RegisterMedia(void) 
{
	cgs.jkmodMedia.bactaModel			= trap_R_RegisterModel("models/items/bacta.md3");
	cgs.jkmodMedia.jetpackModel			= trap_R_RegisterModel("models/items/jkmod_jetpack.md3");
	cgs.jkmodMedia.hatSanta				= trap_R_RegisterModel("models/items/jkmod_hats/santa.md3");
	cgs.jkmodMedia.hatPumpkin			= trap_R_RegisterModel("models/items/jkmod_hats/pumpkin.md3");
	cgs.jkmodMedia.hatCap				= trap_R_RegisterModel("models/items/jkmod_hats/cap.md3");
	cgs.jkmodMedia.hatCowboy			= trap_R_RegisterModel("models/items/jkmod_hats/cowboy.md3");
	cgs.jkmodMedia.hatCringe			= trap_R_RegisterModel("models/items/jkmod_hats/cringe.md3");
	cgs.jkmodMedia.hatSombrero			= trap_R_RegisterModel("models/items/jkmod_hats/sombrero.md3");
	cgs.jkmodMedia.hatGentleman			= trap_R_RegisterModel("models/items/jkmod_hats/gentleman.md3");

	cgs.jkmodMedia.hitBox				= trap_R_RegisterShader("jkmod_hitbox");
	cgs.jkmodMedia.hitBoxNoCull			= trap_R_RegisterShader("jkmod_hitbox_nocull");
	cgs.jkmodMedia.clockBg				= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_clock_bg");
	cgs.jkmodMedia.jetpackIcon			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_jetpack_icon");

	
	cgs.jkmodMedia.jetpackActive		= trap_FX_RegisterEffect("jkmod_jetpack/idle");
	cgs.jkmodMedia.jetpackIdle			= trap_FX_RegisterEffect("jkmod_jetpack/ground");

	cgs.jkmodMedia.jetpackActiveSound	= trap_S_RegisterSound("sound/effects/fire_lp.wav");
	cgs.jkmodMedia.jetpackIdleSound		= trap_S_RegisterSound("sound/effects/torch_burning_lp.wav");

	
	Com_Printf( S_COLOR_CYAN "----------- JKPlus Media Complete -----------\n" );
}
