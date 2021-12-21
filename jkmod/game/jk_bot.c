/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Custom bots AI
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header
#include "../../code/game/q_shared.h"
#include "../../code/game/botlib.h"
#include "../../code/game/be_aas.h"
#include "../../code/game/be_ea.h"
#include "../../code/game/be_ai_char.h"
#include "../../code/game/be_ai_chat.h"
#include "../../code/game/be_ai_gen.h"
#include "../../code/game/be_ai_goal.h"
#include "../../code/game/be_ai_move.h"
#include "../../code/game/be_ai_weap.h"

#include "../../code/game/ai_main.h"
#include "../../code/game/w_saber.h"

#include "../../code/game/chars.h"
#include "../../code/game/inv.h"
#include "../../code/game/syn.h"

// Extern stuff
extern bot_state_t *botstates[MAX_CLIENTS];

// Bot forgimmick actions
#define JK_BOT_STATIC		( 1 << 0 )
#define JK_BOT_ATTACK		( 1 << 1 )
#define JK_BOT_ATTACK_ALT	( 1 << 2 )
#define JK_BOT_CROUCH		( 1 << 3 )
#define JK_BOT_JUMP			( 1 << 4 )
#define JK_BOT_TAUNT		( 1 << 5 )
#define JK_BOT_TALK			( 1 << 6 )
#define JK_BOT_GOD			( 1 << 7 )

