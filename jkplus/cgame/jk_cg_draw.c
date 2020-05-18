/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Client draw module
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header

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
