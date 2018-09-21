/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Main emotes functions
=====================================================================
*/

#include "../game/g_local.h"	// Original header
#include "../game/bg_local.h"	// Get PM_ stuff

/*
=====================================================================
Emote list
=====================================================================
*/

typedef enum {
	EMOTE_BAR = 0,
	EMOTE_BEG,
	EMOTE_COCKY,
	EMOTE_COMEON,
	EMOTE_COMTALK,
	EMOTE_CROSSARMS,
	EMOTE_DONTKILLME,
	EMOTE_DONTKNOW,
	EMOTE_DONTKNOW2,
	EMOTE_EXPLAIN,
	EMOTE_EXPLAIN2,
	EMOTE_FAKEDEAD,
	EMOTE_FLIP,
	EMOTE_HANDHIPS,
	EMOTE_HUG,
	EMOTE_KISS,
	EMOTE_KNEEL,
	EMOTE_LAUGH,
	EMOTE_LOOKAROUND,
	EMOTE_LOOKAROUND2,
	EMOTE_NOD,
	EMOTE_POINT,
	EMOTE_POINT2,
	EMOTE_PUNCH,
	EMOTE_PUNCH2,
	EMOTE_SIT,
	EMOTE_SIT2,
	EMOTE_SIT3,
	EMOTE_SHAKE,
	EMOTE_SUPER,
	EMOTE_SUPER2,
	EMOTE_SURRENDER,
	EMOTE_SPIN,
	EMOTE_SPINR,
	EMOTE_SPIN2,
	EMOTE_SPIN2R,
	EMOTE_TAUNT2,
	EMOTE_TAUNT3,
	EMOTE_THINK,
	EMOTE_THREATEN,
	EMOTE_THUMBSUP,
	EMOTE_THUMBSDOWN,
	EMOTE_TOSSBACK,
	EMOTE_TOSSOVER,
	EMOTE_TOSSUP,
	EMOTE_TYPE,
	EMOTE_TYPE2,
	EMOTE_VICTORY,
	EMOTE_WAITING,
	EMOTE_WATCHOUT,
	EMOTE_WAVE,
	EMOTE_WAVE2,
	EMOTE_WRITE,
	EMOTE_WRITE2,

	EMOTE_NUM_EMOTES

} emotes_t;

typedef struct {

	emotes_t	emoteIndex;	// Emote number
	char		*cmd;		// Command name
	qboolean	special;	// Special emote? (Walkable)
	int			startAnim;	// Beginning animation
	int			endAnim;	// Ending animation, -1 for no ending animation meaning the emote shouldn't be frozen

} emoteData_t;

/*
=====================================================================
Emote table list
=====================================================================
*/

