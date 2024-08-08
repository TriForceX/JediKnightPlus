/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Local definitions for game module
=====================================================================
*/

#ifndef __JK_LOCAL_H__
#define __JK_LOCAL_H__

#include "../../jkmod/game/jk_version.h" // Version header

/*
=====================================================================
Global definitions
=====================================================================
*/

#define MAX_NAME_BIG				64
#define	MAX_NAME_PRINT				28 // 24
#define	MAX_NAME_CHECK				(MAX_NAME_BIG - 8)
#define MAX_FILE_VARS				64
#define MAX_FILE_CHARS				1024
#define MAX_FILE_LENGTH				131072 // 128kb
#define MAX_IP_STRING				16
#define MAX_CHALLENGE				5 // 10
#define MAX_MSG_CHECK				99999
#define MAX_DIMENSION				((1 << DIMENSION_MAX) + MAX_CLIENTS)
#define MIN_PRIVATE_TIME			3
#define TEAM_CHANGE_DELAY			5000
#define STATION_SHIELD_TYPE			1
#define STATION_HEALTH_TYPE			2
#define STATION_AMMO_TYPE			3

#define DEFAULT						Q3_INFINITE // Workaround
#define NEWLINES					"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"

/*
=====================================================================
Player / world information
=====================================================================
*/

// Vote Control
typedef enum
{
	VOTE_MAP_RESTART = 0,
	VOTE_NEXTMAP,
	VOTE_MAP,
	VOTE_G_GAMETYPE,
	VOTE_KICK,
	VOTE_CLIENTKICK,
	VOTE_G_DOWARMUP,
	VOTE_TIMELIMIT,
	VOTE_FRAGLIMIT,
	VOTE_POLL,
	VOTE_SCORE_RESTART,
	VOTE_PAUSE,
	VOTE_MAP_NUMBER,

} jkmod_vote_t;

// Emotes list
typedef enum 
{
	EMOTE_BAR = 0,
	EMOTE_BEG,
	EMOTE_BURIED,
	EMOTE_COCKY,
	EMOTE_COMEON,
	EMOTE_COMTALK,
	EMOTE_CONVULSION,
	EMOTE_CROSSARMS,
	EMOTE_DONTKILLME,
	EMOTE_DONTKNOW,
	EMOTE_DONTKNOW2,
	EMOTE_EXPLAIN,
	EMOTE_EXPLAIN2,
	EMOTE_FAKEDEAD,
	EMOTE_FAKEDEAD2,
	EMOTE_FALLING,
	EMOTE_FLIP,
	EMOTE_GETUP,
	EMOTE_GREET,
	EMOTE_HANDHIPS,
	EMOTE_HUG,
	EMOTE_KISS,
	EMOTE_KNEEL,
	EMOTE_LAUGH,
	EMOTE_LOOK,
	EMOTE_LOOK2,
	EMOTE_NOD,
	EMOTE_POINT,
	EMOTE_POINT2,
	EMOTE_POSING,
	EMOTE_PUNCH,
	EMOTE_REVERENCE,
	EMOTE_SIT,
	EMOTE_SIT2,
	EMOTE_SIT3,
	EMOTE_SHAKE,
	EMOTE_SUPER,
	EMOTE_SUPER2,
	EMOTE_SURRENDER,
	EMOTE_SPIN,
	EMOTE_SPIN2,
	EMOTE_SPIN3,
	EMOTE_SPIN4,
	EMOTE_TAUNT3,
	EMOTE_TAUNT4,
	EMOTE_THINK,
	EMOTE_THREATEN,
	EMOTE_THUMBSUP,
	EMOTE_THUMBSDOWN,
	EMOTE_TOSSBACK,
	EMOTE_TOSSOVER,
	EMOTE_TOSSUP,
	EMOTE_TYPE,
	EMOTE_TYPE2,
	EMOTE_VICTORY,
	EMOTE_VICTORY2,
	EMOTE_WAITING,
	EMOTE_WATCHOUT,
	EMOTE_WRITING,
	EMOTE_WRITING2,

	EMOTE_NUM_EMOTES

} jkmod_emotes_t;

// Duel options
typedef enum 
{
	DUEL_SABER,
	DUEL_FORCE,
	DUEL_GUNS,
	DUEL_KICK

} jkmod_duel_options_t;

