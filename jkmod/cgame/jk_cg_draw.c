/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
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
	centity_t *cent = &cg_entities[cg.snap->ps.clientNum];

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

	// Draw movement keys
	if (jkcvar_cg_drawMovementKeys.integer)
	{
		JKMod_CG_DrawMovementKeys(); 
	}

	// Draw Jetpack Fuel
	if (cent->currentState.eFlags & JK_JETPACK_ACTIVE)
	{
		JKMod_CG_DrawJetPackFuel();
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

	// Server game check
	if (!strstr(cgs.jkmodCGS.gameName, JK_SHORTNAME))
	{
		CG_CenterPrint(CG_GetStripEdString("JKINGAME", "MOD_ALERT"), cgs.screenHeight * .30, 0);
	}

	// Server mod check
	if (!cgs.jkmodCGS.pluginCheck && (trap_Key_GetCatcher() & KEYCATCH_UI))
	{
		float color[4] = { 1, 1, 1, 0.4 };
		UI_DrawScaledProportionalString(0.5f * cgs.screenWidth, cgs.screenHeight-15, CG_GetStripEdString("JKINGAME", "MOD_MISMATCH"), UI_CENTER, color, 0.6);
	}

	// Calculate speed
	if (jkcvar_cg_strafeHelper.integer || jkcvar_cg_speedMeter.integer & SMETER_ENABLE)
	{
		JKMod_CG_CalculateSpeed(cent);
	}

	// Strafe helper
	if (jkcvar_cg_strafeHelper.integer)
	{
		JKMod_CG_StrafeHelper(cent);

		if (jkcvar_cg_strafeHelper.integer & SHELPER_CROSSHAIR)
		{
			vec4_t	hcolor = { 1.0f, 1.0f, 1.0f, 1.0f };
			float	lineWidth;

			lineWidth = jkcvar_cg_sHelperLineWidth.value;

			if (lineWidth < 0.25f) lineWidth = 0.25f;
			else if (lineWidth > 5) lineWidth = 5;

			JKMod_CG_StrafeHelperDrawLine(cgs.screenWidth / 2, (SCREEN_HEIGHT / 2) - 5, cgs.screenWidth / 2, (SCREEN_HEIGHT / 2) + 5, lineWidth, hcolor, hcolor[3], 0); // 640x480, 320x240
		}
	}

	// Speed meter
	if (jkcvar_cg_speedMeter.integer & SMETER_ENABLE)
	{
		if ((jkcvar_cg_speedMeter.integer & SMETER_ACCELMETER) || (jkcvar_cg_strafeHelper.integer & SHELPER_ACCELMETER)) JKMod_CG_SpeedMeterAccel();
		if (jkcvar_cg_speedMeter.integer & SMETER_JUMPHEIGHT) JKMod_CG_SpeedMeterJumpHeight(cent);
		if (jkcvar_cg_speedMeter.integer & SMETER_JUMPDISTANCE) JKMod_CG_SpeedMeterJumpDistance();
		if (jkcvar_cg_speedMeter.integer & SMETER_VERTICALSPEED) JKMod_CG_SpeedMeterVerticalSpeed();

		JKMod_CG_SpeedMeter();
	}
		

	// Update console print lines
	if (cg.jkmodCG.consolePrint > 3) cg.jkmodCG.consolePrint = 3;
	if (cg.jkmodCG.consolePrint && (cg.jkmodCG.consolePrintTime + (cg.jkmodCG.consoleNotifyTime * 1000)) < cg.time) cg.jkmodCG.consolePrint = 0;
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
Check leading color code
=====================================================================
*/
qboolean JKMod_CG_HasLeadingColorCode(char *msg) 
{
	if (msg[0] == '^') {
#if 0
	if (msg[1] == '1' || msg[1] == '2' || msg[1] == '3' || msg[1] == '4' || msg[1] == '5' || msg[1] == '6' || msg[1] == '7' || msg[1] == '8' || msg[1] == '9' || msg[1] == '0') {
#else
	if (msg[1] >= '0' && msg[1] <= '9') {
#endif
		return qtrue;
	} }
	return qfalse;
}

/*
=====================================================================
Draw clock function
=====================================================================
*/
void JKMod_CG_DrawClock(void)
{
	qtime_t		systemTime;
	char		*systemTimeType;
	int			systemTimeHour;
	int			x, y;

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;

	trap_R_SetColor(colorTable[CT_WHITE]); // Don't tint hud
	trap_RealTime(&systemTime);
	systemTimeType = systemTime.tm_hour > 12 ? "pm" : "am";
	systemTimeHour = systemTime.tm_hour > 12 && jkcvar_cg_drawClock.integer == 2 ? systemTime.tm_hour - 12 : systemTime.tm_hour;

	x = cgs.screenWidth - 68;
	y = cgs.screenHeight - (cg.snap->ps.pm_type == PM_SPECTATOR ? 36 : 123);

	if (cg.snap->ps.pm_type == PM_SPECTATOR && cgs.gametype == GT_TOURNAMENT) y -= 97;

	CG_DrawPic(x, y, 64, 32, cgs.jkmodMedia.clockBg);

	CG_Text_Paint(x + 8, y + 4, 0.85f, colorTable[CT_HUD_GREEN], va("%02i", systemTimeHour), 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);
	CG_Text_Paint(x + 29, y + 4, 0.85f, colorTable[CT_HUD_GREEN], va("%02i", systemTime.tm_min), 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);
	CG_Text_Paint(x + 47, y + (jkcvar_cg_drawClock.integer == 2 ? 8.5f : 13.5f), 0.4f, colorTable[CT_HUD_GREEN], va("%02i", systemTime.tm_sec), 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);

	if (jkcvar_cg_drawClock.integer == 2) CG_Text_Paint(x + 47, y + 13.5f, 0.4f, colorTable[CT_HUD_GREEN], va("%s", systemTimeType), 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);
	if ((cg.time >> 9) & 1) CG_Text_Paint(x + 24, y + 6, 0.7f, colorTable[CT_HUD_GREEN], ":", 0, 0, UI_SMALLFONT | UI_DROPSHADOW, FONT_SMALL);
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
	char *token;
	char *search = "^7: ^2";
	char name[MAX_SAY_TEXT + 64 + 36], msg[MAX_SAY_TEXT + 64];
	qboolean regular = qfalse, teamchat = qfalse, personal = qfalse;

	if (jkcvar_cg_chatBoxTime.integer <= 0) // Don't bother then.
	{ 
		return;
	}

	memset(chat, 0, sizeof(jkmod_chatbox_t));

	if (strlen(chatStr) > sizeof(chat->string)) // Too long, terminate at proper len.
	{ 
		chatStr[sizeof(chat->string) - 1] = 0;
	}

	// Check chat type
	if (strstr(chatStr, "^7: ^2")) {
		regular = qtrue;
		search = "^7: ^2";
	}
	if (strstr(chatStr, "^7): ^5")) {
		teamchat = qtrue;
		search = "^7): ^5";
	}
	if (strstr(chatStr, "^7]: ^6")) {
		personal = qtrue;
		search = "^7]: ^6";
	}

	// Check 1.02 dropshadows
	if (regular || teamchat || personal) 
	{
		token = JKMod_CG_StrTok(chatStr, search);
		if (token) Q_strncpyz(name, token, sizeof(name));
		token = JKMod_CG_StrTok(NULL, search);

		if (token) Q_strncpyz(msg, token, sizeof(msg));

		if (regular) {
			if (!JKMod_CG_HasLeadingColorCode(msg))
				Q_strcat(name, sizeof(name), "^7: ^0^0^2");
			else
				Q_strcat(name, sizeof(name), "^7: ^0^0");
		}
		else if (teamchat) {
			if (!JKMod_CG_HasLeadingColorCode(msg))
				Q_strcat(name, sizeof(name), "^7): ^0^0^5");
			else
				Q_strcat(name, sizeof(name), "^7): ^0^0"); 
		}
		else if (personal) {
			if (!JKMod_CG_HasLeadingColorCode(msg))
				Q_strcat(name, sizeof(name), "^7]: ^0^0^6");
			else
				Q_strcat(name, sizeof(name), "^7]: ^0^0"); 
		}

		Q_strcat(name, sizeof(name), msg);
		strcpy(chatStr, name);
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
			const char *checkColor = (const char *)(chat->string + i);

			// Check special colored strings
			if (Q_IsColorString(checkColor) || Q_IsColorString_1_02(checkColor)) {
				i += 2;
				continue;
			}

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

	y += 5;

	if (!cg.renderingThirdPerson && (cg.snap->ps.eFlags & JK_JETPACK_ACTIVE)) 
	{
		CG_DrawPic(xAlign, y, ico_size, ico_size, cgs.jkmodMedia.jetpackIcon);
		if ((cgs.jkmodCGS.jetPack == 2 || cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_CHEAT) && (cg.snap->ps.eFlags & JK_JETPACK_FLAMING)) CG_DrawPic(xAlign + 4, y - 2, 8, 8, cgs.jkmodMedia.dotRed);
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
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return y;

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
	if (!(cg.snap->ps.stats[JK_FUEL] < 100)) return;
	if (cg.snap->ps.stats[STAT_HEALTH] <= 0) return;
	
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
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;

	switch (cg.snap->ps.stats[JK_DIMENSION]) 
	{
		case DIMENSION_DUEL: dimensionStr = "[Private Duel]"; break;
		case DIMENSION_GUNS: dimensionStr = "[Guns Arena]"; break;
		case DIMENSION_RACE: dimensionStr = "[Race Defrag]"; break;
		case DIMENSION_SABER: dimensionStr = "[Saber Only]"; break;
		case DIMENSION_INSTA: dimensionStr = "[Insta Kill]"; break;
		case DIMENSION_CHEAT: dimensionStr = "[Cheats Mode]"; break;
	}

	if (dimensionStr) {
		if (cg_hudFiles.integer) {
			CG_Text_Paint(85, cgs.screenHeight - 20, 0.7, colorTable[CT_LTORANGE], dimensionStr, 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL);
		} else {
			UI_DrawScaledProportionalString(101, cgs.screenHeight - 23, dimensionStr, UI_LEFT | UI_DROPSHADOW, colorTable[CT_LTORANGE], 0.7);
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
		float		scale = 0.5;
		float		max = 0.62;
		float		distance = jkcvar_cg_drawPlayerNames.integer == 1 ? 520 : 800;

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
		if (VectorLength(diff) >= distance)
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
		if (jkcvar_cg_drawPlayerNames.integer == 1) scale = 120 / VectorLength(diff);

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
		int consoleLines = cg.jkmodCG.consolePrint;

		x = 5;
		y = 5;
		scale = 1.7;

		if (consoleLines == 1) y += 7;
		else if (consoleLines == 2) y += 7*2.3;
		else if (consoleLines == 3) y += 7*3.3;
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
		x = cgs.screenWidth - (cg_hudFiles.integer ? 185 : 165);
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

/*
=====================================================================
Strafe helper
=====================================================================
*/

// Strafe helper sound
void JKMod_CG_StrafeHelperSound(float difference) 
{
	// Under aiming by a bit, but still good?
	if (difference > -40.0f && difference < 10.0f) trap_S_StartLocalSound(cgs.jkmodMedia.strafeHelperSound, CHAN_LOCAL_SOUND);
}

// Strafe helper draw line
void JKMod_CG_StrafeHelperDrawLine(float x1, float y1, float x2, float y2, float size, vec4_t color, float alpha, float ycutoff)
{
	float stepx, stepy, length = (float)sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	int i;

	if (trap_Key_GetCatcher() & (KEYCATCH_UI|KEYCATCH_MESSAGE)) return;
	if (cg.scoreBoardShowing) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;

	if (length < 1) length = 1;
	else if (length > 2000) length = 2000;

	if (!ycutoff) ycutoff = 480;

	stepx = (x2 - x1) / (length / size);
	stepy = (y2 - y1) / (length / size);

	trap_R_SetColor(color);

	for (i = 0; i <= (length / size); i++) 
	{
		if (x1 < cgs.screenWidth && y1 < SCREEN_HEIGHT && y1 < ycutoff) CG_DrawPic(x1, y1, size, size, cgs.media.whiteShader);
		x1 += stepx;
		y1 += stepy;
	}
}

// Strafe helper draw speed
void JKMod_CG_StrafeHelperDrawSpeed(int moveDir) 
{
	float length;
	float diff;
	float midx;
	float midy;
	vec3_t velocity_copy;
	vec3_t viewangle_copy;
	vec3_t velocity_angle;
	float g_speed;
	float accel;
	float optiangle;
	usercmd_t cmd = { 0 };

	if (cg.clientNum == cg.predictedPlayerState.clientNum && !cg.demoPlayback) 
	{
		trap_GetUserCmd(trap_GetCurrentCmdNumber(), &cmd);
	}
	else if (cg.snap) 
	{
		moveDir = cg.snap->ps.movementDir;
		switch (moveDir) 
		{
			case 0: // W
				cmd.forwardmove = 127; break;
			case 1: // WA
				cmd.forwardmove = 127; cmd.rightmove = -127; break;
			case 2: // A
				cmd.rightmove = -127;	break;
			case 3: // AS
				cmd.rightmove = -127;	cmd.forwardmove = -127; break;
			case 4: // S
				cmd.forwardmove = -127; break;
			case 5: // SD
				cmd.forwardmove = -127; cmd.rightmove = 127; break;
			case 6: // D
				cmd.rightmove = 127; break;
			case 7: // DW
				cmd.rightmove = 127; cmd.forwardmove = 127;	break;
			default:
				break;
		}
	}
	else 
	{
		return; // No cg.snap causes this to return.
	}

	midx = cgs.screenWidth / 2;
	midy = SCREEN_HEIGHT / 2;
	VectorCopy(cg.predictedPlayerState.velocity, velocity_copy);
	velocity_copy[2] = 0;
	VectorCopy(cg.refdefViewAngles, viewangle_copy);
	viewangle_copy[PITCH] = 0;
	length = VectorNormalize(velocity_copy);
	g_speed = cg.predictedPlayerState.speed;
	accel = g_speed;
	accel *= 8.0f;
	accel /= 1000;
	optiangle = (g_speed - accel) / length;

	if ((optiangle <= 1) && (optiangle >= -1))
		optiangle = acos(optiangle);
	else
		optiangle = 0;

	length /= 5;

	if (length > (SCREEN_HEIGHT / 2)) length = (float)(SCREEN_HEIGHT / 2);

	vectoangles(velocity_copy, velocity_angle);

	diff = AngleSubtract(viewangle_copy[YAW], velocity_angle[YAW]);
	diff = diff / 180 * M_PI;

	JKMod_CG_StrafeHelperDrawLine(midx, midy, midx + length * sin(diff), midy - length * cos(diff), 1, colorRed, 0.75f, 0);
	JKMod_CG_StrafeHelperDrawLine(midx, midy, midx + cmd.rightmove, midy - cmd.forwardmove, 1, colorCyan, 0.75f, 0);
	JKMod_CG_StrafeHelperDrawLine(midx, midy, midx + length / 2 * sin(diff + optiangle), midy - length / 2 * cos(diff + optiangle), 1, colorRed, 0.75f, 0);
	JKMod_CG_StrafeHelperDrawLine(midx, midy, midx + length / 2 * sin(diff - optiangle), midy - length / 2 * cos(diff - optiangle), 1, colorRed, 0.75f, 0);
}

// Strafe helper show line
void JKMod_CG_StrafeHelperLine(vec3_t velocity, float diff, qboolean active, int moveDir) 
{
	vec3_t start, angs, forward, delta, line;
	float x, y, startx, starty, lineWidth;
	int sensitivity = jkcvar_cg_sHelperPrecision.integer;
	static const int LINE_HEIGHT = 230; // 240 is midpoint, so it should be a little higher so crosshair is always on it.
	static const vec4_t activeColor = { 0, 1, 0, 0.75 }, normalColor = { 1, 1, 1, 0.75 }, invertColor = { 0.5f, 1, 1, 0.75 }, wColor = { 1, 0.5, 0.5, 0.75 }, rearColor = { 0.5, 1,1, 0.75 }, centerColor = { 0.5, 1, 1, 0.75 };
	vec4_t color = { 1, 1, 1, 0.75 };

	if (jkcvar_cg_sHelperPrecision.integer < 100) sensitivity = 100;
	else if (jkcvar_cg_sHelperPrecision.integer > 10000) sensitivity = 10000;

	lineWidth = jkcvar_cg_sHelperLineWidth.value;

	if (lineWidth < 0.25f) lineWidth = 0.25f;
	else if (lineWidth > 5) lineWidth = 5;

	if (active) 
	{
		color[0] = cg.jkmodCG.strafeHelperActiveColor[0];
		color[1] = cg.jkmodCG.strafeHelperActiveColor[1];
		color[2] = cg.jkmodCG.strafeHelperActiveColor[2];
		color[3] = cg.jkmodCG.strafeHelperActiveColor[3];
	}
	else 
	{
		if (moveDir == 1 || moveDir == 7)
			memcpy(color, normalColor, sizeof(vec4_t));
		else if (moveDir == 2 || moveDir == 6)
			memcpy(color, invertColor, sizeof(vec4_t));
		else if (moveDir == 0)
			memcpy(color, wColor, sizeof(vec4_t));
		else if (moveDir == 8)
			memcpy(color, centerColor, sizeof(vec4_t));
		else if (moveDir == 9 || moveDir == 10)
			memcpy(color, rearColor, sizeof(vec4_t));

		color[3] = jkcvar_cg_sHelperInactiveAlpha.value / 255.0f;
	}

	if (!(jkcvar_cg_strafeHelper.integer & SHELPER_SUPEROLDSTYLE) && !cg.renderingThirdPerson) {
		VectorCopy(cg.refdef.vieworg, start);
	} else {
		VectorCopy(cg.predictedPlayerState.origin, start);
	}

	VectorCopy(velocity, angs);

	angs[YAW] += diff;
	AngleVectors(angs, forward, NULL, NULL);
	VectorScale(forward, sensitivity, delta); // Line length

	line[0] = delta[0] + start[0];
	line[1] = delta[1] + start[1];
	line[2] = start[2];

	if (!CG_WorldCoordToScreenCoord(line, &x, &y))
		return;

	if (jkcvar_cg_strafeHelper.integer & SHELPER_NEWBARS) 
	{
		JKMod_CG_StrafeHelperDrawLine(x, (SCREEN_HEIGHT / 2) + 20, x, (SCREEN_HEIGHT / 2) - 20, lineWidth, color, 0.75f, 0);
	}
	// Not sure how to deal with multiple lines for W only so just fuck it for now.. Proper way is to tell which line we are closest to aiming at and display the shit for that...
	if (jkcvar_cg_strafeHelper.integer & SHELPER_OLDBARS && active && moveDir != 0) 
	{ 
		CG_FillRect(cgs.screenWidth / 2, SCREEN_HEIGHT / 2, (-4.444 * AngleSubtract(cg.predictedPlayerState.viewangles[YAW], angs[YAW])), 12, colorTable[CT_RED]);
	}
	if (jkcvar_cg_strafeHelper.integer & SHELPER_OLDSTYLE) 
	{
		int cutoff = SCREEN_HEIGHT - jkcvar_cg_sHelperCutoff.integer; // Should be between 480 and LINE_HEIGHT

		if (cutoff > SCREEN_HEIGHT) cutoff = SCREEN_HEIGHT;
		if (cutoff < LINE_HEIGHT + 20) cutoff = LINE_HEIGHT + 20;

		JKMod_CG_StrafeHelperDrawLine(cgs.screenWidth * 0.5, SCREEN_HEIGHT, x, LINE_HEIGHT, lineWidth, color, color[3], cutoff);
	}
	if (jkcvar_cg_strafeHelper.integer & SHELPER_SUPEROLDSTYLE) 
	{
		int cutoff = SCREEN_HEIGHT - jkcvar_cg_sHelperCutoff.integer; //Should be between 480 and LINE_HEIGHT

		if (cutoff > SCREEN_HEIGHT) cutoff = SCREEN_HEIGHT;
		if (cutoff < LINE_HEIGHT + 20) cutoff = LINE_HEIGHT + 20;
		if (CG_WorldCoordToScreenCoord(start, &startx, &starty)) JKMod_CG_StrafeHelperDrawLine(startx, starty, x, y, lineWidth, color, color[3], cutoff);
	}
	if (jkcvar_cg_strafeHelper.integer & SHELPER_WEZE) 
	{
		JKMod_CG_StrafeHelperDrawSpeed(moveDir);
	}
	// Dont do this shit for the center line since its not really a strafe
	if (jkcvar_cg_strafeHelper.integer & SHELPER_SOUND && active && moveDir != 8) 
	{
		JKMod_CG_StrafeHelperSound(100 * AngleSubtract(cg.predictedPlayerState.viewangles[YAW], angs[YAW]));
	}
}

// Strafe helper main draw
void JKMod_CG_StrafeHelper(centity_t* cent)
{
	vec_t* velocity = cg.predictedPlayerState.velocity;
	static vec3_t velocityAngle;
	const float currentSpeed = cg.jkmodCG.currentSpeed;
	float pmAccel = 10.0f, pmAirAccel = 1.0f, pmFriction = 6.0f, frametime, optimalDeltaAngle, baseSpeed = cg.predictedPlayerState.speed;
	const int moveStyle = MV_JKA; // To-Do
	int moveDir;
	qboolean onGround;
	usercmd_t cmd = { 0 };

	if (trap_Key_GetCatcher() & (KEYCATCH_UI|KEYCATCH_MESSAGE)) return;
	if (cg.scoreBoardShowing) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;
	if (moveStyle == MV_SIEGE) return; // No strafe in siege

	if (cg.clientNum == cg.predictedPlayerState.clientNum && !cg.demoPlayback) 
	{
		trap_GetUserCmd(trap_GetCurrentCmdNumber(), &cmd);
	}
	else if (cg.snap) 
	{
		moveDir = cg.snap->ps.movementDir;
		switch (moveDir) 
		{
			case 0: // W
				cmd.forwardmove = 1; break;
			case 1: // WA
				cmd.forwardmove = 1; cmd.rightmove = -1; break;
			case 2: // A
				cmd.rightmove = -1;	break;
			case 3: // AS
				cmd.rightmove = -1;	cmd.forwardmove = -1; break;
			case 4: // S
				cmd.forwardmove = -1; break;
			case 5: // SD
				cmd.forwardmove = -1; cmd.rightmove = 1; break;
			case 6: // D
				cmd.rightmove = 1; break;
			case 7: // DW
				cmd.rightmove = 1; cmd.forwardmove = 1;	break;
			default:
				break;
		}

		if (cg.snap->ps.pm_flags & PMF_JUMP_HELD) cmd.upmove = 1;
	}
	else 
	{
		return; // No cg.snap causes this to return.
	}

	onGround = (qboolean)(cg.snap->ps.groundEntityNum == ENTITYNUM_WORLD); // Sadly predictedPlayerState makes it jerky so need to use cg.snap groundentityNum, and check for cg.snap earlier

	if (moveStyle == MV_WSW) 
	{
		pmAccel = 12.0f;
		pmFriction = 8.0f;
	}
	else if (moveStyle == MV_CPM || moveStyle == MV_RJCPM || moveStyle == MV_BOTCPM) 
	{
		pmAccel = 15.0f;
		pmFriction = 8.0f;
	}
	else if (moveStyle == MV_SP) 
	{
		pmAirAccel = 4.0f;
		pmAccel = 12.0f;
	}
	else if (moveStyle == MV_SLICK) 
	{
		pmFriction = 0.0f; // Unless walking?
		pmAccel = 30.0f;
	}

	if (currentSpeed < (baseSpeed - 1)) return;

	if (moveStyle == MV_SP) 
	{
		if (!(cg.predictedPlayerState.pm_flags & PMF_JUMP_HELD) && cmd.upmove > 0) { // Also, wishspeed *= scale.  Scale is different cuz of upmove in air.  Only works ingame not from spec
			baseSpeed /= 1.41421356237f; // umm.. dunno.. divide by sqrt(2)
		}
	}

	if (jkcvar_cg_sHelperFPS.value < 1)
		frametime = ((float)cg.frametime * 0.001f);
	else if (jkcvar_cg_sHelperFPS.value > 1000) // Invalid
		frametime = 1;
	else 
		frametime = 1 / jkcvar_cg_sHelperFPS.value;

	if (onGround)
		optimalDeltaAngle = acos((double)((baseSpeed - (pmAccel * baseSpeed * frametime)) / (currentSpeed * (1 - pmFriction * (frametime))))) * (180.0f / M_PI) - 45.0f;
	else
		optimalDeltaAngle = acos((double)((baseSpeed - (pmAirAccel * baseSpeed * frametime)) / currentSpeed)) * (180.0f / M_PI) - 45.0f;

	if (optimalDeltaAngle < 0 || optimalDeltaAngle > 360) optimalDeltaAngle = 0;

	velocity[2] = 0;
	vectoangles(velocity, velocityAngle); // We have the offset from our Velocity angle that we should be aiming at, so now we need to get our velocity angle.

	//QW, CPM, PJK, WSW, RJCPM have center line
	if (moveStyle == MV_QW || moveStyle == MV_CPM || moveStyle == MV_PJK || moveStyle == MV_WSW || moveStyle == MV_RJCPM || moveStyle == MV_SWOOP || moveStyle == MV_BOTCPM || (moveStyle == MV_SLICK && !onGround)) 
	{
		if (jkcvar_cg_strafeHelper.integer & SHELPER_CENTER) JKMod_CG_StrafeHelperLine(velocityAngle, 0, (qboolean)(cmd.forwardmove == 0 && cmd.rightmove != 0), 8);
	}

	// Every style but QW has WA/WD lines
	if (moveStyle != MV_QW && moveStyle != MV_SWOOP) 
	{ 
		if (jkcvar_cg_strafeHelper.integer & SHELPER_WA) JKMod_CG_StrafeHelperLine(velocityAngle, (optimalDeltaAngle + (jkcvar_cg_sHelperOffset.value * 0.01f)), (qboolean)(cmd.forwardmove > 0 && cmd.rightmove < 0), 1);
		if (jkcvar_cg_strafeHelper.integer & SHELPER_WD) JKMod_CG_StrafeHelperLine(velocityAngle, (-optimalDeltaAngle - (jkcvar_cg_sHelperOffset.value * 0.01f)), (qboolean)(cmd.forwardmove > 0 && cmd.rightmove > 0), 7);
	}

	// JKA, Q3, RJQ3, Jetpack? have A/D
	if (moveStyle == MV_JKA || moveStyle == MV_Q3 || moveStyle == MV_RJQ3 || moveStyle == MV_JETPACK || moveStyle == MV_SPEED || moveStyle == MV_SP || (moveStyle == MV_SLICK && onGround)) 
	{ 
		if (jkcvar_cg_strafeHelper.integer & SHELPER_A) JKMod_CG_StrafeHelperLine(velocityAngle, -(45.0f - (optimalDeltaAngle + (jkcvar_cg_sHelperOffset.value * 0.01f))), (qboolean)(cmd.forwardmove == 0 && cmd.rightmove < 0), 2);
		if (jkcvar_cg_strafeHelper.integer & SHELPER_D) JKMod_CG_StrafeHelperLine(velocityAngle, (45.0f - (optimalDeltaAngle + (jkcvar_cg_sHelperOffset.value * 0.01f))), (qboolean)(cmd.forwardmove == 0 && cmd.rightmove > 0), 6);

		//A/D backwards strafe?																																		  
		if (jkcvar_cg_strafeHelper.integer & SHELPER_REAR) 
		{
			JKMod_CG_StrafeHelperLine(velocityAngle, (225.0f - (optimalDeltaAngle + (jkcvar_cg_sHelperOffset.value * 0.01f))), (qboolean)(cmd.forwardmove == 0 && cmd.rightmove < 0), 9);
			JKMod_CG_StrafeHelperLine(velocityAngle, (135.0f + (optimalDeltaAngle + (jkcvar_cg_sHelperOffset.value * 0.01f))), (qboolean)(cmd.forwardmove == 0 && cmd.rightmove > 0), 10);
		}
	}

	// W only
	if (moveStyle == MV_JKA || moveStyle == MV_Q3 || moveStyle == MV_RJQ3 || moveStyle == MV_SWOOP || moveStyle == MV_JETPACK || moveStyle == MV_SPEED || moveStyle == MV_SP) 
	{
		if (jkcvar_cg_strafeHelper.integer & SHELPER_W) 
		{
			JKMod_CG_StrafeHelperLine(velocityAngle, (45.0f + (optimalDeltaAngle + (jkcvar_cg_sHelperOffset.value * 0.01f))), (qboolean)(cmd.forwardmove > 0 && cmd.rightmove == 0), 0);
			JKMod_CG_StrafeHelperLine(velocityAngle, (-45.0f - (optimalDeltaAngle + (jkcvar_cg_sHelperOffset.value * 0.01f))), (qboolean)(cmd.forwardmove > 0 && cmd.rightmove == 0), 0);
		}
	}
}

/*
=====================================================================
Draw speed meter
=====================================================================
*/

// Draw accel meter
void JKMod_CG_SpeedMeterAccel(void)
{
	const float optimalAccel = cg.predictedPlayerState.speed * ((float)cg.frametime / 1000.0f);
	const float potentialSpeed = (float)sqrt(cg.jkmodCG.previousSpeed * cg.jkmodCG.previousSpeed - optimalAccel * optimalAccel + 2 * (250 * optimalAccel));
	float actualAccel, total, percentAccel, percentAccelFinal, x, y, width, height;
	const float accel = cg.jkmodCG.currentSpeed - cg.jkmodCG.previousSpeed;
	static int t, i, previous, lastupdate;
	unsigned int frameTime;
	static float previousTimes[PERCENT_SAMPLES];
	static unsigned int index;
	vec4_t bgColor = { 0.5f, 0.5f, 0.5f, 0.5f };
	vec4_t accelColor1 = { 1.0f, 0.0f, 0.0f, 0.5f }; // colorTable[CT_GREEN]
	vec4_t accelColor2 = { 0.0f, 1.0f, 0.0f, 0.5f }; // colorTable[CT_RED]

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (cg.scoreBoardShowing) return;
	if (JKMod_CG_IconHUDActive()) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;

	width = 43;
	height = 13;
	x = 0.5f * cgs.screenWidth - (width / 2);
	y = cgs.screenHeight - 19;

	CG_FillRect(x, y, width, height, bgColor);
	CG_DrawRect(x - 1, y - 1, width + 1, height + 2, 0.5f, colorTable[CT_BLACK]);

	actualAccel = accel;

	if (actualAccel < 0) actualAccel = 0.001f;
	else if (actualAccel >(potentialSpeed - cg.jkmodCG.currentSpeed)) actualAccel = (potentialSpeed - cg.jkmodCG.currentSpeed) * 0.99f;

	t = trap_Milliseconds();
	frameTime = t - previous;
	previous = t;
	lastupdate = t;
	previousTimes[index % PERCENT_SAMPLES] = actualAccel / (potentialSpeed - cg.jkmodCG.currentSpeed);
	index++;

	total = 0;
	
	for (i = 0; i < PERCENT_SAMPLES; i++) total += previousTimes[i];
	if (!total) total = 1;

	percentAccel = total / (float)PERCENT_SAMPLES;

	if (percentAccel && cg.jkmodCG.currentSpeed) 
	{
		percentAccelFinal = 36 * percentAccel;
		CG_FillRect(x, y, percentAccelFinal, height, (percentAccelFinal > (width / 2) ? accelColor1 : accelColor2));
	}
	cg.jkmodCG.previousSpeed = cg.jkmodCG.currentSpeed;
}

// Draw jump height
void JKMod_CG_SpeedMeterJumpHeight(centity_t *cent)
{
	const vec_t* const velocity = (cent->currentState.clientNum == cg.clientNum ? cg.predictedPlayerState.velocity : cent->currentState.pos.trDelta);
	char jumpHeightStr[32] = { 0 };
	float x, y;

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (cg.scoreBoardShowing) return;
	if (JKMod_CG_IconHUDActive()) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;

	x = 0.5f * cgs.screenWidth;
	y = cgs.screenHeight - 23;

	// Coming back from a tele or w/e
	if (cg.predictedPlayerState.fd.forceJumpZStart == -65536) return;

	// If we were going up, and we are now going down, print our height.
	if (cg.predictedPlayerState.fd.forceJumpZStart && (cg.jkmodCG.lastZSpeed > 0) && (velocity[2] <= 0)) 
	{
		cg.jkmodCG.lastJumpHeight = cg.predictedPlayerState.origin[2] - cg.predictedPlayerState.fd.forceJumpZStart;
		cg.jkmodCG.lastJumpHeightTime = cg.time;
	}

	if ((cg.jkmodCG.lastJumpHeightTime > cg.time - 1500) && (cg.jkmodCG.lastJumpHeight > 0.0f)) 
	{
		if (!(jkcvar_cg_speedMeter.integer & SMETER_KPH) && !(jkcvar_cg_speedMeter.integer & SMETER_MPH))
			Com_sprintf(jumpHeightStr, sizeof(jumpHeightStr), "%.0f", cg.jkmodCG.lastJumpHeight);
		else if (jkcvar_cg_speedMeter.integer & SMETER_KPH)
			Com_sprintf(jumpHeightStr, sizeof(jumpHeightStr), "%.1f", cg.jkmodCG.lastJumpHeight * 0.05f);
		else if (jkcvar_cg_speedMeter.integer & SMETER_MPH)
			Com_sprintf(jumpHeightStr, sizeof(jumpHeightStr), "%.1f", cg.jkmodCG.lastJumpHeight * 0.03106855f);

		UI_DrawScaledProportionalString(x + 60, y - 10, "Height:", UI_CENTER | UI_DROPSHADOW, colorWhite, 0.5);
		UI_DrawScaledProportionalString(x + 60, y, jumpHeightStr, UI_CENTER | UI_DROPSHADOW, colorWhite, 0.7);
	}
	cg.jkmodCG.lastZSpeed = velocity[2];
}

// Draw jump distance
void JKMod_CG_SpeedMeterJumpDistance(void)
{
	char jumpDistanceStr[64] = { 0 };
	float x, y;

	if (!cg.snap) return;
	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (cg.scoreBoardShowing) return;
	if (JKMod_CG_IconHUDActive()) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;

	x = 0.5f * cgs.screenWidth;
	y = cgs.screenHeight - 23;

	if (cg.predictedPlayerState.groundEntityNum == ENTITYNUM_WORLD) 
	{
		//We were just in the air, but now we arnt
		if (!cg.jkmodCG.wasOnGround) 
		{
			vec3_t distance;

			VectorSubtract(cg.predictedPlayerState.origin, cg.jkmodCG.lastGroundPosition, distance);
			cg.jkmodCG.lastJumpDistance = (float)sqrt(distance[0] * distance[0] + distance[1] * distance[1]);
			cg.jkmodCG.lastJumpDistanceTime = cg.time;
		}

		VectorCopy(cg.predictedPlayerState.origin, cg.jkmodCG.lastGroundPosition);
		cg.jkmodCG.wasOnGround = qtrue;
	}
	else 
	{
		cg.jkmodCG.wasOnGround = qfalse;
	}

	if ((cg.jkmodCG.lastJumpDistanceTime > cg.time - 1500) && (cg.jkmodCG.lastJumpDistance > 0.0f)) 
	{
		if (!(jkcvar_cg_speedMeter.integer & SMETER_KPH) && !(jkcvar_cg_speedMeter.integer & SMETER_MPH))
			Com_sprintf(jumpDistanceStr, sizeof(jumpDistanceStr), "%.0f", cg.jkmodCG.lastJumpDistance);
		else if (jkcvar_cg_speedMeter.integer & SMETER_KPH)
			Com_sprintf(jumpDistanceStr, sizeof(jumpDistanceStr), "%.1f", cg.jkmodCG.lastJumpDistance * 0.05f);
		else if (jkcvar_cg_speedMeter.integer & SMETER_MPH)
			Com_sprintf(jumpDistanceStr, sizeof(jumpDistanceStr), "%.1f", cg.jkmodCG.lastJumpDistance * 0.03106855f);

		UI_DrawScaledProportionalString(x - 60, y - 10, "Distance:", UI_CENTER | UI_DROPSHADOW, colorWhite, 0.5);
		UI_DrawScaledProportionalString(x - 60, y, jumpDistanceStr, UI_CENTER | UI_DROPSHADOW, colorWhite, 0.7);
	}
}

// Draw vertical speed
void JKMod_CG_SpeedMeterVerticalSpeed(void) 
{
	char speedStr5[64] = { 0 };
	float vertspeed = cg.predictedPlayerState.velocity[2];
	float x, y;

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (cg.scoreBoardShowing) return;
	if (JKMod_CG_IconHUDActive()) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;

	x = 0.5f * cgs.screenWidth;
	y = cgs.screenHeight - 23;

	if (vertspeed < 0) vertspeed = -vertspeed;
	if (vertspeed) 
	{
		qboolean lastGroundShown = (cg.jkmodCG.lastGroundTime > cg.time - 1500) && cg.jkmodCG.lastGroundSpeed;

		if (!(jkcvar_cg_speedMeter.integer & SMETER_KPH) && !(jkcvar_cg_speedMeter.integer & SMETER_MPH))
			Com_sprintf(speedStr5, sizeof(speedStr5), "%.0f", vertspeed);
		else if (jkcvar_cg_speedMeter.integer & SMETER_KPH)
			Com_sprintf(speedStr5, sizeof(speedStr5), "%.1f", vertspeed * 0.05f);
		else if (jkcvar_cg_speedMeter.integer & SMETER_MPH)
			Com_sprintf(speedStr5, sizeof(speedStr5), "%.1f", vertspeed * 0.03106855f);

		UI_DrawScaledProportionalString(x, y - (lastGroundShown ? 60 : 30)  - 10, "Vertical:", UI_CENTER | UI_DROPSHADOW, colorWhite, 0.5);
		UI_DrawScaledProportionalString(x, y - (lastGroundShown ? 60 : 30), speedStr5, UI_CENTER | UI_DROPSHADOW, colorWhite, 0.7);
	}
}

// Draw speed meter
void JKMod_CG_SpeedMeter(void)
{
	const char *accelStr;
	char speedStr[32] = { 0 };
	vec4_t colorSpeed = { 1, 1, 1, 1 };
	const float currentSpeed = cg.jkmodCG.currentSpeed;
	static float lastSpeed = 0, previousAccels[ACCEL_SAMPLES];
	const float accel = currentSpeed - lastSpeed;
	float total, avgAccel;
	int t, i;
	unsigned int frameTime;
	static unsigned int index;
	static int	previous, lastupdate;
	float x, y;

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	if (cg.scoreBoardShowing) return;
	if (JKMod_CG_IconHUDActive()) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;

	x = 0.5f * cgs.screenWidth;
	y = cgs.screenHeight - 23;

	lastSpeed = currentSpeed;

	if (currentSpeed > 250)
	{
		colorSpeed[1] = 1 / ((currentSpeed / 250)*(currentSpeed / 250));
		colorSpeed[2] = 1 / ((currentSpeed / 250)*(currentSpeed / 250));
	}

	t = trap_Milliseconds();
	frameTime = t - previous;
	previous = t;

	// Don't sample faster than this
	if (t - lastupdate > 5)
	{
		lastupdate = t;
		previousAccels[index % ACCEL_SAMPLES] = accel;
		index++;
	}

	total = 0;

	for (i = 0; i < ACCEL_SAMPLES; i++) total += previousAccels[i];
	if (!total) total = 1;

	avgAccel = total / (float)ACCEL_SAMPLES - 0.0625f;

	if (!(jkcvar_cg_speedMeter.integer & SMETER_KPH) && !(jkcvar_cg_speedMeter.integer & SMETER_MPH))
	{
		accelStr = avgAccel > 0.0f ? S_COLOR_GREEN "µps:" : avgAccel < 0.0f ? S_COLOR_RED "µps:" : S_COLOR_WHITE "µps:";
		Com_sprintf(speedStr, sizeof(speedStr), "%.0f", currentSpeed);
	}
	else if (jkcvar_cg_speedMeter.integer & SMETER_KPH)
	{
		accelStr = avgAccel > 0.0f ? S_COLOR_GREEN "kph:" : avgAccel < 0.0f ? S_COLOR_RED "kph:" : S_COLOR_WHITE "kph:";
		Com_sprintf(speedStr, sizeof(speedStr), "%.1f", currentSpeed * 0.05f);
	}
	else if (jkcvar_cg_speedMeter.integer & SMETER_MPH)
	{
		accelStr = avgAccel > 0.0f ? S_COLOR_GREEN "mph:" : avgAccel < 0.0f ? S_COLOR_RED "mph:" : S_COLOR_WHITE "mph:";
		Com_sprintf(speedStr, sizeof(speedStr), "%.1f", currentSpeed * 0.03106855f);
	}

	UI_DrawScaledProportionalString(x, y - 10, accelStr, UI_CENTER | UI_DROPSHADOW, colorWhite, 0.5);
	UI_DrawScaledProportionalString(x, y, speedStr, UI_CENTER | UI_DROPSHADOW, colorSpeed, 0.7);

	if (jkcvar_cg_speedMeter.integer & SMETER_GROUNDSPEED) 
	{
		char speedStr4[32] = { 0 };
		vec4_t colorGroundSpeed = { 1, 1, 1, 1 };

		// On ground or Moving down
		if (cg.predictedPlayerState.groundEntityNum != ENTITYNUM_NONE || cg.predictedPlayerState.velocity[2] < 0) 
		{
			cg.jkmodCG.firstTimeInAir = qfalse;
		}
		// Moving up for first time
		else if (!cg.jkmodCG.firstTimeInAir) 
		{
			cg.jkmodCG.firstTimeInAir = qtrue;
			cg.jkmodCG.lastGroundSpeed = currentSpeed;
			cg.jkmodCG.lastGroundTime = cg.time;
		}

		if (cg.jkmodCG.lastGroundSpeed > 250) 
		{
			colorGroundSpeed[1] = 1 / ((cg.jkmodCG.lastGroundSpeed / 250)*(cg.jkmodCG.lastGroundSpeed / 250));
			colorGroundSpeed[2] = 1 / ((cg.jkmodCG.lastGroundSpeed / 250)*(cg.jkmodCG.lastGroundSpeed / 250));
		}
		if ((cg.jkmodCG.lastGroundTime > cg.time - 1500)) 
		{
			if (cg.jkmodCG.lastGroundSpeed) 
			{
				if (!(jkcvar_cg_speedMeter.integer & SMETER_KPH) && !(jkcvar_cg_speedMeter.integer & SMETER_MPH))
					Com_sprintf(speedStr4, sizeof(speedStr4), "%.0f", cg.jkmodCG.lastGroundSpeed);
				else if (jkcvar_cg_speedMeter.integer & SMETER_KPH)
					Com_sprintf(speedStr4, sizeof(speedStr4), "%.1f", cg.jkmodCG.lastGroundSpeed * 0.05f);
				else if (jkcvar_cg_speedMeter.integer & SMETER_MPH)
					Com_sprintf(speedStr4, sizeof(speedStr4), "%.1f", cg.jkmodCG.lastGroundSpeed * 0.03106855f);

				UI_DrawScaledProportionalString(x, y - 30 - 10, "Ground:", UI_CENTER | UI_DROPSHADOW, colorWhite, 0.5);
				UI_DrawScaledProportionalString(x, y - 30, speedStr4, UI_CENTER | UI_DROPSHADOW, colorSpeed, 0.7);
			}
		}
	}
}