emoteData_t emotes[EMOTE_NUM_EMOTES] =
{
	// Emote Constant		Command			Special?	Start Animation				End Animation
	{ EMOTE_BAR,			"bar",			qfalse,		BOTH_BARTENDER_IDLE1,		-1 },
	{ EMOTE_BEG,			"beg",			qfalse,		BOTH_KNEES1,				-1 },
	{ EMOTE_COCKY,			"cocky",		qfalse,		645,						646 },
	{ EMOTE_COMEON,			"comeon",		qfalse,		712,						-1 },
	{ EMOTE_COMTALK,		"comtalk",		qtrue,		BOTH_TALKCOMM1,				BOTH_TALKCOMM1STOP },
	{ EMOTE_CROSSARMS,		"crossarms",	qtrue,		655,						656 },
	{ EMOTE_DONTKILLME,		"dontkillme",	qfalse,		BOTH_BARTENDER_COWERLOOP,	656 },
	{ EMOTE_DONTKNOW,		"dontknow",		qfalse,		635,						-1 },
	{ EMOTE_DONTKNOW2,		"dontknow2",	qfalse,		BOTH_TALKGESTURE10,			-1 },
	{ EMOTE_EXPLAIN,		"explain",		qfalse,		637,						-1 },
	{ EMOTE_EXPLAIN2,		"explain2",		qfalse,		653,						-1 },
	{ EMOTE_FAKEDEAD,		"fakedead",		qfalse,		1,							BOTH_FORCE_GETUP_B1 },
	{ EMOTE_FLIP,			"flip",			qfalse,		587,						-1 },
	{ EMOTE_HANDHIPS,		"handhips",		qtrue,		BOTH_STAND5TOSTAND8,		BOTH_STAND8TOSTAND5 },
	{ EMOTE_HUG,			"hug",			qfalse,		-1,							-1 },
	{ EMOTE_KISS,			"kiss",			qfalse,		-1,							-1 },
	{ EMOTE_KNEEL,			"kneel",		qfalse,		1049,						1050 },
	{ EMOTE_LAUGH,			"laugh",		qfalse,		733,						-1 },
	{ EMOTE_LOOKAROUND,		"lookaround",	qfalse,		572,						-1 },
	{ EMOTE_LOOKAROUND2,	"lookaround2",	qfalse,		581,						-1 },
	{ EMOTE_NOD,			"nod",			qfalse,		BOTH_HEADNOD,				-1 },
	{ EMOTE_POINT,			"point",		qfalse,		657,						-1 },
	{ EMOTE_POINT2,			"point2",		qfalse,		644,						-1 },
	{ EMOTE_PUNCH,			"punch",		qtrue,		-1,							-1 },
	{ EMOTE_PUNCH2,			"punch2",		qtrue,		-1,							-1 },
	// Using 0 for end anim because its a random anim, and -1 = no return but sit does have a return...
	{ EMOTE_SIT,			"sit",			qfalse,		BOTH_SIT2,					782 },
	{ EMOTE_SIT2,			"sit2",			qfalse,		BOTH_SIT1,					1090 },
	{ EMOTE_SIT3,			"sit3",			qfalse,		595,						787 },
	{ EMOTE_SHAKE,			"shake",		qfalse,		BOTH_HEADSHAKE,				-1 },
	{ EMOTE_SUPER,			"super",		qfalse,		708,						BOTH_CONSOLE2HOLDCOMSTOP },
	{ EMOTE_SUPER2,			"super2",		qfalse,		710,						-1 },
	{ EMOTE_SURRENDER,		"surrender",	qtrue,		TORSO_SURRENDER_START,		TORSO_SURRENDER_STOP },
	{ EMOTE_SPIN,			"spin",			qfalse,		949,						-1 },
	{ EMOTE_SPINR,			"spinR",		qfalse,		950,						-1 },
	{ EMOTE_SPIN2,			"spin2",		qfalse,		913,						-1 },
	{ EMOTE_SPIN2R,			"spin2R",		qfalse,		956,						-1 },
	{ EMOTE_TAUNT2,			"taunt2",		qfalse,		626,						-1 },
	{ EMOTE_TAUNT3,			"taunt3",		qfalse,		726,						-1 },
	{ EMOTE_THINK,			"think",		qfalse,		641,						656 },
	{ EMOTE_THREATEN,		"threaten",		qfalse,		BOTH_THREATEN1,				-1 },
	{ EMOTE_THUMBSUP,		"thumbsup",		qfalse,		BOTH_THUMBING1,				-1 },
	{ EMOTE_THUMBSDOWN,		"thumbsdown",	qfalse,		704,						-1 },
	{ EMOTE_TOSSBACK,		"tossback",		qfalse,		BOTH_BARTENDER_THROW1,		-1 },
	{ EMOTE_TOSSOVER,		"tossover",		qfalse,		769,						-1 },
	{ EMOTE_TOSSUP,			"tossup",		qfalse,		767,						-1 },
	{ EMOTE_TYPE,			"type",			qfalse,		BOTH_CONSOLE1,				617 },
	{ EMOTE_TYPE2,			"type2",		qfalse,		BOTH_CONSOLE2,				BOTH_STAND1 },
	{ EMOTE_VICTORY,		"victory",		qfalse,		768,						-1 },
	{ EMOTE_WAITING,		"waiting",		qtrue,		BOTH_STAND4,				BOTH_STAND1 },
	{ EMOTE_WATCHOUT,		"watchout",		qfalse,		667,						-1 },
	{ EMOTE_WAVE,			"wave",			qfalse,		668,						-1 },
	{ EMOTE_WAVE2,			"wave2",		qfalse,		674,						-1 },
	{ EMOTE_WRITE,			"write",		qfalse,		715,						BOTH_STAND1 },
	{ EMOTE_WRITE2,			"write2",		qfalse,		804,						BOTH_STAND1 },
};

