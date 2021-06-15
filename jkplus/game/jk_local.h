/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Local definitions for game module
=====================================================================
*/

#ifndef __JK_LOCAL_H__
#define __JK_LOCAL_H__

#include "../../jkplus/game/jk_version.h" // Version header

/*
=====================================================================
Global definitions
=====================================================================
*/

#define MAX_NETNAME					64
#define	MAX_NAME_PRINT				28 // 24
#define	MAX_NAME_CHECK				(MAX_NETNAME - 8)
#define MAX_FILE_VARS				64
#define MAX_FILE_CHARS				1024
#define MAX_FILE_LENGTH				131072
#define MAX_LINES					64
#define MAX_BRUSH_MODELS			150

#define	ITEM_RESPAWN_ARMOR			20
#define	ITEM_RESPAWN_HEALTH			30
#define	ITEM_RESPAWN_AMMO			40
#define	ITEM_RESPAWN_HOLDABLE		60

#define DEFAULT						Q3_INFINITE // Workaround

#define VALIDSTRING(a)				((a != NULL) && (a[0] != '\0'))
#define VALIDSTRINGCVAR(a)			((a != NULL) && (a[0] != '\0') && (*a != '0'))

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
	VOTE_GAMEPLAY,
	VOTE_POLL

} jkmod_vote_t;

// Server entity
typedef struct
{
	vec3_t		ItemFirstOrigin;		// First original item position
	qboolean    ItemSpawnedBefore;		// Reset original item position
	int			dimensionOwner;			// Dimension owner number
	unsigned	dimensionNumber;		// Current dimension number
	int			time1;					// Entity think time for pause
	int			time2;					// Entity sync time for pause

} jkmod_ent_t;

// Client session
typedef struct
{
	qboolean	MotdSeen;			// Server motd seen
	char		ClientIP[64];		// Client IP
	int			IgnoredPlayer[2];	// Client ignored player chats & duels
	int			IgnoredAll[2];		// Client ignored all chats & duels

} jkmod_sess_t;

// Client persistant
typedef struct
{
	qboolean	ClientPlugin;			// Client plugin check
	int			CustomDuel;				// Client is in force duel
	int			TeleportChatOrigin[5];	// Player saved pos x y z pitch yaw
	char		*TeleportChatSaved;		// Player saved pos saved
	int			jetackUseDelay;			// Jetpack usage delay
	qboolean	buttonUseAnim;			// Button use animation use check
	qboolean	buttonUseAnimValid;		// Button use animation valid target
	int			raceStartTime;			// Start time for racers
	qboolean	invulnerability;		// Persistant take damage
	qboolean	passThrough;			// Check for pass-through
	qboolean	passThroughPerm;		// Check for permanent pass-through
	int			customSettings;			// Custom settings check
	int			customSettingsCount;	// Custom settings modification count
	float		customGravity;			// Custom speed for custom settings
	float		customSpeed;			// Custom gravity for custom settings

} jkmod_pers_t;

// Client clear
typedef struct 
{
	vec3_t		PauseSavedView;		// Player saved view angles
	int			TimeResidual;		// Player time residual
	int			DropFlagTime;		// Player time
	int			DimensionTime;		// Dimension change time
	int			ConnectTime;		// Client connect time
	int			ChatTime;			// Chat protect time
	int			MotdTime;			// Server motd time
	int			VoteWaitTime;		// Player callvote wait time
	int			TeleportChatTime;	// Teleport chat wait time
	qboolean	TeleportChatUsed;	// Teleport chat check used
	
} jkmod_client_t;

// Level data
typedef struct
{
	char		RandomBegin[MAX_FILE_VARS][MAX_FILE_CHARS];		// Store messages
	int			RandomBeginCount;								// Message counter
	char		ServerNews[MAX_FILE_VARS][MAX_FILE_CHARS];		// Store messages
	int			ServerNewsCount;								// News counter
	int			ServerNewsNum;									// News timer
	char		*TeleportChats[MAX_FILE_CHARS];					// Chat teleport info
	int			TeleportChatsCount;								// Chat teleport counter
	int			idleTime;										// Server idle time
	int			pauseTime;										// Pause time stop
	unsigned	pauseTimeCustom;								// Pause custom seconds
	unsigned	cvarTempUnlock;									// Cvar latch unlock status

} jkmod_locals_t;

