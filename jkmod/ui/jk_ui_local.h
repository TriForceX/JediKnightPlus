/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Local definitions for user interface module
=====================================================================
*/

#ifndef __JK_UI_LOCAL_H__
#define __JK_UI_LOCAL_H__

#include "../../jkmod/game/jk_version.h" // Version header

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

// Cvar table
typedef struct 
{
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;

} jkmod_ui_cvar_table_t;

// Command table
typedef struct
{
	char		*cmd;	// Item command
	char		*title;	// Item title

} jkmod_ui_command_table_t;

// Reset client table
typedef struct
{
	char		*cvar;		// Cvar name
	char		*defVal;	// Cvar default value
	char		*recVal;	// Cvar recommended value

} jkmod_ui_reset_client_t;

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern vmCvar_t						jkcvar_ui_votePoll;
extern vmCvar_t						jkcvar_ui_emoteToggle;
extern vmCvar_t						jkcvar_ui_dimensionToggle;
extern vmCvar_t						jkcvar_ui_resetClient;

extern vmCvar_t						jkcvar_ui_test1;
extern vmCvar_t						jkcvar_ui_test2;

/*
=====================================================================
Common / new functions
=====================================================================
*/

// ui_main.c
void		BaseJK2_UI_RegisterCvars(void);
void		BaseJK2_UI_UpdateCvars(void);
const char	*BaseJK2_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3, qhandle_t *handle4, qhandle_t *handle5, qhandle_t *handle6);
int			BaseJK2_UI_FeederCount(float feederID);
qboolean	BaseJK2_UI_FeederSelection(float feederID, int index);

// jk_atoms.c
void		JKMod_UI_MacroScan(void);
void		JKMod_UI_MacroEnable(void);
qboolean	JKMod_UI_ConsoleCommand(char *cmd);

// jk_main.c
const char *JKMod_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3, qhandle_t *handle4, qhandle_t *handle5, qhandle_t *handle6);
int			JKMod_UI_FeederCount(float feederID);
qboolean	JKMod_UI_FeederSelection(float feederID, int index);
void		JKMod_UI_Update(const char *name, int val);
qboolean	JKMod_UI_RunMenuScript(const char **args, const char *name);

#endif // __JK_UI_LOCAL_H__
