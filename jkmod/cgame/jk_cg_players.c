/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Client player module
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header
#include "../../code/ghoul2/g2.h"		// G2 header

// Extern stuff
extern qboolean CG_InRollAnim(centity_t *cent);
extern void CG_ForcePushBlur(vec3_t org);

/*
=====================================================================
Custom player functions
=====================================================================
*/
void JKMod_CG_Player(centity_t *cent)
{
	// Draw the hitbox
	if (jkcvar_cg_drawHitBox.integer)
	{
		JKMod_CG_AddHitBox(cent);
	}

	// Fullbody push effect
	if (jkcvar_cg_customEffects.integer)
	{
		JKMod_CG_ForcePushBodyBlur(cent);
	}

	// Render bacta model
	if (jkcvar_cg_drawBactaModel.integer)
	{
		if (cent->currentState.number == cg.predictedPlayerState.clientNum && (cg.snap->ps.stats[STAT_HOLDABLE_ITEMS] & (1 << HI_MEDPAC)))
		{
			vec4_t jkmod_itemModelDetails = { 0, 0, -5, 0.5 };
			JKMod_CG_AddModelOnPlayer(cent, cg.time, cgs.gameModels, cgs.jkmodMedia.bactaModel, "*hip_bl", jkmod_itemModelDetails);
		}
	}

	// Render custom hats models
	if (cgs.jkmodCGS.customHats)
	{
		qhandle_t	jkmod_hatFile = 0;
		vec4_t		jkmod_hatDetails = { -0.3, 0, -2, 1 };

		switch (cgs.clientinfo[cent->currentState.number].jkmod_hat) 
		{
			case 1: jkmod_hatFile = cgs.jkmodMedia.hatSanta; break;
			case 2: jkmod_hatFile = cgs.jkmodMedia.hatPumpkin; break;
			case 3: jkmod_hatFile = cgs.jkmodMedia.hatCap; break;
			case 4: jkmod_hatFile = cgs.jkmodMedia.hatCowboy; break;
			case 5: jkmod_hatFile = cgs.jkmodMedia.hatCringe; break;
			case 6: jkmod_hatFile = cgs.jkmodMedia.hatSombrero; break;
			case 7: jkmod_hatFile = cgs.jkmodMedia.hatGentleman; break;
			case 8: jkmod_hatFile = cgs.jkmodMedia.hatPirate; break;
			case 9: jkmod_hatFile = cgs.jkmodMedia.hatProbe; break;
			case 10: jkmod_hatFile = cgs.jkmodMedia.hatDroid; break;
			case 11: jkmod_hatFile = cgs.jkmodMedia.hatYsalamiri; break;
		}

		// Special cases
		if (jkmod_hatFile == cgs.jkmodMedia.hatYsalamiri) {
			jkmod_hatDetails[2] = -1;
			jkmod_hatDetails[3] = 0.5;
		}

		if (jkmod_hatFile == cgs.jkmodMedia.hatDroid) {
			jkmod_hatDetails[0] = -0.8;
			jkmod_hatDetails[2] = -1;
			jkmod_hatDetails[3] = 0.5;
		}

		if (jkmod_hatFile == cgs.jkmodMedia.hatProbe) {
			jkmod_hatDetails[2] = 0;
			jkmod_hatDetails[3] = 0.2;
		}

		if (!Q_stricmp(cgs.clientinfo[cent->currentState.number].modelName, "desann"))  {
			jkmod_hatDetails[0] += 2.5;
			jkmod_hatDetails[2] -= 0.5;
		}

		// Show hat
		if (jkmod_hatFile) JKMod_CG_AddModelOnPlayer(cent, cg.time, cgs.gameModels, jkmod_hatFile, "*head_top", jkmod_hatDetails);
	}

	// Render jetpack model
	if (cent->currentState.eFlags & JK_JETPACK_ACTIVE)
	{
		vec4_t jkmod_jetPackDetails = { 0, 0, 0, 1 };
		JKMod_CG_AddModelOnPlayer(cent, cg.time, cgs.gameModels, cgs.jkmodMedia.jetpackModel, "*chestg", jkmod_jetPackDetails);
	}

	// Render saber model
	if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
	{
		vec4_t jkmod_saberDetails = { 0, 0, -1.5, 0 };
		JKMod_CG_AddModelOnPlayer(cent, cg.time, cgs.gameModels, cgs.jkmodMedia.saberModel, "*hip_r", jkmod_saberDetails);
	}

	// Play custom animations
	if (jkcvar_cg_customAnims.integer)
	{
		// Running
		if (cent->currentState.weapon == WP_SABER && !cent->currentState.saberInFlight && !cent->currentState.shouldtarget > 0 && !cent->currentState.bolt2)
		{
			if ((cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN1) cent->currentState.torsoAnim = BOTH_RUN2;
		}
		// Thermal detonator
		if (cent->currentState.weapon == WP_THERMAL)
		{
			if ((cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) == TORSO_WEAPONREADY10) cent->currentState.torsoAnim = TORSO_WEAPONIDLE10;
		}
		// Lightning force
		if (cent->currentState.forcePowersActive & (1 << FP_LIGHTNING))
		{
			if ((cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_FORCEGRIP_HOLD) cent->currentState.torsoAnim = BOTH_FORCELIGHTNING_HOLD;
		}
		// Mind trick force
		if (cent->currentState.forcePowersActive & (1 << FP_TELEPATHY))
		{
			if ((cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_FORCEPUSH) cent->currentState.torsoAnim = BOTH_MINDTRICK1;
		}
	}

	// Dual saber custom animation
	if ((cg.snap->ps.stats[JK_MOVEMENT] & JK_DUAL_MOVES) && cent->currentState.weapon == WP_SABER && !cent->currentState.saberInFlight && !cent->currentState.shouldtarget && cent->currentState.bolt2)
	{
		if ((cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN1) cent->currentState.torsoAnim = BOTH_T1_T__BR;
	}
}

/*
=====================================================================
Show player bounding hit boxes
=====================================================================
*/
void JKMod_CG_AddHitBox(centity_t *cent)
{
	polyVert_t verts[4];
	clientInfo_t *ci;
	int i;
	vec3_t mins = { -15, -15, -24 };
	vec3_t maxs = { 15, 15, 32 };
	float extx, exty, extz;
	vec3_t corners[8];
	qhandle_t hitboxShader, hitboxShader_nocull;

	// Don't draw it if it's us in first-person
	if (cent->currentState.number == cg.predictedPlayerState.clientNum && !cg.renderingThirdPerson) return;

	// Don't draw it for dead players
	if (cent->currentState.eFlags & EF_DEAD) return;

	// Don't draw if is mindtricked
	if (CG_IsMindTricked(cent->currentState.trickedentindex,
		cent->currentState.trickedentindex2,
		cent->currentState.trickedentindex3,
		cent->currentState.trickedentindex4,
		cg.snap->ps.clientNum)) {
		return;
	}

	// Get the shader handles
	hitboxShader = cgs.jkmodMedia.hitBox;
	hitboxShader_nocull = cgs.jkmodMedia.hitBoxNoCull;

	// Get the player's client info
	ci = &cgs.clientinfo[cent->currentState.clientNum];

	// If it's us
	if (cent->currentState.number == cg.predictedPlayerState.clientNum)
	{
		maxs[2] = (CG_InRollAnim(cent) ? CROUCH_MAXS_2 : cg.predictedPlayerState.viewheight) + 5;
	}
	else
	{
		int x, zd, zu;

		// Otherwise grab the encoded hit box
		x = (cent->currentState.solid & 255);
		zd = ((cent->currentState.solid >> 8) & 255);
		zu = ((cent->currentState.solid >> 16) & 255) - 32;

		mins[0] = mins[1] = -x;
		maxs[0] = maxs[1] = x;
		mins[2] = -zd;
		maxs[2] = zu;
	}

	// Get the extents (size)
	extx = maxs[0] - mins[0];
	exty = maxs[1] - mins[1];
	extz = maxs[2] - mins[2];


	// Set the polygon's texture coordinates
	verts[0].st[0] = 0;
	verts[0].st[1] = 0;
	verts[1].st[0] = 0;
	verts[1].st[1] = 1;
	verts[2].st[0] = 1;
	verts[2].st[1] = 1;
	verts[3].st[0] = 1;
	verts[3].st[1] = 0;

	// Set the polygon's vertex colors
	if (ci->team == TEAM_RED) {
		for (i = 0; i < 4; i++) {
			verts[i].modulate[0] = 160;
			verts[i].modulate[1] = 0;
			verts[i].modulate[2] = 0;
			verts[i].modulate[3] = 255;
		}
	}
	else if (ci->team == TEAM_BLUE) {
		for (i = 0; i < 4; i++) {
			verts[i].modulate[0] = 0;
			verts[i].modulate[1] = 0;
			verts[i].modulate[2] = 192;
			verts[i].modulate[3] = 255;
		}
	}
	else {
		for (i = 0; i < 4; i++) {
			verts[i].modulate[0] = 0;
			verts[i].modulate[1] = 128;
			verts[i].modulate[2] = 0;
			verts[i].modulate[3] = 255;
		}
	}

	VectorAdd(cent->lerpOrigin, maxs, corners[3]);

	VectorCopy(corners[3], corners[2]);
	corners[2][0] -= extx;

	VectorCopy(corners[2], corners[1]);
	corners[1][1] -= exty;

	VectorCopy(corners[1], corners[0]);
	corners[0][0] += extx;

	for (i = 0; i < 4; i++) {
		VectorCopy(corners[i], corners[i + 4]);
		corners[i + 4][2] -= extz;
	}

	// Top
	VectorCopy(corners[0], verts[0].xyz);
	VectorCopy(corners[1], verts[1].xyz);
	VectorCopy(corners[2], verts[2].xyz);
	VectorCopy(corners[3], verts[3].xyz);
	trap_R_AddPolyToScene(hitboxShader, 4, verts);

	// Bottom
	VectorCopy(corners[7], verts[0].xyz);
	VectorCopy(corners[6], verts[1].xyz);
	VectorCopy(corners[5], verts[2].xyz);
	VectorCopy(corners[4], verts[3].xyz);
	trap_R_AddPolyToScene(hitboxShader, 4, verts);

	// Top side
	VectorCopy(corners[3], verts[0].xyz);
	VectorCopy(corners[2], verts[1].xyz);
	VectorCopy(corners[6], verts[2].xyz);
	VectorCopy(corners[7], verts[3].xyz);
	trap_R_AddPolyToScene(hitboxShader_nocull, 4, verts);

	// Left side
	VectorCopy(corners[2], verts[0].xyz);
	VectorCopy(corners[1], verts[1].xyz);
	VectorCopy(corners[5], verts[2].xyz);
	VectorCopy(corners[6], verts[3].xyz);
	trap_R_AddPolyToScene(hitboxShader_nocull, 4, verts);

	// Right side
	VectorCopy(corners[0], verts[0].xyz);
	VectorCopy(corners[3], verts[1].xyz);
	VectorCopy(corners[7], verts[2].xyz);
	VectorCopy(corners[4], verts[3].xyz);
	trap_R_AddPolyToScene(hitboxShader_nocull, 4, verts);

	// Bottom side
	VectorCopy(corners[1], verts[0].xyz);
	VectorCopy(corners[0], verts[1].xyz);
	VectorCopy(corners[4], verts[2].xyz);
	VectorCopy(corners[5], verts[3].xyz);
	trap_R_AddPolyToScene(hitboxShader_nocull, 4, verts);
}

/*
=====================================================================
Show custom model on player
=====================================================================
*/
void JKMod_CG_AddModelOnPlayer(centity_t *cent, int time, qhandle_t *gameModels, qhandle_t modelFile,  char *modelBolt, vec4_t modelDetails)
{
	int newBolt;
	mdxaBone_t matrix;
	vec3_t boltOrg, bAngles;
	refEntity_t re;

	if (!cent->ghoul2) 
		return;
	if (cent->currentState.eFlags & EF_DEAD) 
		return;
	if (!cg.renderingThirdPerson && cent->currentState.clientNum == cg.clientNum)
		return;
	if (!cg.renderingThirdPerson && cg.snap->ps.clientNum == cent->currentState.clientNum && cgs.clientinfo[cg.clientNum].team == TEAM_SPECTATOR && (cg.snap->ps.pm_flags & PMF_FOLLOW))
		return;
	if (cg.snap->ps.duelIndex && cg.snap->ps.duelInProgress && cg.snap->ps.duelIndex != cent->currentState.number && cent->currentState.clientNum != cg.clientNum)
		return;
	if (cent->currentState.weapon == WP_EMPLACED_GUN) // Fix me
		return;
	if (CG_IsMindTricked(cent->currentState.trickedentindex, 
		cent->currentState.trickedentindex2, 
		cent->currentState.trickedentindex3, 
		cent->currentState.trickedentindex4, 
		cg.snap->ps.clientNum)) {
		return;
	}

	newBolt = trap_G2API_AddBolt(cent->ghoul2, 0, modelBolt);

	if (newBolt != -1)
	{
		memset(&re, 0, sizeof(refEntity_t));

		VectorCopy(cent->lerpAngles, bAngles);
		bAngles[PITCH] = (jk2gameplay == VERSION_1_02) ? cent->turAngles[PITCH] : 0;
		bAngles[YAW] = cent->turAngles[YAW];
		bAngles[ROLL] = 0;

		trap_G2API_GetBoltMatrix(cent->ghoul2, 0, newBolt, &matrix, bAngles, cent->lerpOrigin, time, gameModels, cent->modelScale);
		trap_G2API_GiveMeVectorFromMatrix(&matrix, ORIGIN, boltOrg);
		trap_G2API_GiveMeVectorFromMatrix(&matrix, POSITIVE_X, re.axis[0]);
		trap_G2API_GiveMeVectorFromMatrix(&matrix, POSITIVE_Y, re.axis[1]);
		trap_G2API_GiveMeVectorFromMatrix(&matrix, POSITIVE_Z, re.axis[2]);
		VectorMA(boltOrg, modelDetails[0], re.axis[0], boltOrg);
		VectorMA(boltOrg, modelDetails[1], re.axis[1], boltOrg);
		VectorMA(boltOrg, modelDetails[2], re.axis[2], boltOrg);

		re.hModel = modelFile;
		VectorCopy(boltOrg, re.lightingOrigin);
		VectorCopy(boltOrg, re.origin);

		if (modelDetails[3] != 1) 
		{
			re.modelScale[0] = modelDetails[3];
			re.modelScale[1] = modelDetails[3];
			re.modelScale[2] = modelDetails[3];
			ScaleModelAxis(&re);
		}

		// Check chat player transparency
		if ((cent->currentState.eFlags & EF_TALK) && (cent->currentState.eFlags & JK_PASS_THROUGH) && jkcvar_cg_chatPlayerOpacity.integer)
		{
			if (!JKMod_CG_EmoteUI())
			{
				re.renderfx |= RF_FORCE_ENT_ALPHA;
				re.shaderRGBA[3] = 100; // Fixed num ?
			}
		}

		// Special case
		if (modelFile == cgs.jkmodMedia.jetpackModel)
		{
			vec3_t flamePos, flameDir;
			vec3_t flamePos2, flameDir2;

			trap_G2API_GiveMeVectorFromMatrix(&matrix, ORIGIN, flamePos);
			trap_G2API_GiveMeVectorFromMatrix(&matrix, ORIGIN, flamePos2);

			trap_G2API_GiveMeVectorFromMatrix(&matrix, NEGATIVE_Y, flameDir);
			VectorMA(flamePos, -10, flameDir, flamePos);
			trap_G2API_GiveMeVectorFromMatrix(&matrix, POSITIVE_X, flameDir);
			VectorMA(flamePos, -6, flameDir, flamePos);
			trap_G2API_GiveMeVectorFromMatrix(&matrix, POSITIVE_Z, flameDir);
			VectorMA(flamePos, -6, flameDir, flamePos);

			trap_G2API_GiveMeVectorFromMatrix(&matrix, POSITIVE_Y, flameDir2);
			VectorMA(flamePos2, -10, flameDir2, flamePos2);
			trap_G2API_GiveMeVectorFromMatrix(&matrix, POSITIVE_X, flameDir2);
			VectorMA(flamePos2, -6, flameDir2, flamePos2);
			trap_G2API_GiveMeVectorFromMatrix(&matrix, POSITIVE_Z, flameDir2);
			VectorMA(flamePos2, -6, flameDir2, flamePos2);

			if (cent->currentState.eFlags & JK_JETPACK_FLAMING)
			{
				trap_FX_PlayEffectID(cgs.jkmodMedia.jetpackActive, flamePos, flameDir);
				trap_FX_PlayEffectID(cgs.jkmodMedia.jetpackActive, flamePos2, flameDir2);

				trap_S_AddLoopingSound(cent->currentState.number, cent->lerpOrigin, vec3_origin, cgs.jkmodMedia.jetpackActiveSound);
			}
			else if (jkcvar_cg_jetPackIdle.integer)
			{
				trap_FX_PlayEffectID(cgs.jkmodMedia.jetpackIdle, flamePos, flameDir);
				trap_FX_PlayEffectID(cgs.jkmodMedia.jetpackIdle, flamePos2, flameDir2);

				trap_S_AddLoopingSound(cent->currentState.number, cent->lerpOrigin, vec3_origin, cgs.jkmodMedia.jetpackIdleSound);
			}
		}

		// if (cent->currentState.isJediMaster) re.renderfx |= RF_DEPTHHACK;

		trap_R_AddRefEntityToScene(&re);
	}
}

/*
=====================================================================
Duel end check
=====================================================================
*/
void JKMod_CG_DuelEnd(qboolean winner) 
{
	if (!jkcvar_cg_duelEnd.integer) return;
	if (cg.jkmodCG.duelEndLead < cg.time) CG_AddBufferedSound(winner ? cgs.media.winnerSound : cgs.media.loserSound);

	cg.jkmodCG.duelEnd = qtrue;
	cg.jkmodCG.duelEndWinner = winner;
	cg.jkmodCG.duelEndLead = 0;
	cg.jkmodCG.duelEndRange = cg_thirdPersonRange.value;

	JKMod_CG_Printf(S_COLOR_YELLOW "Client %i duel %s\n", cg.snap->ps.clientNum, (winner ? "win" : "lose"));
}

/*
=====================================================================
Check player emote UI
=====================================================================
*/
qboolean JKMod_CG_EmoteUI(void)
{
	return (trap_Key_GetCatcher() & KEYCATCH_UI) && cg.jkmodCG.emoteCamera ? qtrue : qfalse;
}

/*
=====================================================================
Check player ground distance
=====================================================================
*/
float JKMod_CG_GroundDistance(void)
{
	trace_t tr;
	vec3_t down;

	VectorCopy(cg.predictedPlayerState.origin, down);
	down[2] -= 4096;
	CG_Trace(&tr, cg.predictedPlayerState.origin, NULL, NULL, down, cg.predictedPlayerState.clientNum, MASK_SOLID);
	VectorSubtract(cg.predictedPlayerState.origin, tr.endpos, down);

	return VectorLength(down) - 24.0f;
}

/*
=====================================================================
Check player speed
=====================================================================
*/
void JKMod_CG_CalculateSpeed(centity_t* cent)
{
	const vec_t* const velocity = (cent->currentState.clientNum == cg.clientNum ? cg.predictedPlayerState.velocity : cent->currentState.pos.trDelta);
	cg.jkmodCG.currentSpeed = (float)sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1]);
}

/*
=====================================================================
Force push body effect
=====================================================================
*/
void JKMod_CG_ForcePushBodyBlur(centity_t *cent)
{
	vec3_t fxOrg;
	mdxaBone_t	boltMatrix;
	int bolt;
	int i;
	const char *cg_pushBoneNames[] =
	{
		"cranium",
		"lower_lumbar",
		"rhand",
		"lhand",
		"ltibia",
		"rtibia",
		"lradius",
		"rradius",
		NULL
	};

	if (!(cent->currentState.eFlags & JK_BODY_PUSH)) return;

	if (cg.snap &&
		CG_IsMindTricked(cent->currentState.trickedentindex,
		cent->currentState.trickedentindex2,
		cent->currentState.trickedentindex3,
		cent->currentState.trickedentindex4,
		cg.snap->ps.clientNum))
	{
		return; // this entity is mind-tricking the current client, so don't render it
	}

	assert(cent->ghoul2);

	// go through all the bones we want to put a blur effect on
	for (i = 0; cg_pushBoneNames[i]; i++)
	{
		bolt = trap_G2API_AddBolt(cent->ghoul2, 0, cg_pushBoneNames[i]);

		if (bolt == -1)
		{
			assert(!"You've got an invalid bone/bolt name in cg_pushBoneNames");
			continue;
		}

		trap_G2API_GetBoltMatrix(cent->ghoul2, 0, bolt, &boltMatrix, cent->turAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
		JKMod_BG_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, fxOrg);

		// standard effect, don't be refractive (for now)
		CG_ForcePushBlur(fxOrg);
	}
}
