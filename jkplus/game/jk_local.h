/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Local definitions for game module
=====================================================================
*/

#include "../../jkplus/game/jk_version.h" // Version header

/*
=====================================================================
Player / world information struct and flags
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

/*
=====================================================================
Global definitions
=====================================================================
*/

#define	MAX_IP						22
#define	MAX_NAME_PRINT				28 // 24
#define	MAX_NAME_CHECK				(MAX_NETNAME - 8)
#define MAX_FILE_TEXT				8192
#define MAX_LINES					64

#define	ITEM_RESPAWN_ARMOR			20
#define	ITEM_RESPAWN_HEALTH			30
#define	ITEM_RESPAWN_AMMO			40
#define	ITEM_RESPAWN_HOLDABLE		60

#define ClientCommand				JKMod_ClientCommand
#define ClientThink_real			JKMod_ClientThink_real
#define ClientTimerActions			JKMod_ClientTimerActions
#define G_RegisterCvars 			JKMod_G_RegisterCvars
#define G_UpdateCvars				JKMod_G_UpdateCvars
#define ClientConnect				JKMod_ClientConnect
#define ClientBegin					JKMod_ClientBegin
#define ClientSpawn					JKMod_ClientSpawn
#define ConsoleCommand				JKMod_ConsoleCommand
#define G_InitGame					JKMod_G_InitGame
#define ClientCleanName(a, b, c)	JKMod_ClientCleanName(ent, a, b, c)
#define Cmd_EngageDuel_f			JKMod_EngageDuel
#define Cmd_Say_f					JKMod_Say

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern	vmCvar_t					jkcvar_test1;
extern	vmCvar_t					jkcvar_test2;

extern	vmCvar_t					jkcvar_serverMotd;
extern	vmCvar_t					jkcvar_serverMotdTime;
extern	vmCvar_t					jkcvar_serverClosed;
extern	vmCvar_t					jkcvar_serverClosedIP;

extern	vmCvar_t					jkcvar_allowBlackNames;
extern	vmCvar_t					jkcvar_allowMultiDuel;
extern	vmCvar_t					jkcvar_allowCustomDuel;
extern	vmCvar_t					jkcvar_allowDuelChat;
extern	vmCvar_t					jkcvar_noDuplicatedNames;
extern	vmCvar_t					jkcvar_chatProtect;
extern	vmCvar_t					jkcvar_chatProtectTime;
extern	vmCvar_t					jkcvar_playerIgnore;
extern	vmCvar_t					jkcvar_teleportChat;

extern	vmCvar_t					jkcvar_voteCustomMap;
extern	vmCvar_t					jkcvar_voteControl;
extern	vmCvar_t					jkcvar_voteResults;
extern	vmCvar_t					jkcvar_voteWaitTime;
extern	vmCvar_t					jkcvar_itemForcePhysics;
extern	vmCvar_t					jkcvar_duelStartHealth;
extern	vmCvar_t					jkcvar_duelStartArmor;
extern	vmCvar_t					jkcvar_duelEndStats;

extern	vmCvar_t					jkcvar_dropFlag;
extern	vmCvar_t					jkcvar_dropFlagTime;
extern	vmCvar_t					jkcvar_damagePlums;
extern	vmCvar_t					jkcvar_pauseGame;

extern	vmCvar_t					jkcvar_emotesEnabled;
extern	vmCvar_t					jkcvar_emotesBreak;
extern	vmCvar_t					jkcvar_emotesFreeze;
extern	vmCvar_t					jkcvar_emotesPunchDamage;

extern	vmCvar_t					jkcvar_gamePlay;
extern	vmCvar_t					jkcvar_altDimensions;
extern	vmCvar_t					jkcvar_randomBegin;
extern	vmCvar_t					jkcvar_serverNews;
extern	vmCvar_t					jkcvar_serverNewsTime;

extern	vmCvar_t					jkcvar_forcePlugin;

/*
=====================================================================
Common / new functions
=====================================================================
*/

// g_client.c
char		*BaseJK2_ClientConnect(int clientNum, qboolean firstTime, qboolean isBot);

// g_main.c
void		BaseJK2_G_InitGame(int levelTime, int randomSeed, int restart);

// jk_active.c
void		JKMod_ClientThink_real(gentity_t *ent);

// jk_client.c
void		JKMod_ClientCleanName(gentity_t *ent, const char *in, char *out, int outSize);

// jk_cmds.c
qboolean	JKMod_IsClientIgnored(char *option, int ignorer, int ignored);
void		JKMod_CallVote(gentity_t *ent);

// jk_common.c
const char	*JKMod_msToString(const int ms, qboolean abbr);
int			JKMod_compareCmd(char *cmd, char *required);
void		JKMod_stringEscape(char *in, char *out, int outSize);
void		JKMod_cleanString(char *in, char *out);
void		JKMod_dropPlayer(gentity_t *ent, char *msg);
void		JKMod_sendCommand(int target, char *cmd, char *string);
int			JKMod_ClientNumberFromArg(char* name);
char		*JKMod_ConcatArgs(int start);
char		*JKMod_ReadFile(char *filename);
void		JKMod_sRand(unsigned seed);
int			JKMod_Rand(void);

// jk_dimensions.c
void		JKMod_Dimensions(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask);

// jk_emotes.c
int			JKMod_emoteCheck(char *cmd, gentity_t *ent);
int			JKMod_emoteIn(gentity_t *ent, int type);
void		JKMod_emoteDo(gentity_t *ent, int emoteIndex);
void		JKMod_emoteDoHug(gentity_t *ent);
void		JKMod_emoteDoKiss(gentity_t *ent);
void		JKMod_emoteDoPunch(gentity_t *ent);

// jk_misc.c
void		JKMod_TeleportPlayer(gentity_t *player, vec3_t origin, vec3_t angles, qboolean spitplayer, qboolean killbox);

// jk_svcmds.c
void		JKMod_gamePlay(char *gameplay);