// Private options
typedef enum 
{
	PRIVATE_INDEX,
	PRIVATE_TIME,
	PRIVATE_INVITE,
	PRIVATE_REQUEST,
	PRIVATE_NUM,

	PRIVATE_MAX

} jkmod_private_options_t;

// Private settings
typedef enum 
{
	PRIVATE_WEAPONDISABLE,
	PRIVATE_FORCEDISABLE,
	PRIVATE_FORCELEVEL,
	PRIVATE_HOLDABLES,
	PRIVATE_JETPACK,
	PRIVATE_INVULNERABILITY,
	PRIVATE_PASSTHROUGH,
	PRIVATE_SPEED,
	PRIVATE_GRAVITY,

	PRIVATE_SETTINGS

} jkmod_private_settings_t;

// Bots control options
typedef enum 
{
	BOT_ENABLED,
	BOT_INDEX,

	BOT_MAX

} jkmod_bots_control_t;

// Cvar table
typedef struct
{
	vmCvar_t		*vmCvar;
	char			*cvarName;
	char			*defaultString;
	void			(*update)(void);
	int				cvarFlags;
	int				modificationCount;
	qboolean		trackChange;

} jkmod_cvar_table_t;

// Emotes data
typedef struct 
{
	jkmod_emotes_t	emoteIndex;		// Emote number
	char			*cmd;			// Command name
	qboolean		special;		// Is a Walkable emote
	int				startAnim;		// Beginning animation
	int				endAnim;		// Ending animation, -1 for no ending animation (no frozen)
	qboolean		compatible;		// It support all game versions
	qboolean		toggleSaber;	// Toggle lightsaber

} jkmod_emotes_data_t;

// Server entity
typedef struct
{
	vec3_t			itemFirstOrigin;	// First original item position
	qboolean		itemSpawnedBefore;	// Reset original item position
	int				dimensionOwner;		// Dimension owner number
	unsigned		dimensionPrevious;	// Previous dimension id
	unsigned		dimensionNumber;	// Current dimension number
	unsigned		dimensionNumberOld;	// Previous dimension number
	int				time1;				// Entity think time for pause
	int				time2;				// Entity sync time for pause

} jkmod_ent_t;

// Client session
typedef struct
{
	qboolean		motdSeen;					// Server motd seen
	qboolean		privateDuel;				// Private duel challenges
	qboolean		autoDuel;					// Auto accept duel challenges
	qboolean		playerStatus;				// Player status tracking
	qboolean		playerStatusSeen;			// Player status tracking check
	char			clientIP[MAX_IP_STRING];	// Client IP string
	int				ignoredPlayer[3];			// Client ignored player stuff
	int				ignoredAll[3];				// Client ignored all stuff
	int				chatColor;					// Custom chat color

} jkmod_sess_t;

// Client persistant
typedef struct
{
	qboolean		clientPlugin;				// Client plugin check
	float			clientVersion;				// Client plugin version
	int				customDuel;					// Client is in custom duel
	int				emoteLastTime;				// Emote last time used
	int				teleportChat[5];			// Player teleport x y z pitch yaw
	int				teleportChatCheck;			// Player teleport check
	int				jetpackUseDelay;			// Jetpack usage delay
	int				jetpackFxDelay;				// Jetpack effects delay
	qboolean		jetpackFxDisplay;			// Jetpack effects display
	int				playerStatusDelay;			// Player status tracking delay
	qboolean		buttonUseAnim;				// Button use animation use check
	qboolean		buttonUseAnimValid;			// Button use animation valid target
	int				raceStartTime;				// Start time for racers
	int				raceLastTime;				// Last time for racers
	int				raceBestTime;				// Best time for racers
	qboolean		invulnerability;			// Persistant take damage
	qboolean		passThrough;				// Check for pass-through
	qboolean		passThroughPerm;			// Check for permanent pass-through
	int				customSettings;				// Custom settings check
	int				customSettingsCount;		// Custom settings modification count
	int				customHealth;				// Custom player health
	int				customArmor;				// Custom player armor
	float			customGravity;				// Custom speed for custom settings
	float			customSpeed;				// Custom gravity for custom settings
	int				customSpawn[5];				// Player spawn x y z pitch yaw
	int				customSpawnCheck;			// Player spawn check
	int				duelHitCount;				// Number of hits that damage the opponent
	qboolean		dualSaber;					// Check for permanent dual saber
	char			modelName[MAX_QPATH];		// Player model name
	char			modelSounds[MAX_QPATH];		// Player sounds path
	char			skinName[MAX_QPATH];		// Player skin name
	char			forcePowers[MAX_QPATH];		// Player force powers
	unsigned		privateRoom[PRIVATE_MAX];	// Private room options
	int				botControl[BOT_MAX];		// Control bot check
	int				tempModelNum;				// Temp model entity number
	int				tempModelSettings[4];		// Temp model settings
	qboolean		ghostPlayer;				// Check player ghost invisibility

} jkmod_pers_t;

