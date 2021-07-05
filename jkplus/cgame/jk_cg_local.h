/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Local definitions for client module
=====================================================================
*/

#ifndef __JK_CG_LOCAL_H__
#define __JK_CG_LOCAL_H__

#include "../../jkplus/game/jk_version.h" // Version header

/*
=====================================================================
Global definitions
=====================================================================
*/

#define CHATBOX_ITEMS				5
#define CHATBOX_CUTOFF_LEN			550
#define CHATBOX_FONT_HEIGHT			20
#define PLAYER_LABEL_DIST			512 // 1014

/*
=====================================================================
Player / world information
=====================================================================
*/

// Chat box
typedef struct jkmod_chatbox_s
{
	char		string[MAX_SAY_TEXT];
	int			time;
	int			lines;

} jkmod_chatbox_t;

// Client game static
typedef struct
{
	int			pluginRequired;	// Tr!Force: [Plugin] Main cvar
	int			macroScan;		// Tr!Force: [MacroScan] Main cvar
	int			customHats;		// Tr!Force: [CustomHats] Main cvar
	int			jetPack;		// Tr!Force: [JetPack] Main cvar
	int			pauseTime;		// Tr!Force: [Pause] Server pause time

} jkmod_cgs_t;

// Client game
typedef struct
{
	jkmod_chatbox_t		chatItems[CHATBOX_ITEMS];	// Chat items count
	int					chatItemActive;				// Chat item active
	qboolean			duelEnd;					// Duel end active
	float				duelEndRange;				// Duel end original range
	int					duelEndLead;				// Duel end lead check
	qboolean			emoteCamera;				// Check emote camera
	int					prevCameraAngle;			// Saved camera angle
	int					prevCameraRange;			// Saved camera range
	int					prevCameraVertOffset;		// Saved camera vertical offset
	int					raceBestTime;				// Saved camera vertical offset

} jkmod_cg_t;

// Media
typedef struct 
{
	qhandle_t	saberModel;
	qhandle_t	bactaModel;
	qhandle_t	jetpackModel;
	qhandle_t	hatSanta;
	qhandle_t	hatPumpkin;
	qhandle_t	hatCap;
	qhandle_t	hatCowboy;
	qhandle_t	hatCringe;
	qhandle_t	hatSombrero;
	qhandle_t	hatGentleman;
	qhandle_t	hatPirate;
	
	qhandle_t	hitBox;
	qhandle_t	hitBoxNoCull;
	qhandle_t	dotRed;
	qhandle_t	dotGreen;
	qhandle_t	dotYellow;
	qhandle_t	clockBg;
	qhandle_t	jetpackIcon;

	sfxHandle_t	jetpackActiveSound;
	sfxHandle_t	jetpackIdleSound;

	fxHandle_t	jetpackActive;
	fxHandle_t	jetpackIdle;

	qhandle_t	keyCrouchOff;
	qhandle_t	keyCrouchOn;
	qhandle_t	keyJumpOff;
	qhandle_t	keyJumpOn;
	qhandle_t	keyBackOff;
	qhandle_t	keyBackOn;
	qhandle_t	keyForwardOff;
	qhandle_t	keyForwardOn;
	qhandle_t	keyLeftOff;
	qhandle_t	keyLeftOn;
	qhandle_t	keyRightOff;
	qhandle_t	keyRightOn;
	qhandle_t	keyAttackOn;
	qhandle_t	keyAttackOff;
	qhandle_t	keyAltOn;
	qhandle_t	keyAltOff;

} jkmod_media_t;

/*
=====================================================================
Re-routed functions
=====================================================================
*/

