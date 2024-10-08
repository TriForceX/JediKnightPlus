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
	{ "jk_cg_drawInventory",		"0",		"1" },
	{ "jk_cg_drawRaceTimer",		"1",		"1" },
	{ "jk_cg_drawMovementKeys",		"0",		NULL },
	{ "jk_cg_enhancedInterface",	"0",		"1" },
	{ "jk_cg_chatBox",				"0",		"1" },
	{ "jk_cg_chatBoxTime",			"10",		"10" },
	{ "jk_cg_chatBoxHeight",		"360",		"360" },
	{ "jk_cg_chatBoxHistory",		"0",		"1" },
	{ "jk_cg_speedoMeter",			"320",		"320" },
	{ "jk_cg_strafeHelper",			"992",		"992" },
	{ "jk_cg_scoreboardOpacity",	"0",		"127" },
	{ "jk_ui_teleportMenu",			"0",		"1" },
	{ "cg_drawTeamOverlay",			"0",		"2" },
	{ "cg_drawTimer",				"0",		"2" },
	{ "cg_drawFPS",					"0",		"2" },
	{ "cg_lagometer",				"0",		NULL },
	{ "cg_hudFiles",				"0",		NULL },

	// visuals cvars				default		recommended
	{ "jk_cg_privateDuel",			"0",		"0" },
	{ "jk_cg_autoDuel",				"0",		"0" },
	{ "jk_cg_duelGlow",				"1",		"1" },
	{ "jk_cg_duelEndOrbit",			"3",		"3" },
	{ "jk_cg_duelEndDelay",			"1",		"1" },
	{ "jk_cg_drawHitBox",			"0",		NULL },
	{ "jk_cg_drawSaberBox",			"0",		NULL },
	{ "jk_cg_drawBactaModel",		"0",		"1" },
	{ "jk_cg_drawPlayerNames",		"0",		NULL },
	{ "jk_cg_chatIcon",				"0",		"1" },
	{ "jk_cg_saberTrailSpeed",		"40",		"40" },
	{ "jk_cg_damageBlend",			"0",		"1" },
	{ "jk_cg_flagOpacity",			"255",		"127" },
	{ "jk_cg_flagAlignment",		"0",		"1" },
	{ "jk_cg_chatPlayerOpacity",	"1",		"1" },
	{ "jk_cg_chatAutoStatus",		"0",		NULL },
	{ "jk_cg_chatColor",			"0",		NULL },
	{ "cg_dismember",				"0",		"2" },
	{ "cg_simpleItems",				"0",		NULL },
	{ "cg_shadows",					"1",		NULL },
	{ "cg_scorePlums",				"1",		"2" },

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
	{ "jk_cg_loadingIcons",			"0",		"1" },
	{ "jk_cg_jetPackIdle",			"0",		NULL },
	{ "jk_cg_identifyObjects",		"0",		"1" },
	{ "jk_ui_hideMotd",				"0",		"1" },
	{ "jk_ui_screenClear",			"0",		"8" },
	{ "r_consoleFont",				"1",		"0" },
};

int JKModUIresetClientSize = ARRAY_LEN(JKModUIresetClient);

