/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main dimension stuff...
=====================================================================
*/

#include "../../code/game/g_local.h" // Main header

// Undo re-routing for calls made from here
#undef trap_Trace

// Re-declare trace function
void trap_Trace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask);

// Save entity owners nums
static int g_entities_dimension[MAX_GENTITIES];

/*
=====================================================================
Start trace parse function
=====================================================================
*/
static void trap_Trace_Start(int entityNum)
{
	if (0 <= entityNum && entityNum < MAX_CLIENTS)
	{
		// Check temp dimension
		if (g_entities[entityNum].client && (g_entities[entityNum].client->ps.eFlags & JK_PASS_THROUGH)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_MOVER && ((!Q_stricmp(g_entities[i].classname, "func_door") || !Q_stricmp(g_entities[i].classname, "func_plat")))) ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						g_entities_dimension[i] = g_entities[i].r.ownerNum;
						g_entities[i].r.ownerNum = entityNum;
					}
				}
			}
		}
		// Check duel dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[entityNum].client && (level.clients[entityNum].ps.stats[JK_DIMENSION] & JK_DUEL_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN) != (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN)) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack"))))) 
					{
						g_entities_dimension[i] = g_entities[i].r.ownerNum;
						g_entities[i].r.ownerNum = entityNum;
					}
				}
			}
		}
		// Check chat dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_CHAT)) && g_entities[entityNum].client && (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_CHAT_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_MOVER && ((!Q_stricmp(g_entities[i].classname, "func_door") || !Q_stricmp(g_entities[i].classname, "func_plat")))) ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						g_entities_dimension[i] = g_entities[i].r.ownerNum;
						g_entities[i].r.ownerNum = entityNum;
					}
				}
			}
		}
		// Check guns dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)) && g_entities[entityNum].client && (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN) != (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_MOVER && ((!Q_stricmp(g_entities[i].classname, "func_door") || !Q_stricmp(g_entities[i].classname, "func_plat")))) ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						g_entities_dimension[i] = g_entities[i].r.ownerNum;
						g_entities[i].r.ownerNum = entityNum;
					}
				}
			}
		}
		// Check race dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_RACE)) && g_entities[entityNum].client && (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_RACE_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_MOVER && ((!Q_stricmp(g_entities[i].classname, "func_door") || !Q_stricmp(g_entities[i].classname, "func_plat")))) ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						g_entities_dimension[i] = g_entities[i].r.ownerNum;
						g_entities[i].r.ownerNum = entityNum;
					}
				}
			}
		}
		// Final check
		else {
			int i;
			if (g_entities[entityNum].inuse) {
				const int saberOwner = g_entities[entityNum].r.ownerNum;
				if (g_entities[saberOwner].client && 
					((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN)) ||
					((jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) )
				{
					return;
				}
			}
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse && g_entities[i].client && (g_entities[i].client->ps.eFlags & JK_PASS_THROUGH) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN)) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_CHAT)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_CHAT_IN)) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_RACE)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_RACE_IN)) ) 
					{
						g_entities_dimension[i] = g_entities[i].r.ownerNum;
						g_entities[i].r.ownerNum = entityNum;
					}
				}
			}
		}
	}
}

/*
=====================================================================
End trace parse and clean up
=====================================================================
*/
static void trap_Trace_End(int entityNum) 
{
	if (0 <= entityNum && entityNum < MAX_CLIENTS)
	{
		// Check temp dimension
		if (g_entities[entityNum].client && (g_entities[entityNum].client->ps.eFlags & JK_PASS_THROUGH)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_MOVER && ((!Q_stricmp(g_entities[i].classname, "func_door") || !Q_stricmp(g_entities[i].classname, "func_plat")))) ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						g_entities[i].r.ownerNum = g_entities_dimension[i];
					}
				}
			}
		}
		// Check duel dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[entityNum].client && (level.clients[entityNum].ps.stats[JK_DIMENSION] & JK_DUEL_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN) != (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN)) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack"))))) 
					{
						g_entities[i].r.ownerNum = g_entities_dimension[i];
					}
				}
			}
		}
		// Check chat dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_CHAT)) && g_entities[entityNum].client && (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_CHAT_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_MOVER && ((!Q_stricmp(g_entities[i].classname, "func_door") || !Q_stricmp(g_entities[i].classname, "func_plat")))) ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						g_entities[i].r.ownerNum = g_entities_dimension[i];
					}
				}
			}
		}
		// Check guns dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)) && g_entities[entityNum].client && (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN) != (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_MOVER && ((!Q_stricmp(g_entities[i].classname, "func_door") || !Q_stricmp(g_entities[i].classname, "func_plat")))) ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						g_entities[i].r.ownerNum = g_entities_dimension[i];
					}
				}
			}
		}
		// Check race dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_RACE)) && g_entities[entityNum].client && (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_RACE_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_MOVER && ((!Q_stricmp(g_entities[i].classname, "func_door") || !Q_stricmp(g_entities[i].classname, "func_plat")))) ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						g_entities[i].r.ownerNum = g_entities_dimension[i];
					}
				}
			}
		}
		// Final check
		else {
			int i;
			if (g_entities[entityNum].inuse) {
				const int saberOwner = g_entities[entityNum].r.ownerNum;
				if (g_entities[saberOwner].client &&
					((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN)) ||
					((jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) )
				{
					return;
				}
			}
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse && g_entities[i].client && (g_entities[i].client->ps.eFlags & JK_PASS_THROUGH) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN)) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_CHAT)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_CHAT_IN)) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_RACE)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_RACE_IN)) ) 
					{
						g_entities[i].r.ownerNum = g_entities_dimension[i];
					}
				}
			}
		}
	}
}

/*
=====================================================================
Dimensions trace functions
=====================================================================
*/
void JKMod_Dimensions(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask)
{
	// Start trace parse
	trap_Trace_Start(passEntityNum);

	// Continue normal trace
	trap_Trace(results, start, mins, maxs, end, passEntityNum, contentmask);

	// End clean up
	trap_Trace_End(passEntityNum);
}
