/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Main emotes functions
=====================================================================
*/

#include "../../code/game/g_local.h"	// Original header
#include "../../code/game/bg_local.h"	// Get PM_ stuff

// Extern stuff
extern void StandardSetBodyAnim(gentity_t *self, int anim, int flags);
extern qboolean SaberAttacking(gentity_t *self);

/*
=====================================================================
Emote table list
=====================================================================
*/
jkmod_emotes_data_t JKModEmotesData[EMOTE_NUM_EMOTES] =
{
	// Emote				Command			Special		Start animation				End animation				Compatible all	Toggle saber
	{ EMOTE_BAR,			"bar",			qfalse,		BOTH_BARTENDER_IDLE1,		-1,							qtrue,			qfalse },
	{ EMOTE_BEG,			"beg",			qtrue,		BOTH_KNEES1,				-1,							qtrue,			qfalse },
	{ EMOTE_BURIED,			"buried",		qfalse,		BOTH_DISMEMBER_RARM,		BOTH_FORCE_GETUP_B1,		qtrue,			qfalse },
	{ EMOTE_COCKY,			"cocky",		qfalse,		BOTH_TALKGESTURE6,			BOTH_TALKGESTURE6STOP,		qfalse,			qfalse },
	{ EMOTE_COMEON,			"comeon",		qfalse,		BOTH_COME_ON1,				-1,							qtrue,			qfalse },
	{ EMOTE_COMTALK,		"comtalk",		qtrue,		BOTH_TALKCOMM1,				BOTH_TALKCOMM1STOP,			qfalse,			qfalse },
	{ EMOTE_CONVULSION,		"convulsion",	qtrue,		BOTH_PAIN20,				BOTH_PAIN20,				qtrue,			qfalse },
	{ EMOTE_CROSSARMS,		"crossarms",	qtrue,		BOTH_TALKGESTURE11START,	BOTH_TALKGESTURE11STOP,		qfalse,			qfalse },
	{ EMOTE_DONTKILLME,		"dontkillme",	qfalse,		BOTH_BARTENDER_COWERLOOP,	BOTH_FORCE_GETUP_B1,		qtrue,			qfalse },
	{ EMOTE_DONTKNOW,		"dontknow",		qfalse,		BOTH_TALKGESTURE1,			-1,							qfalse,			qfalse },
	{ EMOTE_DONTKNOW2,		"dontknow2",	qfalse,		BOTH_TALKGESTURE10,			-1,							qfalse,			qfalse },
	{ EMOTE_EXPLAIN,		"explain",		qfalse,		BOTH_TALKGESTURE3,			-1,							qfalse,			qfalse },
	{ EMOTE_EXPLAIN2,		"explain2",		qfalse,		BOTH_TALKGESTURE9,			-1,							qfalse,			qfalse },
	{ EMOTE_FAKEDEAD,		"fakedead",		qfalse,		BOTH_DEATH1,				BOTH_FORCE_GETUP_B1,		qtrue,			qfalse },
	{ EMOTE_FAKEDEAD2,		"fakedead2",	qfalse,		BOTH_RIGHTHANDCHOPPEDOFF,	BOTH_RIGHTHANDCHOPPEDOFF,	qtrue,			qfalse },
	{ EMOTE_FALLING,		"falling",		qfalse,		BOTH_FALLDEATH1INAIR,		BOTH_FALLDEATH1INAIR,		qtrue,			qfalse },
	{ EMOTE_FLIP,			"flip",			qfalse,		BOTH_STAND1TO2,				-1,							qtrue,			qfalse },
	{ EMOTE_GETUP,			"getup",		qfalse,		BOTH_FORCE_GETUP_B6,		-1,							qtrue,			qfalse },
	{ EMOTE_GREET,			"greet",		qfalse,		BOTH_PAUSE1START,			-1,							qtrue,			qfalse },
	{ EMOTE_HANDHIPS,		"handhips",		qtrue,		BOTH_STAND5TOSTAND8,		BOTH_STAND1,				qtrue,			qfalse },
	{ EMOTE_HUG,			"hug",			qfalse,		-1,							-1,							qfalse,			qfalse },
	{ EMOTE_KISS,			"kiss",			qfalse,		-1,							-1,							qfalse,			qfalse },
	{ EMOTE_KNEEL,			"kneel",		qfalse,		BOTH_FORCEHEAL_START,		BOTH_FORCEHEAL_STOP,		qtrue,			qfalse },
	{ EMOTE_LAUGH,			"laugh",		qfalse,		BOTH_LAUGH1STOP,			-1,							qtrue,			qfalse },
	{ EMOTE_LOOK,			"look",			qfalse,		BOTH_TALKGESTURE18,			-1,							qfalse,			qfalse },
	{ EMOTE_LOOK2,			"look2",		qfalse,		BOTH_EXAMINE3,				-1,							qtrue,			qfalse },
	{ EMOTE_NOD,			"nod",			qfalse,		BOTH_HEADNOD,				-1,							qtrue,			qfalse },
	{ EMOTE_POINT,			"point",		qfalse,		BOTH_TALKGESTURE12,			-1,							qfalse,			qfalse },
	{ EMOTE_POINT2,			"point2",		qtrue,		BOTH_TALKGESTURE6START,		BOTH_TALKGESTURE6STOP,		qfalse,			qfalse },
	{ EMOTE_POSING,			"posing",		qtrue,		BOTH_HUGGER1,				BOTH_HUGGERSTOP1,			qfalse,			qfalse },
	{ EMOTE_PUNCH,			"punch",		qtrue,		-1,							-1,							qtrue,			qfalse },
	{ EMOTE_REVERENCE,		"reverence",	qfalse,		BOTH_CROUCH3,				BOTH_CROUCH3,				qtrue,			qfalse },
	{ EMOTE_SIT,			"sit",			qfalse,		BOTH_SIT2,					BOTH_SIT2TOSTAND5,			qtrue,			qfalse },
	{ EMOTE_SIT2,			"sit2",			qfalse,		BOTH_SIT1,					BOTH_STAND1,				qtrue,			qfalse },
	{ EMOTE_SIT3,			"sit3",			qfalse,		BOTH_STAND5TOSIT3,			BOTH_SIT3TOSTAND5,			qtrue,			qfalse },
	{ EMOTE_SHAKE,			"shake",		qfalse,		BOTH_HEADSHAKE,				-1,							qtrue,			qfalse },
	{ EMOTE_SUPER,			"super",		qtrue,		BOTH_FORCEFOUNTAIN1_MIDDLE,	BOTH_STAND1,				qtrue,			qfalse },
	{ EMOTE_SUPER2,			"super2",		qfalse,		BOTH_FORCEFOUNTAIN1_STOP,	-1,							qtrue,			qfalse },
	{ EMOTE_SURRENDER,		"surrender",	qtrue,		TORSO_SURRENDER_START,		BOTH_STAND1,				qtrue,			qfalse },
	{ EMOTE_SPIN,			"spin",			qtrue,		BOTH_FJSS_TL_BR,			-1,							qtrue,			qtrue },
	{ EMOTE_SPIN2,			"spin2",		qtrue,		BOTH_FJSS_TR_BL,			-1,							qtrue,			qtrue },
	{ EMOTE_SPIN3,			"spin3",		qtrue,		BOTH_BUTTERFLY_FR1,			-1,							qtrue,			qfalse },
	{ EMOTE_SPIN4,			"spin4",		qtrue,		BOTH_BUTTERFLY_FL1,			-1,							qtrue,			qfalse },
	{ EMOTE_TAUNT3,			"taunt3",		qfalse,		BOTH_GESTURE1,				-1,							qtrue,			qfalse },
	{ EMOTE_TAUNT4,			"taunt4",		qfalse,		BOTH_SABERKILLER1,			-1,							qtrue,			qtrue },
	{ EMOTE_THINK,			"think",		qfalse,		BOTH_TALKGESTURE5START,		BOTH_TALKGESTURE5START,		qfalse,			qfalse },
	{ EMOTE_THREATEN,		"threaten",		qfalse,		BOTH_THREATEN1,				-1,							qtrue,			qfalse },
	{ EMOTE_THUMBSUP,		"thumbsup",		qfalse,		BOTH_THUMBING1,				-1,							qtrue,			qfalse },
	{ EMOTE_THUMBSDOWN,		"thumbsdown",	qfalse,		BOTH_LEANRIGHT3START,		-1,							qtrue,			qfalse },
	{ EMOTE_TOSSBACK,		"tossback",		qfalse,		BOTH_BARTENDER_THROW1,		-1,							qtrue,			qfalse },
	{ EMOTE_TOSSOVER,		"tossover",		qfalse,		BOTH_SABERTHROW2START,		-1,							qtrue,			qfalse },
	{ EMOTE_TOSSUP,			"tossup",		qfalse,		BOTH_SABERTHROW1START,		-1,							qtrue,			qfalse },
	{ EMOTE_TYPE,			"type",			qfalse,		BOTH_CONSOLE1,				BOTH_CONSOLE1STOP,			qtrue,			qfalse },
	{ EMOTE_TYPE2,			"type2",		qfalse,		BOTH_CONSOLE2,				BOTH_STAND1,				qtrue,			qfalse },
	{ EMOTE_VICTORY,		"victory",		qfalse,		BOTH_SABERTHROW1STOP,		-1,							qtrue,			qfalse },
	{ EMOTE_VICTORY2,		"victory2",		qfalse,		BOTH_TALKGESTURE19START,	-1,							qfalse,			qfalse },
	{ EMOTE_WAITING,		"waiting",		qtrue,		BOTH_STAND4,				BOTH_STAND1,				qtrue,			qfalse },
	{ EMOTE_WATCHOUT,		"watchout",		qfalse,		BOTH_TALKGESTURE18,			-1,							qfalse,			qfalse },
	{ EMOTE_WRITING,		"writing",		qfalse,		BOTH_SILENCEGESTURE1,		BOTH_STAND1,				qtrue,			qtrue },
	{ EMOTE_WRITING2,		"writing2",		qfalse,		BOTH_UNCROUCH3,				BOTH_STAND1,				qtrue,			qtrue },
};

