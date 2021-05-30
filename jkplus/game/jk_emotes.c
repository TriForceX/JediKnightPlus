/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main emotes functions
=====================================================================
*/

#include "../../code/game/g_local.h"	// Original header
#include "../../code/game/bg_local.h"	// Get PM_ stuff

// Extern stuff
extern void StandardSetBodyAnim(gentity_t *self, int anim, int flags);

/*
=====================================================================
Emote list
=====================================================================
*/

typedef enum {
	EMOTE_BAR = 0,
	EMOTE_BEG,
	EMOTE_BURIED,
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
	EMOTE_HI,
	EMOTE_HUG,
	EMOTE_KISS,
	EMOTE_KNEEL,
	EMOTE_LAUGH,
	EMOTE_LOOK,
	EMOTE_LOOK2,
	EMOTE_NOD,
	EMOTE_POINT,
	EMOTE_POINT2,
	EMOTE_PUNCH,
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
	EMOTE_VICTORY2,
	EMOTE_WAITING,
	EMOTE_WATCHOUT,
	EMOTE_WRITING,
	EMOTE_WRITING2,

	EMOTE_NUM_EMOTES

} emotes_t;

typedef struct {

	emotes_t	emoteIndex;	// Emote number
	char		*cmd;		// Command name
	qboolean	special;	// Is a Walkable emote
	int			startAnim;	// Beginning animation
	int			endAnim;	// Ending animation, -1 for no ending animation meaning the emote shouldn't be frozen
	qboolean	compatible;	// It support all game versions

} emoteData_t;

/*
=====================================================================
Emote table list
=====================================================================
*/

