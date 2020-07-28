/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Client player module
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header
#include "../../code/ghoul2/g2.h"		// G2 header

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
	if (cent->currentState.number == cg.predictedPlayerState.clientNum && !cg.renderingThirdPerson) {
		return;
	}

	// Don't draw it for dead players
	if (cent->currentState.eFlags & EF_DEAD) {
		return;
	}

	// Don't draw if is mindtricked
	if (CG_IsMindTricked(cent->currentState.trickedentindex,
		cent->currentState.trickedentindex2,
		cent->currentState.trickedentindex3,
		cent->currentState.trickedentindex4,
		cg.snap->ps.clientNum)) {
		return;
	}

	// Get the shader handles
	hitboxShader = trap_R_RegisterShader("jkmod_hitbox");
	hitboxShader_nocull = trap_R_RegisterShader("jkmod_hitbox_nocull");

	// If they don't exist, forget it
	if (!hitboxShader || !hitboxShader_nocull) {
		return;
	}

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
	{
		return;
	}

	if (cent->currentState.eFlags & EF_DEAD)
	{
		return;
	}

	if (!cg.renderingThirdPerson && cent->currentState.clientNum == cg.clientNum)
	{
		return;
	}

	if (!cg.renderingThirdPerson && cg.snap->ps.clientNum == cent->currentState.clientNum && cgs.clientinfo[cg.clientNum].team == TEAM_SPECTATOR && (cg.snap->ps.pm_flags & PMF_FOLLOW))
	{
		return;
	}

	if ((cg.snap->ps.duelIndex && cg.snap->ps.duelInProgress && cg.snap->ps.duelIndex != cent->currentState.number && cent->currentState.clientNum != cg.clientNum) && jkcvar_cg_privateDuel.integer)
	{
		return;
	}

	if (CG_IsMindTricked(cent->currentState.trickedentindex, 
		cent->currentState.trickedentindex2, 
		cent->currentState.trickedentindex3, 
		cent->currentState.trickedentindex4, 
		cg.snap->ps.clientNum))
	{
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

		if (modelDetails[3] != 1) {
			re.modelScale[0] = modelDetails[3];
			re.modelScale[1] = modelDetails[3];
			re.modelScale[2] = modelDetails[3];
			ScaleModelAxis(&re);
		}

		// if (cent->currentState.isJediMaster) re.renderfx |= RF_DEPTHHACK;

		trap_R_AddRefEntityToScene(&re);
	}
}