// Dimension data
typedef struct 
{
	unsigned	dimension;
	int			weapons;
	int			forcepowers;
	int			forcelevel;
	qboolean	holdables;
	qboolean	jetpack;
	qboolean	invulnerability;
	qboolean	passthrough;
	float		speed;
	float		gravity;
	char		*command;
	char		*name;

} jkmod_dimension_data_t;

/*
=====================================================================
Re-routed functions
=====================================================================
*/

#define ClientCommand				JKMod_ClientCommand
#define G_RunClient					JKMod_RunClient
#define ClientThink_real			JKMod_ClientThink_real
#define ClientTimerActions			JKMod_ClientTimerActions
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

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern	vmCvar_t					jkcvar_test1;
extern	vmCvar_t					jkcvar_test2;

extern	vmCvar_t					jkcvar_serverMotd;
extern	vmCvar_t					jkcvar_serverMotdTime;
extern	vmCvar_t					jkcvar_serverIdle;
extern	vmCvar_t					jkcvar_serverIdleFile;
extern	vmCvar_t					jkcvar_serverClosed;
extern	vmCvar_t					jkcvar_serverClosedText;
extern	vmCvar_t					jkcvar_serverJoinSound;

extern	vmCvar_t					jkcvar_allowBlackNames;
extern	vmCvar_t					jkcvar_allowMultiDuel;
extern	vmCvar_t					jkcvar_allowCustomDuel;
extern	vmCvar_t					jkcvar_allowDuelChat;
extern	vmCvar_t					jkcvar_noDuplicatedNames;
extern	vmCvar_t					jkcvar_chatProtect;
extern	vmCvar_t					jkcvar_chatProtectTime;
extern	vmCvar_t					jkcvar_playerIgnore;
extern	vmCvar_t					jkcvar_teleportChat;
extern	vmCvar_t					jkcvar_teleportChatTime;
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
extern	vmCvar_t					jkcvar_duelEndStats;
extern	vmCvar_t					jkcvar_duelEndSlowFix;
extern	vmCvar_t					jkcvar_duelEndTimeScale;

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
extern	vmCvar_t					jkcvar_altDimensionTime;
extern	vmCvar_t					jkcvar_randomBegin;
extern	vmCvar_t					jkcvar_serverNews;
extern	vmCvar_t					jkcvar_serverNewsTime;

extern	vmCvar_t					jkcvar_pluginRequired;
extern	vmCvar_t					jkcvar_macroScan;
extern	vmCvar_t					jkcvar_antiWarp;
extern	vmCvar_t					jkcvar_antiWarpTime;
extern	vmCvar_t					jkcvar_mapFixes;
extern	vmCvar_t					jkcvar_mapDefaultMusic;
extern	vmCvar_t					jkcvar_mapCycleFromFile;

extern	vmCvar_t					jkcvar_jetPack;
extern	vmCvar_t					jkcvar_playerMovement;
extern	vmCvar_t					jkcvar_botsAI;

/*
=====================================================================
Common / new functions
=====================================================================
*/

// g_active.c
void		BaseJK2_ClientTimerActions(gentity_t *ent, int msec);
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
qboolean	BaseJK2_ConsoleCommand(void);

// jk_active.c
void		JKMod_ClientTimerActions(gentity_t *ent, int msec);
void		JKMod_ClientThink_real(gentity_t *ent);
void		JKMod_PauseClientThink(gentity_t *ent);

// jk_client.c
void		JKMod_ClientCleanName(const char *in, char *out, int outSize, gentity_t *ent);

// jk_cmds.c
qboolean	JKMod_IsClientIgnored(int option, int ignorer, int ignored);
void		JKMod_CallVote(gentity_t *ent);
void		JKMod_EngageDuel(gentity_t *ent, int type);
void		JKMod_Say(gentity_t *ent, int mode, qboolean arg0);
void		JKMod_WhoIs(gentity_t *ent);

