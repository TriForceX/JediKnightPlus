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

// Base spawn list
extern	spawn_t	spawns[];
extern char *G_AddSpawnVarToken(const char *string);

// Spawn functions list
void JKMod_SP_TimerStart(gentity_t *self);
void JKMod_SP_TimerStop(gentity_t *self);
void JKMod_SP_TimerCheckpoint(gentity_t *self);

void JKMod_SP_ShieldPowerConverter(gentity_t *self);
void JKMod_SP_HealthPowerConverter(gentity_t *self);
void JKMod_SP_AmmoPowerConverter(gentity_t *self);

spawn_t	jkmod_spawns[] = {

	// Custom spawn fields
	{"jkmod_timer_start",				JKMod_SP_TimerStart},
	{"jkmod_timer_stop",				JKMod_SP_TimerStop},
	{"jkmod_timer_checkpoint",			JKMod_SP_TimerCheckpoint},

	{"jkmod_shield_power_converter",	JKMod_SP_ShieldPowerConverter},
	{"jkmod_health_power_converter",	JKMod_SP_HealthPowerConverter},
	{"jkmod_ammo_power_converter",		JKMod_SP_AmmoPowerConverter},
	
	// Support for regular JK2 & JKA race maps
	{"target_startTimer",				JKMod_SP_TimerStart},
	{"target_stopTimer",				JKMod_SP_TimerStop},
	{"target_checkpoint",				JKMod_SP_TimerCheckpoint},
	{"df_trigger_start",				JKMod_SP_TimerStart},
	{"df_trigger_finish",				JKMod_SP_TimerStop},
	{"df_trigger_checkpoint",			JKMod_SP_TimerCheckpoint},

};

/*
=====================================================================
Call spawn function
=====================================================================
*/
qboolean JKMod_G_CallSpawn(gentity_t *ent) 
{ 
	spawn_t	*s;
	gitem_t	*item;

	if (!ent->classname) {
		G_Printf("G_CallSpawn: NULL classname\n");
		return qfalse;
	}

	// check item spawn functions
	for (item = bg_itemlist + 1; item->classname; item++) {
		if (!strcmp(item->classname, ent->classname)) {
			G_SpawnItem(ent, item);
			return qtrue;
		}
	}

	// check normal spawn functions
	for (s = spawns; s->name; s++) {
		if (!strcmp(s->name, ent->classname)) {
			// found it
			s->spawn(ent);
			return qtrue;
		}
	}

	// Check custom spawn functions
	for (s = jkmod_spawns; s->name; s++) {
		if (!strcmp(s->name, ent->classname)) {
			// found it
			s->spawn(ent);
			return qtrue;
		}
	}

	G_Printf("%s doesn't have a spawn function\n", ent->classname);
	return qfalse;
}

/*
=====================================================================
Call spawn function
=====================================================================
*/
void JKMod_AddSpawnField(char *field, char *value)
{
	int	i;

	for (i = 0; i < level.numSpawnVars; i++)
	{
		if (Q_stricmp(level.spawnVars[i][0], field) == 0)
		{
			level.spawnVars[i][1] = G_AddSpawnVarToken(value);
			return;
		}
	}

	level.spawnVars[level.numSpawnVars][0] = G_AddSpawnVarToken(field);
	level.spawnVars[level.numSpawnVars][1] = G_AddSpawnVarToken(value);
	level.numSpawnVars++;
}
