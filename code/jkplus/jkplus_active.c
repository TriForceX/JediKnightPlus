/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Client active module
=====================================================================
*/

#include "../game/g_local.h" // Original header

/*
=====================================================================
Client think real function
=====================================================================
*/

void JKPlus_ClientThink_real(gentity_t *ent)
{
	// Stuff here...

	// Launch original client think real function
	BaseJK2_ClientThink_real(ent);
}