/*
=====================================================================
Custom bot AI
=====================================================================
*/
void JKMod_CustomBotAI(bot_state_t *bs, float thinktime)
{
	int wp, enemy;
	int desiredIndex;
	int goalWPIndex;
	int doingFallback = 0;
	int fjHalt;
	vec3_t a, ang, headlevel, eorg, noz_x, noz_y, dif, a_fo;
	float reaction;
	float bLeadAmount;
	int meleestrafe = 0;
	int useTheForce = 0;
	int forceHostile = 0;
	gentity_t *friendInLOF = 0;
	float mLen;
	int visResult = 0;
	int selResult = 0;
	int mineSelect = 0;
	int detSelect = 0;

	if (gDeactivated)
	{
		bs->wpCurrent = NULL;
		bs->currentEnemy = NULL;
		bs->wpDestination = NULL;
		bs->wpDirection = 0;
		return;
	}

	if (g_entities[bs->client].inuse &&
		g_entities[bs->client].client &&
		g_entities[bs->client].client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		bs->wpCurrent = NULL;
		bs->currentEnemy = NULL;
		bs->wpDestination = NULL;
		bs->wpDirection = 0;
		return;
	}

	trap_Cvar_Update(&bot_forgimmick);
	trap_Cvar_Update(&bot_honorableduelacceptance);

	if (bot_forgimmick.integer)
	{
		// if (bot_forgimmick.integer & JK_BOT_STATIC)
		// {
			bs->wpCurrent = NULL;
			bs->currentEnemy = NULL;
			bs->wpDestination = NULL;
			bs->wpDirection = 0;
		// }

		if (bot_forgimmick.integer & JK_BOT_ATTACK) trap_EA_Attack(bs->client);
		if (bot_forgimmick.integer & JK_BOT_ATTACK_ALT) trap_EA_Alt_Attack(bs->client);
		if (bot_forgimmick.integer & JK_BOT_CROUCH) trap_EA_Crouch(bs->client);
		if (bot_forgimmick.integer & JK_BOT_JUMP) trap_EA_Jump(bs->client);
		if (bot_forgimmick.integer & JK_BOT_TAUNT) trap_EA_Gesture(bs->client);
		if (bot_forgimmick.integer & JK_BOT_TALK) trap_EA_Talk(bs->client);
		if (bot_forgimmick.integer & JK_BOT_GOD) {
			g_entities[bs->client].flags |= FL_GODMODE;
		}else {
			g_entities[bs->client].flags &= ~FL_GODMODE;
		}
		return;
	}

	// Check god mode
	if (g_entities[bs->client].flags & FL_GODMODE) g_entities[bs->client].flags &= ~FL_GODMODE;

	// Add talk balloon
	if (bs->doChat && bs->chatTime > level.time) {
		trap_EA_Talk(bs->client);
	}

	if (!bs->lastDeadTime)
	{ //just spawned in?
		bs->lastDeadTime = level.time;
	}

	if (g_entities[bs->client].health < 1)
	{
		bs->lastDeadTime = level.time;

		if (!bs->deathActivitiesDone && bs->lastHurt && bs->lastHurt->client && bs->lastHurt->s.number != bs->client)
		{
			BotDeathNotify(bs);
			if (PassLovedOneCheck(bs, bs->lastHurt))
			{
				//CHAT: Died
				bs->chatObject = bs->lastHurt;
				bs->chatAltObject = NULL;
				BotDoChat(bs, "Died", 0);
			}
			else if (!PassLovedOneCheck(bs, bs->lastHurt) &&
				botstates[bs->lastHurt->s.number] &&
				PassLovedOneCheck(botstates[bs->lastHurt->s.number], &g_entities[bs->client]))
			{ //killed by a bot that I love, but that does not love me
				bs->chatObject = bs->lastHurt;
				bs->chatAltObject = NULL;
				BotDoChat(bs, "KilledOnPurposeByLove", 0);
			}

			bs->deathActivitiesDone = 1;
		}
		
		bs->wpCurrent = NULL;
		bs->currentEnemy = NULL;
		bs->wpDestination = NULL;
		bs->wpCamping = NULL;
		bs->wpCampingTo = NULL;
		bs->wpStoreDest = NULL;
		bs->wpDestIgnoreTime = 0;
		bs->wpDestSwitchTime = 0;
		bs->wpSeenTime = 0;
		bs->wpDirection = 0;

		if (rand()%10 < 5 &&
			(!bs->doChat || bs->chatTime < level.time))
		{
			trap_EA_Attack(bs->client);
		}

		return;
	}

	bs->doAttack = 0;
	bs->doAltAttack = 0;
	//reset the attack states

	if (bs->isSquadLeader)
	{
		CommanderBotAI(bs);
	}
	else
	{
		BotDoTeamplayAI(bs);
	}

	if (!bs->currentEnemy)
	{
		bs->frame_Enemy_Vis = 0;
	}

	if (bs->revengeEnemy && bs->revengeEnemy->client &&
		bs->revengeEnemy->client->pers.connected != CON_CONNECTED)
	{
		bs->revengeEnemy = NULL;
		bs->revengeHateLevel = 0;
	}

	if (bs->currentEnemy && bs->currentEnemy->client &&
		bs->currentEnemy->client->pers.connected != CON_CONNECTED)
	{
		bs->currentEnemy = NULL;
	}

	fjHalt = 0;

#ifndef FORCEJUMP_INSTANTMETHOD
	if (bs->forceJumpChargeTime > level.time)
	{
		useTheForce = 1;
		forceHostile = 0;
	}

	if (bs->currentEnemy && bs->currentEnemy->client && bs->frame_Enemy_Vis && bs->forceJumpChargeTime < level.time)
#else
	if (bs->currentEnemy && bs->currentEnemy->client && bs->frame_Enemy_Vis)
#endif
	{
		VectorSubtract(bs->currentEnemy->client->ps.origin, bs->eye, a_fo);
		vectoangles(a_fo, a_fo);

		//do this above all things
		if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_PUSH)) && bs->doForcePush > level.time && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_PUSH]][FP_PUSH] && InFieldOfVision(bs->viewangles, 50, a_fo))
		{
			level.clients[bs->client].ps.fd.forcePowerSelected = FP_PUSH;
			useTheForce = 1;
			forceHostile = 1;
		}
		else if (bs->cur_ps.fd.forceSide == FORCE_DARKSIDE)
		{ //try dark side powers
		  //in order of priority top to bottom
			if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_GRIP)) && (bs->cur_ps.fd.forcePowersActive & (1 << FP_GRIP)) && InFieldOfVision(bs->viewangles, 50, a_fo))
			{ //already gripping someone, so hold it
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_GRIP;
				useTheForce = 1;
				forceHostile = 1;
			}
			else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_LIGHTNING)) && bs->frame_Enemy_Len < FORCE_LIGHTNING_RADIUS && level.clients[bs->client].ps.fd.forcePower > 50 && InFieldOfVision(bs->viewangles, 50, a_fo))
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_LIGHTNING;
				useTheForce = 1;
				forceHostile = 1;
			}
			else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_GRIP)) && bs->frame_Enemy_Len < MAX_GRIP_DISTANCE && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_GRIP]][FP_GRIP] && InFieldOfVision(bs->viewangles, 50, a_fo))
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_GRIP;
				useTheForce = 1;
				forceHostile = 1;
			}
			else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_RAGE)) && g_entities[bs->client].health < 25 && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_RAGE]][FP_RAGE])
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_RAGE;
				useTheForce = 1;
				forceHostile = 0;
			}
			else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_DRAIN)) && bs->frame_Enemy_Len < MAX_DRAIN_DISTANCE && level.clients[bs->client].ps.fd.forcePower > 50 && InFieldOfVision(bs->viewangles, 50, a_fo) && bs->currentEnemy->client->ps.fd.forcePower > 10 && bs->currentEnemy->client->ps.fd.forceSide == FORCE_LIGHTSIDE)
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_DRAIN;
				useTheForce = 1;
				forceHostile = 1;
			}
		}
		else if (bs->cur_ps.fd.forceSide == FORCE_LIGHTSIDE)
		{ //try light side powers
			if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_TELEPATHY)) && bs->frame_Enemy_Len < MAX_TRICK_DISTANCE && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_TELEPATHY]][FP_TELEPATHY] && InFieldOfVision(bs->viewangles, 50, a_fo) && !(bs->currentEnemy->client->ps.fd.forcePowersActive & (1 << FP_SEE)))
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_TELEPATHY;
				useTheForce = 1;
				forceHostile = 1;
			}
			else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_ABSORB)) && g_entities[bs->client].health < 75 && bs->currentEnemy->client->ps.fd.forceSide == FORCE_DARKSIDE && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_ABSORB]][FP_ABSORB])
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_ABSORB;
				useTheForce = 1;
				forceHostile = 0;
			}
			else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_PROTECT)) && g_entities[bs->client].health < 35 && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_PROTECT]][FP_PROTECT])
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_PROTECT;
				useTheForce = 1;
				forceHostile = 0;
			}
		}

		if (!useTheForce)
		{ //try neutral powers
			if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_PUSH)) && bs->cur_ps.fd.forceGripBeingGripped > level.time && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_PUSH]][FP_PUSH] && InFieldOfVision(bs->viewangles, 50, a_fo))
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_PUSH;
				useTheForce = 1;
				forceHostile = 1;
			}
			else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_SPEED)) && g_entities[bs->client].health < 25 && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_SPEED]][FP_SPEED])
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_SPEED;
				useTheForce = 1;
				forceHostile = 0;
			}
			else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_SEE)) && BotMindTricked(bs->client, bs->currentEnemy->s.number) && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_SEE]][FP_SEE])
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_SEE;
				useTheForce = 1;
				forceHostile = 0;
			}
			else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_PULL)) && bs->frame_Enemy_Len < 256 && level.clients[bs->client].ps.fd.forcePower > 75 && InFieldOfVision(bs->viewangles, 50, a_fo))
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_PULL;
				useTheForce = 1;
				forceHostile = 1;
			}
		}
	}

	if (!useTheForce)
	{ //try powers that we don't care if we have an enemy for
		if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_HEAL)) && g_entities[bs->client].health < 50 && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_HEAL]][FP_HEAL] && bs->cur_ps.fd.forcePowerLevel[FP_HEAL] > FORCE_LEVEL_1)
		{
			level.clients[bs->client].ps.fd.forcePowerSelected = FP_HEAL;
			useTheForce = 1;
			forceHostile = 0;
		}
		else if ((bs->cur_ps.fd.forcePowersKnown & (1 << FP_HEAL)) && g_entities[bs->client].health < 50 && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_HEAL]][FP_HEAL] && !bs->currentEnemy && bs->isCamping > level.time)
		{ //only meditate and heal if we're camping
			level.clients[bs->client].ps.fd.forcePowerSelected = FP_HEAL;
			useTheForce = 1;
			forceHostile = 0;
		}
	}

	if (useTheForce && forceHostile)
	{
		if (bs->currentEnemy && bs->currentEnemy->client &&
			!ForcePowerUsableOn(&g_entities[bs->client], bs->currentEnemy, level.clients[bs->client].ps.fd.forcePowerSelected))
		{
			useTheForce = 0;
			forceHostile = 0;
		}
	}

	doingFallback = 0;

	bs->deathActivitiesDone = 0;

	if (BotUseInventoryItem(bs))
	{
		if (rand()%10 < 5)
		{
			trap_EA_Use(bs->client);
		}
	}

	if (bs->cur_ps.ammo[weaponData[bs->cur_ps.weapon].ammoIndex] < weaponData[bs->cur_ps.weapon].energyPerShot)
	{
		if (BotTryAnotherWeapon(bs))
		{
			return;
		}
	}
	else
	{
		if (bs->currentEnemy && bs->lastVisibleEnemyIndex == bs->currentEnemy->s.number &&
			bs->frame_Enemy_Vis && bs->forceWeaponSelect /*&& bs->plantContinue < level.time*/)
		{
			bs->forceWeaponSelect = 0;
		}

		if (bs->plantContinue > level.time)
		{
			bs->doAttack = 1;
			bs->destinationGrabTime = 0;
		}

		if (!bs->forceWeaponSelect && bs->cur_ps.hasDetPackPlanted && bs->plantKillEmAll > level.time)
		{
			bs->forceWeaponSelect = WP_DET_PACK;
		}

		if (bs->forceWeaponSelect)
		{
			selResult = BotSelectChoiceWeapon(bs, bs->forceWeaponSelect, 1);
		}

		if (selResult)
		{
			if (selResult == 2)
			{ //newly selected
				return;
			}
		}
		else if (BotSelectIdealWeapon(bs))
		{
			return;
		}
	}
	/*if (BotSelectMelee(bs))
	{
		return;
	}*/

	reaction = bs->skills.reflex/bs->settings.skill;

	if (reaction < 0)
	{
		reaction = 0;
	}
	if (reaction > 2000)
	{
		reaction = 2000;
	}

	if (!bs->currentEnemy)
	{
		bs->timeToReact = level.time + reaction;
	}

	if (bs->cur_ps.weapon == WP_DET_PACK && bs->cur_ps.hasDetPackPlanted && bs->plantKillEmAll > level.time)
	{
		bs->doAltAttack = 1;
	}

	if (bs->wpCamping)
	{
		if (bs->isCamping < level.time)
		{
			bs->wpCamping = NULL;
			bs->isCamping = 0;
		}

		if (bs->currentEnemy && bs->frame_Enemy_Vis)
		{
			bs->wpCamping = NULL;
			bs->isCamping = 0;
		}
	}

	if (bs->wpCurrent &&
		(bs->wpSeenTime < level.time || bs->wpTravelTime < level.time))
	{
		bs->wpCurrent = NULL;
	}

	if (bs->currentEnemy)
	{
		if (bs->enemySeenTime < level.time ||
			!PassStandardEnemyChecks(bs, bs->currentEnemy))
		{
			if (bs->revengeEnemy == bs->currentEnemy &&
				bs->currentEnemy->health < 1 &&
				bs->lastAttacked && bs->lastAttacked == bs->currentEnemy)
			{
				//CHAT: Destroyed hated one [KilledHatedOne section]
				bs->chatObject = bs->revengeEnemy;
				bs->chatAltObject = NULL;
				BotDoChat(bs, "KilledHatedOne", 1);
				bs->revengeEnemy = NULL;
				bs->revengeHateLevel = 0;
			}
			else if (bs->currentEnemy->health < 1 && PassLovedOneCheck(bs, bs->currentEnemy) &&
				bs->lastAttacked && bs->lastAttacked == bs->currentEnemy)
			{
				//CHAT: Killed
				bs->chatObject = bs->currentEnemy;
				bs->chatAltObject = NULL;
				BotDoChat(bs, "Killed", 0);
			}

			bs->currentEnemy = NULL;
		}
	}

	if (bot_honorableduelacceptance.integer)
	{
		if (bs->currentEnemy && bs->currentEnemy->client &&
			bs->cur_ps.weapon == WP_SABER &&
			g_privateDuel.integer &&
			bs->frame_Enemy_Vis &&
			bs->frame_Enemy_Len < 400 &&
			bs->currentEnemy->client->ps.weapon == WP_SABER &&
			bs->currentEnemy->client->ps.saberHolstered)
		{
			vec3_t e_ang_vec;

			VectorSubtract(bs->currentEnemy->client->ps.origin, bs->eye, e_ang_vec);

			if (InFieldOfVision(bs->viewangles, 100, e_ang_vec))
			{ //Our enemy has his saber holstered and has challenged us to a duel, so challenge him back
				if (!bs->cur_ps.saberHolstered)
				{
					Cmd_ToggleSaber_f(&g_entities[bs->client]);
				}
				else
				{
					if (bs->currentEnemy->client->ps.duelIndex == bs->client &&
						bs->currentEnemy->client->ps.duelTime > level.time &&
						!bs->cur_ps.duelInProgress)
					{
						Cmd_EngageDuel_f(&g_entities[bs->client]);
					}
				}

				bs->doAttack = 0;
				bs->doAltAttack = 0;
				bs->botChallengingTime = level.time + 100;
				bs->beStill = level.time + 100;
			}
		}
	}
	//Apparently this "allows you to cheese" when fighting against bots. I'm not sure why you'd want to con bots
	//into an easy kill, since they're bots and all. But whatever.

	if (!bs->wpCurrent)
	{
		wp = GetNearestVisibleWP(bs->origin, bs->client);

		if (wp != -1)
		{
			bs->wpCurrent = gWPArray[wp];
			bs->wpSeenTime = level.time + 1500;
			bs->wpTravelTime = level.time + 10000; //never take more than 10 seconds to travel to a waypoint
		}
	}

	if (bs->enemySeenTime < level.time || !bs->frame_Enemy_Vis || !bs->currentEnemy ||
		(bs->currentEnemy /*&& bs->cur_ps.weapon == WP_SABER && bs->frame_Enemy_Len > 300*/))
	{
		enemy = ScanForEnemies(bs);

		if (enemy != -1)
		{
			bs->currentEnemy = &g_entities[enemy];
			bs->enemySeenTime = level.time + ENEMY_FORGET_MS;
		}
	}

	if (!bs->squadLeader && !bs->isSquadLeader)
	{
		BotScanForLeader(bs);
	}

	if (!bs->squadLeader && bs->squadCannotLead < level.time)
	{ //if still no leader after scanning, then become a squad leader
		bs->isSquadLeader = 1;
	}

	if (bs->isSquadLeader && bs->squadLeader)
	{ //we don't follow anyone if we are a leader
		bs->squadLeader = NULL;
	}

	//ESTABLISH VISIBILITIES AND DISTANCES FOR THE WHOLE FRAME HERE
	if (bs->wpCurrent)
	{
		VectorSubtract(bs->wpCurrent->origin, bs->origin, a);
		bs->frame_Waypoint_Len = VectorLength(a);

		visResult = WPOrgVisible(&g_entities[bs->client], bs->origin, bs->wpCurrent->origin, bs->client);

		if (visResult == 2)
		{
			bs->frame_Waypoint_Vis = 0;
			bs->wpSeenTime = 0;
			bs->wpDestination = NULL;
			bs->wpDestIgnoreTime = level.time + 5000;

			if (bs->wpDirection)
			{
				bs->wpDirection = 0;
			}
			else
			{
				bs->wpDirection = 1;
			}
		}
		else if (visResult)
		{
			bs->frame_Waypoint_Vis = 1;
		}
		else
		{
			bs->frame_Waypoint_Vis = 0;
		}
	}

	if (bs->currentEnemy)
	{
		if (bs->currentEnemy->client)
		{
			VectorCopy(bs->currentEnemy->client->ps.origin, eorg);
			eorg[2] += bs->currentEnemy->client->ps.viewheight;
		}
		else
		{
			VectorCopy(bs->currentEnemy->s.origin, eorg);
		}

		VectorSubtract(eorg, bs->eye, a);
		bs->frame_Enemy_Len = VectorLength(a);

		if (OrgVisible(bs->eye, eorg, bs->client))
		{
			bs->frame_Enemy_Vis = 1;
			VectorCopy(eorg, bs->lastEnemySpotted);
			VectorCopy(bs->origin, bs->hereWhenSpotted);
			bs->lastVisibleEnemyIndex = bs->currentEnemy->s.number;
			//VectorCopy(bs->eye, bs->lastEnemySpotted);
			bs->hitSpotted = 0;
		}
		else
		{
			bs->frame_Enemy_Vis = 0;
		}
	}
	else
	{
		bs->lastVisibleEnemyIndex = ENTITYNUM_NONE;
	}
	//END

	if (bs->frame_Enemy_Vis)
	{
		bs->enemySeenTime = level.time + ENEMY_FORGET_MS;
	}

	if (bs->wpCurrent)
	{
		WPConstantRoutine(bs);

		if (!bs->wpCurrent)
		{ //WPConstantRoutine has the ability to nullify the waypoint if it fails certain checks, so..
			return;
		}

		if (bs->wpCurrent->flags & WPFLAG_WAITFORFUNC)
		{
			if (!CheckForFunc(bs->wpCurrent->origin, -1))
			{
				bs->beStill = level.time + 500; //no func brush under.. wait
			}
		}
		if (bs->wpCurrent->flags & WPFLAG_NOMOVEFUNC)
		{
			if (CheckForFunc(bs->wpCurrent->origin, -1))
			{
				bs->beStill = level.time + 500; //func brush under.. wait
			}
		}

		if (bs->frame_Waypoint_Vis || (bs->wpCurrent->flags & WPFLAG_NOVIS))
		{
			bs->wpSeenTime = level.time + 1500; //if we lose sight of the point, we have 1.5 seconds to regain it before we drop it
		}
		VectorCopy(bs->wpCurrent->origin, bs->goalPosition);
		if (bs->wpDirection)
		{
			goalWPIndex = bs->wpCurrent->index-1;
		}
		else
		{
			goalWPIndex = bs->wpCurrent->index+1;
		}

		if (bs->wpCamping)
		{
			VectorSubtract(bs->wpCampingTo->origin, bs->origin, a);
			vectoangles(a, ang);
			VectorCopy(ang, bs->goalAngles);

			VectorSubtract(bs->origin, bs->wpCamping->origin, a);
			if (VectorLength(a) < 64)
			{
				VectorCopy(bs->wpCamping->origin, bs->goalPosition);
				bs->beStill = level.time + 1000;

				if (!bs->campStanding)
				{
					bs->duckTime = level.time + 1000;
				}
			}
		}
		else if (gWPArray[goalWPIndex] && gWPArray[goalWPIndex]->inuse &&
			!(gLevelFlags & LEVELFLAG_NOPOINTPREDICTION))
		{
			VectorSubtract(gWPArray[goalWPIndex]->origin, bs->origin, a);
			vectoangles(a, ang);
			VectorCopy(ang, bs->goalAngles);
		}
		else
		{
			VectorSubtract(bs->wpCurrent->origin, bs->origin, a);
			vectoangles(a, ang);
			VectorCopy(ang, bs->goalAngles);
		}

		if (bs->destinationGrabTime < level.time /*&& (!bs->wpDestination || (bs->currentEnemy && bs->frame_Enemy_Vis))*/)
		{
			GetIdealDestination(bs);
		}
		
		if (bs->wpCurrent && bs->wpDestination)
		{
			if (TotalTrailDistance(bs->wpCurrent->index, bs->wpDestination->index, bs) == -1)
			{
				bs->wpDestination = NULL;
				bs->destinationGrabTime = level.time + 10000;
			}
		}

		if (bs->frame_Waypoint_Len < BOT_WPTOUCH_DISTANCE)
		{
			WPTouchRoutine(bs);

			if (!bs->wpDirection)
			{
				desiredIndex = bs->wpCurrent->index+1;
			}
			else
			{
				desiredIndex = bs->wpCurrent->index-1;
			}

			if (gWPArray[desiredIndex] &&
				gWPArray[desiredIndex]->inuse &&
				desiredIndex < gWPNum &&
				desiredIndex >= 0 &&
				PassWayCheck(bs, desiredIndex))
			{
				bs->wpCurrent = gWPArray[desiredIndex];
			}
			else
			{
				if (bs->wpDestination)
				{
					bs->wpDestination = NULL;
					bs->destinationGrabTime = level.time + 10000;
				}

				if (bs->wpDirection)
				{
					bs->wpDirection = 0;
				}
				else
				{
					bs->wpDirection = 1;
				}
			}
		}
	}
	else //We can't find a waypoint, going to need a fallback routine.
	{
		/*if (g_gametype.integer == GT_TOURNAMENT)*/
		{ //helps them get out of messy situations
			/*if ((level.time - bs->forceJumpChargeTime) > 3500)
			{
				bs->forceJumpChargeTime = level.time + 2000;
				trap_EA_MoveForward(bs->client);
			}
			*/
			bs->jumpTime = level.time + 1500;
			bs->jumpHoldTime = level.time + 1500;
			bs->jDelay = 0;
		}
		doingFallback = BotFallbackNavigation(bs);
	}

	if (doingFallback)
	{
		bs->doingFallback = qtrue;
	}
	else
	{
		bs->doingFallback = qfalse;
	}

	if (bs->timeToReact < level.time && bs->currentEnemy && bs->enemySeenTime > level.time + (ENEMY_FORGET_MS - (ENEMY_FORGET_MS*0.2)))
	{
		if (bs->frame_Enemy_Vis)
		{
			CombatBotAI(bs, thinktime);
		}
		else if (bs->cur_ps.weaponstate == WEAPON_CHARGING_ALT)
		{ //keep charging in case we see him again before we lose track of him
			bs->doAltAttack = 1;
		}
		else if (bs->cur_ps.weaponstate == WEAPON_CHARGING)
		{ //keep charging in case we see him again before we lose track of him
			bs->doAttack = 1;
		}

		if (bs->destinationGrabTime > level.time + 100)
		{
			bs->destinationGrabTime = level.time + 100; //assures that we will continue staying within a general area of where we want to be in a combat situation
		}

		if (bs->currentEnemy->client)
		{
			VectorCopy(bs->currentEnemy->client->ps.origin, headlevel);
			headlevel[2] += bs->currentEnemy->client->ps.viewheight;
		}
		else
		{
			VectorCopy(bs->currentEnemy->client->ps.origin, headlevel);
		}

		if (!bs->frame_Enemy_Vis)
		{
			//if (!bs->hitSpotted && VectorLength(a) > 256)
			if (OrgVisible(bs->eye, bs->lastEnemySpotted, -1))
			{
				VectorCopy(bs->lastEnemySpotted, headlevel);
				VectorSubtract(headlevel, bs->eye, a);
				vectoangles(a, ang);
				VectorCopy(ang, bs->goalAngles);

				if (bs->cur_ps.weapon == WP_FLECHETTE &&
					bs->cur_ps.weaponstate == WEAPON_READY &&
					bs->currentEnemy && bs->currentEnemy->client)
				{
					mLen = VectorLength(a) > 128;
					if (mLen > 128 && mLen < 1024)
					{
						VectorSubtract(bs->currentEnemy->client->ps.origin, bs->lastEnemySpotted, a);

						if (VectorLength(a) < 300)
						{
							bs->doAltAttack = 1;
						}
					}
				}
			}
		}
		else
		{
			bLeadAmount = BotWeaponCanLead(bs);
			if ((bs->skills.accuracy/bs->settings.skill) <= 8 &&
				bLeadAmount)
			{
				BotAimLeading(bs, headlevel, bLeadAmount);
			}
			else
			{
				VectorSubtract(headlevel, bs->eye, a);
				vectoangles(a, ang);
				VectorCopy(ang, bs->goalAngles);
			}

			BotAimOffsetGoalAngles(bs);
		}
	}

	if (bs->cur_ps.saberInFlight)
	{
		bs->saberThrowTime = level.time + Q_irand(4000, 10000);
	}

	if (bs->currentEnemy)
	{
		if (BotGetWeaponRange(bs) == BWEAPONRANGE_SABER)
		{
			int saberRange = SABER_ATTACK_RANGE;

			VectorSubtract(bs->currentEnemy->client->ps.origin, bs->eye, a_fo);
			vectoangles(a_fo, a_fo);

			if (bs->saberPowerTime < level.time)
			{ //Don't just use strong attacks constantly, switch around a bit
				if (Q_irand(1, 10) <= 5)
				{
					bs->saberPower = qtrue;
				}
				else
				{
					bs->saberPower = qfalse;
				}

				bs->saberPowerTime = level.time + Q_irand(3000, 15000);
			}

			if (bs->currentEnemy->health > 75 && g_entities[bs->client].client->ps.fd.forcePowerLevel[FP_SABERATTACK] > 2)
			{
				if (g_entities[bs->client].client->ps.fd.saberAnimLevel != FORCE_LEVEL_3 && bs->saberPower)
				{ //if we are up against someone with a lot of health and we have a strong attack available, then h4q them
					Cmd_SaberAttackCycle_f(&g_entities[bs->client]);
				}
			}
			else if (bs->currentEnemy->health > 40 && g_entities[bs->client].client->ps.fd.forcePowerLevel[FP_SABERATTACK] > 1)
			{
				if (g_entities[bs->client].client->ps.fd.saberAnimLevel != FORCE_LEVEL_2)
				{ //they're down on health a little, use level 2 if we can
					Cmd_SaberAttackCycle_f(&g_entities[bs->client]);
				}
			}
			else
			{
				if (g_entities[bs->client].client->ps.fd.saberAnimLevel != FORCE_LEVEL_1)
				{ //they've gone below 40 health, go at them with quick attacks
					Cmd_SaberAttackCycle_f(&g_entities[bs->client]);
				}
			}

			if (g_gametype.integer == GT_SINGLE_PLAYER)
			{
				saberRange *= 3;
			}

			if (bs->frame_Enemy_Len <= saberRange)
			{
				SaberCombatHandling(bs);

				if (bs->frame_Enemy_Len < 80)
				{
					meleestrafe = 1;
				}
			}
			else if (bs->saberThrowTime < level.time && !bs->cur_ps.saberInFlight &&
				(bs->cur_ps.fd.forcePowersKnown & (1 << FP_SABERTHROW)) &&
				InFieldOfVision(bs->viewangles, 30, a_fo) &&
				bs->frame_Enemy_Len < BOT_SABER_THROW_RANGE)
			{
				bs->doAltAttack = 1;
				bs->doAttack = 0;
			}
			else if (bs->cur_ps.saberInFlight && bs->frame_Enemy_Len > 300 && bs->frame_Enemy_Len < BOT_SABER_THROW_RANGE)
			{
				bs->doAltAttack = 1;
				bs->doAttack = 0;
			}
		}
		else if (BotGetWeaponRange(bs) == BWEAPONRANGE_MELEE)
		{
			if (bs->frame_Enemy_Len <= MELEE_ATTACK_RANGE)
			{
				MeleeCombatHandling(bs);
				meleestrafe = 1;
			}
		}
	}

	if (doingFallback && bs->currentEnemy) //just stand and fire if we have no idea where we are
	{
		VectorCopy(bs->origin, bs->goalPosition);
	}

	if (bs->forceJumping > level.time)
	{
		VectorCopy(bs->origin, noz_x);
		VectorCopy(bs->goalPosition, noz_y);

		noz_x[2] = noz_y[2];

		VectorSubtract(noz_x, noz_y, noz_x);

		if (VectorLength(noz_x) < 32)
		{
			fjHalt = 1;
		}
	}

	if (bs->doChat && bs->chatTime > level.time && (!bs->currentEnemy || !bs->frame_Enemy_Vis))
	{
		return;
	}
	else if (bs->doChat && bs->currentEnemy && bs->frame_Enemy_Vis)
	{
		//bs->chatTime = level.time + bs->chatTime_stored;
		bs->doChat = 0; //do we want to keep the bot waiting to chat until after the enemy is gone?
		bs->chatTeam = 0;
	}
	else if (bs->doChat && bs->chatTime <= level.time)
	{
		if (bs->chatTeam)
		{
			trap_EA_SayTeam(bs->client, bs->currentChat);
			bs->chatTeam = 0;
		}
		else
		{
			trap_EA_Say(bs->client, bs->currentChat);
		}
		if (bs->doChat == 2)
		{
			BotReplyGreetings(bs);
		}
		bs->doChat = 0;
	}

	CTFFlagMovement(bs);

	if (/*bs->wpDestination &&*/ bs->shootGoal &&
		/*bs->wpDestination->associated_entity == bs->shootGoal->s.number &&*/
		bs->shootGoal->health > 0 && bs->shootGoal->takedamage)
	{
		dif[0] = (bs->shootGoal->r.absmax[0]+bs->shootGoal->r.absmin[0])/2;
		dif[1] = (bs->shootGoal->r.absmax[1]+bs->shootGoal->r.absmin[1])/2;
		dif[2] = (bs->shootGoal->r.absmax[2]+bs->shootGoal->r.absmin[2])/2;

		if (!bs->currentEnemy || bs->frame_Enemy_Len > 256)
		{ //if someone is close then don't stop shooting them for this
			VectorSubtract(dif, bs->eye, a);
			vectoangles(a, a);
			VectorCopy(a, bs->goalAngles);

			if (InFieldOfVision(bs->viewangles, 30, a) &&
				EntityVisibleBox(bs->origin, NULL, NULL, dif, bs->client, bs->shootGoal->s.number))
			{
				bs->doAttack = 1;
			}
		}
	}

	if (bs->cur_ps.hasDetPackPlanted)
	{ //check if our enemy gets near it and detonate if he does
		BotCheckDetPacks(bs);
	}
	else if (bs->currentEnemy && bs->lastVisibleEnemyIndex == bs->currentEnemy->s.number && !bs->frame_Enemy_Vis && bs->plantTime < level.time &&
		!bs->doAttack && !bs->doAltAttack)
	{
		VectorSubtract(bs->origin, bs->hereWhenSpotted, a);

		if (bs->plantDecided > level.time || (bs->frame_Enemy_Len < BOT_PLANT_DISTANCE*2 && VectorLength(a) < BOT_PLANT_DISTANCE))
		{
			mineSelect = BotSelectChoiceWeapon(bs, WP_TRIP_MINE, 0);
			detSelect = BotSelectChoiceWeapon(bs, WP_DET_PACK, 0);
			if (bs->cur_ps.hasDetPackPlanted)
			{
				detSelect = 0;
			}

			if (bs->plantDecided > level.time && bs->forceWeaponSelect &&
				bs->cur_ps.weapon == bs->forceWeaponSelect)
			{
				bs->doAttack = 1;
				bs->plantDecided = 0;
				bs->plantTime = level.time + BOT_PLANT_INTERVAL;
				bs->plantContinue = level.time + 500;
				bs->beStill = level.time + 500;
			}
			else if (mineSelect || detSelect)
			{
				if (BotSurfaceNear(bs))
				{
					if (!mineSelect)
					{ //if no mines use detpacks, otherwise use mines
						mineSelect = WP_DET_PACK;
					}
					else
					{
						mineSelect = WP_TRIP_MINE;
					}

					detSelect = BotSelectChoiceWeapon(bs, mineSelect, 1);

					if (detSelect && detSelect != 2)
					{ //We have it and it is now our weapon
						bs->plantDecided = level.time + 1000;
						bs->forceWeaponSelect = mineSelect;
						return;
					}
					else if (detSelect == 2)
					{
						bs->forceWeaponSelect = mineSelect;
						return;
					}
				}
			}
		}
	}
	else if (bs->plantContinue < level.time)
	{
		bs->forceWeaponSelect = 0;
	}

	if (g_gametype.integer == GT_JEDIMASTER && !bs->cur_ps.isJediMaster && bs->jmState == -1 && gJMSaberEnt && gJMSaberEnt->inuse)
	{
		vec3_t saberLen;
		float fSaberLen = 0;

		VectorSubtract(bs->origin, gJMSaberEnt->r.currentOrigin, saberLen);
		fSaberLen = VectorLength(saberLen);

		if (fSaberLen < 256)
		{
			if (OrgVisible(bs->origin, gJMSaberEnt->r.currentOrigin, bs->client))
			{
				VectorCopy(gJMSaberEnt->r.currentOrigin, bs->goalPosition);
			}
		}
	}

	if (bs->beStill < level.time && !WaitingForNow(bs, bs->goalPosition) && !fjHalt)
	{
		VectorSubtract(bs->goalPosition, bs->origin, bs->goalMovedir);
		VectorNormalize(bs->goalMovedir);

		if (bs->jumpTime > level.time && bs->jDelay < level.time &&
			level.clients[bs->client].pers.cmd.upmove > 0)
		{
		//	trap_EA_Move(bs->client, bs->origin, 5000);
			bs->beStill = level.time + 200;
		}
		else
		{
			trap_EA_Move(bs->client, bs->goalMovedir, 5000);
		}

		if (meleestrafe)
		{
			StrafeTracing(bs);
		}

		if (bs->meleeStrafeDir && meleestrafe && bs->meleeStrafeDisable < level.time)
		{
			trap_EA_MoveRight(bs->client);
		}
		else if (meleestrafe && bs->meleeStrafeDisable < level.time)
		{
			trap_EA_MoveLeft(bs->client);
		}

		if (BotTrace_Jump(bs, bs->goalPosition))
		{
			bs->jumpTime = level.time + 100;
		}
		else if (BotTrace_Duck(bs, bs->goalPosition))
		{
			bs->duckTime = level.time + 100;
		}
#ifdef BOT_STRAFE_AVOIDANCE
		else
		{
			int strafeAround = BotTrace_Strafe(bs, bs->goalPosition);

			if (strafeAround == STRAFEAROUND_RIGHT)
			{
				trap_EA_MoveRight(bs->client);
			}
			else if (strafeAround == STRAFEAROUND_LEFT)
			{
				trap_EA_MoveLeft(bs->client);
			}
		}
#endif
	}

