// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_scoreboard -- draw the scoreboard on top of the game screen
#include "cg_local.h"
#include "../ui/ui_shared.h"

#define	SCOREBOARD_X		(0.5f * cgs.screenWidth - 320.0f)

#define SB_HEADER			86
#define SB_TOP				(SB_HEADER+32)

// Where the status bar starts, so we don't overwrite it
#define SB_STATUSBAR		420

#define SB_NORMAL_HEIGHT	25
#define SB_INTER_HEIGHT		15 // interleaved height

#define SB_MAXCLIENTS_NORMAL  ((SB_STATUSBAR - SB_TOP) / SB_NORMAL_HEIGHT)
#define SB_MAXCLIENTS_INTER   ((SB_STATUSBAR - SB_TOP) / SB_INTER_HEIGHT - 1)

// Used when interleaved



#define SB_LEFT_BOTICON_X	(SCOREBOARD_X+0)
#define SB_LEFT_HEAD_X		(SCOREBOARD_X+32)
#define SB_RIGHT_BOTICON_X	(SCOREBOARD_X+64)
#define SB_RIGHT_HEAD_X		(SCOREBOARD_X+96)
// Normal
#define SB_BOTICON_X		(SCOREBOARD_X+32)
#define SB_HEAD_X			(SCOREBOARD_X+64)

#define SB_SCORELINE_X		(SCOREBOARD_X+100)
#define SB_SCORELINE_WIDTH	(cgs.screenWidth - SB_SCORELINE_X * 2)

#define SB_RATING_WIDTH	    0 // (6 * BIGCHAR_WIDTH)
#define SB_NAME_X			(SB_SCORELINE_X)
#define SB_SCORE_X			(SB_SCORELINE_X + .55 * SB_SCORELINE_WIDTH)
#define SB_PING_X			(SB_SCORELINE_X + (jkcvar_cg_scoreboardExtras.integer ? .72 : .70) * SB_SCORELINE_WIDTH) // Tr!Force: [Scoreboard] Extra info
#define SB_TIME_X			(SB_SCORELINE_X + (jkcvar_cg_scoreboardExtras.integer ? .87 : .85) * SB_SCORELINE_WIDTH) // Tr!Force: [Scoreboard] Extra info

// The new and improved score board
//
// In cases where the number of clients is high, the score board heads are interleaved
// here's the layout

//
//	0   32   80  112  144   240  320  400   <-- pixel position
//  bot head bot head score ping time name
//  
//  wins/losses are drawn on bot icon now

