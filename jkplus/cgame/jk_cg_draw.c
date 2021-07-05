/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Client draw module
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header
#include "../../code/ui/ui_shared.h"	// UI shared header

// Extern stuff
extern int trap_RealTime(qtime_t *qtime);
extern qboolean CG_WorldCoordToScreenCoord(vec3_t worldCoord, float *x, float *y);
extern qboolean CG_InRollAnim(centity_t *cent);

int jkmod_pause_count = 0; // Pause announce counter
int jkmod_popup_count = 0; // Client pop-up timer

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

	// Draw race timer
	if (jkcvar_cg_drawRaceTimer.integer)
	{
		JKMod_CG_DrawRaceTimer();
	}

	// Draw player label
	if (jkcvar_cg_drawPlayerNames.integer)
	{
		JKMod_CG_DrawPlayerLabels();
	}

	// Draw Jetpack Fuel
	if (cgs.jkmodCGS.jetPack == 1)
	{
		JKMod_CG_DrawJetPackFuel();
	}

	// Draw movement keys
	if (jkcvar_cg_drawMovementKeys.integer)
	{
		JKMod_CG_DrawMovementKeys(); 
	}

	// Draw Dimension indicator
	if (cg.snap->ps.stats[JK_DIMENSION] > 1)
	{
		JKMod_CG_DrawDimensionString();
	}

	// Draw pause text
	if (cgs.jkmodCGS.pauseTime > cg.snap->serverTime)
	{
		JKMod_CG_DrawPauseString();
	}
}

/*
=====================================================================
Check icon hud active (when cg_hudfiles is 1)
=====================================================================
*/
qboolean JKMod_CG_IconHUDActive(void)
{
	float inTime = cg.invenSelectTime+WEAPON_SELECT_TIME;
	float wpTime = cg.weaponSelectTime+WEAPON_SELECT_TIME;
	float fpTime = cg.forceSelectTime+WEAPON_SELECT_TIME;

	return (cg.iconHUDActive || cg_hudFiles.integer && (inTime > cg.time) || (wpTime > cg.time) || (fpTime > cg.time));
}

/*
=====================================================================
Check when center print is showing
=====================================================================
*/
qboolean JKMod_CG_CenterPrintActive(void)
{
	return ((cg.centerPrintTime + (1000 * cg_centertime.value)) >= cg.time);
}

/*
=====================================================================
Convert milliseconds to string
=====================================================================
*/
const char *JKMod_CG_MsToString(const int ms)
{
	int	timeSec, timeMin, timeMsec;

	timeMsec = ms;
	timeSec = timeMsec / 1000;
	timeMsec -= timeSec * 1000;
	timeMin = timeSec / 60;
	timeSec -= timeMin * 60;

	return !ms ? "00:00:000" : va("%02i:%02i:%03i", timeMin, timeSec, timeMsec);
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

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;

	trap_R_SetColor(colorTable[CT_WHITE]); // Don't tint hud
	trap_RealTime(&serverTime);
	serverTimeType = (serverTime.tm_hour > 11 && serverTime.tm_hour < 24) ? "pm" : "am";

	x = cgs.screenWidth - (cg_lagometer.integer ? 68 : 68);
	y = cgs.screenHeight - (cg_lagometer.integer ? 175 : 123);

	CG_DrawPic(x, y, 64, 32, cgs.jkmodMedia.clockBg);

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
Draw custom chat box
=====================================================================
*/

// Chatbox string insert
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

// Chatbox stirng add
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

	if (cg.jkmodCG.chatItemActive >= CHATBOX_ITEMS)
	{
		cg.jkmodCG.chatItemActive = 0;
	}
}

// Chatbox array insert
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