emoteData_t emotes[EMOTE_NUM_EMOTES] =
{
	// Emote constant		Command			Special?	Start animation				End animation				Compatible all?
	{ EMOTE_BAR,			"bar",			qfalse,		BOTH_BARTENDER_IDLE1,		-1,							qtrue },
	{ EMOTE_BEG,			"beg",			qtrue,		BOTH_KNEES1,				-1,							qtrue },
	{ EMOTE_BURIED,			"buried",		qfalse,		BOTH_DISMEMBER_RARM,		BOTH_FORCE_GETUP_B1,		qtrue },
	{ EMOTE_COCKY,			"cocky",		qfalse,		BOTH_TALKGESTURE6,			BOTH_TALKGESTURE6STOP,		qfalse },
	{ EMOTE_COMEON,			"comeon",		qfalse,		BOTH_COME_ON1,				-1,							qtrue },
	{ EMOTE_COMTALK,		"comtalk",		qtrue,		BOTH_TALKCOMM1,				BOTH_TALKCOMM1STOP,			qfalse },
	{ EMOTE_CROSSARMS,		"crossarms",	qtrue,		BOTH_TALKGESTURE11START,	BOTH_TALKGESTURE11STOP,		qfalse },
	{ EMOTE_DONTKILLME,		"dontkillme",	qfalse,		BOTH_BARTENDER_COWERLOOP,	BOTH_FORCE_GETUP_B1,		qtrue },
	{ EMOTE_DONTKNOW,		"dontknow",		qfalse,		BOTH_TALKGESTURE1,			-1,							qfalse },
	{ EMOTE_DONTKNOW2,		"dontknow2",	qfalse,		BOTH_TALKGESTURE10,			-1,							qfalse },
	{ EMOTE_EXPLAIN,		"explain",		qfalse,		BOTH_TALKGESTURE3,			-1,							qfalse },
	{ EMOTE_EXPLAIN2,		"explain2",		qfalse,		BOTH_TALKGESTURE9,			-1,							qfalse },
	{ EMOTE_FAKEDEAD,		"fakedead",		qfalse,		BOTH_DEATH1,				BOTH_FORCE_GETUP_B1,		qtrue },
	{ EMOTE_FLIP,			"flip",			qfalse,		BOTH_STAND1TO2,				-1,							qtrue },
	{ EMOTE_HANDHIPS,		"handhips",		qtrue,		BOTH_STAND5TOSTAND8,		BOTH_STAND1,				qtrue },
	{ EMOTE_HI,				"hi",			qfalse,		BOTH_PAUSE1START,			-1,							qtrue },
	{ EMOTE_HUG,			"hug",			qfalse,		-1,							-1,							qfalse },
	{ EMOTE_KISS,			"kiss",			qfalse,		-1,							-1,							qfalse },
	{ EMOTE_KNEEL,			"kneel",		qfalse,		BOTH_FORCEHEAL_START,		BOTH_FORCEHEAL_STOP,		qtrue },
	{ EMOTE_LAUGH,			"laugh",		qfalse,		BOTH_LAUGH1STOP,			-1,							qtrue },
	{ EMOTE_LOOK,			"look",			qfalse,		BOTH_TALKGESTURE18,			-1,							qfalse },
	{ EMOTE_LOOK2,			"look2",		qfalse,		BOTH_EXAMINE3,				-1,							qtrue },
	{ EMOTE_NOD,			"nod",			qfalse,		BOTH_HEADNOD,				-1,							qtrue },
	{ EMOTE_POINT,			"point",		qfalse,		BOTH_TALKGESTURE12,			-1,							qfalse },
	{ EMOTE_POINT2,			"point2",		qtrue,		BOTH_TALKGESTURE6START,		BOTH_TALKGESTURE6STOP,		qfalse },
	{ EMOTE_PUNCH,			"punch",		qtrue,		-1,							-1,							qtrue },
	{ EMOTE_SIT,			"sit",			qfalse,		BOTH_SIT2,					BOTH_SIT2TOSTAND5,			qtrue },
	{ EMOTE_SIT2,			"sit2",			qfalse,		BOTH_SIT1,					BOTH_STAND1,				qtrue },
	{ EMOTE_SIT3,			"sit3",			qfalse,		BOTH_STAND5TOSIT3,			BOTH_SIT3TOSTAND5,			qtrue },
	{ EMOTE_SHAKE,			"shake",		qfalse,		BOTH_HEADSHAKE,				-1,							qtrue },
	{ EMOTE_SUPER,			"super",		qtrue,		BOTH_FORCEFOUNTAIN1_MIDDLE,	BOTH_STAND1,				qtrue },
	{ EMOTE_SUPER2,			"super2",		qfalse,		BOTH_FORCEFOUNTAIN1_STOP,	-1,							qtrue },
	{ EMOTE_SURRENDER,		"surrender",	qtrue,		TORSO_SURRENDER_START,		BOTH_STAND1,				qtrue },
	{ EMOTE_SPIN,			"spin",			qtrue,		BOTH_FJSS_TL_BR,			-1,							qtrue },
	{ EMOTE_SPINR,			"spin2",		qtrue,		BOTH_FJSS_TR_BL,			-1,							qtrue },
	{ EMOTE_SPIN2,			"spin3",		qtrue,		BOTH_BUTTERFLY_FR1,			-1,							qtrue },
	{ EMOTE_SPIN2R,			"spin4",		qtrue,		BOTH_BUTTERFLY_FL1,			-1,							qtrue },
	{ EMOTE_TAUNT2,			"taunt2",		qfalse,		BOTH_GESTURE1,				-1,							qtrue },
	{ EMOTE_TAUNT3,			"taunt3",		qfalse,		BOTH_SABERKILLER1,			-1,							qtrue },
	{ EMOTE_THINK,			"think",		qfalse,		BOTH_TALKGESTURE5START,		BOTH_TALKGESTURE5START,		qfalse },
	{ EMOTE_THREATEN,		"threaten",		qfalse,		BOTH_THREATEN1,				-1,							qtrue },
	{ EMOTE_THUMBSUP,		"thumbsup",		qfalse,		BOTH_THUMBING1,				-1,							qtrue },
	{ EMOTE_THUMBSDOWN,		"thumbsdown",	qfalse,		BOTH_LEANRIGHT3START,		-1,							qtrue },
	{ EMOTE_TOSSBACK,		"tossback",		qfalse,		BOTH_BARTENDER_THROW1,		-1,							qtrue },
	{ EMOTE_TOSSOVER,		"tossover",		qfalse,		BOTH_SABERTHROW2START,		-1,							qtrue },
	{ EMOTE_TOSSUP,			"tossup",		qfalse,		BOTH_SABERTHROW1START,		-1,							qtrue },
	{ EMOTE_TYPE,			"type",			qfalse,		BOTH_CONSOLE1,				BOTH_CONSOLE1STOP,			qtrue },
	{ EMOTE_TYPE2,			"type2",		qfalse,		BOTH_CONSOLE2,				BOTH_STAND1,				qtrue },
	{ EMOTE_VICTORY,		"victory",		qfalse,		BOTH_SABERTHROW1STOP,		-1,							qtrue },
	{ EMOTE_VICTORY2,		"victory2",		qfalse,		BOTH_TALKGESTURE19START,	-1,							qfalse },
	{ EMOTE_WAITING,		"waiting",		qtrue,		BOTH_STAND4,				BOTH_STAND1,				qtrue },
	{ EMOTE_WATCHOUT,		"watchout",		qfalse,		BOTH_TALKGESTURE18,			-1,							qfalse },
	{ EMOTE_WRITING,		"writing",		qfalse,		BOTH_SILENCEGESTURE1,		BOTH_STAND1,				qtrue },
	{ EMOTE_WRITING2,		"writing2",		qfalse,		BOTH_UNCROUCH3,				BOTH_STAND1,				qtrue },
};