int JKModEmotesDataSize = ARRAY_LEN(JKModEmotesData);

/*
=====================================================================
Emote command check
=====================================================================
*/
int JKMod_EmoteCheck(char *cmd, gentity_t *ent)
{
	int	i;

	// Now start on the checks for each command possibility
	for (i = 0; i < JKModEmotesDataSize; i++)
	{
		if (!Q_stricmp(cmd, JKModEmotesData[i].cmd) || !Q_stricmp(cmd, va("am%s", JKModEmotesData[i].cmd)))
		{
			return JKMod_EmotePlay(ent, JKModEmotesData[i].emoteIndex);
		}
	}

	// It does not seem to be picked up, ignore it
	return 0;
}

/*
=====================================================================
Emote check in list
=====================================================================
*/
int JKMod_EmoteIn(gentity_t *ent, int type)
{
	int	i;

	if (ent->client->ps.stats[JK_PLAYER] & JK_EMOTE_IN)
	{
		for (i = 0; i < EMOTE_NUM_EMOTES; i++)
		{
			if ((ent->client->ps.torsoAnim & ~ANIM_TOGGLEBIT) == JKModEmotesData[i].startAnim)
			{
				switch (type)
				{
					// Looking for any emote
					case -1:
					default: return 1; break;
					// Looking for freeze emote
					case 0: if (!JKModEmotesData[i].special && JKModEmotesData[i].endAnim != -1 && (ent->client->ps.forceHandExtend == HANDEXTEND_DODGE)) return 1; break;
					// Looking for non-freezing emotes
					case 1: if (!JKModEmotesData[i].special && JKModEmotesData[i].endAnim == -1) return 1; break;
					// Looking for special emote
					case 2: if (JKModEmotesData[i].special && (ent->client->ps.forceHandExtend == HANDEXTEND_TAUNT)) return 1; break;
				}
			}
		}
	}

	return 0;
}

