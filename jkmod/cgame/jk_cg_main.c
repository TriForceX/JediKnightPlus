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

vmCvar_t	jkcvar_cg_privateDuel;
vmCvar_t	jkcvar_cg_autoDuel;
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
vmCvar_t	jkcvar_cg_chatAutoStatus;
vmCvar_t	jkcvar_cg_specialMoveCamera;
vmCvar_t	jkcvar_cg_identifyObjects;

vmCvar_t	jkcvar_cg_chatBox;
vmCvar_t	jkcvar_cg_chatBoxTime;
vmCvar_t	jkcvar_cg_chatBoxHeight;
vmCvar_t	jkcvar_cg_chatBoxHistory;

vmCvar_t	jkcvar_cg_speedMeter;
vmCvar_t	jkcvar_cg_strafeHelper;
vmCvar_t	jkcvar_cg_sHelperCutoff;
vmCvar_t	jkcvar_cg_sHelperPrecision;
vmCvar_t	jkcvar_cg_sHelperLineWidth;
vmCvar_t	jkcvar_cg_sHelperActiveColor;
vmCvar_t	jkcvar_cg_sHelperInactiveAlpha;
vmCvar_t	jkcvar_cg_sHelperOffset;
vmCvar_t	jkcvar_cg_sHelperFPS;

vmCvar_t	jkcvar_cg_customHats;
vmCvar_t	jkcvar_cg_customAnims;
vmCvar_t	jkcvar_cg_customEffects;
vmCvar_t	jkcvar_cg_jetPackIdle;

vmCvar_t	jkcvar_cg_test1;
vmCvar_t	jkcvar_cg_test2;

