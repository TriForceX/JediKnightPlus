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

#define CG_RegisterCvars 			JKMod_CG_RegisterCvars
#define CG_UpdateCvars				JKMod_CG_UpdateCvars

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

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern vmCvar_t						jkcvar_cg_privateDuel;
extern vmCvar_t						jkcvar_cg_drawClock;
extern vmCvar_t						jkcvar_cg_drawHitBox;
extern vmCvar_t						jkcvar_cg_drawForcePoints;

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

#endif // __JK_CG_LOCAL_H__
