/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Local definitions for client module
=====================================================================
*/

#ifndef __JK_CG_LOCAL_H__
#define __JK_CG_LOCAL_H__

#include "../../jkmod/game/jk_version.h" // Version header

/*
=====================================================================
Global definitions
=====================================================================
*/

#define CHATBOX_ITEMS				5
#define CHATBOX_CUTOFF_LEN			550
#define CHATBOX_FONT_HEIGHT			20
#define PLAYER_LABEL_DIST			512 // 1014
#define USE_DISTANCE				64.0f
#define ACCEL_SAMPLES				16
#define PERCENT_SAMPLES				16

/*
=====================================================================
Player / world information
=====================================================================
*/

// Cvar table
typedef struct 
{
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	void		(*update)(void);
	int			cvarFlags;
	int			modificationCount;

} jkmod_cg_cvar_table_t;

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
	int			pluginRequired;		// Tr!Force: [Plugin] Main cvar
	int			macroScan;			// Tr!Force: [MacroScan] Main cvar
	int			customHats;			// Tr!Force: [CustomHats] Main cvar
	int			jetPack;			// Tr!Force: [JetPack] Main cvar
	int			dualSaber;			// Tr!Force: [DualSaber] Main cvar
	int			duelPassThrough;	// Tr!Force: [DuelPassThrough] Main cvar
	int			pauseTime;			// Tr!Force: [Pause] Server pause time
	qboolean	modCheck;			// Tr!Force: [ModCheck] Server mod check

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
	int					consolePrint;				// Console print lines
	int					consolePrintTime;			// Console print timestamp
	int					consoleNotifyTime;			// Console notify time
	
	vec4_t				strafeHelperActiveColor;	// Strafe helper active color
	float				currentSpeed;				// Strafe helper & speed meter current speed
	float				maxSpeed;					// Speed meter max speed
	float				previousSpeed;				// Speed meter prev speed
	float				lastZSpeed;					// Speed meter lastZspeed
	float				lastJumpHeight;				// Speed meter last jump height
	int					lastJumpHeightTime;			// Speed meter last jump height tie
	float				lastJumpDistance;			// Speed meter jump distance
	int					lastJumpDistanceTime;		// Speed meter jump distance time
	float				lastGroundSpeed;			// Speed meter ground speed
	int					lastGroundTime;				// Speed meter ground speed time
	vec3_t				lastGroundPosition;			// Speed meter ground speed position
	qboolean			firstTimeInAir;				// Speed meter first time in air
	qboolean			wasOnGround;				// Speed meter was on ground

} jkmod_cg_t;

// Strafe helper options
typedef enum
{
	SHELPER_SUPEROLDSTYLE	= ( 1 << 0 ),
	SHELPER_OLDSTYLE		= ( 1 << 1 ),
	SHELPER_NEWBARS			= ( 1 << 2 ),
	SHELPER_OLDBARS			= ( 1 << 3 ),
	SHELPER_SOUND			= ( 1 << 4 ),
	SHELPER_W				= ( 1 << 5 ),
	SHELPER_WA				= ( 1 << 6 ),
	SHELPER_WD				= ( 1 << 7 ),
	SHELPER_A				= ( 1 << 8 ),
	SHELPER_D				= ( 1 << 9 ),
	SHELPER_REAR			= ( 1 << 10 ),
	SHELPER_CENTER			= ( 1 << 11 ),
	SHELPER_ACCELMETER		= ( 1 << 12 ),
	SHELPER_WEZE			= ( 1 << 13 ),
	SHELPER_CROSSHAIR		= ( 1 << 14 ),

} jkmod_shelper_option_t;

// Speed meter options
typedef enum
{
	SMETER_ENABLE			= ( 1 << 0 ),
	SMETER_GROUNDSPEED		= ( 1 << 1 ),
	SMETER_JUMPHEIGHT		= ( 1 << 2 ),
	SMETER_JUMPDISTANCE		= ( 1 << 3 ),
	SMETER_VERTICALSPEED	= ( 1 << 4 ),
	SMETER_YAWSPEED			= ( 1 << 5 ),
	SMETER_ACCELMETER		= ( 1 << 6 ),
	SMETER_SPEEDGRAPH		= ( 1 << 7 ),
	SMETER_KPH				= ( 1 << 8 ),
	SMETER_MPH				= ( 1 << 9 ),

} jkmod_smeter_option_t;

// Movement style
typedef enum
{
	MV_SIEGE,
	MV_JKA,
	MV_QW,
	MV_CPM,
	MV_Q3,
	MV_PJK,
	MV_WSW,
	MV_RJQ3,
	MV_RJCPM,
	MV_SWOOP,
	MV_JETPACK,
	MV_SPEED,
	MV_SP,
	MV_SLICK,
	MV_BOTCPM,
	MV_NUMSTYLES

} jkmod_movement_style_t;

