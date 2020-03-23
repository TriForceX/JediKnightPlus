/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
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

	char			*emotesCmd;			// Emotes command
	char			*emotesTitle;		// Emotes title

} emoteData_t;

typedef struct {

	int				emotesCount;		// Emotes count
	emoteData_t		emotesList;		// Emotes list

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
#define UI_FeederSelection			JKPlus_UI_FeederSelection

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern vmCvar_t						jkplus_ui_test1;
extern vmCvar_t						jkplus_ui_test2;

/*
=====================================================================
Common / new functions
=====================================================================
*/

// ui_main.c
const char	*BaseJK2_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3, qhandle_t *handle4, qhandle_t *handle5, qhandle_t *handle6);
int			BaseJK2_UI_FeederCount(float feederID);
qboolean	BaseJK2_UI_FeederSelection(float feederID, int index);

// jkplus_main.c
const char *JKPlus_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3, qhandle_t *handle4, qhandle_t *handle5, qhandle_t *handle6);
int			JKPlus_UI_FeederCount(float feederID);
qboolean	JKPlus_UI_FeederSelection(float feederID, int index);
