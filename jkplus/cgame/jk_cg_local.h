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

#define CHATBOX_CUTOFF_LEN			550
#define CHATBOX_FONT_HEIGHT			20
#define MAX_CHATBOX_ITEMS			5

/*
=====================================================================
Player / world information
=====================================================================
*/

// Server cvars
typedef struct
{
	int			altDimensions;	// Tr!Force: [Dimensions] Main cvar
	int			macroScan;		// Tr!Force: [MacroScan] Main cvar

} jkmod_cvar_t;

// Chat box
typedef struct jkmod_chatbox_s
{
	char		string[MAX_SAY_TEXT];
	int			time;
	int			lines;

} jkmod_chatbox_t;

// Client game
typedef struct
{
	jkmod_chatbox_t		chatItems[MAX_CHATBOX_ITEMS];	// Chat items count
	int					chatItemActive;					// Chat item active

} jkmod_cg_t;

/*
=====================================================================
Re-routed functions
=====================================================================
*/

#define CG_RegisterCvars 			JKMod_CG_RegisterCvars
#define CG_UpdateCvars				JKMod_CG_UpdateCvars

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern vmCvar_t						jkcvar_cg_privateDuel;
extern vmCvar_t						jkcvar_cg_duelGlow;
extern vmCvar_t						jkcvar_cg_drawClock;
extern vmCvar_t						jkcvar_cg_drawHitBox;
extern vmCvar_t						jkcvar_cg_drawForcePoints;
extern vmCvar_t						jkcvar_cg_drawInventory;
extern vmCvar_t						jkcvar_cg_scoreboardIcons;
extern vmCvar_t						jkcvar_cg_chatIcon;
extern vmCvar_t						jkcvar_cg_saberTrailSpeed;
extern vmCvar_t						jkcvar_cg_damageBlend;
extern vmCvar_t						jkcvar_cg_customAnims;

extern vmCvar_t						jkcvar_cg_chatBox;
extern vmCvar_t						jkcvar_cg_chatBoxTime;
extern vmCvar_t						jkcvar_cg_chatBoxHeight;
extern vmCvar_t						jkcvar_cg_chatBoxHistory;

extern vmCvar_t						jkcvar_cg_test1;
extern vmCvar_t						jkcvar_cg_test2;

/*
=====================================================================
Common / new functions
=====================================================================
*/

// jk_cg_draw.c
void		JKMod_CG_Draw2D(void);
void		JKMod_CG_DrawClock(void);
void		JKMod_CG_AddHitBox(centity_t *cent);
void		JKMod_CG_ChatBox_StrInsert(char *buffer, int place, char *str);
void		JKMod_CG_ChatBox_AddString(char *chatStr);
void		JKMod_CG_ChatBox_ArrayInsert(jkmod_chatbox_t **array, int insPoint, int maxNum, jkmod_chatbox_t *item);
void		JKMod_CG_ChatBox_DrawStrings(void);
void		JKMod_CG_DrawInventory(int y);
float		JKMod_CG_DrawPowerupIcons(int y);

#endif // __JK_CG_LOCAL_H__
