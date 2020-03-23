/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main combat functions
=====================================================================
*/

#include "../../code/game/g_local.h"	// Original header

/*
=====================================================================
Game damage function
=====================================================================
*/

void JKPlus_G_Damage(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod)
{
	// If valid attacker, check his flags for attacking
	if (attacker - g_entities >= 0 && attacker - g_entities < MAX_CLIENTS)
	{
		if(JKPlus_emoteIn(attacker, -1))
		{
			return;
		}
	}

	// Launch original game damage function
	BaseJK2_G_Damage(targ, inflictor, attacker, dir, point, damage, dflags, mod);
}
