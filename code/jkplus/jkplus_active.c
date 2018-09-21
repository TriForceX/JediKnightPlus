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
	// Already in an emote
	if (JKPlus_emoteIn(ent, -1))
	{
		if (ent->client->ps.pm_type == PM_DEAD)
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		else if (JKPlus_emoteIn(ent, 0))
		{ // In a 'frozen' emote
			if (ent->client->pers.cmd.upmove > 0)
			{
				ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
				ent->client->ps.forceDodgeAnim = 0;
				ent->client->ps.forceHandExtendTime = 0;
			}
			else
			{
				ent->client->ps.forceHandExtend = HANDEXTEND_DODGE;
				ent->client->ps.forceDodgeAnim = (ent->client->ps.legsAnim & ~ANIM_TOGGLEBIT);
				ent->client->ps.forceHandExtendTime = level.time + INFINITE;
			}
		}
		else if (JKPlus_emoteIn(ent, 1) && ent->client->pers.cmd.upmove > 0)
		{ // In an animation emote
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		else if (JKPlus_emoteIn(ent, 2))
		{ // In a special emote (walkable)
			ent->client->ps.forceHandExtend = HANDEXTEND_TAUNT;
			ent->client->ps.forceDodgeAnim = (ent->client->ps.torsoAnim & ~ANIM_TOGGLEBIT);
			ent->client->ps.forceHandExtendTime = level.time + INFINITE;
		}

		// Keep the client informed about to predict the emote leg timers
		if (!(ent->client->ps.eFlags & EF_IN_EMOTE))
		{
			ent->client->ps.eFlags |= EF_IN_EMOTE;
		}
	}
	else if (ent->client->ps.eFlags & EF_IN_EMOTE)
	{
		ent->client->ps.eFlags &= ~EF_IN_EMOTE;
	}

	// Launch original client think real function
	BaseJK2_ClientThink_real(ent);
}