// Chatbox draw strings
void JKMod_CG_ChatBox_DrawStrings(void)
{
	jkmod_chatbox_t *drawThese[CHATBOX_ITEMS];
	int numToDraw = 0;
	int linesToDraw = 0;
	int i = 0;
	int x = 30;
	int y = cg.scoreBoardShowing ? 475 : jkcvar_cg_chatBoxHeight.integer;
	float fontScale = 0.65f;
	qboolean drawAnyway = qfalse;

	if (jkcvar_cg_chatBoxHistory.integer)
	{
		drawAnyway = (qboolean)(trap_Key_GetCatcher() & (jkcvar_cg_chatBoxHistory.integer == 2 ? (KEYCATCH_CONSOLE | KEYCATCH_MESSAGE) : KEYCATCH_CONSOLE));
	}

	memset(drawThese, 0, sizeof(drawThese));

	while (i < CHATBOX_ITEMS)
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
			JKMod_CG_ChatBox_ArrayInsert(drawThese, insertionPoint, CHATBOX_ITEMS, &cg.jkmodCG.chatItems[i]);
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

/*
=====================================================================
Client options pop-up
=====================================================================
*/
void JKMod_CG_ClientPopUp(void)
{
	if (!cgs.jkmodCGS.pluginRequired) return;
	if (!cg.snap) return;
	if (jkcvar_cg_clientPopUp.integer == jkcvar_cg_resetClient.integer) return;
	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (cg.snap->ps.pm_flags & PMF_FOLLOW) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;
	
	// Scan once every second
	if ((jkmod_popup_count + 1000) < cg.time)
	{
		jkmod_popup_count = cg.time;

		// Launch only when all center prints dissapear
		if (!JKMod_CG_CenterPrintActive())
		{
			trap_SendConsoleCommand("jk_ui_cmd_clientPopUp\n");
			trap_Cvar_Set("jk_cg_clientPopUp", va("%i", jkcvar_cg_resetClient.integer));
		}
	}
}

/*
=====================================================================
Draw inventory
=====================================================================
*/
void JKMod_CG_DrawInventory(int y)
{
	int i;
	int ico_size = 32;
	float xAlign = cgs.screenWidth - ico_size * 1.1f;

	if (!cg.snap) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;
	if (cg.snap->ps.stats[STAT_HEALTH] <= 0) return;

	y += 8;

	if (!cg.renderingThirdPerson && (cg.snap->ps.eFlags & JK_JETPACK_ACTIVE)) 
	{
		CG_DrawPic(xAlign, y, ico_size, ico_size, cgs.jkmodMedia.jetpackIcon);
		if (cg.snap->ps.eFlags & JK_JETPACK_FLAMING) CG_DrawPic(xAlign + 20, y - 2.5, 8, 8, cgs.jkmodMedia.dotRed);
		y += ico_size;
	}

	if (cg.snap->ps.stats[STAT_HOLDABLE_ITEM] || cg.snap->ps.stats[STAT_HOLDABLE_ITEMS])
	{
		for (i = 0; i < HI_NUM_HOLDABLE; i++)
		{
			if (i && cg.snap->ps.stats[STAT_HOLDABLE_ITEMS] & (1 << i) && i != HI_DATAPAD)
			{
				CG_DrawPic(xAlign, y, ico_size, ico_size, cgs.media.invenIcons[i]);
				y += ico_size;
			}
		}
	}
}

/*
=====================================================================
Custom draw powerup to work together with draw inventory
=====================================================================
*/
float JKMod_CG_DrawPowerupIcons(int y) 
{
	int j;
	int ico_size = 64;
	float xAlign = cgs.screenWidth - ico_size * 1.1f;
	float yAlign = 8;
	gitem_t	*item;

	if (!cg.snap) return y;
	if (cg.snap->ps.pm_type == PM_SPECTATOR)return y;

	if (jkcvar_cg_drawInventory.integer && !(cgs.gametype == GT_CTF || cgs.gametype == GT_CTY)) { 
		ico_size = ICON_SIZE * 1.25;
		xAlign = (xAlign - ico_size);
		yAlign = (ICON_SIZE / 2);
		y += 5;
	} else {
		y += 16;
	}

	for (j = 0; j < PW_NUM_POWERUPS; j++)
	{
		if (cg.snap->ps.powerups[j] > cg.time)
		{
			int secondsleft = (cg.snap->ps.powerups[j] - cg.time) / 1000;

			item = BG_FindItemForPowerup(j);

			if (item)
			{
				int icoShader = 0;
				if (cgs.gametype == GT_CTY && (j == PW_REDFLAG || j == PW_BLUEFLAG))
				{
					if (j == PW_REDFLAG) {
						icoShader = trap_R_RegisterShaderNoMip("gfx/hud/mpi_rflag_ys");
					} else {
						icoShader = trap_R_RegisterShaderNoMip("gfx/hud/mpi_bflag_ys");
					}
				}
				else
				{
					icoShader = trap_R_RegisterShader(item->icon);
				}

				CG_DrawPic(xAlign, y, ico_size, ico_size, icoShader);

				y += ico_size;

				if (j != PW_REDFLAG && j != PW_BLUEFLAG && secondsleft < 999)
				{
					UI_DrawProportionalString(xAlign + (ico_size / 2), y - yAlign, va("%i", secondsleft), UI_CENTER | UI_BIGFONT | UI_DROPSHADOW, colorTable[CT_WHITE]);
				}

				y += (ico_size / 3);
			}
		}
	}
	return y;
}

/*
=====================================================================
Draw jetpack fuel function
=====================================================================
*/
void JKMod_CG_DrawJetPackFuel(void)
{
	vec4_t aColor, bColor, cColor;
	int	x, y;
	int width = 20;
	int height = 100;
	float percent;

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (!(cg.snap->ps.eFlags & JK_JETPACK_ACTIVE))  return;
	if (!(cg.snap->ps.stats[JK_FUEL] < 100)) return;
	
	x = 5;
	y = (cgs.screenHeight / 2) - (height / 2);
	percent = ((float)cg.snap->ps.stats[JK_FUEL] / 100.0f)*height;

	if (percent < 5.0f) percent = 1.3f;

	// Fuel
	aColor[0] = 0.5f;
	aColor[1] = 0.0f;
	aColor[2] = 0.0f;
	aColor[3] = 0.8f;

	// Border
	bColor[0] = 0.0f;
	bColor[1] = 0.0f;
	bColor[2] = 0.0f;
	bColor[3] = 0.3f;

	// Background
	cColor[0] = 0.5f;
	cColor[1] = 0.5f;
	cColor[2] = 0.5f;
	cColor[3] = 0.2f;
	
	CG_DrawRect(x, y, width, height, 1.0f, colorTable[CT_BLACK]);
	CG_FillRect(x + 0.8f, y + 1.3f + (height - percent), width - 1.5f, height - 2.0f - (height - percent), aColor);
	CG_FillRect(x + 1.0f, y + 1.0f, width - 1.0f, height - percent, cColor);
}

/*
=====================================================================
Draw dimension indicator
=====================================================================
*/
void JKMod_CG_DrawDimensionString(void)
{
	const char *dimensionStr = NULL;

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (cg.scoreBoardShowing) return;
	if (JKMod_CG_IconHUDActive()) return;

	if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_DUEL) dimensionStr = "Private Duel";
	else if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_GUNS) dimensionStr = "Guns Arena";
	else if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_RACE) dimensionStr = "Race Defrag";
	else if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_SABER) dimensionStr = "Saber Only";
	else if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_INSTA) dimensionStr = "Insta Kill";
	else if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_CHEAT) dimensionStr = "Cheats Mode";

	if (dimensionStr) {
		if (cg_hudFiles.integer) {
			vec4_t textColor = { .875f, .718f, .121f, 1.0f };
			UI_DrawProportionalString(0.5f * cgs.screenWidth, cgs.screenHeight - 25, va("%s", dimensionStr), UI_CENTER | UI_SMALLFONT | UI_DROPSHADOW, textColor);
		} else {
			UI_DrawScaledProportionalString(101, cgs.screenHeight - 23, va("[%s]", dimensionStr), UI_LEFT | UI_DROPSHADOW, colorTable[CT_LTORANGE], 0.7);
		}
	}
}