/*
=====================================================================
Emote check leaving out
=====================================================================
*/
int JKMod_EmoteOut(gentity_t* ent)
{
	int	i;

	if (ent->client->ps.stats[JK_PLAYER] & JK_EMOTE_IN)
	{
		for (i = 0; i < EMOTE_NUM_EMOTES; i++)
		{
			if ((ent->client->ps.torsoAnim & ~ANIM_TOGGLEBIT) == JKModEmotesData[i].endAnim && JKModEmotesData[i].endAnim != -1 && JKModEmotesData[i].endAnim != BOTH_STAND1)
			{
				return 1;
			}
		}
	}

	return 0;
}

/*
=====================================================================
Emote launch function
=====================================================================
*/
int JKMod_EmotePlay(gentity_t *ent, int emoteIndex)
{
	qboolean showAlert = qtrue;

	// Check client
	if (!ent || !ent->client) return 0;

	// check alert
	if (ent->client->ps.duelInProgress && ent->client->ps.duelTime >= level.time) showAlert = qfalse;

	// Compatible check
	if (JKModEmotesData[emoteIndex].compatible == qfalse && jk2startversion == VERSION_1_02)
	{
		if (showAlert) trap_SendServerCommand(ent - g_entities, "cp \"Emote not available\"");
		return 0;
	}

	// Exception for duel gametype
	if (g_gametype.integer == GT_TOURNAMENT || g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY)
	{
		if (showAlert) trap_SendServerCommand(ent - g_entities, "cp \"Emotes disabled in this gametype\"");
		return 0;
	}

	// Don't allow in spectator
	if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		if (showAlert) trap_SendServerCommand(ent - g_entities, "cp \"Emotes disabled in spectator mode\"");
		return 0;
	}

	// Don't allow in race dimension
	if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
	{
		if (showAlert) trap_SendServerCommand(ent - g_entities, "cp \"Emotes disabled in this dimension\"");
		return 0;
	}

	// Don't allow in duel
	if (ent->client->ps.duelInProgress && ent->client->ps.duelTime <= level.time)
	{
		if (showAlert) trap_SendServerCommand(ent - g_entities, "cp \"Emotes disabled in private duels\"");
		return 0;
	}

	// Bit values for emotes! Let the people choose!
	if (!(jkcvar_emotesEnabled.integer & (1 << emoteIndex)))
	{
		if (showAlert) trap_SendServerCommand(ent - g_entities, "cp \"Emote disabled by the server\"");
		return 0;
	}

	// Don't allow when you are moving
	if (JKMod_PlayerMoving(ent, qtrue, qtrue) && !JKModEmotesData[emoteIndex].special && !(ent->client->ps.duelTime >= level.time))
	{
		if (showAlert) trap_SendServerCommand(ent - g_entities, "cp \"You can't use this emote while moving\"");
		return 0;
	}

	// You have a weapon but the emote isnt a freezing/walking one, disallow
	if (ent->client->ps.weapon != WP_SABER && !JKModEmotesData[emoteIndex].special && JKModEmotesData[emoteIndex].endAnim == -1)
	{
		return 0;
	}

	// Now that everything goes through here, we have to check if its a hug or a kiss to use their special commands
	if (JKModEmotesData[emoteIndex].startAnim == -1 && jkcvar_emotesEnabled.integer)
	{
		if (Q_stricmp(JKModEmotesData[emoteIndex].cmd, "hug") == 0)
		{
			JKMod_EmoteCmdHug(ent, showAlert);
			return 1;
		}
		else if (Q_stricmp(JKModEmotesData[emoteIndex].cmd, "kiss") == 0)
		{
			JKMod_EmoteCmdKiss(ent, showAlert);
			return 1;
		}
		else if (Q_stricmp(JKModEmotesData[emoteIndex].cmd, "punch") == 0)
		{
			JKMod_EmoteCmdPunch(ent, showAlert);
			return 1;
		}
	}

	// If emotes are disabled, you can still exit an emote
	if (jkcvar_emotesEnabled.integer == 0 && !JKMod_EmoteIn(ent, -1)) return 0;

	// This is a special type of emote
	if (JKModEmotesData[emoteIndex].endAnim != -1)
	{
		// You seem to be airbourne, please stay at the ground
		if (ent->client->ps.groundEntityNum == ENTITYNUM_NONE) return 0;

		ent->client->ps.saberMove = LS_NONE;
		ent->client->ps.saberBlocked = 0;
		ent->client->ps.saberBlocking = 0;

		// Are we already using the emote? Then lets go back to standing up position
		if ((ent->client->ps.legsAnim & ~ANIM_TOGGLEBIT) == JKModEmotesData[emoteIndex].startAnim)
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;

			StandardSetBodyAnim(ent, JKModEmotesData[emoteIndex].endAnim, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
			ent->client->ps.stats[JK_PLAYER] |= JK_EMOTE_IN;
			return 1;
		}
		// We are starting the given animation
		else
		{
			// Toggle on/off lightsaber
			if (ent->client->ps.weapon == WP_SABER && (!ent->client->ps.saberHolstered && !JKModEmotesData[emoteIndex].toggleSaber) || (ent->client->ps.saberHolstered && JKModEmotesData[emoteIndex].toggleSaber))
			{
				Cmd_ToggleSaber_f(ent);
			}

			// Not a walkable emote, just set it being locked and all
			if (!JKModEmotesData[emoteIndex].special)
			{
				ent->client->ps.forceHandExtend = HANDEXTEND_DODGE;
				ent->client->ps.forceDodgeAnim = JKModEmotesData[emoteIndex].startAnim;
				ent->client->ps.forceHandExtendTime = level.time + INFINITE;
			}
			// Allright, we have to walk
			else
			{
				pmove_t		pmv;

				ent->client->ps.forceHandExtend = HANDEXTEND_TAUNT;
				ent->client->ps.forceDodgeAnim = JKModEmotesData[emoteIndex].startAnim;
				ent->client->ps.forceHandExtendTime = level.time + INFINITE;

				memset(&pmv, 0, sizeof(pmv));
				pmv.ps = &ent->client->ps;
				pmv.animations = bgGlobalAnimations;
				pmv.cmd = ent->client->pers.cmd;
				pmv.trace = trap_Trace;
				pmv.pointcontents = trap_PointContents;
				pmv.gametype = g_gametype.integer;

				// Rare exception, you should never really call PM_ utility functions from game or cgame (despite the fact that it's technically possible)
				pm = &pmv;
				PM_SetAnim(SETANIM_LEGS, JKModEmotesData[emoteIndex].startAnim, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS, 100);

				pm->ps->legsTimer = ((pm->ps->ping < 20) ? 20 : pm->ps->ping) * 3.5;
			}
			ent->client->ps.stats[JK_PLAYER] |= JK_EMOTE_IN;
			return 1;
		}
	}
	// Normal type of emote.
	else
	{
		// If you are not allowed emote breaks, you cant do it while being in air / on player
		if (!jkcvar_emotesBreak.value && (ent->client->ps.groundEntityNum == ENTITYNUM_NONE || ent->client->ps.groundEntityNum < MAX_CLIENTS))
		{
			return 0;
		}

		// Toggle on/off lightsaber
		if (ent->client->ps.weapon == WP_SABER && (!ent->client->ps.saberHolstered && !JKModEmotesData[emoteIndex].toggleSaber) || (ent->client->ps.saberHolstered && JKModEmotesData[emoteIndex].toggleSaber))
		{
			Cmd_ToggleSaber_f(ent);
		}

		// Server wants you to freeze while the emote plays
		if (jkcvar_emotesFreeze.integer && !JKModEmotesData[emoteIndex].special)
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_DODGE;
			ent->client->ps.forceDodgeAnim = JKModEmotesData[emoteIndex].startAnim;
			ent->client->ps.forceHandExtendTime = level.time + (bgGlobalAnimations[JKModEmotesData[emoteIndex].startAnim].numFrames * fabs(bgGlobalAnimations[JKModEmotesData[emoteIndex].startAnim].frameLerp));
		}
		// Do a standard setting of emote
		else
		{
			StandardSetBodyAnim(ent, JKModEmotesData[emoteIndex].startAnim, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
		}
		ent->client->ps.stats[JK_PLAYER] |= JK_EMOTE_IN;
		return 1;
	}
}

