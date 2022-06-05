/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Spawn functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// Undo re-routing for calls made from here
#undef trap_SetBrushModel

// Re-declare trace function
void trap_SetBrushModel(gentity_t *ent, const char *name);

// Extern stuff
extern char *G_AddSpawnVarToken(const char *string);
extern int G_ItemDisabled(gitem_t *item);

/*
=====================================================================
Spawn fields list and variables
=====================================================================
*/

// Spawn table struct
typedef struct 
{ 
	char	*name;
	void	(*spawn)(gentity_t *ent);

} spawn_t;

// Base spawn list
extern spawn_t spawns[];

// Spawn functions list
void JKMod_SP_MiscModel(gentity_t *ent);
void JKMod_SP_MiscPowerConverter(gentity_t *ent);
void JKMod_SP_DrainModel(gentity_t* ent);
void JKMod_SP_ChairModel(gentity_t* ent);
void JKMod_SP_DoorModel(gentity_t* ent);

void JKMod_SP_TimerStart(gentity_t *self);
void JKMod_SP_TimerStop(gentity_t *self);
void JKMod_SP_TimerCheckpoint(gentity_t *self);

void JKMod_SP_MiscModelBreakable(gentity_t *ent);
void JKMod_SP_MiscIonCannon(gentity_t *ent);

spawn_t	jkmod_spawns[] = {

	// Custom spawn fields
	{"jkmod_misc_model",				JKMod_SP_MiscModel},
	{"jkmod_misc_power_converter",		JKMod_SP_MiscPowerConverter},
	{"jkmod_drain_model",				JKMod_SP_DrainModel},
	{"jkmod_chair_model",				JKMod_SP_ChairModel},
	{"jkmod_door_model",				JKMod_SP_DoorModel},

	{"jkmod_timer_start",				JKMod_SP_TimerStart},
	{"jkmod_timer_stop",				JKMod_SP_TimerStop},
	{"jkmod_timer_checkpoint",			JKMod_SP_TimerCheckpoint},

	// Single player fields
	{"misc_model_breakable",			JKMod_SP_MiscModelBreakable},
	{"misc_ion_cannon",					JKMod_SP_MiscIonCannon},
	
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

	// Print only in developer mode
	if (jkcvar_mapFixes.integer) {
		JKMod_Printf("%s doesn't have a spawn function\n", ent->classname);
	} else {
		G_Printf("%s doesn't have a spawn function\n", ent->classname);
	}
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

/*
=====================================================================
Check valid map items function
=====================================================================
*/
void JKMod_CheckValidMapItems(void) 
{
	gentity_t	*check;
	int			e;
	int			wDisable = 0;

	JKMod_Printf(S_COLOR_RED "Removing non valid map items...\n");

	wDisable = g_gametype.integer == GT_TOURNAMENT ? g_duelWeaponDisable.integer : g_weaponDisable.integer;
	check = g_entities + 1;

	for (e = 0; e < level.num_entities; e++, check++)
	{
		if (!check->inuse) continue;

		if (check->item->giType == IT_WEAPON && wDisable && (wDisable & (1 << check->item->giTag)))
		{
			if (g_gametype.integer != GT_JEDIMASTER)
			{
				G_FreeEntity(check);
			}
		}

		if (G_ItemDisabled(check->item))
		{
			G_FreeEntity(check);
		}

		if (g_gametype.integer != GT_JEDIMASTER)
		{
			if (HasSetSaberOnly())
			{
				if (check->item->giType == IT_AMMO)
				{
					G_FreeEntity(check);
				}

				if (check->item->giType == IT_HOLDABLE)
				{
					if (check->item->giTag == HI_SEEKER ||
						check->item->giTag == HI_SHIELD ||
						check->item->giTag == HI_SENTRY_GUN)
					{
						G_FreeEntity(check);
					}
				}
			}
		}
		else
		{ 
			// No powerups in jedi master
			if (check->item->giType == IT_POWERUP)
			{
				G_FreeEntity(check);
			}
		}

		if (g_gametype.integer == GT_HOLOCRON)
		{
			if (check->item->giType == IT_POWERUP)
			{
				if (check->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT ||
					check->item->giTag == PW_FORCE_ENLIGHTENED_DARK)
				{
					G_FreeEntity(check);
				}
			}
		}

		if (g_forcePowerDisable.integer)
		{ 
			// If force powers disabled, don't add force powerups
			if (check->item->giType == IT_POWERUP)
			{
				if (check->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT ||
					check->item->giTag == PW_FORCE_ENLIGHTENED_DARK ||
					check->item->giTag == PW_FORCE_BOON)
				{
					G_FreeEntity(check);
				}
			}
		}

		if (g_gametype.integer == GT_TOURNAMENT)
		{
			if (check->item->giType == IT_ARMOR ||
				check->item->giType == IT_HEALTH ||
				(check->item->giType == IT_HOLDABLE && check->item->giTag == HI_MEDPAC))
			{
				G_FreeEntity(check);
			}
		}

		if (g_gametype.integer != GT_CTF && g_gametype.integer != GT_CTY && check->item->giType == IT_TEAM)
		{
			int killMe = 0;

			switch (check->item->giTag)
			{
				case PW_REDFLAG:
				case PW_BLUEFLAG:
				case PW_NEUTRALFLAG:
					killMe = 1;
					break;
				default:
					break;
			}

			if (killMe) G_FreeEntity(check);
		}
	}
}

/*
=====================================================================
Custom set brush model function
=====================================================================
*/
void JKMod_SetBrushModel(gentity_t *ent, const char *name)
{
	if (jkcvar_mapFixes.integer)
	{
		if (strlen(name) < 2 || name[0] != '*' || !name) return;

		if (atoi(name + 1) == 0)
		{
			G_FreeEntity(ent);
			JKMod_Printf(S_COLOR_MAGENTA "Entitity freed (zero)\n");
			return;
		}

		if (atoi(name + 1) > 127 && strstr(ent->classname, "func_"))
		{
			G_FreeEntity(ent);
			JKMod_Printf(S_COLOR_MAGENTA "Entitity freed (%i > %i)\n", atoi(name + 1), 127);
			return;
		}
	}

	trap_SetBrushModel(ent, name);
}
