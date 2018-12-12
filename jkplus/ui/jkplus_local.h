/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Local definitions for user interface module
=====================================================================
*/

/*
=====================================================================
Player / world information struct and flags
=====================================================================
*/

typedef struct {

	int				emotesCount;		// Emotes Count

} JKPlusUiInfo_t;

extern JKPlusUiInfo_t JKPlusUiInfo;

/*
=====================================================================
Global definitions
=====================================================================
*/

#define UI_RegisterCvars 			JKPlus_UI_RegisterCvars
#define UI_UpdateCvars				JKPlus_UI_UpdateCvars
#define UI_FeederCount				JKPlus_UI_FeederCount
#define UI_FeederItemText			JKPlus_UI_FeederItemText

/*
=====================================================================
Common / new functions
=====================================================================
*/

// ui_main.c
const char	*BaseJK2_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3);
int			BaseJK2_UI_FeederCount(float feederID);

// jkplus_main.c
const char *JKPlus_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3);
int			JKPlus_UI_FeederCount(float feederID);
void		JKPlus_UI_RunMenuScript(char **args); // WIP
