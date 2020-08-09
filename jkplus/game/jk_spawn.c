/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Spawn functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Spawn fields list and variables
=====================================================================
*/

// Spawn table struct
typedef struct { 
	char	*name;
	void(*spawn)(gentity_t *ent);
} spawn_t;

// Spawn functions list
void JKMod_SP_TimerStart(gentity_t *self);
void JKMod_SP_TimerStop(gentity_t *self);
void JKMod_SP_TimerCheckpoint(gentity_t *self);

spawn_t	jkmod_spawns[] = {

	// Custom spawn fields
	{"jkmod_timer_start",		JKMod_SP_TimerStart},
	{"jkmod_timer_stop",		JKMod_SP_TimerStop},
	{"jkmod_timer_checkpoint",	JKMod_SP_TimerCheckpoint},
	
	// Support for regular JK2 & JKA race maps
	{"target_startTimer",		JKMod_SP_TimerStart},
	{"target_stopTimer",		JKMod_SP_TimerStop},
	{"target_checkpoint",		JKMod_SP_TimerCheckpoint},
	{"df_trigger_start",		JKMod_SP_TimerStart},
	{"df_trigger_finish",		JKMod_SP_TimerStop},
	{"df_trigger_checkpoint",	JKMod_SP_TimerCheckpoint},

};

/*
=====================================================================
Call spawn function
=====================================================================
*/
qboolean JKMod_G_CallSpawn(gentity_t *ent) 
{ 
	spawn_t	*s;

	// Check custom spawn functions
	for (s = jkmod_spawns; s->name; s++) {
		if (!strcmp(s->name, ent->classname)) {
			// found it
			s->spawn(ent);
			return qtrue;
		}
	}

	// Launch original call spawn function
	return BaseJK2_G_CallSpawn(ent);
}