/*
=====================================================================
Draw race timer
=====================================================================
*/
void JKMod_CG_DrawRaceTimer(void)
{
	int			w1, w2;
	int			timeMsec;
	const char	*t1, *t2;
	float		color[4];

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (cg.snap->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;
	if (cgs.jkmodCGS.pauseTime > cg.snap->serverTime) return;
	if (!cg.predictedPlayerState.duelTime) return;

	timeMsec = cg.time - cg.predictedPlayerState.duelTime;

	t1 = CG_GetStripEdString("MENUS3", "TIME");
	w1 = CG_DrawStrlen(t1) * SMALLCHAR_WIDTH;

	t2 = va("%s", JKMod_CG_MsToString(timeMsec));
	w2 = CG_DrawStrlen(t2) * BIGCHAR_WIDTH;

	color[0] = color[1] = color[2] = 1.0;
	color[3] = 1.0f;

	CG_DrawStringExt(0.5f * (cgs.screenWidth - w1), 35, t1, color, qfalse, qtrue, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0);
	CG_DrawStringExt(0.5f * (cgs.screenWidth - w2), 50, t2, color, qfalse, qtrue, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0);
}

/*
=====================================================================
Draw pause texts
=====================================================================
*/
void JKMod_CG_DrawPauseString(void)
{
	int			w1, w2;
	int			sec;
	const char	*t1, *t2;
	float		color[4];

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;

	color[0] = color[1] = color[2] = 1.0;
	color[3] = 1.0f;

	if (cgs.jkmodCGS.pauseTime != INT_MAX) 
	{
		sec = (cgs.jkmodCGS.pauseTime - cg.snap->serverTime) / 1000 + 1;

		t1 = CG_GetStripEdString("JKINGAME", "PAUSE_GAME");
		w1 = CG_DrawStrlen(t1) * SMALLCHAR_WIDTH;

		t2 = va(CG_GetStripEdString("JKINGAME", "PAUSE_GAME_SEC"), sec);
		w2 = CG_DrawStrlen(t2) * BIGCHAR_WIDTH;

		CG_DrawStringExt(0.5f * (cgs.screenWidth - w1), 35, t1, color, qfalse, qtrue, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0);
		CG_DrawStringExt(0.5f * (cgs.screenWidth - w2), 50, t2, color, qfalse, qtrue, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0);
		
		if (sec <= 3 && sec != jkmod_pause_count) 
		{
			jkmod_pause_count = sec;

			switch (sec) 
			{
				case 1: trap_S_StartLocalSound(cgs.media.count1Sound, CHAN_ANNOUNCER); break;
				case 2: trap_S_StartLocalSound(cgs.media.count2Sound, CHAN_ANNOUNCER); break;
				case 3: trap_S_StartLocalSound(cgs.media.count3Sound, CHAN_ANNOUNCER); break;
			}
		}
	}
	else 
	{
		t1 = CG_GetStripEdString("JKINGAME", "PAUSE_GAME");
		w1 = CG_DrawStrlen(t1) * BIGCHAR_WIDTH;

		CG_DrawStringExt(0.5f * (cgs.screenWidth - w1), 35, t1, color, qfalse, qtrue, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0);
	}
}

/*
=====================================================================
Draw player labels
=====================================================================
*/
void JKMod_CG_DrawPlayerLabels(void)
{
	int i;

	for (i = 0; i < MAX_CLIENTS; i++) {
		vec3_t		pos;
		float		x, y;
		trace_t		trace;
		centity_t	*cent = &cg_entities[i];
		vec3_t		diff;
		float		scale, max = 0.9;

		// Skip
		if (!cent || !cent->currentValid)
			continue;
		if (i == cg.clientNum)
			continue;
		if (i == cg.snap->ps.clientNum)
			continue;
		if (cent->currentState.eFlags & EF_DEAD)
			continue;
		if (cent->currentState.eType != ET_PLAYER)	
			continue;
		if (!cgs.clientinfo[i].infoValid)
			continue;
		if (cgs.clientinfo[i].team == TEAM_SPECTATOR)	
			continue;
		if (CG_IsMindTricked(cent->currentState.trickedentindex,
			cent->currentState.trickedentindex2,
			cent->currentState.trickedentindex3,
			cent->currentState.trickedentindex4,
			cg.snap->ps.clientNum))
			continue;

		// Max distance
		VectorSubtract(cent->lerpOrigin, cg.predictedPlayerState.origin, diff);
		if (VectorLength(diff) >= PLAYER_LABEL_DIST)
			continue;

		// Do trace
		CG_Trace( &trace, cg.predictedPlayerState.origin, NULL, NULL, cent->lerpOrigin, cg.clientNum, CONTENTS_SOLID|CONTENTS_BODY );
		if (trace.entityNum == ENTITYNUM_WORLD)
			continue;

		// Set height
		VectorCopy(cent->lerpOrigin, pos);
		pos[2] += 60;

		// Check coords
		if (!CG_WorldCoordToScreenCoord(pos, &x, &y))
			continue;

		// Set dynamic scale
		scale = 150 / VectorLength(diff);

		// Show name
		UI_DrawScaledProportionalString(x, y, cgs.clientinfo[i].name, UI_CENTER, colorTable[CT_WHITE], (scale > max ? max : scale));
	}
}

/*
=====================================================================
Draw movement keys
=====================================================================
*/
void JKMod_CG_DrawMovementKeys(void)
{
	centity_t *cent;
	usercmd_t cmd = { 0 };
	int moveDir;
	float w, h, x, y, xOffset, yOffset, scale;

	if (!cg.snap) return;
	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return; // Spec following is still allowed
	if (cg.scoreBoardShowing) return;
	if (jkcvar_cg_drawMovementKeys.integer == 2 && JKMod_CG_CenterPrintActive()) return;
	if (jkcvar_cg_drawMovementKeys.integer > 1 && (trap_Key_GetCatcher() & KEYCATCH_MESSAGE)) return;
	if (jkcvar_cg_drawMovementKeys.integer < 2 && JKMod_CG_IconHUDActive()) return;

	trap_R_SetColor(colorTable[CT_WHITE]); // Don't tint hud

	cent = &cg_entities[cg.predictedPlayerState.clientNum];
	moveDir = cg.snap->ps.movementDir;

	// If it's us
	if (cg.clientNum == cg.predictedPlayerState.clientNum && !cg.demoPlayback)
	{
		trap_GetUserCmd(trap_GetCurrentCmdNumber(), &cmd);

		if ((cg.snap->ps.pm_flags & PMF_DUCKED) || CG_InRollAnim(cent)) {
			cmd.upmove = -1;
		}
	}
	else
	{
		float xyspeed = (float)sqrt(cg.snap->ps.velocity[0] * cg.snap->ps.velocity[0] + cg.snap->ps.velocity[1] * cg.snap->ps.velocity[1]);
		float zspeed = cg.snap->ps.velocity[2];
		static float lastZSpeed = 0.0f;

		if ((JKMod_CG_GroundDistance() > 1 && zspeed > 8 && zspeed > lastZSpeed && !cg.snap->ps.fd.forceGripCripple) || (cg.snap->ps.pm_flags & PMF_JUMP_HELD)) {
			cmd.upmove = 1;
		} else if ((cg.snap->ps.pm_flags & PMF_DUCKED) || CG_InRollAnim(cent)) {
			cmd.upmove = -1;
		}

		if ((cent->currentState.eFlags & EF_FIRING) && !(cent->currentState.eFlags & EF_ALT_FIRING)) {
			cmd.buttons |= BUTTON_ATTACK;
			cmd.buttons &= ~BUTTON_ALT_ATTACK;
		} else if (cent->currentState.eFlags & EF_ALT_FIRING) {
			cmd.buttons |= BUTTON_ALT_ATTACK;
			cmd.buttons &= ~BUTTON_ATTACK;
		}

		if (xyspeed < 9) moveDir = -1;

		lastZSpeed = zspeed;

		switch (moveDir)
		{
			case 0: // W
				cmd.forwardmove = 1;
				break;
			case 1: // WA
				cmd.forwardmove = 1;
				cmd.rightmove = -1;
				break;
			case 2: // A
				cmd.rightmove = -1;
				break;
			case 3: // AS
				cmd.rightmove = -1;
				cmd.forwardmove = -1;
				break;
			case 4: // S
				cmd.forwardmove = -1;
				break;
			case 5: // SD
				cmd.forwardmove = -1;
				cmd.rightmove = 1;
				break;
			case 6: // D
				cmd.rightmove = 1;
				break;
			case 7: // DW
				cmd.rightmove = 1;
				cmd.forwardmove = 1;
				break;
			default:
				break;
		}
	}

	// Big
	if (jkcvar_cg_drawMovementKeys.integer == 3) 
	{
		x = 16;
		y = 44;
		scale = 2;
	} 
	// Medium
	else if (jkcvar_cg_drawMovementKeys.integer == 2) 
	{
		x = (0.5f * cgs.screenWidth) - 48;
		y = 215;
		scale = 1.5;
	} 
	// Small
	else 
	{
		x = cgs.screenWidth - 165;
		y = cg_hudFiles.integer ? 443 : 423;
		scale = 1;
	}

	w = 16 * scale;
	h = 16 * scale;;

	xOffset = 0;
	yOffset = 0;

	x += xOffset;
	y += yOffset;

	if (cmd.upmove < 0)
		CG_DrawPic(w * 2 + x, y, w, h, cgs.jkmodMedia.keyCrouchOn);
	else
		CG_DrawPic(w * 2 + x, y, w, h, cgs.jkmodMedia.keyCrouchOff);

	if (cmd.upmove > 0)
		CG_DrawPic(x, y, w, h, cgs.jkmodMedia.keyJumpOn);
	else
		CG_DrawPic(x, y, w, h, cgs.jkmodMedia.keyJumpOff);

	if (cmd.forwardmove < 0)
		CG_DrawPic(w + x, h + y, w, h, cgs.jkmodMedia.keyBackOn);
	else
		CG_DrawPic(w + x, h + y, w, h, cgs.jkmodMedia.keyBackOff);

	if (cmd.forwardmove > 0)
		CG_DrawPic(w + x, y, w, h, cgs.jkmodMedia.keyForwardOn);
	else
		CG_DrawPic(w + x, y, w, h, cgs.jkmodMedia.keyForwardOff);

	if (cmd.rightmove < 0)
		CG_DrawPic(x, h + y, w, h, cgs.jkmodMedia.keyLeftOn);
	else
		CG_DrawPic(x, h + y, w, h, cgs.jkmodMedia.keyLeftOff);

	if (cmd.rightmove > 0)
		CG_DrawPic(w * 2 + x, h + y, w, h, cgs.jkmodMedia.keyRightOn);
	else
		CG_DrawPic(w * 2 + x, h + y, w, h, cgs.jkmodMedia.keyRightOff);

	if (cmd.buttons & BUTTON_ATTACK)
		CG_DrawPic(w * 3 + x, y, w, h, cgs.jkmodMedia.keyAttackOn);
	else
		CG_DrawPic(w * 3 + x, y, w, h, cgs.jkmodMedia.keyAttackOff);

	if (cmd.buttons & BUTTON_ALT_ATTACK)
		CG_DrawPic(w * 3 + x, h + y, w, h, cgs.jkmodMedia.keyAltOn);
	else
		CG_DrawPic(w * 3 + x, h + y, w, h, cgs.jkmodMedia.keyAltOff);
}
