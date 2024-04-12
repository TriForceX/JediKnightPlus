/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Main user interface module, cvar definitions, etc...
=====================================================================
*/

#include "../../code/ui/ui_local.h"	// Original header

/*
=====================================================================
Emotes table list
=====================================================================
*/
jkmod_ui_command_table_t JKModUIemotesTable[] =
{
	// cmd				title
	{ "bar",			"Bartender" },
	{ "beg",			"Beg" },
	{ "buried",			"Buried in Ground" },
	{ "cocky",			"Cocky" },
	{ "comeon",			"Come on!" },
	{ "comtalk",		"Communicator Talk" },
	{ "convulsion",		"Convulsion" },
	{ "crossarms",		"Cross Arms" },
	{ "dontkillme",		"Don't Kill Me!" },
	{ "dontknow",		"Don't Know" },
	{ "dontknow2",		"Don't Know 2" },
	{ "explain",		"Explain" },
	{ "explain2",		"Explain 2" },
	{ "fakedead",		"Fake Dead" },
	{ "fakedead2",		"Fake Dead 2" },
	{ "falling",		"Falling" },
	{ "flip",			"Flip Saber" },
	{ "getup",			"Get Up" },
	{ "greet",			"Greet!" },
	{ "handhips",		"Hand Hips" },
	{ "hug",			"Hug a friend" },
	{ "kiss",			"Kiss Someone" },
	{ "kneel",			"Kneel" },
	{ "laugh",			"Laugh!" },
	{ "look",			"Look Arround" },
	{ "look2",			"Look Arround 2" },
	{ "nod",			"Nod" },
	{ "point",			"Point" },
	{ "point2",			"Point 2" },
	{ "posing",			"Posing" },
	{ "punch",			"Punch Him!" },
	{ "reverence",		"Reverence" },
	{ "sit",			"Sit Down" },
	{ "sit2",			"Sit Down 2" },
	{ "sit3",			"Sit Down 3" },
	{ "shake",			"Head Shake" },
	{ "super",			"I'm a Hero" },
	{ "super2",			"I'm a Hero 2" },
	{ "surrender",		"Surrender" },
	{ "spin",			"Spin" },
	{ "spin2",			"Spin 2" },
	{ "spin3",			"Spin 3" },
	{ "spin4",			"Spin 4" },
	{ "taunt3",			"Taunt 3" },
	{ "taunt4",			"Taunt 4" },
	{ "think",			"Thinking" },
	{ "threaten",		"Threaten" },
	{ "thumbsup",		"Thumbs Up" },
	{ "thumbsdown",		"Thumbs Down" },
	{ "tossback",		"Toss Back" },
	{ "tossover",		"Toss Over" },
	{ "tossup",			"Toss Up" },
	{ "type",			"Typing" },
	{ "type2",			"Typing 2" },
	{ "victory",		"Victory" },
	{ "victory2",		"Victory 2" },
	{ "waiting",		"Waiting" },
	{ "watchout",		"Watch Out!" },
	{ "writing",		"Writing" },
	{ "writing2",		"Writing 2" },
};

int JKModUIemotesTableSize = ARRAY_LEN(JKModUIemotesTable);

/*
=====================================================================
Dimensions table list
=====================================================================
*/
jkmod_ui_command_table_t JKModUIdimensionsTable[] =
{
	// cmd				title
	{ "normal",			"Normal" },
	{ "guns",			"Guns Arena" },
	{ "race",			"Race Defrag" },
	{ "saber",			"Saber Only" },
	{ "force",			"Full Force" },
	{ "insta",			"Insta Kill" },
	{ "cheats",			"Cheats Mode" },
	{ "private",		"Private Room" },
};

int JKModUIdimensionsTableSize = ARRAY_LEN(JKModUIdimensionsTable);