static qboolean localClient; // true if local client has been displayed


							 /*
=================
CG_DrawScoreboard
=================
*/
static void CG_DrawClientScore( int y, score_t *score, float *color, float fade, qboolean largeFormat ) 
{
	//vec3_t	headAngles;
	clientInfo_t	*ci;
	float		iconx;
	float		scale;

	if ( largeFormat )
	{
		scale = 1.0f;
	}
	else
	{
		scale = 0.75f;
	}

	if ( score->client < 0 || score->client >= cgs.maxclients ) {
		Com_Printf( "Bad score->client: %i\n", score->client );
		return;
	}
	
	ci = &cgs.clientinfo[score->client];

	iconx = SB_BOTICON_X + (SB_RATING_WIDTH / 2);

	// draw the handicap or bot skill marker (unless player has flag)
	if ( ci->powerups & ( 1 << PW_NEUTRALFLAG ) ) {
		if( largeFormat ) {
			CG_DrawFlagModel( iconx, y - ( 32 - BIGCHAR_HEIGHT ) / 2, 32, 32, TEAM_FREE, qfalse );
		}
		else {
			CG_DrawFlagModel( iconx, y, 16, 16, TEAM_FREE, qfalse );
		}
	} else if ( ci->powerups & ( 1 << PW_REDFLAG ) ) {
		if( largeFormat ) {
			CG_DrawFlagModel( iconx, y, 32, 32, TEAM_RED, qfalse );
		}
		else {
			CG_DrawFlagModel( iconx, y, 32, 32, TEAM_RED, qfalse );
		}
	} else if ( ci->powerups & ( 1 << PW_BLUEFLAG ) ) {
		if( largeFormat ) {
			CG_DrawFlagModel( iconx, y, 32, 32, TEAM_BLUE, qfalse );
		}
		else {
			CG_DrawFlagModel( iconx, y, 32, 32, TEAM_BLUE, qfalse );
		}
	} else {
		// draw the wins / losses
		/*
		if ( cgs.gametype == GT_TOURNAMENT ) 
		{
			CG_DrawSmallStringColor( iconx, y + SMALLCHAR_HEIGHT/2, va("%i/%i", ci->wins, ci->losses ), color );
		}
		*/
		//rww - in duel, we now show wins/losses in place of "frags". This is because duel now defaults to 1 kill per round.
	}

	// highlight your position
	if ( score->client == cg.snap->ps.clientNum ) 
	{
		float	hcolor[4];
		int		rank;

		localClient = qtrue;

		if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR 
			|| cgs.gametype >= GT_TEAM ) {
			rank = -1;
		} else {
			rank = cg.snap->ps.persistant[PERS_RANK] & ~RANK_TIED_FLAG;
		}
		if ( rank == 0 ) {
			hcolor[0] = 0;
			hcolor[1] = 0;
			hcolor[2] = 0.7f;
		} else if ( rank == 1 ) {
			hcolor[0] = 0.7f;
			hcolor[1] = 0;
			hcolor[2] = 0;
		} else if ( rank == 2 ) {
			hcolor[0] = 0.7f;
			hcolor[1] = 0.7f;
			hcolor[2] = 0;
		} else {
			hcolor[0] = 0.7f;
			hcolor[1] = 0.7f;
			hcolor[2] = 0.7f;
		}

		hcolor[3] = fade * 0.7;

		// Tr!Force: [Dimension] Show race time
		if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
		{
			float timeWidth = jkcvar_cg_scoreboardExtras.integer ? 19 : 12;
			if (largeFormat) timeWidth += 8;

			CG_FillRect( SB_SCORELINE_X - 5, y + 2, SB_SCORELINE_WIDTH + timeWidth, largeFormat?SB_NORMAL_HEIGHT:SB_INTER_HEIGHT, hcolor );
		} 
		else 
		{
			CG_FillRect( SB_SCORELINE_X - 5, y + 2, SB_SCORELINE_WIDTH + 10, largeFormat?SB_NORMAL_HEIGHT:SB_INTER_HEIGHT, hcolor );
		}
	}

	// Tr!Force: [ScoreboardIcons] Show players icons
	if (jkcvar_cg_scoreboardIcons.integer)
	{
		float iconScale = largeFormat ? 25 : 15;

		CG_DrawPic(SB_NAME_X - iconScale - 5, y + 2, iconScale, iconScale, (ci->modelIcon ? ci->modelIcon : trap_R_RegisterShaderNoMip("gfx/mp/jkmod_missing_icon")));
	}

	CG_Text_Paint (SB_NAME_X, y, 0.9f * scale, colorWhite, ci->name,0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );

	if ( ci->team != TEAM_SPECTATOR || cgs.gametype == GT_TOURNAMENT )
	{
		if (cgs.gametype == GT_TOURNAMENT)
		{
			CG_Text_Paint (SB_SCORE_X, y, 1.0f * scale, colorWhite, va("%i/%i", ci->wins, ci->losses),0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL );
		}
		else
		{
			// Tr!Force: [Scoreboard] Extra info
			if (jkcvar_cg_scoreboardExtras.integer)
			{
				float scoreScale = 1.0f;
				float scoreAlign = 0;

				if (cgs.gametype == GT_CTF || cgs.gametype == GT_CTY)
				{
					if (largeFormat && (score->score > 999 || (score->score > 99 && score->captures > 9))) {
						scoreScale = 0.84f;
						scoreAlign = 3;
					}

					CG_Text_Paint (SB_SCORE_X, y + scoreAlign, scoreScale * scale, colorWhite, va("%i/%i", score->score, score->captures),0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL );
				}
				else
				{
					if (largeFormat && (score->score > 99 || score->deaths > 99)) {
						scoreScale = 0.84f;
						scoreAlign = 3;
					}

					if (cgs.jkmodCGS.pluginCheck)
						CG_Text_Paint (SB_SCORE_X, y + scoreAlign, scoreScale * scale, colorWhite, va("%i/%i", score->score, score->deaths),0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL );
					else
						CG_Text_Paint (SB_SCORE_X, y + scoreAlign, scoreScale * scale, colorWhite, va("%i", score->score),0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL );
				}
			}
			else
			{
				CG_Text_Paint (SB_SCORE_X, y, 1.0f * scale, colorWhite, va("%i", score->score),0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL );
			}
		}
	}

	// Tr!Force: [Scoreboard] Extra info
	if (jkcvar_cg_scoreboardExtras.integer)
	{
		char *clientPing;
		
		if (score->ping != -1 && score->ping != 999)
			clientPing = ci->botSkill != 0 ? "Bot" : va("%i", score->ping);
		else
			clientPing = "-";

		CG_Text_Paint (SB_PING_X, y, 1.0f * scale, colorWhite, clientPing, 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL);
		
		if (ci->team == TEAM_SPECTATOR && cgs.gametype != GT_TOURNAMENT) CG_Text_Paint (SB_SCORE_X, y, 1.0f * scale, colorWhite, "-",0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL );
	}
	else
	{
		CG_Text_Paint (SB_PING_X, y, 1.0f * scale, colorWhite, va("%i", score->ping),0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL );
	}

	// Tr!Force: [Dimension] Show race time
	if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_RACE) 
	{
		float timeScale = 1.0f;
		float timeAlign = 0;

		if (largeFormat)
		{
			timeScale = 0.84f;
			timeAlign = 3;
		}
		
		CG_Text_Paint (SB_TIME_X, y + timeAlign, timeScale * scale, ci->jkmod_race ? colorWhite : colorMdGrey, va("%s", JKMod_CG_MsToString(ci->jkmod_race)),0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL );
	} 
	else 
	{
		CG_Text_Paint (SB_TIME_X, y, 1.0f * scale, colorWhite, va("%i", score->time),0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_SMALL );
	}

	// add the "ready" marker for intermission exiting
	if ( cg.snap->ps.stats[ STAT_CLIENTS_READY ] & ( 1 << score->client ) ) 
	{
		float readyAlign = jkcvar_cg_scoreboardIcons.integer && largeFormat ? 74 : 64; // Tr!Force: [ScoreboardIcons] Adjust ready label

		CG_Text_Paint (SB_NAME_X - readyAlign, y + 2, 0.7f * scale, colorWhite, CG_GetStripEdString("INGAMETEXT", "READY"),0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );
	}
}