// Client clear
typedef struct 
{
	int				dropFlagTime;		// Player time
	int				dimensionTime;		// Dimension change time
	int				chatTime;			// Chat protect time
	int				motdTime;			// Server motd time
	int				voteWaitTime;		// Player callvote wait time
	int				teleportChatTime;	// Teleport chat wait time
	qboolean		teleportChatUsed;	// Teleport chat check used
	int				pushEffectTime;		// Force push effect time
	int				chairModelNum;		// Check chair model number
	int				chairModelUsed;		// Check chair model usage
	int				chairModelDelay;	// Check chair model delay
	int				forceChangeDelay;	// Force powers change delay
	int				teamChangeDelay;	// Team change delay
	
} jkmod_client_t;

// Bots persistant
typedef struct
{
	int				inactivityTime;		// Custom inactivity time
	int				actionFlags;		// Custom action flags

} jkmod_bots_t;

// Level data
typedef struct
{
	float			serverVersion;									// Server mod version
	char			randomBegin[MAX_FILE_VARS][MAX_FILE_CHARS];		// Random begin strings
	int				randomBeginCount;								// Random begin total count
	char			serverNews[MAX_FILE_VARS][MAX_FILE_CHARS];		// Server news strings
	int				serverNewsCount;								// Server news total count
	int				serverNewsNum;									// Server news current number
	int				serverNewsTime;									// Server news timer
	char			*teleportChats[MAX_FILE_CHARS];					// Teleport chat strings
	int				teleportChatsCount;								// Teleport chat total count
	int				idleTime;										// Server idle time
	int				pauseTime;										// Pause time stop
	unsigned		pauseTimeCustom;								// Pause custom seconds
	unsigned		cvarTempUnlock;									// Cvar latch unlock status
	qboolean		cvarToggleMod;									// Check cvar changes from toggleMod
	qboolean		mapRestarted;									// Map restart check for current players
	char			reconnectedIP[MAX_IP_STRING];					// Check IP for client reconnect
	int				closedCheck[MAX_MSG_CHECK];						// Closed server check
	int				messageCheck[MAX_MSG_CHECK];					// Connect message check
	int				chairModelCheck[MAX_GENTITIES][MAX_DIMENSION];	// Chair model usage check
	unsigned		dimensionBase;									// Global base dimension
	int				jetpackFxActive;								// Server jetpack active effect
	int				jetpackFxIdle;									// Server jetpack idle effect
	int				lockedTeam[TEAM_NUM_TEAMS];						// Current locked teams
	int				privateRoom[MAX_CLIENTS][PRIVATE_SETTINGS];		// Private room settings

} jkmod_locals_t;

// Dimension data
typedef struct 
{
	unsigned		dimension;
	int				weapondisable;
	int				forcedisable;
	int				forcelevel;
	qboolean		holdables;
	qboolean		jetpack;
	qboolean		invulnerability;
	qboolean		passthrough;
	float			speed;
	float			gravity;
	char			*command;
	char			*name;

} jkmod_dimension_data_t;

// Bit value data
typedef struct 
{ 
	const char		*string;

} jkmod_bit_info_t;

// Player tweaks data
typedef struct
{
	int				feature;
	qboolean		plugin;

} jkmod_tweaks_t;

// Callvote data
typedef struct
{
	int				bitmask;
	const char		*option;
	const char		*hint;

} jkmod_callvote_t;

// Commands data
typedef struct
{
	const char		*name;
	void			(*func)(gentity_t *ent);
	int				flags;

} jkmod_commands_t;

/*
=====================================================================
Re-routed functions
=====================================================================
*/