/*
=====================================================================
// Reset client pop-up list
=====================================================================
*/
jkmod_ui_reset_client_t JKModUIresetClient[] =
{
	// interface cvars				default		recommended
	{ "jk_cg_drawClock",			"0",		"2" },
	{ "jk_cg_drawForcePoints",		"0",		"1" },
	{ "jk_cg_drawInventory",		"0",		"1" },
	{ "jk_cg_drawRaceTimer",		"1",		"1" },
	{ "jk_cg_drawMovementKeys",		"0",		NULL },
	{ "jk_cg_scoreboardIcons",		"0",		"1" },
	{ "jk_cg_scoreboardExtras",		"0",		"1" },
	{ "jk_cg_chatBox",				"0",		"1" },
	{ "jk_cg_chatBoxTime",			"10",		"10" },
	{ "jk_cg_chatBoxHeight",		"360",		"360" },
	{ "jk_cg_chatBoxHistory",		"1",		"2" },
	{ "jk_cg_speedMeter",			"256",		"256" },
	{ "jk_cg_strafeHelper",			"992",		"992" },
	{ "cg_drawTimer",				"0",		"2" },
	{ "cg_drawFPS",					"0",		"2" },
	{ "cg_lagometer",				"0",		NULL },
	{ "cg_hudFiles",				"0",		"0" },

	// visuals cvars				default		recommended
	{ "jk_cg_privateDuel",			"0",		"0" },
	{ "jk_cg_autoDuel",				"0",		"0" },
	{ "jk_cg_duelGlow",				"1",		"1" },
	{ "jk_cg_duelEndOrbit",			"3",		"3" },
	{ "jk_cg_duelEndDelay",			"1",		"1" },
	{ "jk_cg_drawHitBox",			"0",		NULL },
	{ "jk_cg_drawBactaModel",		"0",		"1" },
	{ "jk_cg_drawPlayerNames",		"0",		NULL },
	{ "jk_cg_chatIcon",				"0",		"1" },
	{ "jk_cg_saberTrailSpeed",		"40",		"40" },
	{ "jk_cg_damageBlend",			"0",		"1" },
	{ "jk_cg_flagOpacity",			"255",		"127" },
	{ "jk_cg_flagAlignment",		"0",		"1" },
	{ "jk_cg_chatPlayerOpacity",	"1",		"1" },
	{ "jk_cg_chatAutoStatus",		"0",		"0" },
	{ "cg_dismember",				"0",		"2" },
	{ "cg_simpleItems",				"0",		NULL },
	{ "cg_shadows",					"1",		NULL },
	{ "cg_scorePlums",				"1",		"2" },
	{ "r_clear",					"0",		"8" },

	// camera cvars					default		recommended
	{ "jk_cg_duelEnd",				"0",		"1" },
	{ "jk_cg_specialMoveCamera",	"0",		"1" },
	{ "cg_fpls",					"0",		"0" },
	{ "cg_fovAspectAdjust",			"0",		"1" },
	{ "cg_fov",						"80",		NULL },
	{ "cg_thirdPersonRange",		"80",		NULL },

	// others cvars					default		recommended
	{ "jk_cg_customHats",			"0",		NULL },
	{ "jk_cg_customAnims",			"0",		"1" },
	{ "jk_cg_customEffects",		"0",		"1" },
	{ "jk_cg_jetPackIdle",			"0",		NULL },
	{ "jk_cg_identifyObjects",		"0",		"1" },
};

int JKModUIresetClientSize = ARRAY_LEN(JKModUIresetClient);

/*
=====================================================================
Cvar table list
=====================================================================
*/

vmCvar_t	jkcvar_ui_votePoll;
vmCvar_t	jkcvar_ui_emoteToggle;
vmCvar_t	jkcvar_ui_dimensionToggle;
vmCvar_t	jkcvar_ui_clientPopUp;

vmCvar_t	jkcvar_ui_test1;
vmCvar_t	jkcvar_ui_test2;

static jkmod_ui_cvar_table_t JKModUIcvarTable[] = {

	{ &jkcvar_ui_votePoll,			"jk_ui_votePoll",			"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_emoteToggle,		"jk_ui_emoteToggle",		"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_dimensionToggle,	"jk_ui_dimensionToggle",	"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_clientPopUp,		"jk_ui_clientPopUp",		"0",	CVAR_ARCHIVE },

	{ &jkcvar_ui_test1,				"jk_ui_test1",				"0",	CVAR_ARCHIVE },
	{ &jkcvar_ui_test2,				"jk_ui_test2",				"0",	CVAR_ARCHIVE },

};

static int JKModUIcvarTableSize = ARRAY_LEN(JKModUIcvarTable);

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/

// Register cvars
void JKMod_UI_RegisterCvars(void)
{
	int					i;
	jkmod_ui_cvar_table_t	*cv;

	// Register all the cvars
	for (i = 0, cv = JKModUIcvarTable; i < JKModUIcvarTableSize; i++, cv++) {
		trap_Cvar_Register(cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags);
	}

	// Set default cvars values
	trap_Cvar_Set("jk_ui_votePoll", "");
	trap_Cvar_Set("jk_ui_emoteToggle", va("%s", JKModUIemotesTable[0].cmd));
	trap_Cvar_Set("jk_ui_dimensionToggle", va("%s", JKModUIdimensionsTable[0].cmd));

	// Launch original register cvars function
	BaseJK2_UI_RegisterCvars();
}

// Update cvars
void JKMod_UI_UpdateCvars(void)
{
	int						i;
	jkmod_ui_cvar_table_t	*cv;

	// Update all the cvars
	for (i = 0, cv = JKModUIcvarTable; i < JKModUIcvarTableSize; i++, cv++) {
		trap_Cvar_Update(cv->vmCvar);
	}

	// Check client options pop-up
	if (jkcvar_ui_clientPopUp.integer != JKModUIresetClientSize) 
	{
		if (Menus_FindByName("ingame")->window.flags & WINDOW_VISIBLE)
		{
			Menus_ActivateByName("ingame_jkmod_popup");
			trap_Cvar_Set("jk_ui_clientPopUp", va("%i", JKModUIresetClientSize));
		}
	}
	
	// Launch original update cvars function
	BaseJK2_UI_UpdateCvars();
}

/*
=====================================================================
Emotes list functions
=====================================================================
*/
static char *JKMod_UI_EmotesList(int index, int *actual) 
{
	int i, c = 0;
	*actual = 0;

	for (i = 0; i < JKModUIemotesTableSize; i++)
	{
		if (c == index) {
			*actual = i;
			return JKModUIemotesTable[i].title;
		} else {
			c++;
		}
	}
	return "";
}

/*
=====================================================================
Dimensions list functions
=====================================================================
*/
static char *JKMod_UI_DimensionsList(int index, int *actual)
{
	int i, c = 0;
	*actual = 0;

	for (i = 0; i < JKModUIdimensionsTableSize; i++)
	{
		if (c == index) {
			*actual = i;
			return JKModUIdimensionsTable[i].title;
		} else {
			c++;
		}
	}
	return "";
}

/*
=====================================================================
Feeder item text function
=====================================================================
*/
const char *JKMod_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3, qhandle_t *handle4, qhandle_t *handle5, qhandle_t *handle6)
{
	static char info[MAX_STRING_CHARS];
	static char hostname[1024];
	static char clientBuff[32];
	static char needPass[32];
	static int lastColumn = -1;
	static int lastTime = 0;
	*handle1 = *handle2 = *handle3 = *handle4 = -1;

	if(feederID == FEEDER_JK_EMOTES)
	{
		int actual;
		return JKMod_UI_EmotesList(index, &actual);
	}
	else if (feederID == FEEDER_JK_DIMENSIONS)
	{
		int actual;
		return JKMod_UI_DimensionsList(index, &actual);
	}

	// Final return, probably NULL
	return BaseJK2_UI_FeederItemText(feederID, index, column, handle1, handle2, handle3, handle4, handle5, handle6);
}

