/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Local definitions for game module
=====================================================================
*/

#undef	GAMEVERSION

#define JKPLUS_LONGNAME			"Jedi Knight Plus"
#define JKPLUS_SHORTNAME   		"JK+"
#define JKPLUS_MAJOR 			"0"
#define JKPLUS_MINOR 			"8"
#define JKPLUS_PATCH  			"1"
#define GAMEVERSION				S_COLOR_CYAN JKPLUS_SHORTNAME " Mod v" JKPLUS_MAJOR "." JKPLUS_MINOR "." JKPLUS_PATCH

/*
=====================================================================
Player / world information struct and flags
=====================================================================
*/

/*typedef struct {

	int			something1;					// Info 1
	int			something2;					// Info 2

} someInfo_t;*/

/*typedef enum {

	EFFECT_SOMETHING1 = 0,					// Effect 1
	EFFECT_SOMETHING2,						// Effect 2

} someEffects_t;*/

/*
=====================================================================
Global definitions
=====================================================================
*/

#define	MAX_IP						22
#define	MAX_NAME_PRINT				28 // 24
#define	MAX_NAME_CHECK				(MAX_NETNAME - 8)

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

extern	vmCvar_t					jkcvar_serverClosed;
extern	vmCvar_t					jkcvar_serverClosedIP;
extern	vmCvar_t					jkcvar_serverClosedBroadcast;

extern	vmCvar_t					jkcvar_allowBlackNames;
extern	vmCvar_t					jkcvar_allowMultiDuel;
extern	vmCvar_t					jkcvar_allowDuelChat;
extern	vmCvar_t					jkcvar_noDuplicatedNames;

extern	vmCvar_t					jkcvar_voteCustomMap;
extern	vmCvar_t					jkcvar_voteGameplay;
extern	vmCvar_t					jkcvar_itemForcePhysics;
extern	vmCvar_t					jkcvar_duelStartHealth;
extern	vmCvar_t					jkcvar_duelStartShield;
extern	vmCvar_t					jkcvar_duelEndStats;

extern	vmCvar_t					jkcvar_dropFlag;
extern	vmCvar_t					jkcvar_dropFlagTime;
extern	vmCvar_t					jkcvar_damagePlums;
extern	vmCvar_t					jkcvar_pauseGame;
extern	vmCvar_t					jkcvar_pauseGameCenterPrint;

extern	vmCvar_t					jkcvar_emotesEnabled;
extern	vmCvar_t					jkcvar_emotesBreak;
extern	vmCvar_t					jkcvar_emotesFreeze;
extern	vmCvar_t					jkcvar_emotesPunchDamage;

/*
=====================================================================
Common / new functions
=====================================================================
*/

// g_client.c
char		*BaseJK2_ClientConnect(int clientNum, qboolean firstTime, qboolean isBot);

// jk_client.c
void		JKPlus_ClientCleanName(gentity_t *ent, const char *in, char *out, int outSize);

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
const char	*JKPlus_msToString(const int ms);
int			JKPlus_compareCmd(char *cmd, char *required);
void		JKPlus_stringEscape(char *in, char *out);
void		JKPlus_cleanString(char *in, char *out);
void		JKPlus_dropPlayer(gentity_t *ent, char *msg);
void		JKPlus_sendCommand(int target, char *cmd, char *string);