#define ClientCommand				JKMod_ClientCommand
#define G_RunClient					JKMod_RunClient
#define ClientThink_real			JKMod_ClientThink_real
#define G_RegisterCvars 			JKMod_G_RegisterCvars
#define G_UpdateCvars				JKMod_G_UpdateCvars
#define ClientConnect				JKMod_ClientConnect
#define ClientBegin					JKMod_ClientBegin
#define ConsoleCommand				JKMod_ConsoleCommand
#define G_InitGame					JKMod_G_InitGame
#define Cmd_Say_f					JKMod_Say
#define G_CallSpawn 				JKMod_G_CallSpawn
#define trap_Trace					JKMod_DimensionTrace
#define trap_SetBrushModel			JKMod_SetBrushModel
#define Svcmd_ForceTeam_f			JKMod_svCmd_forceTeam

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern	vmCvar_t					jkcvar_serverMotd;
extern	vmCvar_t					jkcvar_serverMotdTime;
extern	vmCvar_t					jkcvar_serverMotdTimePrint;
extern	vmCvar_t					jkcvar_serverMotdTele;
extern	vmCvar_t					jkcvar_serverMotdOnce;
extern	vmCvar_t					jkcvar_serverIdle;
extern	vmCvar_t					jkcvar_serverIdleFile;
extern	vmCvar_t					jkcvar_serverClosed;
extern	vmCvar_t					jkcvar_serverClosedText;
extern	vmCvar_t					jkcvar_serverConnectMsg;
extern	vmCvar_t					jkcvar_serverJoinSound;

extern	vmCvar_t					jkcvar_allowBlackNames;
extern	vmCvar_t					jkcvar_allowSaberHolocrons;
extern	vmCvar_t					jkcvar_allowTaunt2;
extern	vmCvar_t					jkcvar_allowMultiDuel;
extern	vmCvar_t					jkcvar_allowCustomDuel;
extern	vmCvar_t					jkcvar_allowDuelChat;

extern	vmCvar_t					jkcvar_noDuplicatedNames;
extern	vmCvar_t					jkcvar_chatProtect;
extern	vmCvar_t					jkcvar_chatProtectTime;
extern	vmCvar_t					jkcvar_chatAutoStatus;
extern	vmCvar_t					jkcvar_chatAutoStatusTime;
extern	vmCvar_t					jkcvar_chatColors;
extern	vmCvar_t					jkcvar_playerIgnore;
extern	vmCvar_t					jkcvar_spawnActivateSaber;
extern	vmCvar_t					jkcvar_saberIdleDamage;
extern	vmCvar_t					jkcvar_teleportChat;
extern	vmCvar_t					jkcvar_teleportChatTime;
extern	vmCvar_t					jkcvar_teleportChatFx;
extern	vmCvar_t					jkcvar_teleportChatSound;
extern	vmCvar_t					jkcvar_teleportFrag;

extern	vmCvar_t					jkcvar_voteAnyMap;
extern	vmCvar_t					jkcvar_voteControl;
extern	vmCvar_t					jkcvar_voteResults;
extern	vmCvar_t					jkcvar_voteWaitTime;
extern	vmCvar_t					jkcvar_voteMinPlayers;
extern	vmCvar_t					jkcvar_voteMaxPass;
extern	vmCvar_t					jkcvar_itemForcePhysics;
extern	vmCvar_t					jkcvar_duelStartHealth;
extern	vmCvar_t					jkcvar_duelStartArmor;
extern	vmCvar_t					jkcvar_duelStartEmote;
extern	vmCvar_t					jkcvar_duelEndStats;
extern	vmCvar_t					jkcvar_duelEndSlow;
extern	vmCvar_t					jkcvar_duelEndSlowScale;
extern	vmCvar_t					jkcvar_duelDistance;
extern	vmCvar_t					jkcvar_duelPassThrough;
extern	vmCvar_t					jkcvar_duelAutoAccept;

extern	vmCvar_t					jkcvar_dropFlag;
extern	vmCvar_t					jkcvar_dropFlagTime;
extern	vmCvar_t					jkcvar_damagePlums;
extern	vmCvar_t					jkcvar_customHats;

extern	vmCvar_t					jkcvar_emotesEnabled;
extern	vmCvar_t					jkcvar_emotesBreak;
extern	vmCvar_t					jkcvar_emotesFreeze;
extern	vmCvar_t					jkcvar_emotesPunchDamage;