/*
=================
CG_TeamScoreboard
=================
*/
static int CG_TeamScoreboard( int y, team_t team, float fade, int maxClients, int lineHeight, qboolean countOnly ) 
{
	int		i;
	score_t	*score;
	float	color[4];
	int		count;
	clientInfo_t	*ci;

	color[0] = color[1] = color[2] = 1.0;
	color[3] = fade;

	count = 0;
	for ( i = 0 ; i < cg.numScores && count < maxClients ; i++ ) {
		score = &cg.scores[i];
		ci = &cgs.clientinfo[ score->client ];

		if ( team != ci->team ) {
			continue;
		}

		if ( !countOnly )
		{
			CG_DrawClientScore( y + lineHeight * count, score, color, fade, lineHeight == SB_NORMAL_HEIGHT );
		}

		count++;
	}

	return count;
}

int CG_GetTeamCount(team_t team, int maxClients)
{
	int i = 0;
	int count = 0;
	clientInfo_t	*ci;
	score_t	*score;

	for ( i = 0 ; i < cg.numScores && count < maxClients ; i++ )
	{
		score = &cg.scores[i];
		ci = &cgs.clientinfo[ score->client ];

		if ( team != ci->team )
		{
			continue;
		}

		count++;
	}

	return count;
}
/*
=================
CG_DrawScoreboard

Draw the normal in-game scoreboard
=================
*/
qboolean CG_DrawOldScoreboard( void ) {
	float	x;
	int		y, i, n1, n2;
	// int		w;
	float	fade;
	float	*fadeColor;
	char	*s;
	int maxClients;
	int lineHeight;
	int topBorderSize, bottomBorderSize;

	// don't draw amuthing if the menu or console is up
	if ( cg_paused.integer ) {
		cg.deferredPlayerLoading = 0;
		return qfalse;
	}

	// don't draw scoreboard during death while warmup up
	if ( cg.warmup && !cg.showScores ) {
		return qfalse;
	}

	if ( cg.showScores || cg.predictedPlayerState.pm_type == PM_DEAD ||
		 cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		fade = 1.0;
		fadeColor = colorWhite;
	} else {
		fadeColor = CG_FadeColor( cg.scoreFadeTime, FADE_TIME );
		
		if ( !fadeColor ) {
			// next time scoreboard comes up, don't print killer
			cg.deferredPlayerLoading = 0;
			cg.killerName[0] = 0;
			return qfalse;
		}
		fade = *fadeColor;
	}

	// fragged by ... line
	// or if in intermission and duel, prints the winner of the duel round
	if (cgs.gametype == GT_TOURNAMENT && cgs.duelWinner != -1 &&
		cg.predictedPlayerState.pm_type == PM_INTERMISSION)
	{
		s = va("%s" S_COLOR_WHITE " %s", cgs.clientinfo[cgs.duelWinner].name, CG_GetStripEdString("INGAMETEXT", "DUEL_WINS") );
		/*w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
		x = 0.5f * ( cgs.screenWidth - w );
		y = 40;
		CG_DrawBigString( x, y, s, fade );
		*/
		x = 0.5f * ( cgs.screenWidth - CG_Text_Width ( s, 1.0f, FONT_MEDIUM ) );
		y = 40;
		CG_Text_Paint ( x, y, 1.0f, colorWhite, s, 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );
	}
	else if (cgs.gametype == GT_TOURNAMENT && cgs.duelist1 != -1 && cgs.duelist2 != -1 &&
		cg.predictedPlayerState.pm_type == PM_INTERMISSION)
	{
		s = va("%s" S_COLOR_WHITE " %s %s", cgs.clientinfo[cgs.duelist1].name, CG_GetStripEdString("INGAMETEXT", "SPECHUD_VERSUS"), cgs.clientinfo[cgs.duelist2].name );
		/*w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
		x = 0.5f * ( cgs.screenWidth - w );
		y = 40;
		CG_DrawBigString( x, y, s, fade );
		*/
		x = 0.5f * ( cgs.screenWidth - CG_Text_Width ( s, 1.0f, FONT_MEDIUM ) );
		y = 40;
		CG_Text_Paint ( x, y, 1.0f, colorWhite, s, 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );
	}
	else if ( cg.killerName[0] ) {
		s = va("%s %s", CG_GetStripEdString("INGAMETEXT", "KILLEDBY"), cg.killerName );
		/*w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
		x = 0.5f * ( cgs.screenWidth - w );
		y = 40;
		CG_DrawBigString( x, y, s, fade );
		*/
		x = 0.5f * (cgs.screenWidth - CG_Text_Width(s, 1.0f, FONT_MEDIUM));
		y = 40;
		CG_Text_Paint ( x, y, 1.0f, colorWhite, s, 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );
	}

	// current rank
	if ( cgs.gametype < GT_TEAM) {
		if (cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR ) 
		{
			char sPlace[256];
			char sOf[256];
			char sWith[256];

			trap_SP_GetStringTextString("INGAMETEXT_PLACE",	sPlace,	sizeof(sPlace));
			trap_SP_GetStringTextString("INGAMETEXT_OF",	sOf,	sizeof(sOf));
			trap_SP_GetStringTextString("INGAMETEXT_WITH",	sWith,	sizeof(sWith));

			s = va("%s %s (%s %i) %s %i",
				CG_PlaceString( cg.snap->ps.persistant[PERS_RANK] + 1 ),
				sPlace,
				sOf,
				cg.numScores,
				sWith,
				cg.snap->ps.persistant[PERS_SCORE] );
			// w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
			x = 0.5f * cgs.screenWidth;
			y = 60;
			// CG_DrawBigString( x, y, s, fade );
			UI_DrawProportionalString(x, y, s, UI_CENTER|UI_DROPSHADOW, colorTable[CT_WHITE]);
		}
	} else {
		const char *sTo = CG_GetStripEdString("JKINGAME", "TO"); // Tr!Force: [CGameGeneral] Use translated text

		if ( cg.teamScores[0] == cg.teamScores[1] ) {
			s = va("%s %i", CG_GetStripEdString("JKINGAME", "TIED_AT"), cg.teamScores[0] ); // Tr!Force: [CGameGeneral] Use translated text
		} else if ( cg.teamScores[0] >= cg.teamScores[1] ) {
			s = va("%s %i %s %i", CG_GetStripEdString("JKINGAME", "RED_LEADS"), cg.teamScores[0], sTo, cg.teamScores[1] ); // Tr!Force: [CGameGeneral] Use translated text
		} else {
			s = va("%s %i %s %i", CG_GetStripEdString("JKINGAME", "BLUE_LEADS"), cg.teamScores[1], sTo, cg.teamScores[0] ); // Tr!Force: [CGameGeneral] Use translated text
		}

		x = 0.5f * ( cgs.screenWidth - CG_Text_Width ( s, 1.0f, FONT_MEDIUM ) );
		y = 60;
		
		CG_Text_Paint ( x, y, 1.0f, colorWhite, s, 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );
	}

	// scoreboard
	y = SB_HEADER;

	CG_DrawPic ( SB_SCORELINE_X - 40, y - 5, SB_SCORELINE_WIDTH + 80, 40, trap_R_RegisterShaderNoMip ( "gfx/menus/menu_buttonback.tga" ) );

	// "NAME", "SCORE", "PING", "TIME" weren't localised, GODDAMMIT!!!!!!!!     
	//
	// Unfortunately, since it's so sodding late now and post release I can't enable the localisation code (REM'd) since some of 
	//	the localised strings don't fit - since no-one's ever seen them to notice this.  Smegging brilliant. Thanks people.
	//
	CG_Text_Paint ( SB_NAME_X, y, 1.0f, colorWhite, CG_GetStripEdString("MENUS3", "NAME"), 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM ); // Tr!Force: [CGameGeneral] Use translated text
	if (cgs.gametype == GT_TOURNAMENT)
	{
		char sWL[100];
		trap_SP_GetStringTextString("INGAMETEXT_W_L", sWL,	sizeof(sWL));

		CG_Text_Paint ( SB_SCORE_X, y, 1.0f, colorWhite, sWL, 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );
	}
	else if (jkcvar_cg_scoreboardExtras.integer) // Tr!Force: [Scoreboard] Extra info
	{
		CG_Text_Paint ( SB_SCORE_X, y, 1.0f, colorWhite, CG_GetStripEdString("JKINGAME", "SCORE"), 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM ); // Tr!Force: [CGameGeneral] Use translated text

		// Subtitle
		if (cgs.gametype == GT_CTF || cgs.gametype == GT_CTY) {
			CG_Text_Paint ( SB_SCORE_X, y + 19, 0.5f, colorWhite, va("& %s", CG_GetStripEdString("JKINGAME", "CAPTURES")), 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );
		} else if (cgs.jkmodCGS.pluginCheck) {
			CG_Text_Paint ( SB_SCORE_X, y + 19, 0.5f, colorWhite, va("& %s", CG_GetStripEdString("JKINGAME", "DEATHS")), 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );
		}
	}
	else
	{
		CG_Text_Paint ( SB_SCORE_X, y, 1.0f, colorWhite, CG_GetStripEdString("JKINGAME", "SCORE"), 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM ); // Tr!Force: [CGameGeneral] Use translated text
	}
	CG_Text_Paint ( SB_PING_X, y, 1.0f, colorWhite, CG_GetStripEdString("MENUS0", "PING"), 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM ); // Tr!Force: [CGameGeneral] Use translated text
	CG_Text_Paint ( SB_TIME_X, y, 1.0f, colorWhite, CG_GetStripEdString("MENUS3", "TIME"), 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM ); // Tr!Force: [CGameGeneral] Use translated text

	// Tr!Force: [Scoreboard] Extra info
	if (cg.snap->ps.stats[JK_DIMENSION] == DIMENSION_RACE) {
		CG_Text_Paint ( SB_TIME_X, y + 19, 0.5f, colorWhite, CG_GetStripEdString("JKINGAME", "RECORD"), 0, 0, ITEM_TEXTSTYLE_OUTLINED, FONT_MEDIUM );
	}

	y = SB_TOP;

	// If there are more than SB_MAXCLIENTS_NORMAL, use the interleaved scores
	if ( cg.numScores > SB_MAXCLIENTS_NORMAL ) {
		maxClients = SB_MAXCLIENTS_INTER;
		lineHeight = SB_INTER_HEIGHT;
		topBorderSize = 8;
		bottomBorderSize = 16;
	} else {
		maxClients = SB_MAXCLIENTS_NORMAL;
		lineHeight = SB_NORMAL_HEIGHT;
		topBorderSize = 8;
		bottomBorderSize = 8;
	}

	// Tr!Force: [ScoreboardIcons] Adjust team background
	if (jkcvar_cg_scoreboardIcons.integer) {
		topBorderSize = 6;
		if (cg.numScores > SB_MAXCLIENTS_NORMAL) bottomBorderSize = 8;
	}

	// Tr!Force: [Scoreboard] Team thin border
	if (jkcvar_cg_scoreboardExtras.integer) {
		topBorderSize = -2;
		bottomBorderSize = 0;
	}

	localClient = qfalse;


	//I guess this should end up being able to display 19 clients at once.
	//In a team game, if there are 9 or more clients on the team not in the lead,
	//we only want to show 10 of the clients on the team in the lead, so that we
	//have room to display the clients in the lead on the losing team.

	//I guess this can be accomplished simply by printing the first teams score with a maxClients
	//value passed in related to how many players are on both teams.
	if ( cgs.gametype >= GT_TEAM ) {
		//
		// teamplay scoreboard
		//
		y += lineHeight/2;

		if ( cg.teamScores[0] >= cg.teamScores[1] ) {
			int team1MaxCl = CG_GetTeamCount(TEAM_RED, maxClients);
			int team2MaxCl = CG_GetTeamCount(TEAM_BLUE, maxClients);

			if (team1MaxCl > 10 && (team1MaxCl+team2MaxCl) > maxClients)
			{
				team1MaxCl -= team2MaxCl;
				//subtract as many as you have to down to 10, once we get there
				//we just set it to 10

				if (team1MaxCl < 10)
				{
					team1MaxCl = 10;
				}
			}

			team2MaxCl = (maxClients-team1MaxCl); //team2 can display however many is left over after team1's display

			n1 = CG_TeamScoreboard( y, TEAM_RED, fade, team1MaxCl, lineHeight, qtrue );
			CG_DrawTeamBackground( SB_SCORELINE_X - 5, y - topBorderSize, SB_SCORELINE_WIDTH + 10, n1 * lineHeight + bottomBorderSize, 0.33f, TEAM_RED );
			CG_TeamScoreboard( y, TEAM_RED, fade, team1MaxCl, lineHeight, qfalse );
			if (jkcvar_cg_scoreboardExtras.integer)  CG_DrawTeamBackground( SB_SCORELINE_X - 5, y + 2, SB_SCORELINE_WIDTH + 10, 1, 0.5f, TEAM_RED ); // Tr!Force: [Scoreboard] Team thin border
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			//maxClients -= n1;

			n2 = CG_TeamScoreboard( y, TEAM_BLUE, fade, team2MaxCl, lineHeight, qtrue );
			CG_DrawTeamBackground( SB_SCORELINE_X - 5, y - topBorderSize, SB_SCORELINE_WIDTH + 10, n2 * lineHeight + bottomBorderSize, 0.33f, TEAM_BLUE );
			CG_TeamScoreboard( y, TEAM_BLUE, fade, team2MaxCl, lineHeight, qfalse );
			if (jkcvar_cg_scoreboardExtras.integer)  CG_DrawTeamBackground( SB_SCORELINE_X - 5, y + 2, SB_SCORELINE_WIDTH + 10, 1, 0.5f, TEAM_BLUE); // Tr!Force: [Scoreboard] Team thin border
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;

			//maxClients -= n2;

			maxClients -= (team1MaxCl+team2MaxCl);
		} else {
			int team1MaxCl = CG_GetTeamCount(TEAM_BLUE, maxClients);
			int team2MaxCl = CG_GetTeamCount(TEAM_RED, maxClients);

			if (team1MaxCl > 10 && (team1MaxCl+team2MaxCl) > maxClients)
			{
				team1MaxCl -= team2MaxCl;
				//subtract as many as you have to down to 10, once we get there
				//we just set it to 10

				if (team1MaxCl < 10)
				{
					team1MaxCl = 10;
				}
			}

			team2MaxCl = (maxClients-team1MaxCl); //team2 can display however many is left over after team1's display

			n1 = CG_TeamScoreboard( y, TEAM_BLUE, fade, team1MaxCl, lineHeight, qtrue );
			CG_DrawTeamBackground( SB_SCORELINE_X - 5, y - topBorderSize, SB_SCORELINE_WIDTH + 10, n1 * lineHeight + bottomBorderSize, 0.33f, TEAM_BLUE );
			CG_TeamScoreboard( y, TEAM_BLUE, fade, team1MaxCl, lineHeight, qfalse );
			if (jkcvar_cg_scoreboardExtras.integer)  CG_DrawTeamBackground( SB_SCORELINE_X - 5, y + 2, SB_SCORELINE_WIDTH + 10, 1, 0.5f, TEAM_BLUE ); // Tr!Force: [Scoreboard] Team thin border
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;

			//maxClients -= n1;

			n2 = CG_TeamScoreboard( y, TEAM_RED, fade, team2MaxCl, lineHeight, qtrue );
			CG_DrawTeamBackground( SB_SCORELINE_X - 5, y - topBorderSize, SB_SCORELINE_WIDTH + 10, n2 * lineHeight + bottomBorderSize, 0.33f, TEAM_RED );
			CG_TeamScoreboard( y, TEAM_RED, fade, team2MaxCl, lineHeight, qfalse );
			if (jkcvar_cg_scoreboardExtras.integer)  CG_DrawTeamBackground( SB_SCORELINE_X - 5, y + 2, SB_SCORELINE_WIDTH + 10, 1, 0.5f, TEAM_RED ); // Tr!Force: [Scoreboard] Team thin border
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;

			//maxClients -= n2;

			maxClients -= (team1MaxCl+team2MaxCl);
		}
		n1 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients, lineHeight, qfalse );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;

	} else {
		//
		// free for all scoreboard
		//
		n1 = CG_TeamScoreboard( y, TEAM_FREE, fade, maxClients, lineHeight, qfalse );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
		n2 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients - n1, lineHeight, qfalse );
		y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
	}

	if (!localClient) {
		// draw local client at the bottom
		for ( i = 0 ; i < cg.numScores ; i++ ) {
			if ( cg.scores[i].client == cg.snap->ps.clientNum ) {
				CG_DrawClientScore( y, &cg.scores[i], fadeColor, fade, lineHeight == SB_NORMAL_HEIGHT );
				break;
			}
		}
	}

	// load any models that have been deferred
	if ( ++cg.deferredPlayerLoading > 10 ) {
		CG_LoadDeferredPlayers();
	}

	return qtrue;
}

//================================================================================

