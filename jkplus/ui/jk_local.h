/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Local definitions for user interface module
=====================================================================
*/

#ifndef __JK_UI_LOCAL_H__
#define __JK_UI_LOCAL_H__

/*
=====================================================================
Global definitions
=====================================================================
*/

#define UI_RegisterCvars 			JKMod_UI_RegisterCvars
#define UI_UpdateCvars				JKMod_UI_UpdateCvars
#define UI_FeederCount				JKMod_UI_FeederCount
#define UI_FeederItemText			JKMod_UI_FeederItemText
#define UI_FeederSelection			JKMod_UI_FeederSelection

/*
=====================================================================
Player / world information
=====================================================================
*/

// Emotes data
typedef struct 
{
	int				count;	// Emotes count
	char			*cmd;	// Emotes command

} jkmod_emotes_t;

// Emotes table
typedef struct
{
	char			*cmd;	// Emotes command
	char			*title;	// Emotes title

} jkmod_emotes_table_t;

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern vmCvar_t						jkcvar_ui_test1;
extern vmCvar_t						jkcvar_ui_test2;

/*
=====================================================================
Common / new functions
=====================================================================
*/

// ui_main.c
const char	*BaseJK2_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3, qhandle_t *handle4, qhandle_t *handle5, qhandle_t *handle6);
int			BaseJK2_UI_FeederCount(float feederID);
qboolean	BaseJK2_UI_FeederSelection(float feederID, int index);

// jk_atoms.c
void		JKMod_MacroScan(void);
void		JKMod_MacroEnable(void);

// jk_main.c
const char *JKMod_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3, qhandle_t *handle4, qhandle_t *handle5, qhandle_t *handle6);
int			JKMod_UI_FeederCount(float feederID);
qboolean	JKMod_UI_FeederSelection(float feederID, int index);

#endif // __JK_UI_LOCAL_H__