extern	vmCvar_t					jkcvar_gamePlay;
extern	vmCvar_t					jkcvar_gameTypeConfig;
extern	vmCvar_t					jkcvar_altDimension;
extern	vmCvar_t					jkcvar_altDimensionBase;
extern	vmCvar_t					jkcvar_altDimensionTime;
extern	vmCvar_t					jkcvar_altDimensionSpawn;
extern	vmCvar_t					jkcvar_altDimensionNoScore;
extern	vmCvar_t					jkcvar_privateInviteTime;
extern	vmCvar_t					jkcvar_privateInviteBots;
extern	vmCvar_t					jkcvar_privateInactiveBots;
extern	vmCvar_t					jkcvar_forceChangeInstant;
extern	vmCvar_t					jkcvar_forceChangeTime;
extern	vmCvar_t					jkcvar_randomBegin;
extern	vmCvar_t					jkcvar_serverNews;
extern	vmCvar_t					jkcvar_serverNewsTime;
extern	vmCvar_t					jkcvar_serverNewsExtras;

extern	vmCvar_t					jkcvar_pluginRequired;
extern	vmCvar_t					jkcvar_macroScan;
extern	vmCvar_t					jkcvar_macroScanAlert;
extern	vmCvar_t					jkcvar_antiWarp;
extern	vmCvar_t					jkcvar_antiWarpTime;
extern	vmCvar_t					jkcvar_mapFixes;
extern	vmCvar_t					jkcvar_mapDefaultMusic;
extern	vmCvar_t					jkcvar_mapCycleFromFile;
extern	vmCvar_t					jkcvar_mapName;

extern	vmCvar_t					jkcvar_jetPack;
extern	vmCvar_t					jkcvar_playerTweaks;
extern	vmCvar_t					jkcvar_botsAI;
extern	vmCvar_t					jkcvar_dualSaber;

extern	vmCvar_t					jkcvar_test1;
extern	vmCvar_t					jkcvar_test2;

extern	vmCvar_t					bot_forgimmick;
extern	vmCvar_t					g_gamedate;
extern	vmCvar_t					developer;

/*
=====================================================================
Common / new functions
=====================================================================
*/

// g_active.c
void		BaseJK2_ClientThink_real(gentity_t *ent);

// g_client.c
char		*BaseJK2_ClientConnect(int clientNum, qboolean firstTime, qboolean isBot);
void		BaseJK2_ClientBegin(int clientNum, qboolean allowTeamReset);

// g_cmds.c
void		BaseJK2_ClientCommand(int clientNum);

// g_main.c
void		BaseJK2_G_RegisterCvars(void);
void		BaseJK2_G_UpdateCvars(void);
void		BaseJK2_G_InitGame(int levelTime, int randomSeed, int restart);

// g_svcmds.c
void		BaseJK2_Svcmd_ForceTeam_f(void);
qboolean	BaseJK2_ConsoleCommand(void);

// jk_active.c
void		JKMod_ClientTimerActions(gentity_t *ent, int msec);
void		JKMod_ClientThink_real(gentity_t *ent);
void		JKMod_PauseClientThink(gentity_t *ent);

// jk_client.c
void		JKMod_ClientCleanName(const char *in, char *out, int outSize, gentity_t *ent);

// jk_cmds.c
qboolean	JKMod_IgnoreClientCheck(int option, int ignorer, int ignored);
void		JKMod_IgnoreClientClear(int ignored);
void		JKMod_EngagePrivate(gentity_t* ent, qboolean say);
void		JKMod_InvitePrivate(gentity_t *ent, gentity_t *challenged);
void		JKMod_JoinPrivate(gentity_t* ent, int privateNum, unsigned dimension);
void		JKMod_SettingsPrivate(gentity_t* ent, int privateNum);
int			JKMod_PlayersPrivate(int privateRoom, qboolean skipBots);
void		JKMod_Cmd_ToggleConsole(gentity_t *ent);
void		JKMod_Cmd_WhoIs(gentity_t *ent);
void		JKMod_CallVote(gentity_t *ent);
void		JKMod_EngageDuel(gentity_t *ent, int type);
qboolean	JKMod_playerStatus(gentity_t *ent, qboolean announce);
void		JKMod_botControl(int botIndex, int ownerIndex, char *action);
void		JKMod_Say(gentity_t *ent, int mode, qboolean arg0);