static jkmod_cg_cvar_table_t JKModCGcvarTable[] = 
{
	{ &jkcvar_cg_privateDuel,			"jk_cg_privateDuel",			"0",			NULL,						CVAR_ARCHIVE | CVAR_USERINFO, 0 },
	{ &jkcvar_cg_autoDuel,				"jk_cg_autoDuel",				"0",			NULL,						CVAR_ARCHIVE | CVAR_USERINFO, 0 },
	{ &jkcvar_cg_duelGlow,				"jk_cg_duelGlow",				"1",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_duelEnd,				"jk_cg_duelEnd",				"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_duelEndOrbit,			"jk_cg_duelEndOrbit",			"3",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_duelEndDelay,			"jk_cg_duelEndDelay",			"1",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_drawClock,				"jk_cg_drawClock",				"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_drawHitBox,			"jk_cg_drawHitBox",				"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_drawForcePoints,		"jk_cg_drawForcePoints",		"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_drawInventory,			"jk_cg_drawInventory",			"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_drawRaceTimer,			"jk_cg_drawRaceTimer",			"1",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_drawBactaModel,		"jk_cg_drawBactaModel",			"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_drawPlayerNames,		"jk_cg_drawPlayerNames",		"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_drawMovementKeys,		"jk_cg_drawMovementKeys",		"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_scoreboardIcons,		"jk_cg_scoreboardIcons",		"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_scoreboardExtras,		"jk_cg_scoreboardExtras",		"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_chatIcon,				"jk_cg_chatIcon",				"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_saberTrailSpeed,		"jk_cg_saberTrailSpeed",		"40",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_damageBlend,			"jk_cg_damageBlend",			"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_flagOpacity,			"jk_cg_flagOpacity",			"255",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_flagAlignment,			"jk_cg_flagAlignment",			"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_chatPlayerOpacity,		"jk_cg_chatPlayerOpacity",		"1",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_chatAutoStatus,		"jk_cg_chatAutoStatus",			"0",			NULL,						CVAR_ARCHIVE | CVAR_USERINFO, 0 },
	{ &jkcvar_cg_specialMoveCamera,		"jk_cg_specialMoveCamera",		"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_identifyObjects,		"jk_cg_identifyObjects",		"1",			NULL,						CVAR_ARCHIVE, 0 },

	{ &jkcvar_cg_chatBox,				"jk_cg_chatBox",				"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_chatBoxTime,			"jk_cg_chatBoxTime",			"10",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_chatBoxHeight,			"jk_cg_chatBoxHeight",			"360",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_chatBoxHistory,		"jk_cg_chatBoxHistory",			"1",			NULL,						CVAR_ARCHIVE, 0 },
	
	{ &jkcvar_cg_speedMeter,			"jk_cg_speedMeter",				"256",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_strafeHelper,			"jk_cg_strafeHelper",			"992",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_sHelperCutoff,			"jk_cg_sHelperCutoff",			"240",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_sHelperPrecision,		"jk_cg_sHelperPrecision",		"256",			NULL,						CVAR_NONE, 0 },
	{ &jkcvar_cg_sHelperLineWidth,		"jk_cg_sHelperLineWidth",		"1",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_sHelperActiveColor,	"jk_cg_sHelperActiveColor",		"0 255 0 200",	JKMod_CG_CVU_sHelperColor,	CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_sHelperInactiveAlpha,	"jk_cg_sHelperInactiveAlpha",	"200",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_sHelperOffset,			"jk_cg_sHelperOffset",			"75",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_sHelperFPS,			"jk_cg_sHelperFPS",				"0",			NULL,						CVAR_NONE, 0 },

	{ &jkcvar_cg_customHats,			"jk_cg_customHats",				"0",			NULL,						CVAR_ARCHIVE | CVAR_USERINFO, 0 },
	{ &jkcvar_cg_customAnims,			"jk_cg_customAnims",			"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_customEffects,			"jk_cg_customEffects",			"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_jetPackIdle,			"jk_cg_jetPackIdle",			"0",			NULL,						CVAR_ARCHIVE, 0 },

	{ &jkcvar_cg_test1,					"jk_cg_test1",					"0",			NULL,						CVAR_ARCHIVE, 0 },
	{ &jkcvar_cg_test2,					"jk_cg_test2",					"0",			NULL,						CVAR_ARCHIVE, 0 },

};

static int JKModCGcvarTableSize = ARRAY_LEN(JKModCGcvarTable);

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/

// Register cvars
void JKMod_CG_RegisterCvars(void)
{
	int						i;
	jkmod_cg_cvar_table_t	*cv;
	char					var[MAX_TOKEN_CHARS];

	// Register all the cvars
	for (i = 0, cv = JKModCGcvarTable; i < JKModCGcvarTableSize; i++, cv++) 
	{
		trap_Cvar_Register(cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags);
	}

	// Launch original register cvars function
	BaseJK2_CG_RegisterCvars();

	// Set the client plugin version
	trap_Cvar_Register(NULL, "jkmod_client", "", CVAR_USERINFO | CVAR_ROM);
	trap_Cvar_Set("jkmod_client", JK_VERSION);

	// Check console notify time
	cg.jkmodCG.consoleNotifyTime = CG_Cvar_Get("con_notifytime");
}

// Update cvars
void JKMod_CG_UpdateCvars(void)
{
	int						i;
	jkmod_cg_cvar_table_t	*cv;

	// Update all the cvars
	for (i = 0, cv = JKModCGcvarTable; i < JKModCGcvarTableSize; i++, cv++) 
	{
		trap_Cvar_Update(cv->vmCvar);

		if(cv->modificationCount != cv->vmCvar->modificationCount)
		{
			cv->modificationCount = cv->vmCvar->modificationCount;

			if (cv->update) cv->update();
		}
	}

	// Launch original update cvars function
	BaseJK2_CG_UpdateCvars();

	// Check console notify time
	cg.jkmodCG.consoleNotifyTime = CG_Cvar_Get("con_notifytime");
}

// Update strafe helper active color
void JKMod_CG_CVU_sHelperColor(void)
{
	int i;

	if (sscanf(jkcvar_cg_sHelperActiveColor.string, "%f %f %f %f", &cg.jkmodCG.strafeHelperActiveColor[0], &cg.jkmodCG.strafeHelperActiveColor[1], &cg.jkmodCG.strafeHelperActiveColor[2], &cg.jkmodCG.strafeHelperActiveColor[3]) != 4)
	{
		cg.jkmodCG.strafeHelperActiveColor[0] = 0;
		cg.jkmodCG.strafeHelperActiveColor[1] = 255;
		cg.jkmodCG.strafeHelperActiveColor[2] = 0;
		cg.jkmodCG.strafeHelperActiveColor[3] = 200;
	}

	for (i = 0; i < 4; i++) 
	{
		if (cg.jkmodCG.strafeHelperActiveColor[i] < 0) cg.jkmodCG.strafeHelperActiveColor[i] = 0;
		else if (cg.jkmodCG.strafeHelperActiveColor[i] > 255) cg.jkmodCG.strafeHelperActiveColor[i] = 255;
	}

	cg.jkmodCG.strafeHelperActiveColor[0] /= 255.0f;
	cg.jkmodCG.strafeHelperActiveColor[1] /= 255.0f;
	cg.jkmodCG.strafeHelperActiveColor[2] /= 255.0f;
	cg.jkmodCG.strafeHelperActiveColor[3] /= 255.0f;
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
String token with long delimiter
=====================================================================
*/
char *JKMod_CG_StrTok(char *str, const char *delim)
{
    static char *tok;
    static char *next;
    char *m;

    if (delim == NULL) return NULL;
    tok = (str) ? str : next;
    if (tok == NULL) return NULL;

    m = strstr(tok, delim);

    if (m) {
        next = m + strlen(delim);
        *m = '\0';
    } else {
        next = NULL;
    }
    return tok;
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
	cgs.jkmodMedia.forceSwirl			= trap_R_RegisterShader("gfx/hud/force_swirl");
	cgs.jkmodMedia.useableHint			= trap_R_RegisterShader("gfx/hud/jkmod_useablehint");
	cgs.jkmodMedia.clockBg				= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_clock_bg");
	cgs.jkmodMedia.jetpackIcon			= trap_R_RegisterShaderNoMip("gfx/hud/jkmod_jetpack_icon");

	
	cgs.jkmodMedia.jetpackActive		= trap_FX_RegisterEffect("jkmod_jetpack/idle");
	cgs.jkmodMedia.jetpackIdle			= trap_FX_RegisterEffect("jkmod_jetpack/ground");

	cgs.jkmodMedia.strafeHelperSound	= trap_S_RegisterSound("sound/interface/update.mp3");
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
