/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Local definitions for game module
=====================================================================
*/

#undef	GAMEVERSION

#define JK_LONGNAME			"Jedi Knight Plus"
#define JK_SHORTNAME   		"JK+"
#define JK_MAJOR 			"0"
#define JK_MINOR 			"9"
#define JK_PATCH  			"2"
#define GAMEVERSION			S_COLOR_CYAN JK_SHORTNAME " Mod v" JK_MAJOR "." JK_MINOR "." JK_PATCH

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

} jkplus_vote_t;

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

#define ClientCommand				JKPlus_ClientCommand
#define ClientThink_real			JKPlus_ClientThink_real
#define ClientTimerActions			JKPlus_ClientTimerActions
#define G_RegisterCvars 			JKPlus_G_RegisterCvars
#define G_UpdateCvars				JKPlus_G_UpdateCvars
#define ClientConnect				JKPlus_ClientConnect
#define ClientBegin					JKPlus_ClientBegin
#define ClientSpawn					JKPlus_ClientSpawn
#define ConsoleCommand				JKPlus_ConsoleCommand
#define G_InitGame					JKPlus_G_InitGame
#define ClientCleanName(a, b, c)	JKPlus_ClientCleanName(ent, a, b, c)

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
extern	vmCvar_t					jkcvar_allowDuelChat;
extern	vmCvar_t					jkcvar_noDuplicatedNames;
extern	vmCvar_t					jkcvar_chatProtect;
extern	vmCvar_t					jkcvar_chatProtectTime;
extern	vmCvar_t					jkcvar_playerIgnore;

extern	vmCvar_t					jkcvar_voteCustomMap;
extern	vmCvar_t					jkcvar_voteControl;
extern	vmCvar_t					jkcvar_voteResults;
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

/*
=====================================================================
Common / new functions
=====================================================================
*/

// g_client.c
char		*BaseJK2_ClientConnect(int clientNum, qboolean firstTime, qboolean isBot);

// jk_cmds.c
qboolean	JKPlus_IsClientIgnored(char *option, int ignorer, int ignored);
void		JKPlus_CallVote(gentity_t *ent);

// jk_client.c
void		JKPlus_ClientCleanName(gentity_t *ent, const char *in, char *out, int outSize);

// jk_dimensions.c
void		JKPlus_Dimensions(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask);

// jk_active.c
void		JKPlus_ClientThink_real(gentity_t *ent);

// jk_emotes.c
int			JKPlus_emoteCheck(char *cmd, gentity_t *ent);
int			JKPlus_emoteIn(gentity_t *ent, int type);
void		JKPlus_emoteDo(gentity_t *ent, int emoteIndex);
void		JKPlus_emoteDoHug(gentity_t *ent);
void		JKPlus_emoteDoKiss(gentity_t *ent);
void		JKPlus_emoteDoPunch(gentity_t *ent);

// jk_common.c
const char	*JKPlus_msToString(const int ms, qboolean abbr);
int			JKPlus_compareCmd(char *cmd, char *required);
void		JKPlus_stringEscape(char *in, char *out, int outSize);
void		JKPlus_cleanString(char *in, char *out);
void		JKPlus_dropPlayer(gentity_t *ent, char *msg);
void		JKPlus_sendCommand(int target, char *cmd, char *string);
int			JKPlus_ClientNumberFromArg(char* name);
char		*JKPlus_ConcatArgs(int start);
void		JKPlus_ingameGameplay(char *gameplay);