/*
=====================================================================
Emote command check
=====================================================================
*/

int JKMod_emoteCheck(char *cmd, gentity_t *ent)
{
	int	i;

	// Now start on the checks for each command possibility
	for(i = 0; i < sizeof(emotes) / sizeof(emotes[0]); i++)
	{
		if(Q_stricmp(cmd, emotes[i].cmd) == 0 || Q_stricmp (cmd, va("am%s", emotes[i].cmd)) == 0)
		{
			JKMod_emoteDo(ent, emotes[i].emoteIndex);
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

int JKMod_emoteIn(gentity_t *ent, int type)
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
					if(emotes[i].special && (ent->client->ps.forceHandExtend == HANDEXTEND_TAUNT))
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
extern qboolean SaberAttacking(gentity_t *self);

void JKMod_emoteDo(gentity_t *ent, int emoteIndex)
{
	int	cmd;

	// Check version compatibility
	if (emotes[emoteIndex].compatible == qfalse && jk2gameplay == VERSION_1_02)
	{
		trap_SendServerCommand(ent - g_entities, "cp \"You can't use this emote in JK2 1.02 version\n\"");
		return;
	}

	// Exception for duel gametype
	if(g_gametype.integer == GT_TOURNAMENT || g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY)
	{
		trap_SendServerCommand(ent - g_entities, "cp \"Emotes are disabled in this gametype\n\"");
		return;
	}

	// Exception for when you are moving
	if ((ent->client->ps.velocity[0] != 0 || ent->client->ps.velocity[1] != 0 || ent->client->ps.velocity[2] != 0) && !emotes[emoteIndex].special)
	{
		trap_SendServerCommand(ent - g_entities, "cp \"Please stop moving before you attempt an emote\n\"");
		return;
	}

	// Exception for dimensions
	if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
	{
		trap_SendServerCommand(ent - g_entities, "cp \"Emotes are disabled in this dimension\n\"");
		return;
	}

	// Bit values for emotes! Let the people choose!
	if(!(jkcvar_emotesEnabled.integer & (1 << emoteIndex)))
	{
		trap_SendServerCommand(ent - g_entities, "cp \"This emote has been disabled by the server\n\"");
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
			JKMod_emoteDoHug(ent);
			return;
		}
		else if (Q_stricmp(emotes[emoteIndex].cmd, "kiss") == 0)
		{
			JKMod_emoteDoKiss(ent);
			return;
		}
		else if (Q_stricmp(emotes[emoteIndex].cmd, "punch") == 0)
		{
			JKMod_emoteDoPunch(ent);
			return;
		}
	}

	// If emotes are disabled, you can still exit an emote
	if(jkcvar_emotesEnabled.integer == 0 && !JKMod_emoteIn(ent, -1))
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
					cmd = BOTH_FORCE_GETUP_B5;
					break;
				case 3:
				case 4:
				case 5:
					cmd = BOTH_FORCE_GETUP_B6;
					break;
				case 6:
				case 7:
					cmd = BOTH_SIT2TOSTAND5;
					break;
				case 8:
				case 9:
				case 10:
				default:
					cmd = BOTH_FORCE_GETUP_F1;
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
			if((!ent->client->ps.saberHolstered && emoteIndex != EMOTE_WRITING && emoteIndex != EMOTE_WRITING2) ||
				ent->client->ps.saberHolstered && (emoteIndex == EMOTE_WRITING || emoteIndex == EMOTE_WRITING2))
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
		if(!jkcvar_emotesBreak.value && (ent->client->ps.groundEntityNum == ENTITYNUM_NONE || ent->client->ps.groundEntityNum < MAX_CLIENTS))
		{
			return;
		}

		// Switch saber cases for taunt emotes
		if ((!ent->client->ps.saberHolstered && emoteIndex == EMOTE_TAUNT2) || (ent->client->ps.saberHolstered && emoteIndex == EMOTE_TAUNT3))
		{
			Cmd_ToggleSaber_f(ent);
		}

		// Server wants you to freeze while the emote plays
		if(jkcvar_emotesFreeze.integer && !emotes[emoteIndex].special)
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_DODGE;
			ent->client->ps.forceDodgeAnim = emotes[emoteIndex].startAnim;
			ent->client->ps.forceHandExtendTime = level.time + (bgGlobalAnimations[emotes[emoteIndex].startAnim].numFrames * fabs(bgGlobalAnimations[emotes[emoteIndex].startAnim].frameLerp));
		}
		// Do a standard setting of emote
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
void JKMod_emoteDoHug(gentity_t *ent)
{
	trace_t tr;
	vec3_t fPos;

	if(ent->client->ps.forceRestricted
		|| ent->client->ps.duelInProgress
		|| ent->client->ps.saberInFlight
		|| ent->client->ps.weapon != WP_SABER
		|| !jkcvar_emotesEnabled.integer
		|| BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim)
		|| BG_SaberInSpecialAttack(ent->client->ps.legsAnim))
		return;

	if(!(jkcvar_emotesEnabled.integer & EMOTE_HUG))
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

				StandardSetBodyAnim(ent, BOTH_HUGGER1, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);

				ent->client->ps.saberMove = LS_NONE;
				ent->client->ps.saberBlocked = 0;
				ent->client->ps.saberBlocking = 0;

				VectorSubtract(ent->client->ps.origin, other->client->ps.origin, entDir);
				VectorCopy(other->client->ps.viewangles, otherAngles);
				otherAngles[YAW] = vectoyaw(entDir);
				SetClientViewAngle(other, otherAngles);

				StandardSetBodyAnim(other, BOTH_HUGGEE1, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);

				other->client->ps.saberMove = LS_NONE;
				other->client->ps.saberBlocked = 0;
				other->client->ps.saberBlocking = 0;
			}
		}
	}
	else
	{
		trap_SendServerCommand(ent - g_entities, "cp \"You need someone in front of you\n\"");
		return;
	}
}