// Bit value data
typedef struct 
{ 
	const char		*string;

} jkmod_cg_bit_info_t;

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
	qhandle_t	hatProbe;
	qhandle_t	hatDroid;
	qhandle_t	hatYsalamiri;
	
	qhandle_t	hitBox;
	qhandle_t	hitBoxNoCull;
	qhandle_t	dotRed;
	qhandle_t	dotGreen;
	qhandle_t	dotYellow;
	qhandle_t	clockBg;
	qhandle_t	jetpackIcon;
	qhandle_t	forceSwirl;
	qhandle_t	useableHint;

	sfxHandle_t	strafeHelperSound;
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

extern vmCvar_t						jkcvar_cg_privateDuel;
extern vmCvar_t						jkcvar_cg_autoDuel;
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
extern vmCvar_t						jkcvar_cg_identifyObjects;

extern vmCvar_t						jkcvar_cg_chatBox;
extern vmCvar_t						jkcvar_cg_chatBoxTime;
extern vmCvar_t						jkcvar_cg_chatBoxHeight;
extern vmCvar_t						jkcvar_cg_chatBoxHistory;


extern vmCvar_t						jkcvar_cg_speedMeter;
extern vmCvar_t						jkcvar_cg_strafeHelper;
extern vmCvar_t						jkcvar_cg_sHelperCutoff;
extern vmCvar_t						jkcvar_cg_sHelperPrecision;
extern vmCvar_t						jkcvar_cg_sHelperLineWidth;
extern vmCvar_t						jkcvar_cg_sHelperActiveColor;
extern vmCvar_t						jkcvar_cg_sHelperInactiveAlpha;
extern vmCvar_t						jkcvar_cg_sHelperOffset;
extern vmCvar_t						jkcvar_cg_sHelperFPS;

extern vmCvar_t						jkcvar_cg_customHats;
extern vmCvar_t						jkcvar_cg_customAnims;
extern vmCvar_t						jkcvar_cg_customEffects;
extern vmCvar_t						jkcvar_cg_jetPackIdle;

extern vmCvar_t						jkcvar_cg_test1;
extern vmCvar_t						jkcvar_cg_test2;
extern vmCvar_t						cg_developer;

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
void		JKMod_CG_CVU_sHelperColor(void);
void		JKMod_CG_RegisterMedia(void);
void QDECL	JKMod_CG_Printf(const char *msg, ...) __attribute__ ((format (printf, 1, 2)));

// jk_cg_consolecmds.c
void		JKMod_CG_EmoteCamera(void);
void		JKMod_CG_StrafeHelperToggle(void);
void		JKMod_CG_SpeedMeterToggle(void);

// jk_cg_draw.c
void		JKMod_CG_Draw2D(void);
qboolean	JKMod_CG_IconHUDActive(void);
qboolean	JKMod_CG_CenterPrintActive(void);
int			JKMod_CG_ConsolePrintActive(void);
const char	*JKMod_CG_MsToString(const int ms);
void		JKMod_CG_DrawClock(void);
void		JKMod_CG_ChatBox_StrInsert(char *buffer, int place, char *str);
void		JKMod_CG_ChatBox_AddString(char *chatStr);
void		JKMod_CG_ChatBox_ArrayInsert(jkmod_chatbox_t **array, int insPoint, int maxNum, jkmod_chatbox_t *item);
void		JKMod_CG_ChatBox_DrawStrings(void);
void		JKMod_CG_DrawInventory(int y);
float		JKMod_CG_DrawPowerupIcons(int y);
void		JKMod_CG_DrawJetPackFuel(void);
void		JKMod_CG_DrawDimensionString(void);
void		JKMod_CG_DrawRaceTimer(void);
void		JKMod_CG_DrawPauseString(void);
void		JKMod_CG_DrawPlayerLabels(void);
void		JKMod_CG_DrawMovementKeys(void);
void		JKMod_CG_StrafeHelperSound(float difference);
void		JKMod_CG_StrafeHelperDrawLine(float x1, float y1, float x2, float y2, float size, vec4_t color, float alpha, float ycutoff);
void		JKMod_CG_StrafeHelperDrawSpeed(int moveDir);
void		JKMod_CG_StrafeHelperLine(vec3_t velocity, float diff, qboolean active, int moveDir);
void		JKMod_CG_StrafeHelper(centity_t* cent);
void		JKMod_CG_SpeedMeterAccel(void);
void		JKMod_CG_SpeedMeterJumpHeight(centity_t* cent);
void		JKMod_CG_SpeedMeterJumpDistance(void);
void		JKMod_CG_SpeedMeterVerticalSpeed(void);
void		JKMod_CG_SpeedMeter(void);

// jk_cg_players.c
void		JKMod_CG_Player(centity_t *cent);
void		JKMod_CG_AddHitBox(centity_t *cent);
void		JKMod_CG_AddModelOnPlayer(centity_t *cent, int time, qhandle_t *gameModels, qhandle_t modelFile, char *modelBolt, vec4_t modelDetails);
void		JKMod_CG_DuelEnd(qboolean winner);
qboolean	JKMod_CG_EmoteUI(void);
float		JKMod_CG_GroundDistance(void);
void		JKMod_CG_CalculateSpeed(centity_t* cent);
void		JKMod_CG_ForcePushBodyBlur(centity_t* cent);

#endif // __JK_CG_LOCAL_H__
