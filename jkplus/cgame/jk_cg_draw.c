/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Client draw module
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header
#include "../../code/ui/ui_shared.h"	// UI shared header

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

	// Draw Jetpack Fuel
	if (cgs.jkmodCvar.jetPack == 1)
	{
		JKMod_CG_DrawJetPackFuel();
	}

	// Draw Dimension indicator
	if (cgs.jkmodCvar.altDimension)
	{
		JKMod_CG_DrawDimensionString();
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

	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;

	trap_RealTime(&serverTime);
	serverTimeType = (serverTime.tm_hour > 11 && serverTime.tm_hour < 24) ? "pm" : "am";

	x = cgs.screenWidth - (cg_lagometer.integer ? 68 : 68);
	y = cgs.screenHeight - (cg_lagometer.integer ? 175 : 123);

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

	if (!cg.snap)
		return;

	if (cg.snap->ps.pm_type == PM_SPECTATOR)
		return;

	if (cg.snap->ps.stats[STAT_HEALTH] <= 0)
		return;

	y += 8;

	if (cg.snap->ps.stats[STAT_WEAPONS] & (1 << WP_TRIP_MINE) && cg.snap->ps.ammo[weaponData[WP_TRIP_MINE].ammoIndex] > 0) {
		CG_DrawPic(xAlign, y, ico_size, ico_size, cgs.media.weaponIcons[WP_TRIP_MINE]);
		CG_DrawNumField(xAlign, y, 2, cg.snap->ps.ammo[weaponData[WP_TRIP_MINE].ammoIndex], 6, 12, NUM_FONT_SMALL, qfalse);
		y += ico_size;
	}

	if (!cg.snap->ps.stats[STAT_HOLDABLE_ITEM] || !cg.snap->ps.stats[STAT_HOLDABLE_ITEMS])
		return;

	for (i = 0; i < HI_NUM_HOLDABLE; i++)
	{
		if (i && cg.snap->ps.stats[STAT_HOLDABLE_ITEMS] & (1 << i))
		{
			CG_DrawPic(xAlign, y, ico_size, ico_size, cgs.media.invenIcons[i]);
			y += ico_size;
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

	if (!cg.snap)
	{
		return y;
	}

	if (cg.snap->ps.pm_type == PM_SPECTATOR) { 
		return y;
	}

	if (jkcvar_cg_drawInventory.integer && !(cgs.gametype == GT_CTF || cgs.gametype == GT_CTY)) { 
		ico_size = ICON_SIZE * 1.25;
		xAlign = (xAlign - ico_size);
		yAlign = (ICON_SIZE / 2);
		y += 5;
	}
	else {
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
					if (j == PW_REDFLAG)
					{
						icoShader = trap_R_RegisterShaderNoMip("gfx/hud/mpi_rflag_ys");
					}
					else
					{
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

	if ((cgs.jkmodCvar.altDimension & (1 << DIMENSION_DUEL)) && cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_DUEL) dimensionStr = "Duel";
	else if ((cgs.jkmodCvar.altDimension & (1 << DIMENSION_GUNS)) && cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_GUNS) dimensionStr = "Guns";
	else if ((cgs.jkmodCvar.altDimension & (1 << DIMENSION_RACE)) && cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_RACE) dimensionStr = "Race";

	if (dimensionStr) {
		UI_DrawScaledProportionalString(101, cgs.screenHeight - 23, va("[%s Dimension]", dimensionStr), UI_LEFT | UI_DROPSHADOW, colorTable[CT_LTORANGE], 0.7);
	}
}