/*
=====================================================================
Emote kiss function
=====================================================================
*/
void JKMod_emoteDoKiss(gentity_t *ent)
{
	trace_t tr;
	vec3_t fPos;

	if(ent->client->ps.forceRestricted
		|| ent->client->ps.duelInProgress
		|| ent->client->ps.saberInFlight
		|| ent->client->ps.weapon != WP_SABER
		|| jkcvar_emotesEnabled.integer == 0
		|| BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim)
		|| BG_SaberInSpecialAttack(ent->client->ps.legsAnim))
	{
		return;
	}

	if (jkcvar_emotesEnabled.integer != 2)
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

				StandardSetBodyAnim(ent, BOTH_KISSER1LOOP, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);

				ent->client->ps.saberMove = LS_NONE;
				ent->client->ps.saberBlocked = 0;
				ent->client->ps.saberBlocking = 0;
				VectorSubtract(ent->client->ps.origin, other->client->ps.origin, entDir);

				VectorCopy(other->client->ps.viewangles, otherAngles);
				otherAngles[YAW] = vectoyaw(entDir);
				SetClientViewAngle(other, otherAngles);

				StandardSetBodyAnim(other, BOTH_KISSEE1LOOP, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);

				other->client->ps.saberMove = LS_NONE;
				other->client->ps.saberBlocked = 0;
				other->client->ps.saberBlocking = 0;
			}
		}
	}
	else
	{
		trap_SendServerCommand(ent - g_entities, "cp \"You need someone in front of you\n\"");
		return;
	}
}

/*
=====================================================================
Emote punch function
=====================================================================
*/
void JKMod_emoteDoPunch(gentity_t *ent)
{
	trace_t tr;
	vec3_t fPos;
	int cmd = Q_irand(0, 2) ? BOTH_MELEE1 : BOTH_MELEE2;

	// Special cases to disallow the use of the emote
	if (ent->client->ps.forceRestricted
		|| ent->client->ps.duelInProgress
		|| ent->client->ps.saberInFlight
		|| ent->client->ps.pm_type == PM_SPECTATOR
		|| ent->client->ps.pm_type == PM_DEAD
		|| jkcvar_emotesEnabled.integer == 0)
	{
		return;
	}

	if (jkcvar_emotesEnabled.integer != 2)
	{
		if ((ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_STAND2
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGER1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_HUGGEE1
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSER1LOOP
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_KISSEE1LOOP
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_PAIN20
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_MELEE2
			&& (ent->client->ps.legsAnim&~ANIM_TOGGLEBIT) != BOTH_MELEE1)
		{
			return;
		}
	}

	if (ent->client->ps.weapon == WP_SABER && !ent->client->ps.saberHolstered) Cmd_ToggleSaber_f(ent);

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

		if (other->client->ps.weapon == WP_SABER && !other->client->ps.saberHolstered) Cmd_ToggleSaber_f(other);

		if (other && other->inuse && other->client && !other->client->ps.forceRestricted)
		{
			if (other->client->ps.weapon != WP_SABER) return;
			StandardSetBodyAnim(other, BOTH_PAIN20, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
			other->client->ps.saberMove = LS_NONE;
			other->client->ps.saberBlocked = 0;
			other->client->ps.saberBlocking = 0;
			if (jkcvar_emotesPunchDamage.integer) G_Damage(other, ent, ent, NULL, NULL, jkcvar_emotesPunchDamage.integer, DAMAGE_NO_ARMOR, MOD_MELEE);
		}
	}
}