#ifndef FORCEJUMP_INSTANTMETHOD
	if (bs->forceJumpChargeTime > level.time)
	{
		bs->jumpTime = 0;
	}
#endif

	if (bs->jumpPrep > level.time)
	{
		bs->forceJumpChargeTime = 0;
	}

	if (bs->forceJumpChargeTime > level.time)
	{
		bs->jumpHoldTime = ((bs->forceJumpChargeTime - level.time)/2) + level.time;
		bs->forceJumpChargeTime = 0;
	}

	if (bs->jumpHoldTime > level.time)
	{
		bs->jumpTime = bs->jumpHoldTime;
	}

	if (bs->jumpTime > level.time && bs->jDelay < level.time)
	{
		if (bs->jumpHoldTime > level.time)
		{
			trap_EA_Jump(bs->client);
			if (bs->wpCurrent)
			{
				if ((bs->wpCurrent->origin[2] - bs->origin[2]) < 64)
				{
					trap_EA_MoveForward(bs->client);
				}
			}
			else
			{
				trap_EA_MoveForward(bs->client);
			}
			if (g_entities[bs->client].client->ps.groundEntityNum == ENTITYNUM_NONE)
			{
				g_entities[bs->client].client->ps.pm_flags |= PMF_JUMP_HELD;
			}
		}
		else if (!(bs->cur_ps.pm_flags & PMF_JUMP_HELD))
		{
			trap_EA_Jump(bs->client);
		}
	}

	if (bs->duckTime > level.time)
	{
		trap_EA_Crouch(bs->client);
	}

	if ( bs->dangerousObject && bs->dangerousObject->inuse && bs->dangerousObject->health > 0 &&
		bs->dangerousObject->takedamage && (!bs->frame_Enemy_Vis || !bs->currentEnemy) &&
		(BotGetWeaponRange(bs) == BWEAPONRANGE_MID || BotGetWeaponRange(bs) == BWEAPONRANGE_LONG) &&
		bs->cur_ps.weapon != WP_DET_PACK && bs->cur_ps.weapon != WP_TRIP_MINE &&
		!bs->shootGoal )
	{
		float danLen;

		VectorSubtract(bs->dangerousObject->r.currentOrigin, bs->eye, a);

		danLen = VectorLength(a);

		if (danLen > 256)
		{
			vectoangles(a, a);
			VectorCopy(a, bs->goalAngles);

			if (Q_irand(1, 10) < 5)
			{
				bs->goalAngles[YAW] += Q_irand(0, 3);
				bs->goalAngles[PITCH] += Q_irand(0, 3);
			}
			else
			{
				bs->goalAngles[YAW] -= Q_irand(0, 3);
				bs->goalAngles[PITCH] -= Q_irand(0, 3);
			}

			if (InFieldOfVision(bs->viewangles, 30, a) &&
				EntityVisibleBox(bs->origin, NULL, NULL, bs->dangerousObject->r.currentOrigin, bs->client, bs->dangerousObject->s.number))
			{
				bs->doAttack = 1;
			}			
		}
	}

	if (PrimFiring(bs) ||
		AltFiring(bs))
	{
		friendInLOF = CheckForFriendInLOF(bs);

		if (friendInLOF)
		{
			if (PrimFiring(bs))
			{
				KeepPrimFromFiring(bs);
			}
			if (AltFiring(bs))
			{
				KeepAltFromFiring(bs);
			}
			if (useTheForce && forceHostile)
			{
				useTheForce = 0;
			}

			if (!useTheForce && friendInLOF->client)
			{ //we have a friend here and are not currently using force powers, see if we can help them out
				if (friendInLOF->health <= 50 && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_TEAM_HEAL]][FP_TEAM_HEAL])
				{
					level.clients[bs->client].ps.fd.forcePowerSelected = FP_TEAM_HEAL;
					useTheForce = 1;
					forceHostile = 0;
				}
				else if (friendInLOF->client->ps.fd.forcePower <= 50 && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_TEAM_FORCE]][FP_TEAM_FORCE])
				{
					level.clients[bs->client].ps.fd.forcePowerSelected = FP_TEAM_FORCE;
					useTheForce = 1;
					forceHostile = 0;
				}
			}
		}
	}
	else if (g_gametype.integer >= GT_TEAM)
	{ //still check for anyone to help..
		friendInLOF = CheckForFriendInLOF(bs);

		if (!useTheForce && friendInLOF)
		{
			if (friendInLOF->health <= 50 && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_TEAM_HEAL]][FP_TEAM_HEAL])
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_TEAM_HEAL;
				useTheForce = 1;
				forceHostile = 0;
			}
			else if (friendInLOF->client->ps.fd.forcePower <= 50 && level.clients[bs->client].ps.fd.forcePower > forcePowerNeeded[level.clients[bs->client].ps.fd.forcePowerLevel[FP_TEAM_FORCE]][FP_TEAM_FORCE])
			{
				level.clients[bs->client].ps.fd.forcePowerSelected = FP_TEAM_FORCE;
				useTheForce = 1;
				forceHostile = 0;
			}
		}
	}

	if (bs->doAttack && bs->cur_ps.weapon == WP_DET_PACK &&
		bs->cur_ps.hasDetPackPlanted)
	{ //maybe a bit hackish, but bots only want to plant one of these at any given time to avoid complications
		bs->doAttack = 0;
	}

	if (bs->doAttack && bs->cur_ps.weapon == WP_SABER &&
		bs->saberDefending && bs->currentEnemy && bs->currentEnemy->client &&
		BotWeaponBlockable(bs->currentEnemy->client->ps.weapon) )
	{
		bs->doAttack = 0;
	}

	if (bs->cur_ps.saberLockTime > level.time)
	{
		if (rand()%10 < 5)
		{
			bs->doAttack = 1;
		}
		else
		{
			bs->doAttack = 0;
		}
	}

	if (bs->botChallengingTime > level.time)
	{
		bs->doAttack = 0;
		bs->doAltAttack = 0;
	}

	if (bs->doAttack)
	{
		trap_EA_Attack(bs->client);
	}
	else if (bs->doAltAttack)
	{
		trap_EA_Alt_Attack(bs->client);
	}

	if (useTheForce && forceHostile && bs->botChallengingTime > level.time)
	{
		useTheForce = qfalse;
	}

	if (useTheForce)
	{
#ifndef FORCEJUMP_INSTANTMETHOD
		if (bs->forceJumpChargeTime > level.time)
		{
			level.clients[bs->client].ps.fd.forcePowerSelected = FP_LEVITATION;
			trap_EA_ForcePower(bs->client);
		}
		else
		{
#endif
			if (bot_forcepowers.integer && !g_forcePowerDisable.integer)
			{
				trap_EA_ForcePower(bs->client);
			}
#ifndef FORCEJUMP_INSTANTMETHOD
		}
#endif
	}

	MoveTowardIdealAngles(bs);
}