// jk_common.c
void QDECL	JKMod_Printf(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
float		JKMod_GetVersion(const char *s);
qboolean	JKMod_CheckVersion(const char *s);
qboolean	JKMod_ValidNumber(const char *s);
void		JKMod_DummyEncode(char *buffer, const char *s);
char		*JKMod_TrimWhiteSpace(char* s);
void		JKMod_TruncateString(char *buffer, const char *s, int limit);
char		*JKMod_StrTok(char *str, const char *delim);
int			JKMod_Str_Argc(char *str);
void		JKMod_Str_Argv(int n, char *buffer, int bufferLength, char *src);
void		JKMod_StringClear(char* str, int len);
void		JKMod_StringEscape(char *in, char *out, int outSize);
char		*JKMod_SanitizeString(char *dest, char *source, int destSize);
int			JKMod_DuplicatedNameCheck(gentity_t *ent, char *clientName);
void		JKMod_DropPlayer(gentity_t *ent, char *reason);
const char	*JKMod_TeamName(team_t team, int letterCase);
const char	*JKMod_MsToString(const int ms);
const char	*JKMod_MsToWord(const int ms, qboolean abbr);
int			JKMod_GetClientNumber(char* name);
int			JKMod_CheckValidClient(gentity_t *ent, char *name);
char		*JKMod_ReadFile(char *filename);
int			JKMod_CompcStr(const void * a, const void * b);
void		JKMod_RandSeed(unsigned seed);
int			JKMod_Rand(void);
float		JKMod_RandFloat(float min, float max);

// jk_dimensions.c
int			JKMod_DimensionIndex(unsigned dimension);
void		JKMod_DimensionSettings(gentity_t *ent, unsigned dimension);
void		JKMod_DimensionLoad(void);
qboolean	JKMod_DimensionChange(gentity_t *ent, char *dimension, char *say);
qboolean	JKMod_DimensionCheck(int ent1, int ent2);
void		JKMod_DimensionOwnerCheck(int owner, gentity_t *ent);
unsigned	JKMod_DimensionGetCustom(void);
unsigned	JKMod_DimensionToPrivate(int number);
void		JKMod_DimensionSet(gentity_t *ent, unsigned dimension);
qboolean	JKMod_DimensionCollide(gentity_t *ent1, gentity_t *ent2);
void		JKMod_DimensionTrace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask);
int			JKMod_DimensionEntitiesInBox(const vec3_t mins, const vec3_t maxs, int *entityList, int maxcount, int entityNum);

// jk_emotes.c
int			JKMod_EmoteCheck(char *cmd, gentity_t *ent);
int			JKMod_EmoteIn(gentity_t *ent, int type);
int			JKMod_EmoteOut(gentity_t* ent);
int			JKMod_EmotePlay(gentity_t *ent, int emoteIndex);
void		JKMod_EmoteCmdHug(gentity_t *ent, qboolean showAlert);
void		JKMod_EmoteCmdKiss(gentity_t *ent, qboolean showAlert);
void		JKMod_EmoteCmdPunch(gentity_t *ent, qboolean showAlert);

// jk_main.c
void		JKMod_G_RegisterCvars(void);
void		JKMod_G_UpdateCvars(void);
const char	*JKMod_GetLatchedCvarValue(const char *cvarName, const char *resetString);
void		JKMod_CVU_gameType(void);
void		JKMod_CVU_teleportChat(void);
void		JKMod_CVU_gamePlay(void);
void		JKMod_CVU_altDimension(void);
void		JKMod_CVU_jetPack(void);
void		JKMod_CVU_playerTweaks(void);
void		JKMod_CVU_pluginRequired(void);
void		JKMod_CVU_serverClosed(void);
void		JKMod_CVU_dualSaber(void);
qboolean	JKMod_PauseFrameCheck(int levelTime);
void		JKMod_PauseFrameRun(void);
void		JKMod_TeleportChatLoad(void);
void		JKMod_RandomBeginLoad(void);
void		JKMod_ServerNewsLoad(void);
void		JKMod_ServerNewsCheck(void);
void		JKMod_ServerIdleCheck(void);
void		JKMod_GameTypeConfig(void);
void		JKMod_RemapShaders(void);