/*
=====================================================================
Custom emotes function
=====================================================================
*/

// Hug emote
void JKMod_EmoteCmdHug(gentity_t *ent, qboolean showAlert)
{
	trace_t tr;
	vec3_t fPos;

	if (ent->client->ps.forceRestricted || 
		ent->client->ps.duelInProgress || 
		ent->client->ps.saberInFlight || 
		ent->client->ps.weapon != WP_SABER || 
		BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim) || 
		BG_SaberInSpecialAttack(ent->client->ps.legsAnim))
	{
		return;
	}

	if (!(jkcvar_emotesEnabled.integer & EMOTE_HUG))
	{
		if ((ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND1 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND2 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGER1 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGEE1 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSER1LOOP && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSEE1LOOP)
		{
			return;
		}
	}

	AngleVectors(ent->client->ps.viewangles, fPos, 0, 0);

	fPos[0] = ent->client->ps.origin[0] + fPos[0] * 40;
	fPos[1] = ent->client->ps.origin[1] + fPos[1] * 40;
	fPos[2] = (ent->client->ps.origin[2] + ent->client->ps.viewheight) + fPos[2] * 40;

	trap_Trace(&tr, ent->client->ps.origin, 0, 0, fPos, ent->s.number, ent->clipmask);

	if (tr.entityNum < MAX_CLIENTS && tr.entityNum != ent->s.number)
	{
		gentity_t *other = &g_entities[tr.entityNum];

		if (other && other->inuse && other->client)
		{
			vec3_t entDir;
			vec3_t otherDir;
			vec3_t entAngles;
			vec3_t otherAngles;

			if (JKMod_IgnoreClientCheck(2, other->s.number, ent->s.number))
			{
				trap_SendServerCommand(ent - g_entities, "cp \"He doesn't want a hug\"");
				return;
			}

			if (other->client->ps.forceRestricted || 
				other->client->ps.duelInProgress || 
				other->client->ps.saberInFlight || 
				other->client->ps.weapon != WP_SABER || 
				BG_InRoll(&other->client->ps, other->client->ps.legsAnim) || 
				BG_SaberInSpecialAttack(other->client->ps.legsAnim))
			{
				return;
			}

			if (ent->client->ps.weapon == WP_SABER && !ent->client->ps.saberHolstered) Cmd_ToggleSaber_f(ent);
			if (other->client->ps.weapon == WP_SABER && !other->client->ps.saberHolstered) Cmd_ToggleSaber_f(other);

			if ((ent->client->ps.weapon == WP_SABER && other->client->ps.weapon == WP_SABER) && (ent->client->ps.saberHolstered && other->client->ps.saberHolstered))
			{
				VectorSubtract(other->client->ps.origin, ent->client->ps.origin, otherDir);
				VectorCopy(ent->client->ps.viewangles, entAngles);
				entAngles[YAW] = vectoyaw(otherDir);
				SetClientViewAngle(ent, entAngles);

				StandardSetBodyAnim(ent, BOTH_HUGGER1, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
				ent->client->ps.stats[JK_PLAYER] |= JK_EMOTE_IN;
				ent->client->ps.saberMove = LS_NONE;
				ent->client->ps.saberBlocked = 0;
				ent->client->ps.saberBlocking = 0;

				VectorSubtract(ent->client->ps.origin, other->client->ps.origin, entDir);
				VectorCopy(other->client->ps.viewangles, otherAngles);
				otherAngles[YAW] = vectoyaw(entDir);
				SetClientViewAngle(other, otherAngles);

				StandardSetBodyAnim(other, BOTH_HUGGEE1, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
				other->client->ps.stats[JK_PLAYER] |= JK_EMOTE_IN;
				other->client->ps.saberMove = LS_NONE;
				other->client->ps.saberBlocked = 0;
				other->client->ps.saberBlocking = 0;
			}
		}
	}
	else
	{
		if (showAlert) trap_SendServerCommand(ent - g_entities, "cp \"You need someone in front of you\"");
		return;
	}
}

// Kiss emote
void JKMod_EmoteCmdKiss(gentity_t *ent, qboolean showAlert)
{
	trace_t tr;
	vec3_t fPos;

	if (ent->client->ps.forceRestricted || 
		ent->client->ps.duelInProgress || 
		ent->client->ps.saberInFlight || 
		ent->client->ps.weapon != WP_SABER || 
		BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim) || 
		BG_SaberInSpecialAttack(ent->client->ps.legsAnim))
	{
		return;
	}

	if (jkcvar_emotesEnabled.integer != 2)
	{
		if ((ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND1 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND2 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGER1 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGEE1 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSER1LOOP && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSEE1LOOP)
		{
			return;
		}
	}

	AngleVectors(ent->client->ps.viewangles, fPos, 0, 0);

	fPos[0] = ent->client->ps.origin[0] + fPos[0] * 40;
	fPos[1] = ent->client->ps.origin[1] + fPos[1] * 40;
	fPos[2] = (ent->client->ps.origin[2] + ent->client->ps.viewheight) + fPos[2] * 40;

	trap_Trace(&tr, ent->client->ps.origin, 0, 0, fPos, ent->s.number, ent->clipmask);

	if (tr.entityNum < MAX_CLIENTS && tr.entityNum != ent->s.number)
	{
		gentity_t *other = &g_entities[tr.entityNum];

		if (other && other->inuse && other->client)
		{
			vec3_t entDir;
			vec3_t otherDir;
			vec3_t entAngles;
			vec3_t otherAngles;

			if (JKMod_IgnoreClientCheck(2, other->s.number, ent->s.number))
			{
				trap_SendServerCommand(ent - g_entities, "cp \"He doesn't want a kiss\"");
				return;
			}

			if (other->client->ps.forceRestricted ||
				other->client->ps.duelInProgress ||
				other->client->ps.saberInFlight ||
				other->client->ps.weapon != WP_SABER ||
				BG_InRoll(&other->client->ps, other->client->ps.legsAnim) ||
				BG_SaberInSpecialAttack(other->client->ps.legsAnim))
			{
				return;
			}

			if (ent->client->ps.weapon == WP_SABER && !ent->client->ps.saberHolstered) Cmd_ToggleSaber_f(ent);
			if (other->client->ps.weapon == WP_SABER && !other->client->ps.saberHolstered) Cmd_ToggleSaber_f(other);

			if ((ent->client->ps.weapon == WP_SABER && other->client->ps.weapon == WP_SABER) && (ent->client->ps.saberHolstered && other->client->ps.saberHolstered))
			{
				VectorSubtract(other->client->ps.origin, ent->client->ps.origin, otherDir);
				VectorCopy(ent->client->ps.viewangles, entAngles);
				entAngles[YAW] = vectoyaw(otherDir);
				SetClientViewAngle(ent, entAngles);

				StandardSetBodyAnim(ent, BOTH_KISSER1LOOP, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
				ent->client->ps.stats[JK_PLAYER] |= JK_EMOTE_IN;
				ent->client->ps.saberMove = LS_NONE;
				ent->client->ps.saberBlocked = 0;
				ent->client->ps.saberBlocking = 0;
				VectorSubtract(ent->client->ps.origin, other->client->ps.origin, entDir);

				VectorCopy(other->client->ps.viewangles, otherAngles);
				otherAngles[YAW] = vectoyaw(entDir);
				SetClientViewAngle(other, otherAngles);

				StandardSetBodyAnim(other, BOTH_KISSEE1LOOP, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
				other->client->ps.stats[JK_PLAYER] |= JK_EMOTE_IN;
				other->client->ps.saberMove = LS_NONE;
				other->client->ps.saberBlocked = 0;
				other->client->ps.saberBlocking = 0;
			}
		}
	}
	else
	{
		if (showAlert) trap_SendServerCommand(ent - g_entities, "cp \"You need someone in front of you\"");
		return;
	}
}

// Punch emote
void JKMod_EmoteCmdPunch(gentity_t *ent, qboolean showAlert)
{
	trace_t tr;
	vec3_t fPos;
	int cmd = jk2startversion == VERSION_1_02 ? BOTH_MELEE2 : (JKMod_Rand() % 2 ? BOTH_MELEE1 : BOTH_MELEE2);

	// Special cases to disallow the use of the emote
	if (ent->client->ps.forceRestricted || 
		ent->client->ps.duelInProgress || 
		ent->client->ps.saberInFlight || 
		ent->client->ps.pm_type == PM_SPECTATOR || 
		ent->client->ps.pm_type == PM_DEAD)
	{
		return;
	}

	if (jkcvar_emotesEnabled.integer != 2)
	{
		if ((ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND1 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND2 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGER1 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGEE1 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSER1LOOP && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSEE1LOOP && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_PAIN20 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_MELEE2 && 
			(ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_MELEE1)
		{
			return;
		}
	}

	if (ent->client->ps.weapon == WP_SABER && !ent->client->ps.saberHolstered) Cmd_ToggleSaber_f(ent);

	StandardSetBodyAnim(ent, cmd, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
	ent->client->ps.stats[JK_PLAYER] |= JK_EMOTE_IN;
	ent->client->ps.saberMove = LS_NONE;
	ent->client->ps.saberBlocked = 0;
	ent->client->ps.saberBlocking = 0;

	AngleVectors(ent->client->ps.viewangles, fPos, 0, 0);

	fPos[0] = ent->client->ps.origin[0] + fPos[0] * 40;
	fPos[1] = ent->client->ps.origin[1] + fPos[1] * 40;
	fPos[2] = (ent->client->ps.origin[2] + ent->client->ps.viewheight) + fPos[2] * 40;

	trap_Trace(&tr, ent->client->ps.origin, 0, 0, fPos, ent->s.number, ent->clipmask);

	if (tr.entityNum < MAX_CLIENTS && tr.entityNum != ent->s.number)
	{
		gentity_t *other = &g_entities[tr.entityNum];

		if (JKMod_IgnoreClientCheck(2, other->s.number, ent->s.number))
		{
			if (jkcvar_emotesPunchDamage.integer) {
				G_Damage(ent, ent, ent, NULL, NULL, jkcvar_emotesPunchDamage.integer, DAMAGE_NO_ARMOR, MOD_MELEE);
				if (ent->client->ps.pm_type != PM_DEAD) {
					ent->client->ps.forceHandExtend = HANDEXTEND_KNOCKDOWN;
					ent->client->ps.forceDodgeAnim = 0;
					ent->client->ps.forceHandExtendTime = level.time + 700;
					ent->client->ps.quickerGetup = qtrue;
				}
				trap_SendServerCommand(ent - g_entities, "cp \"You got what you deserve\"");
			} else {
				trap_SendServerCommand(ent - g_entities, "cp \"He is immune to you\"");
			}
			return;
		}

		if (other->client->ps.forceRestricted || 
			other->client->ps.duelInProgress || 
			BG_SaberInSpecialAttack(other->client->ps.legsAnim))
		{
			return;
		}

		if (other->client->ps.weapon == WP_SABER && !other->client->ps.saberHolstered) Cmd_ToggleSaber_f(other);

		if (other && other->inuse && other->client && !other->client->ps.forceRestricted)
		{
			if (other->client->ps.weapon != WP_SABER) return;
			StandardSetBodyAnim(other, BOTH_PAIN20, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
			other->client->ps.stats[JK_PLAYER] |= JK_EMOTE_IN;
			other->client->ps.saberMove = LS_NONE;
			other->client->ps.saberBlocked = 0;
			other->client->ps.saberBlocking = 0;
			if (jkcvar_emotesPunchDamage.integer) G_Damage(other, ent, ent, NULL, NULL, jkcvar_emotesPunchDamage.integer, DAMAGE_NO_ARMOR, MOD_MELEE);
		}
	}
}