#define CG_RegisterCvars 						JKMod_CG_RegisterCvars
#define CG_UpdateCvars							JKMod_CG_UpdateCvars

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern vmCvar_t						jkcvar_cg_duelGlow;
extern vmCvar_t						jkcvar_cg_duelEnd;
extern vmCvar_t						jkcvar_cg_duelEndOrbit;
extern vmCvar_t						jkcvar_cg_duelEndDelay;
extern vmCvar_t						jkcvar_cg_drawClock;
extern vmCvar_t						jkcvar_cg_drawHitBox;
extern vmCvar_t						jkcvar_cg_drawForcePoints;
extern vmCvar_t						jkcvar_cg_drawInventory;
extern vmCvar_t						jkcvar_cg_drawRaceTimer;
extern vmCvar_t						jkcvar_cg_drawBactaModel;
extern vmCvar_t						jkcvar_cg_drawPlayerNames;
extern vmCvar_t						jkcvar_cg_drawMovementKeys;
extern vmCvar_t						jkcvar_cg_scoreboardIcons;
extern vmCvar_t						jkcvar_cg_scoreboardExtras;
extern vmCvar_t						jkcvar_cg_chatIcon;
extern vmCvar_t						jkcvar_cg_saberTrailSpeed;
extern vmCvar_t						jkcvar_cg_damageBlend;
extern vmCvar_t						jkcvar_cg_flagOpacity;
extern vmCvar_t						jkcvar_cg_flagAlignment;
extern vmCvar_t						jkcvar_cg_chatPlayerOpacity;
extern vmCvar_t						jkcvar_cg_specialMoveCamera;

extern vmCvar_t						jkcvar_cg_chatBox;
extern vmCvar_t						jkcvar_cg_chatBoxTime;
extern vmCvar_t						jkcvar_cg_chatBoxHeight;
extern vmCvar_t						jkcvar_cg_chatBoxHistory;

extern vmCvar_t						jkcvar_cg_customHats;
extern vmCvar_t						jkcvar_cg_customAnims;
extern vmCvar_t						jkcvar_cg_jetPackIdle;
extern vmCvar_t						jkcvar_cg_clientPopUp;
extern vmCvar_t						jkcvar_cg_resetClient;

extern vmCvar_t						jkcvar_cg_test1;
extern vmCvar_t						jkcvar_cg_test2;

/*
=====================================================================
Common / new functions
=====================================================================
*/

// g_main.c
void		BaseJK2_CG_RegisterCvars(void);
void		BaseJK2_CG_UpdateCvars(void);

// jk_cg_main.c
void		JKMod_CG_RegisterCvars(void);
void		JKMod_CG_UpdateCvars(void);
void		JKMod_CG_RegisterMedia(void);
void QDECL	JKMod_CG_Printf(const char *msg, ...) __attribute__ ((format (printf, 1, 2)));

// jk_cg_draw.c
void		JKMod_CG_Draw2D(void);
qboolean	JKMod_CG_IconHUDActive(void);
qboolean	JKMod_CG_CenterPrintActive(void);
const char	*JKMod_CG_MsToString(const int ms);
void		JKMod_CG_DrawClock(void);
void		JKMod_CG_ChatBox_StrInsert(char *buffer, int place, char *str);
void		JKMod_CG_ChatBox_AddString(char *chatStr);
void		JKMod_CG_ChatBox_ArrayInsert(jkmod_chatbox_t **array, int insPoint, int maxNum, jkmod_chatbox_t *item);
void		JKMod_CG_ChatBox_DrawStrings(void);
void		JKMod_CG_ClientPopUp(void);
void		JKMod_CG_DrawInventory(int y);
float		JKMod_CG_DrawPowerupIcons(int y);
void		JKMod_CG_DrawJetPackFuel(void);
void		JKMod_CG_DrawDimensionString(void);
void		JKMod_CG_DrawRaceTimer(void);
void		JKMod_CG_DrawPauseString(void);
void		JKMod_CG_DrawPlayerLabels(void);
void		JKMod_CG_DrawMovementKeys(void);

// jk_cg_players.c
void		JKMod_CG_Player(centity_t *cent);
void		JKMod_CG_AddHitBox(centity_t *cent);
void		JKMod_CG_AddModelOnPlayer(centity_t *cent, int time, qhandle_t *gameModels, qhandle_t modelFile, char *modelBolt, vec4_t modelDetails);
void		JKMod_CG_DuelEnd(qboolean winner);
qboolean	JKMod_CG_EmoteUI(void);
void		JKMod_CG_EmoteCamera(void);
float		JKMod_CG_GroundDistance(void);

#endif // __JK_CG_LOCAL_H__