// jk_common.c
void QDECL	JKMod_Printf(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
int			JKMod_compareCmd(char *cmd, char *required);
void		JKMod_stringEscape(char *in, char *out, int outSize);
void		JKMod_cleanString(char *in, char *out);
char		*JKMod_sanitizeString(char *dest, char *source, int destSize);
int			JKMod_duplicatedNameCheck(gentity_t *ent, char *clientName);
void		JKMod_dropPlayer(gentity_t *ent, char *msg);
void		JKMod_sendCommand(int target, char *cmd, char *string);
const char	*JKMod_msToString(const int ms, qboolean abbr);
int			JKMod_ClientNumberFromArg(char* name);
char		*JKMod_ConcatArgs(int start);
char		*JKMod_ReadFile(char *filename);
void		JKMod_RandSeed(unsigned seed);
int			JKMod_Rand(void);
float		JKMod_RandFloat(float min, float max);

// jk_dimensions.c
int			JKMod_DimensionIndex(unsigned dimension);
void		JKMod_DimensionSettings(gentity_t *ent, unsigned dimension);
qboolean	JKMod_DimensionCmd(gentity_t *ent, char *dimension, qboolean say);
qboolean	JKMod_DimensionCheck(int ent1, int ent2);
void		JKMod_DimensionOwnerCheck(int owner, gentity_t *ent);
unsigned	JKMod_DimensionGetFree(void);
void		JKMod_DimensionSet(gentity_t *ent, unsigned dimension);
qboolean	JKMod_DimensionCollide(gentity_t *ent1, gentity_t *ent2);
void		JKMod_DimensionTrace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask);
int			JKMod_DimensionEntitiesInBox(const vec3_t mins, const vec3_t maxs, int *entityList, int maxcount, int entityNum);

// jk_emotes.c
int			JKMod_emoteCheck(char *cmd, gentity_t *ent);
int			JKMod_emoteIn(gentity_t *ent, int type);
void		JKMod_emoteDo(gentity_t *ent, int emoteIndex);
void		JKMod_emoteDoHug(gentity_t *ent);
void		JKMod_emoteDoKiss(gentity_t *ent);
void		JKMod_emoteDoPunch(gentity_t *ent);

// jk_main.c
void		JKMod_G_RegisterCvars(void);
void		JKMod_G_UpdateCvars(void);
qboolean	JKMod_PauseFrameCheck(int levelTime);
void		JKMod_PauseFrameRun(void);
void		JKMod_randomBeginInit(void);
void		JKMod_serverNewsInit(void);
void		JKMod_teleportChatInit(void);
void		JKMod_serverIdleCheck(void);
void		JKMod_gameTypeConfig(void);

// jk_misc.c
const char	*JKMod_GetCurrentMap(void);
const char	*JKMod_GetMapMusic(void);
qboolean	JKMod_ForcePowerValid(forcePowers_t power, playerState_t *ps);
qboolean	JKMod_PlayerMoving(gentity_t *ent, int move, int attack);
void		JKMod_PlayerMovementCheck(gentity_t *ent);
void		JKMod_TeleportPlayer(gentity_t *player, vec3_t origin, vec3_t angles, qboolean spitplayer, int spitspeed, char *efxfile, char *efxsound);
void		JKMod_CustomGameSettings(gentity_t *ent, int weapons, int forcepowers, int forcelevel, qboolean holdables, qboolean jetpack, qboolean invulnerability, qboolean passthrough, float speed, float gravity);
qboolean	JKMod_SPMapCheck(const char *mapname, qboolean normal, qboolean special);

// jk_utils.c
qboolean	JKMod_OthersInBox(gentity_t *ent);
void		JKMod_AntiStuckBox(gentity_t *ent);
void		JKMod_RemoveByClass(gentity_t *ent, char *name);
void		JKMod_G_InitGentity(gentity_t *e, int dimensionOwner);
gentity_t	*JKMod_G_PlayEffect(effectTypes_t fxID, const vec3_t org, const vec3_t ang, int dimensionOwner);
gentity_t	*JKMod_G_PlayEffect_ID(effectTypes_t fxID, const vec3_t org, const vec3_t ang, int dimensionOwner);
gentity_t	*JKMod_G_Spawn(int dimensionOwner);
gentity_t	*JKMod_G_TempEntity(const vec3_t origin, entity_event_t event, int dimensionOwner);
gentity_t	*JKMod_G_SoundTempEntity(const vec3_t origin, int event, int channel, int dimensionOwner);
gentity_t	*JKMod_G_PreDefSound(vec3_t org, pdSounds_t pdSound, int dimensionOwner);
void		JKMod_G_SoundAtLoc(vec3_t loc, soundChannel_t channel, int soundIndex, int dimensionOwner);
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
qboolean	JKMod_SpawnAngleHack(const char *key, const char *defaultString, float *out);

// jk_svcmds.c
void		JKMod_gamePlay(char *gameplay);

#endif // __JK_LOCAL_H__