/*
=====================================================================
Feeder count function
=====================================================================
*/
int JKMod_UI_FeederCount(float feederID)
{
	switch ((int)feederID)
	{
		case FEEDER_JK_EMOTES: 
			return JKModUIemotesTableSize;
		case FEEDER_JK_DIMENSIONS: 
			return JKModUIdimensionsTableSize;
	}

	// Launch original feeder count function
	return BaseJK2_UI_FeederCount(feederID);
}

/*
=====================================================================
Feeder selection function
=====================================================================
*/
qboolean JKMod_UI_FeederSelection(float feederID, int index)
{
	static char info[MAX_STRING_CHARS];

	if (feederID == FEEDER_JK_EMOTES)
	{
		trap_Cvar_Set("jk_ui_emoteToggle", va("%s", JKModUIemotesTable[index].cmd));
	}
	else if (feederID == FEEDER_JK_DIMENSIONS)
	{
		trap_Cvar_Set("jk_ui_dimensionToggle", va("%s", JKModUIdimensionsTable[index].cmd));
	}

	// Final return, probably NULL
	return BaseJK2_UI_FeederSelection(feederID, index);
}

/*
=====================================================================
Menu update function
=====================================================================
*/
void JKMod_UI_Update(const char *name, int val)
{
	// Parse vote poll message
	if (!Q_stricmp(name, "JKMod_votePoll"))
	{
		trap_Cmd_ExecuteText(EXEC_APPEND, va("callvote poll \"%s\"\n", UI_Cvar_VariableString("jk_ui_votePoll")));
	}
	// Reset client menu options
	else if (!Q_stricmp(name, "JKMod_resetClient"))
	{
		int i;
		trap_Cvar_Set("jk_ui_clientPopUp", va("%i", (val ? JKModUIresetClientSize : 0)));

		for (i = 0; i < JKModUIresetClientSize; i++)
		{
			if (val && JKModUIresetClient[i].recVal == NULL) continue;
			trap_Cvar_Set(JKModUIresetClient[i].cvar, (val ? JKModUIresetClient[i].recVal : JKModUIresetClient[i].defVal));
		}
	}
}

/*
=====================================================================
Run menu script function
=====================================================================
*/
qboolean JKMod_UI_RunMenuScript(const char **args, const char *name)
{
	// Rotate camera when using emotes menu
	if (Q_stricmp(name, "JKMod_emoteCamera") == 0)
	{
		trap_Cmd_ExecuteText(EXEC_APPEND, "emotecamera\n");
		trap_Cmd_ExecuteText(EXEC_APPEND, "centerview\n");
		return qtrue;
	}
	// Play and stop emotes from menu
	if (Q_stricmp(name, "JKMod_emoteToggle") == 0)
	{
		trap_Cmd_ExecuteText(EXEC_APPEND, va("emote \"%s\"\n", UI_Cvar_VariableString("jk_ui_emoteToggle")));
		return qtrue;
	}
	// Change dimension from menu
	if (Q_stricmp(name, "JKMod_dimensionToggle") == 0)
	{
		char *dimension = UI_Cvar_VariableString("jk_ui_dimensionToggle");

		if (!Q_stricmp(dimension, "private")) {
			trap_Cmd_ExecuteText(EXEC_APPEND, va("say !dimension \"%s\"\n", dimension));
		} else {
			trap_Cmd_ExecuteText(EXEC_APPEND, va("dimension \"%s\"\n", dimension));
		}
		return qtrue;
	}

	return qfalse;
}