// jk_misc.c
const char	*JKMod_GetMapMusic(void);
void		JKMod_ForcePowerChange(gentity_t *ent, unsigned dimension);
qboolean	JKMod_ForcePowerValid(forcePowers_t power, playerState_t *ps);
qboolean	JKMod_PlayerMoving(gentity_t *ent, int move, int attack);
void		JKMod_PlayerTweaksCheck(gentity_t *ent);
void		JKMod_PlayerModelInfo(gentity_t *ent, const char* modelname);
void		JKMod_SetGamePlay(char *option);
void		JKMod_TeleportPlayer(gentity_t *player, vec3_t origin, vec3_t angles, qboolean spitplayer, int spitspeed, char *efxfile, char *efxsound);
int			JKMod_ItemRespawnTime(gentity_t *ent);
void		JKMod_CustomGameSettings(gentity_t *ent, int weapondisable, int forcedisable, int forcelevel, qboolean holdables, qboolean jetpack, qboolean invulnerability, qboolean passthrough, float speed, float gravity);
int			JKMod_SPMapCheck(const char *mapname);
int			JKMod_MPMapCheck(const char *mapname);
qboolean	JKMod_ValidPlayerModel(const char* modelname);
void		JKMod_ChairModelDisable(gentity_t *ent);

// jk_utils.c
void		JKMod_DrawBoxLines(vec3_t orig, vec3_t mins, vec3_t maxs, int color, int duration, int dimensionOwner);
void		JKMod_EntityScan(gentity_t *ent, int distance, int boxdelay, int linedelay);
void		JKMod_TempEffect(gentity_t *ent, int alignment, int rotation, char *efxfile, int angle, qboolean serverside);
void		JKMod_TempModelAdd(gentity_t *ent, int alignment, int rotation, char *modelname, int angle, qboolean playersolid);
void		JKMod_TempModelRemove(gentity_t *ent, int number);
void		JKMod_GhostAdd(gentity_t *ent, qboolean solid);
void		JKMod_GhostRemove(gentity_t *ent);
qboolean	JKMod_GhostCollide(gentity_t* ent1, gentity_t* ent2);
qboolean	JKMod_OthersInBox(gentity_t *ent);
void		JKMod_AntiStuckBox(gentity_t *ent);
qboolean	JKMod_CheckSolid(gentity_t* ent, int distance, vec3_t mins, vec3_t maxs, qboolean elevation);
void		JKMod_RemoveByClass(gentity_t *ent, char *name);
void		JKMod_JetpackTryUse(gentity_t *ent);
gentity_t	*JKMod_DuelGetPartner(gentity_t* ent);
qboolean	JKMod_DuelEachOther(gentity_t* ent1, gentity_t* ent2);
qboolean	JKMod_DuelIsolationCheck(gentity_t* ent1, gentity_t* ent2);
void		JKMod_DuelRemove(gentity_t *ent);
void		JKMod_G_InitGentity(gentity_t *e, int dimensionOwner);
gentity_t	*JKMod_G_PlayEffect(effectTypes_t fxID, const vec3_t org, const vec3_t ang, int dimensionOwner);
gentity_t	*JKMod_G_PlayEffect_ID(effectTypes_t fxID, const vec3_t org, const vec3_t ang, int dimensionOwner, qboolean serverSide);
gentity_t	*JKMod_G_Spawn(int dimensionOwner);
gentity_t	*JKMod_G_TempEntity(const vec3_t origin, entity_event_t event, int dimensionOwner);
gentity_t	*JKMod_G_SoundTempEntity(const vec3_t origin, int event, int channel, int dimensionOwner);
gentity_t	*JKMod_G_PreDefSound(vec3_t org, pdSounds_t pdSound, int dimensionOwner);
void		JKMod_G_SoundAtLoc(vec3_t loc, soundChannel_t channel, int soundIndex, int dimensionOwner);
void		JKMod_G_TestLine(vec3_t start, vec3_t end, int color, int time, int dimensionOwner);
gentity_t	*JKMod_LaunchItem(gitem_t *item, vec3_t origin, vec3_t velocity, int dimensionOwner);

// jk_session.c
void		JKMod_InitSessionData(gclient_t *client);
void		JKMod_ReadSessionData(gclient_t *client);
void		JKMod_WriteSessionData(gclient_t *client);

// jk_spawn.c
qboolean	JKMod_G_CallSpawn(gentity_t *ent);
void		JKMod_AddSpawnField(char *field, char *value);
void		JKMod_CheckValidMapItems(void);
void		JKMod_SetBrushModel(gentity_t *ent, const char *name);
void		JKMod_DoorFix(gentity_t* ent);

// jk_svcmds.c
void		JKMod_svCmd_forceTeam(void);

#endif // __JK_LOCAL_H__