/*
=====================================================================
Emote command check
=====================================================================
*/

int JKPlus_emoteCheck(char *cmd, gentity_t *ent)
{
	int	i;

	// Now start on the checks for each command possibility
	for(i = 0; i < sizeof(emotes) / sizeof(emotes[0]); i++)
	{
		if(Q_stricmp(cmd, emotes[i].cmd) == 0)
		{
			JKPlus_emoteDo(ent, emotes[i].emoteIndex);
			return 1;
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

int JKPlus_emoteIn(gentity_t *ent, int type)
{
	int	i;

	for(i = 0; i < EMOTE_NUM_EMOTES; i++)
	{

		if((ent->client->ps.torsoAnim & ~ANIM_TOGGLEBIT) == emotes[i].startAnim)
		{
			switch(type)
			{
			case -1: // Looking for any emote
			default:
				return 1;
				break;
			case 0: // Looking for freeze emote
				if(!emotes[i].special && emotes[i].endAnim != -1 &&
					(ent->client->ps.forceHandExtend == HANDEXTEND_DODGE))
				{
					return 1;
				}
				break;
			case 1: // Looking for non-freezing emotes
				if(!emotes[i].special && emotes[i].endAnim == -1)
				{
					return 1;
				}
				break;
			case 2: // Looking for special emote
				if(emotes[i].special &&
					(ent->client->ps.forceHandExtend == HANDEXTEND_TAUNT))
				{
					return 1;
				}
				break;
			}
		}

	}

	// No match, result 0
	return 0;
}

/*
=====================================================================
Emote launch function
=====================================================================
*/
void JKPlus_emoteDo(gentity_t *ent, int emoteIndex)
{
	int	cmd;

	// Exception for duel gametype
	if(g_gametype.integer == GT_TOURNAMENT)
	{
		trap_SendServerCommand(ent - g_entities, "print \"Emotes are disabled in this gametype.\n\"");
		return;
	}

	// Exception for when you are moving
	if(ent->client->ps.velocity[0] != 0 || ent->client->ps.velocity[1] != 0 || ent->client->ps.velocity[2] != 0)
	{
		trap_SendServerCommand(ent - g_entities, "print \"Please stop moving before you attempt an emote.\n\"");
		return;
	}

	// Bit values for emotes! Let the people choose!
	if(!(jkplus_emotesEnabled.integer & (1 << emoteIndex)))
	{
		trap_SendServerCommand(ent - g_entities, "print \"This emote has been disabled by the server.\n\"");
		return;
	}

	// Invalid conditions to start an emote
	if(ent->client->ps.duelInProgress
		|| BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim)
		|| ent->client->ps.saberInFlight
		|| ent->client->ps.forceHandExtend == HANDEXTEND_KNOCKDOWN)
	{
		return;
	}

	// When you have a saber and are in attack, disallow
	if(ent->client->ps.weapon == WP_SABER && SaberAttacking(ent))
	{
		return;
	}

	// You have a weapon but the emote isnt a freezing/walking one, disallow
	if(ent->client->ps.weapon != WP_SABER
		&& !emotes[emoteIndex].special			// Its not walkable
		&& emotes[emoteIndex].endAnim == -1)	// Its not freezing
	{
		return;
	}

	// Now that everything goes through here, we have to check if its a hug or a kiss to use their special commands
	if(emotes[emoteIndex].startAnim == -1)
	{
		if (Q_stricmp(emotes[emoteIndex].cmd, "hug") == 0)
		{
			JKPlus_emoteDoHug(ent);
			return;
		}
		else if (Q_stricmp(emotes[emoteIndex].cmd, "kiss") == 0)
		{
			JKPlus_emoteDoKiss(ent);
			return;
		}
		else if (Q_stricmp(emotes[emoteIndex].cmd, "punch") == 0)
		{
			JKPlus_emoteDoPunch(ent, BOTH_MELEE2);
			return;
		}
		else if (Q_stricmp(emotes[emoteIndex].cmd, "punch2") == 0)
		{
			JKPlus_emoteDoPunch(ent, BOTH_MELEE1);
			return;
		}
	}

	// If emotes are disabled, you can still exit an emote
	if(jkplus_emotesEnabled.integer == 0 && !JKPlus_emoteIn(ent, -1))
	{
		return;
	}

	// This is a special type of emote
	if(emotes[emoteIndex].endAnim != -1)
	{
		// You seem to be airbourne, please stay at the ground
		if(ent->client->ps.groundEntityNum == ENTITYNUM_NONE)
		{
			return;
		}

		ent->client->ps.saberMove = LS_NONE;
		ent->client->ps.saberBlocked = 0;
		ent->client->ps.saberBlocking = 0;

		// Are we already using the emote? Then lets go back to standing up position
		if((ent->client->ps.legsAnim & ~ANIM_TOGGLEBIT) == emotes[emoteIndex].startAnim)
		{
			if (emoteIndex == EMOTE_SIT)
			{
				switch (Q_irand(1, 10))
				{
				case 1:
				case 2:
					cmd = 942;
					break;
				case 3:
				case 4:
				case 5:
					cmd = 943;
					break;
				case 6:
				case 7:
					cmd = BOTH_SIT2TOSTAND5;
					break;
				case 8:
				case 9:
				case 10:
				default:
					cmd = 936;
					break;
				}
			}
			else
			{
				cmd = emotes[emoteIndex].endAnim;
			}

			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;

			StandardSetBodyAnim(ent, emotes[emoteIndex].endAnim, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
		}
		// We are starting the given animation
		else
		{

			// Switch saber cases for write emotes
			if((!ent->client->ps.saberHolstered && emoteIndex != EMOTE_WRITE && emoteIndex != EMOTE_WRITE2) ||
				ent->client->ps.saberHolstered && (emoteIndex == EMOTE_WRITE || emoteIndex == EMOTE_WRITE2))
			{
				Cmd_ToggleSaber_f(ent);
			}

			// Not a walkable emote, just set it being locked and all
			if(!emotes[emoteIndex].special)
			{
				ent->client->ps.forceHandExtend = HANDEXTEND_DODGE;
				ent->client->ps.forceDodgeAnim = emotes[emoteIndex].startAnim;
				ent->client->ps.forceHandExtendTime = level.time + INFINITE;
			}
			// Allright, we have to walk
			else
			{
				pmove_t		pmv;

				ent->client->ps.forceHandExtend = HANDEXTEND_TAUNT;
				ent->client->ps.forceDodgeAnim = emotes[emoteIndex].startAnim;
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
				PM_SetAnim(SETANIM_LEGS, emotes[emoteIndex].startAnim, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS, 100);

				pm->ps->legsTimer = ((pm->ps->ping < 20) ? 20 : pm->ps->ping) * 3.5;
			}
		}
	}
	// Normal type of emote.
	else
	{
		// If you are not allowed emote breaks, you cant do it while being in air / on player
		if(!jkplus_emotesBreak.value &&
			(ent->client->ps.groundEntityNum == ENTITYNUM_NONE ||
			ent->client->ps.groundEntityNum < MAX_CLIENTS))
		{
			return;
		}

		// Server wants you to freeze while the emote plays
		if(jkplus_emotesFreeze.integer)
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_DODGE;
			ent->client->ps.forceDodgeAnim = emotes[emoteIndex].startAnim;
			ent->client->ps.forceHandExtendTime = level.time + (bgGlobalAnimations[emotes[emoteIndex].startAnim].numFrames * fabs(bgGlobalAnimations[emotes[emoteIndex].startAnim].frameLerp));
		}
		// Do a standard setting of emote.
		else
		{
			StandardSetBodyAnim(ent, emotes[emoteIndex].startAnim, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
		}
	}
}

/*
=====================================================================
Emote hug function
=====================================================================
*/
void JKPlus_emoteDoHug(gentity_t *ent)
{
	trace_t tr;
	vec3_t fPos;

	// Check JK2 version
	if(jk2gameplay == VERSION_1_02)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't use this emote in JK2 1.02 version.\n\"");
		return;
	}

	if(ent->client->ps.forceRestricted
		|| ent->client->ps.duelInProgress
		|| ent->client->ps.saberInFlight
		|| ent->client->ps.weapon != WP_SABER
		|| !jkplus_emotesEnabled.integer
		|| BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim)
		|| BG_SaberInSpecialAttack(ent->client->ps.legsAnim))
		return;

	if(!(jkplus_emotesEnabled.integer & EMOTE_HUG))
	{
		if ((ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND2
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGER1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGEE1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSER1LOOP
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSEE1LOOP)
			return;
	}

	AngleVectors(ent->client->ps.viewangles, fPos, 0, 0);

	fPos[0] = ent->client->ps.origin[0] + fPos[0] * 40;
	fPos[1] = ent->client->ps.origin[1] + fPos[1] * 40;
	fPos[2] = (ent->client->ps.origin[2] + ent->client->ps.viewheight) + fPos[2] * 40;

	trap_Trace(&tr, ent->client->ps.origin, 0, 0, fPos, ent->s.number, ent->clipmask);

	if(tr.entityNum < MAX_CLIENTS && tr.entityNum != ent->s.number)
	{
		gentity_t *other = &g_entities[tr.entityNum];
		if(other && other->inuse && other->client)
		{
			vec3_t entDir;
			vec3_t otherDir;
			vec3_t entAngles;
			vec3_t otherAngles;

			if(other->client->ps.forceRestricted
				|| other->client->ps.duelInProgress
				|| other->client->ps.saberInFlight
				|| other->client->ps.weapon != WP_SABER
				|| BG_InRoll(&other->client->ps, other->client->ps.legsAnim)
				|| BG_SaberInSpecialAttack(other->client->ps.legsAnim))
				return;

			if (ent->client->ps.weapon == WP_SABER && !ent->client->ps.saberHolstered) Cmd_ToggleSaber_f(ent);
			if (other->client->ps.weapon == WP_SABER && !other->client->ps.saberHolstered) Cmd_ToggleSaber_f(other);

			if((ent->client->ps.weapon == WP_SABER && other->client->ps.weapon == WP_SABER) && (ent->client->ps.saberHolstered && other->client->ps.saberHolstered))
			{
				VectorSubtract(other->client->ps.origin, ent->client->ps.origin, otherDir);
				VectorCopy(ent->client->ps.viewangles, entAngles);
				entAngles[YAW] = vectoyaw(otherDir);
				SetClientViewAngle(ent, entAngles);

				// Server wants you to freeze while the emote plays
				if (jkplus_emotesFreeze.integer)
				{
					ent->client->ps.forceHandExtend = HANDEXTEND_DODGE;
					ent->client->ps.forceDodgeAnim = BOTH_HUGGER1;
					ent->client->ps.forceHandExtendTime = level.time + (bgGlobalAnimations[BOTH_HUGGER1].numFrames * fabs(bgGlobalAnimations[BOTH_HUGGER1].frameLerp));
				}
				else
				{
					StandardSetBodyAnim(ent, BOTH_HUGGER1, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
				}

				ent->client->ps.saberMove = LS_NONE;
				ent->client->ps.saberBlocked = 0;
				ent->client->ps.saberBlocking = 0;

				VectorSubtract(ent->client->ps.origin, other->client->ps.origin, entDir);
				VectorCopy(other->client->ps.viewangles, otherAngles);
				otherAngles[YAW] = vectoyaw(entDir);
				SetClientViewAngle(other, otherAngles);

				// Server wants you to freeze while the emote plays
				if (jkplus_emotesFreeze.integer)
				{
					other->client->ps.forceHandExtend = HANDEXTEND_DODGE;
					other->client->ps.forceDodgeAnim = BOTH_HUGGEE1;
					other->client->ps.forceHandExtendTime = level.time + (bgGlobalAnimations[BOTH_HUGGEE1].numFrames * fabs(bgGlobalAnimations[BOTH_HUGGEE1].frameLerp));
				}
				else
				{
					StandardSetBodyAnim(other, BOTH_HUGGEE1, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
				}

				other->client->ps.saberMove = LS_NONE;
				other->client->ps.saberBlocked = 0;
				other->client->ps.saberBlocking = 0;
			}
		}
	}
}

/*
=====================================================================
Emote kiss function
=====================================================================
*/
void JKPlus_emoteDoKiss(gentity_t *ent)
{
	trace_t tr;
	vec3_t fPos;

	// Check JK2 version
	if(jk2gameplay == VERSION_1_02)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't use this emote in JK2 1.02 version.\n\"");
		return;
	}

	if(ent->client->ps.forceRestricted
		|| ent->client->ps.duelInProgress
		|| ent->client->ps.saberInFlight
		|| ent->client->ps.weapon != WP_SABER
		|| jkplus_emotesEnabled.integer == 0
		|| BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim)
		|| BG_SaberInSpecialAttack(ent->client->ps.legsAnim))
	{
		return;
	}

	if (jkplus_emotesEnabled.integer != 2)
	{
		if ((ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND2
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGER1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGEE1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSER1LOOP
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSEE1LOOP)
			return;
	}

	AngleVectors(ent->client->ps.viewangles, fPos, 0, 0);

	fPos[0] = ent->client->ps.origin[0] + fPos[0] * 40;
	fPos[1] = ent->client->ps.origin[1] + fPos[1] * 40;
	fPos[2] = (ent->client->ps.origin[2] + ent->client->ps.viewheight) + fPos[2] * 40;

	trap_Trace(&tr, ent->client->ps.origin, 0, 0, fPos, ent->s.number, ent->clipmask);

	if (tr.entityNum < MAX_CLIENTS && tr.entityNum != ent->s.number)
	{
		gentity_t *other = &g_entities[tr.entityNum];

		if (other && other->inuse && other->client && !other->client->ps.duelInProgress)
		{
			vec3_t entDir;
			vec3_t otherDir;
			vec3_t entAngles;
			vec3_t otherAngles;

			if(other->client->ps.forceRestricted
				|| other->client->ps.duelInProgress
				|| other->client->ps.saberInFlight
				|| other->client->ps.weapon != WP_SABER
				|| BG_InRoll(&other->client->ps, other->client->ps.legsAnim)
				|| BG_SaberInSpecialAttack(other->client->ps.legsAnim))
				return;

			if (ent->client->ps.weapon == WP_SABER && !ent->client->ps.saberHolstered) Cmd_ToggleSaber_f(ent);
			if (other->client->ps.weapon == WP_SABER && !other->client->ps.saberHolstered) Cmd_ToggleSaber_f(other);

			if ((ent->client->ps.weapon == WP_SABER && other->client->ps.weapon == WP_SABER) && (ent->client->ps.saberHolstered && other->client->ps.saberHolstered))
			{
				VectorSubtract(other->client->ps.origin, ent->client->ps.origin, otherDir);
				VectorCopy(ent->client->ps.viewangles, entAngles);
				entAngles[YAW] = vectoyaw(otherDir);
				SetClientViewAngle(ent, entAngles);

				// Server wants you to freeze while the emote plays
				if (jkplus_emotesFreeze.integer)
				{
					ent->client->ps.forceHandExtend = HANDEXTEND_DODGE;
					ent->client->ps.forceDodgeAnim = BOTH_KISSER1LOOP;
					ent->client->ps.forceHandExtendTime = level.time + (bgGlobalAnimations[BOTH_KISSER1LOOP].numFrames * fabs(bgGlobalAnimations[BOTH_KISSER1LOOP].frameLerp));
				}
				else
				{
					StandardSetBodyAnim(ent, BOTH_KISSER1LOOP, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
				}

				ent->client->ps.saberMove = LS_NONE;
				ent->client->ps.saberBlocked = 0;
				ent->client->ps.saberBlocking = 0;
				VectorSubtract(ent->client->ps.origin, other->client->ps.origin, entDir);

				VectorCopy(other->client->ps.viewangles, otherAngles);
				otherAngles[YAW] = vectoyaw(entDir);
				SetClientViewAngle(other, otherAngles);

				// Server wants you to freeze while the emote plays
				if (jkplus_emotesFreeze.integer)
				{
					other->client->ps.forceHandExtend = HANDEXTEND_DODGE;
					other->client->ps.forceDodgeAnim = BOTH_KISSEE1LOOP;
					other->client->ps.forceHandExtendTime = level.time + (bgGlobalAnimations[BOTH_KISSEE1LOOP].numFrames * fabs(bgGlobalAnimations[BOTH_KISSEE1LOOP].frameLerp));
				}
				else
				{
					StandardSetBodyAnim(other, BOTH_KISSEE1LOOP, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
				}

				other->client->ps.saberMove = LS_NONE;
				other->client->ps.saberBlocked = 0;
				other->client->ps.saberBlocking = 0;
			}
		}
	}
}

/*
=====================================================================
Emote punch function
=====================================================================
*/
void JKPlus_emoteDoPunch(gentity_t *ent, int cmd)
{
	trace_t tr;
	vec3_t fPos;

	// Special cases to disallow the use of the emote
	if (ent->client->ps.forceRestricted
		|| ent->client->ps.duelInProgress
		|| ent->client->ps.saberInFlight
		|| ent->client->ps.pm_type == PM_SPECTATOR
		|| ent->client->ps.pm_type == PM_DEAD
		|| jkplus_emotesEnabled.integer == 0)
	{
		return;
	}

	if (jkplus_emotesEnabled.integer != 2)
	{
		if ((ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND2
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGER1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGEE1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSER1LOOP
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSEE1LOOP
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != 116
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != 131
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != 130)
		{
			return;
		}
	}

	StandardSetBodyAnim(ent, cmd, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);

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

		if (other->client->ps.forceRestricted
			|| other->client->ps.duelInProgress
			|| BG_SaberInSpecialAttack(other->client->ps.legsAnim)
			|| other->client->ps.duelInProgress)
		{
			return;
		}

		if (other && other->inuse && other->client && !other->client->ps.forceRestricted)
		{
			if (other->client->ps.weapon != WP_SABER) return;
			StandardSetBodyAnim(other, 116, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
			other->client->ps.saberMove = LS_NONE;
			other->client->ps.saberBlocked = 0;
			other->client->ps.saberBlocking = 0;
			if (jkplus_emotesPunchDMG.integer) G_Damage(other, ent, ent, NULL, NULL, jkplus_emotesPunchDMG.integer, DAMAGE_NO_ARMOR, MOD_MELEE);
		}
	}
}