/*
=====================================================================
Teleports table list
=====================================================================
*/
void JKMod_UI_BuildTeleportList(void)
{
	static char teleportsInfo[MAX_INFO_STRING];
	static char *token = "";

	// Clear
	uiInfo.jkmodInfo.teleportCount = 0;
	ARRAY_CLEAR(uiInfo.jkmodInfo.teleportChats);
	ARRAY_CLEAR(uiInfo.jkmodInfo.teleportTitle);

	// Get
	trap_GetConfigString(JK_CS_TELEPORTS, teleportsInfo, MAX_INFO_STRING);
	token = JKMod_UI_StrTok(teleportsInfo, ",");
    
	// Parse
	while (VALIDTEXT(token)) 
	{
		Q_strncpyz(uiInfo.jkmodInfo.teleportChats[uiInfo.jkmodInfo.teleportCount], token, MAX_INFO_STRING);
		Q_strncpyz(uiInfo.jkmodInfo.teleportTitle[uiInfo.jkmodInfo.teleportCount], va("%c%s", toupper(token[1]), token+2), MAX_TELEPORTLENGTH);
		if (!uiInfo.jkmodInfo.teleportCount) trap_Cvar_Set("jk_ui_teleportToggle", token);
		uiInfo.jkmodInfo.teleportCount++;
		token = JKMod_UI_StrTok(NULL, ",");
	}

	// Update cvars
	if (!uiInfo.jkmodInfo.teleportCount) trap_Cvar_Set("jk_ui_teleportToggle", "");
	trap_Cvar_Set("jk_ui_teleportCount", va("%i", uiInfo.jkmodInfo.teleportCount));

	// Done
	uiInfo.jkmodInfo.teleportCheck = qtrue;
}

/*
=====================================================================
Cvar table list
=====================================================================
*/

vmCvar_t	jkcvar_ui_motdString;
vmCvar_t	jkcvar_ui_currentTeam;
vmCvar_t	jkcvar_ui_votePoll;
vmCvar_t	jkcvar_ui_votePause;
vmCvar_t	jkcvar_ui_emoteToggle;
vmCvar_t	jkcvar_ui_dimensionToggle;
vmCvar_t	jkcvar_ui_teleportToggle;
vmCvar_t	jkcvar_ui_teleportCount;
vmCvar_t	jkcvar_ui_teleportMenu;
vmCvar_t	jkcvar_ui_clientPopUp;
vmCvar_t	jkcvar_ui_clientVersion;
vmCvar_t	jkcvar_ui_hideMotd;
vmCvar_t	jkcvar_ui_screenClear;
vmCvar_t	jkcvar_ui_highestVideo;

vmCvar_t	jkcvar_ui_test1;
vmCvar_t	jkcvar_ui_test2;

