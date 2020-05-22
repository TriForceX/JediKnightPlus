/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Client draw module
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header
#include "../../code/ui/ui_shared.h"	// Original header

/*
=====================================================================
Custom draw functions
=====================================================================
*/
void JKMod_CG_Draw2D(void)
{
	// Draw clock
	if (jkcvar_cg_drawClock.integer)
	{
		JKMod_CG_DrawClock();
	}

	// Draw chat box
	if (jkcvar_cg_chatBox.integer)
	{
		JKMod_CG_ChatBox_DrawStrings();
	}
}

/*
=====================================================================
Draw clock function
=====================================================================
*/
void JKMod_CG_DrawClock(void)
{
	qtime_t		serverTime;
	char		*serverTimeType;
	int			x, y;

	trap_RealTime(&serverTime);
	serverTimeType = (serverTime.tm_hour > 11 && serverTime.tm_hour < 24) ? "pm" : "am";

	x = cgs.screenWidth - 66;
	y = cgs.screenHeight - 178;

	CG_DrawPic(x, y, 64, 32, trap_R_RegisterShaderNoMip("gfx/hud/jkmod_clock_bg"));

	CG_Text_Paint(x + 8, y + 4, 0.85f, colorTable[CT_HUD_GREEN], va("%02i", serverTime.tm_hour), 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);
	CG_Text_Paint(x + 29, y + 4, 0.85f, colorTable[CT_HUD_GREEN], va("%02i", serverTime.tm_min), 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);
	CG_Text_Paint(x + 47, y + 13.5f, 0.4f, colorTable[CT_HUD_GREEN], va("%s", serverTimeType), 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);

	if (jkcvar_cg_drawClock.integer == 2) 
	{
		if((cg.time >> 9) & 1) CG_Text_Paint(x + 24, y + 6, 0.7f, colorTable[CT_HUD_GREEN], ":", 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);
		CG_Text_Paint(x + 47, y + 8.5f, 0.4f, colorTable[CT_HUD_GREEN], va("%02i", serverTime.tm_sec), 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);
	}
	else 
	{
		CG_Text_Paint(x + 24, y + 6, 0.7f, colorTable[CT_HUD_GREEN], ":", 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);
	}
}

/*
=====================================================================
Draw player bounding hit boxes
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
		maxs[2] = cg.predictedPlayerState.viewheight + 5;
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
Draw custom chat box
=====================================================================
*/
void JKMod_CG_ChatBox_StrInsert(char *buffer, int place, char *str)
{
	int insLen = strlen(str);
	int i = strlen(buffer);
	int k = 0;

	buffer[i + insLen + 1] = 0; // Terminate the string at its new length
	while (i >= place)
	{
		buffer[i + insLen] = buffer[i];
		i--;
	}

	i++;
	while (k < insLen)
	{
		buffer[i] = str[k];
		i++;
		k++;
	}
}

void JKMod_CG_ChatBox_AddString(char *chatStr)
{
	jkmod_chatbox_t *chat = &cg.jkmodCG.chatItems[cg.jkmodCG.chatItemActive];
	float chatLen;

	if (jkcvar_cg_chatBoxTime.integer <= 0) // Don't bother then.
	{ 
		return;
	}

	memset(chat, 0, sizeof(jkmod_chatbox_t));

	if (strlen(chatStr) > sizeof(chat->string)) // Too long, terminate at proper len.
	{ 
		chatStr[sizeof(chat->string) - 1] = 0;
	}

	strcpy(chat->string, chatStr);
	chat->time = cg.time + (jkcvar_cg_chatBoxTime.integer*1000);

	chat->lines = 1;
	chatLen = CG_Text_Width(chat->string, 1.0f, FONT_SMALL);

	if (chatLen > CHATBOX_CUTOFF_LEN) // We have to break it into segments...
	{ 
		int i = 0;
		int lastLinePt = 0;
		char s[2];

		chatLen = 0;
		while (chat->string[i])
		{
			s[0] = chat->string[i];
			s[1] = 0;
			chatLen += CG_Text_Width(s, 0.65f, FONT_SMALL);

			if (chatLen >= CHATBOX_CUTOFF_LEN)
			{
				int j = i;
				while (j > 0 && j > lastLinePt)
				{
					if (chat->string[j] == ' ')
					{
						break;
					}
					j--;
				}
				if (chat->string[j] == ' ')
				{
					i = j;
				}

				chat->lines++;
				JKMod_CG_ChatBox_StrInsert(chat->string, i, "\n");
				i++;
				chatLen = 0;
				lastLinePt = i + 1;
			}
			i++;
		}
	}

	cg.jkmodCG.chatItemActive++;
	if (cg.jkmodCG.chatItemActive >= MAX_CHATBOX_ITEMS)
	{
		cg.jkmodCG.chatItemActive = 0;
	}
}

void JKMod_CG_ChatBox_ArrayInsert(jkmod_chatbox_t **array, int insPoint, int maxNum, jkmod_chatbox_t *item)
{
	if (array[insPoint]) // Recursively call, to move everything up to the top
	{ 
		if (insPoint + 1 >= maxNum)
		{
			CG_Error("JKMod_CG_ChatBox_ArrayInsert: Exceeded array size");
		}
		JKMod_CG_ChatBox_ArrayInsert(array, insPoint + 1, maxNum, array[insPoint]);
	}

	// Now that we have moved anything that would be in this slot up, insert what we want into the slot
	array[insPoint] = item;
}

void JKMod_CG_ChatBox_DrawStrings(void)
{
	jkmod_chatbox_t *drawThese[MAX_CHATBOX_ITEMS];
	int numToDraw = 0;
	int linesToDraw = 0;
	int i = 0;
	int x = 30;
	int y = cg.scoreBoardShowing ? 475 : jkcvar_cg_chatBoxHeight.integer;
	float fontScale = 0.65f;
	qboolean drawAnyway = qfalse;

	if (jkcvar_cg_chatBoxHistory.integer)
	{
		drawAnyway = (qboolean)(trap_Key_GetCatcher() & KEYCATCH_CONSOLE);
	}

	memset(drawThese, 0, sizeof(drawThese));

	while (i < MAX_CHATBOX_ITEMS)
	{
		if (cg.jkmodCG.chatItems[i].time >= cg.time || drawAnyway)
		{
			int check = numToDraw;
			int insertionPoint = numToDraw;

			while (check >= 0)
			{
				if (drawThese[check] &&
					cg.jkmodCG.chatItems[i].time < drawThese[check]->time)
				{ 
					// Insert here
					insertionPoint = check;
				}
				check--;
			}
			JKMod_CG_ChatBox_ArrayInsert(drawThese, insertionPoint, MAX_CHATBOX_ITEMS, &cg.jkmodCG.chatItems[i]);
			numToDraw++;
			linesToDraw += cg.jkmodCG.chatItems[i].lines;
		}
		i++;
	}

	if (!numToDraw) // Nothing, then, just get out of here now.
	{ 
		return;
	}

	// Move initial point up so we draw bottom-up (visually)
	y -= (CHATBOX_FONT_HEIGHT*fontScale)*linesToDraw;

	// We have the items we want to draw, just quickly loop through them now
	i = 0;
	while (i < numToDraw)
	{
		CG_Text_Paint(x, y, fontScale, colorWhite, drawThese[i]->string, 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL);
		y += ((CHATBOX_FONT_HEIGHT*fontScale)*drawThese[i]->lines);
		i++;
	}
}
