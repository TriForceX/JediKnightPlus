/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Local definitions for game module
=====================================================================
*/

#undef	GAMEVERSION

#define JKPLUS_LONGNAME			"Jedi Knight Plus"
#define JKPLUS_SHORTNAME   		"JKPlus"
#define JKPLUS_PHASE   			"Beta"
#define JKPLUS_RELEASE 			"1"
#define JKPLUS_DEVELOP 			"0-rev1"
#define GAMEVERSION				"^5" JKPLUS_SHORTNAME " " JKPLUS_RELEASE "." JKPLUS_DEVELOP " " JKPLUS_PHASE

/*
=====================================================================
Player / world information struct
=====================================================================
*/

/*typedef struct {

	int			something1;					// Info 1
	int			something2;					// Info 2

} someInfo_t;*/

/*
=====================================================================
Effect flags
=====================================================================
*/

/*typedef enum {

	EFFECT_SOMETHING1 = 0,					// Effect 1
	EFFECT_SOMETHING2,						// Effect 2

} someEffects_t;*/

/*
=====================================================================
Global definitions
=====================================================================
*/

#define ClientCommand				JKPlus_ClientCommand
#define ClientThink_real			JKPlus_ClientThink_real
#define G_RegisterCvars 			JKPlus_G_RegisterCvars
#define G_UpdateCvars				JKPlus_G_UpdateCvars
#define ClientConnect				JKPlus_ClientConnect
#define ClientBegin					JKPlus_ClientBegin
#define ClientSpawn					JKPlus_ClientSpawn

/*
=====================================================================
Cvar registration
=====================================================================
*/

extern	vmCvar_t					jkplus_test1;
extern	vmCvar_t					jkplus_test2;

/*
=====================================================================
Common / new functions
=====================================================================
*/

char		*BaseJK2_ClientConnect(int clientNum, qboolean firstTime, qboolean isBot);