static jkmod_ui_cvar_table_t JKModUIcvarTable[] = {
	
	{ &jkcvar_ui_motdString,		"jk_ui_motdString",			"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_currentTeam,		"jk_ui_currentTeam",		"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_votePoll,			"jk_ui_votePoll",			"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_votePause,			"jk_ui_votePause",			"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_emoteToggle,		"jk_ui_emoteToggle",		"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_dimensionToggle,	"jk_ui_dimensionToggle",	"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_teleportToggle,	"jk_ui_teleportToggle",		"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_teleportCount,		"jk_ui_teleportCount",		"0",	CVAR_ARCHIVE | CVAR_ROM },
	{ &jkcvar_ui_teleportMenu,		"jk_ui_teleportMenu",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_ui_clientPopUp,		"jk_ui_clientPopUp",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_ui_clientVersion,		"jk_ui_clientVersion",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_ui_hideMotd,			"jk_ui_hideMotd",			"0",	CVAR_ARCHIVE },
	{ &jkcvar_ui_screenClear,		"jk_ui_screenClear",		"0",	CVAR_ARCHIVE },
	{ &jkcvar_ui_highestVideo,		"jk_ui_highestVideo",		"0",	CVAR_ARCHIVE },

	{ &jkcvar_ui_test1,				"jk_ui_test1",				"0",	CVAR_ARCHIVE },
	{ &jkcvar_ui_test2,				"jk_ui_test2",				"0",	CVAR_ARCHIVE },

};

static int JKModUIcvarTableSize = ARRAY_LEN(JKModUIcvarTable);
static int JKModUIcvar_r_clear = 0;
static char JKModUIcvar_cl_motdString[MAX_CVAR_VALUE_STRING];
static float JKModUIcvarClientVersion = 0;

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/

// Register cvars
void JKMod_UI_RegisterCvars(void)
{
	int i;
	jkmod_ui_cvar_table_t *cv;
	uiClientState_t	cstate;
	trap_GetClientState(&cstate);

	// Register all the cvars
	for (i = 0, cv = JKModUIcvarTable; i < JKModUIcvarTableSize; i++, cv++) {
		trap_Cvar_Register(cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags);
	}

	// Set default cvars values
	trap_Cvar_Set("jk_ui_votePoll", "");
	trap_Cvar_Set("jk_ui_votePause", "0");
	trap_Cvar_Set("jk_ui_emoteToggle", JKModUIemotesTable[0].cmd);
	trap_Cvar_Set("jk_ui_dimensionToggle", JKModUIdimensionsTable[0].cmd);
	trap_Cvar_Set("jk_ui_motdString", Info_ValueForKey(cstate.updateInfoString, "motd"));

	// Build teleport list
	if (jkcvar_ui_teleportMenu.integer || jkcvar_ui_currentTeam.integer != TEAM_SPECTATOR) JKMod_UI_BuildTeleportList();

	// Check motd
	trap_Cvar_VariableStringBuffer("cl_motdString", JKModUIcvar_cl_motdString, sizeof(JKModUIcvar_cl_motdString));

	// Check clear
	JKModUIcvar_r_clear = trap_Cvar_VariableValue("r_clear");
	
	// Check version
	JKModUIcvarClientVersion = VERSION_FLOAT(JK_MAJOR, JK_MINOR, JK_PATCH);

	// Launch original register cvars function
	BaseJK2_UI_RegisterCvars();
}

// Update cvars
void JKMod_UI_UpdateCvars(void)
{
	int i;
	jkmod_ui_cvar_table_t *cv;
	qboolean versionMismatch = fabs((double)jkcvar_ui_clientVersion.value - (double)JKModUIcvarClientVersion) > 0.000001; // Thanks Q3...
	
	// Update all the cvars
	for (i = 0, cv = JKModUIcvarTable; i < JKModUIcvarTableSize; i++, cv++) {
		trap_Cvar_Update(cv->vmCvar);
	}

	// Check client options pop-up
	if (jkcvar_ui_clientPopUp.integer != JKModUIresetClientSize || versionMismatch)
	{
		if (Menus_FindByName("ingame")->window.flags & WINDOW_VISIBLE)
		{
			if (versionMismatch) {
				Menus_ActivateByName("ingame_jkmod_changelog");
				trap_Cvar_Set("jk_ui_clientVersion", va("%f", JKModUIcvarClientVersion));
				trap_Cvar_Set("jk_ui_clientPopUp", va("%i", JKModUIresetClientSize));
			} else {
				Menus_ActivateByName("ingame_jkmod_popup");
				trap_Cvar_Set("jk_ui_clientPopUp", va("%i", JKModUIresetClientSize));
			}
		}
	}

	// Check teleports
	if (!jkcvar_ui_teleportMenu.integer || jkcvar_ui_currentTeam.integer == TEAM_SPECTATOR)
	{
		if (jkcvar_ui_teleportCount.integer && VALIDSTRING(jkcvar_ui_teleportToggle.string))
		{
			trap_Cvar_Set("jk_ui_teleportCount", "0");
			trap_Cvar_Set("jk_ui_teleportToggle", "");
			uiInfo.jkmodInfo.teleportCheck = qfalse;
		}
	}
	else if (!uiInfo.jkmodInfo.teleportCheck)
	{
		JKMod_UI_BuildTeleportList();
	}

	// Clear motd
	if (jkcvar_ui_hideMotd.integer && VALIDSTRING(JKModUIcvar_cl_motdString)) {
		*JKModUIcvar_cl_motdString = '\0';
		trap_Cvar_Set("cl_motdString", "");
	}
	// Restore motd
	else if (!jkcvar_ui_hideMotd.integer && !VALIDSTRING(JKModUIcvar_cl_motdString) && VALIDCVAR(jkcvar_ui_motdString.string)) {
		Q_strncpyz(JKModUIcvar_cl_motdString, jkcvar_ui_motdString.string, sizeof(JKModUIcvar_cl_motdString));
		trap_Cvar_Set("cl_motdString", jkcvar_ui_motdString.string);
	}

	// Check clear
	if (jkcvar_ui_screenClear.integer != JKModUIcvar_r_clear) {
		JKModUIcvar_r_clear = jkcvar_ui_screenClear.integer;
		trap_Cvar_Set("r_clear", va("%i", jkcvar_ui_screenClear.integer));
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
Teleports list functions
=====================================================================
*/
static char *JKMod_UI_TeleportsList(int index, int *actual)
{
	int i, c = 0;
	*actual = 0;

	for (i = 0; i < uiInfo.jkmodInfo.teleportCount; i++)
	{
		if (c == index) {
			*actual = i;
			return va("%s", uiInfo.jkmodInfo.teleportTitle[i]);
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
	else if (feederID == FEEDER_JK_TELEPORTS)
	{
		int actual;
		return JKMod_UI_TeleportsList(index, &actual);
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
		case FEEDER_JK_TELEPORTS: 
			return uiInfo.jkmodInfo.teleportCount;
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
	else if (feederID == FEEDER_JK_TELEPORTS)
	{
		trap_Cvar_Set("jk_ui_teleportToggle", va("%s", uiInfo.jkmodInfo.teleportChats[index]));
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
	if (!Q_stricmp(name, "jk_ui_votePoll"))
	{
		char *text = UI_Cvar_VariableString("jk_ui_votePoll");
		if (VALIDSTRING(text)) {
			trap_Cmd_ExecuteText(EXEC_APPEND, va("callvote poll \"%s\"\n", text));
		} else {
			Com_Printf("Invalid vote poll message\n");
		}
	}
	// Parse vote pause time
	else if (!Q_stricmp(name, "jk_ui_votePause"))
	{
		char *number = UI_Cvar_VariableString("jk_ui_votePause");
		if (JKMod_UI_ValidNumber(number) && atoi(number) > 0) {
			trap_Cmd_ExecuteText(EXEC_APPEND, va("callvote pause \"%i\"\n", (atoi(number) * 60)));
		} else {
			Com_Printf("The specified value is not a valid number\n");
		}
	}
	// Reset client menu options
	else if (!Q_stricmp(name, "jk_ui_clientPopUp"))
	{
		int i;
		trap_Cvar_Set("jk_ui_clientPopUp", va("%i", (val ? JKModUIresetClientSize : 0)));

		for (i = 0; i < JKModUIresetClientSize; i++)
		{
			if (val && JKModUIresetClient[i].recVal == NULL) continue;
			trap_Cvar_Set(JKModUIresetClient[i].cvar, (val ? JKModUIresetClient[i].recVal : JKModUIresetClient[i].defVal));
		}
	}
	// Highest settings check
	else if (!Q_stricmp(name, "jk_ui_highestVideo"))
	{
		char *file = "configs/jkmod_highest_video.cfg";
		trap_Cmd_ExecuteText(EXEC_APPEND, va("exec \"%s\"\n", file));
		trap_Cmd_ExecuteText(EXEC_APPEND, va("vstr \"%s\"\n", (val ? "jkmod_video_highest" : "jkmod_video_default")));
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
	// Trigger teleport chats
	if (Q_stricmp(name, "JKMod_teleportToggle") == 0)
	{
		trap_Cmd_ExecuteText(EXEC_APPEND, va("say \"%s\"\n", UI_Cvar_VariableString("jk_ui_teleportToggle")));
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
	// Toggle console from menu
	if (Q_stricmp(name, "JKMod_consoleToggle") == 0)
	{
		trap_Cmd_ExecuteText(EXEC_APPEND, "toggleConsole\n");
		trap_Cmd_ExecuteText(EXEC_APPEND, "echo \"" S_COLOR_GREEN "Press ESC to close the console\"\n");
		return qtrue;
	}

	return qfalse;
}
