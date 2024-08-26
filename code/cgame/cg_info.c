// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_info.c -- display information while data is being loading

#include "cg_local.h"

#define MAX_LOADING_PLAYER_ICONS	16
#define MAX_LOADING_ITEM_ICONS		26

// Tr!Force: [CGameGeneral] Restored loading icons
static int			loadingPlayerIconCount;
static int			loadingItemIconCount;
static qhandle_t	loadingPlayerIcons[MAX_LOADING_PLAYER_ICONS];
static qhandle_t	loadingItemIcons[MAX_LOADING_ITEM_ICONS];

extern float CG_Cvar_Get(const char *cvar);
void CG_LoadBar(void);


/*
===================
CG_DrawLoadingIcons
===================
*/
static void CG_DrawLoadingIcons(int currentY) { // Tr!Force: [CGameGeneral] Restored loading icons
	int n, n2;
    int x, y, x2, y2;

	// Player icons horizontal align
    int playerIconWidth = 32;
    int playerIconSpacing = 16;
    int totalPlayerIconWidth = (loadingPlayerIconCount > 0) ? loadingPlayerIconCount * (playerIconWidth + playerIconSpacing) - playerIconSpacing : 0;
    int playerIconsStartX = (cgs.screenWidth - totalPlayerIconWidth) / 2;

    // Item icons horizontal align
    int itemIconWidth = 32;
    int itemIconSpacing = 16;
    int iconsPerRow = (loadingItemIconCount > 13) ? 13 : loadingItemIconCount;
    int totalItemIconWidth = (iconsPerRow > 0) ? iconsPerRow * (itemIconWidth + itemIconSpacing) - itemIconSpacing : 0;
    int itemIconsStartX = (cgs.screenWidth - totalItemIconWidth) / 2;

    // Item icons vertical align
    int paddingBottom = 15;
    int rows = (loadingItemIconCount > 0) ? (loadingItemIconCount + iconsPerRow - 1) / iconsPerRow : 0;
    int totalItemIconHeight = (rows > 0) ? rows * (itemIconWidth + itemIconSpacing) - itemIconSpacing : 0;
    int availableHeight = cgs.screenHeight - currentY - paddingBottom;
    int itemIconsStartY = currentY + (availableHeight - totalItemIconHeight) / 2;

	// Draw player icons
	if (loadingPlayerIconCount > 0) {
		for (n = 0; n < loadingPlayerIconCount; n++) {
			x = playerIconsStartX + n * (playerIconWidth + playerIconSpacing);
			y = (mvapi ? 90 : 80) - 40;
			CG_DrawPic(x, y, playerIconWidth, playerIconWidth, loadingPlayerIcons[n]);
		}
	}

	// Draw item icons
	if (loadingItemIconCount > 0) {
		for (n2 = 0; n2 < loadingItemIconCount; n2++) {
			y2 = itemIconsStartY + (n2 / 13) * (itemIconWidth + itemIconSpacing);
			x2 = itemIconsStartX + (n2 % 13) * (itemIconWidth + itemIconSpacing);
			CG_DrawPic(x2, y2, itemIconWidth, itemIconWidth, loadingItemIcons[n2]);
		}
	}
}

/*
======================
CG_LoadingString

======================
*/
void CG_LoadingString( const char *s ) {
	Q_strncpyz( cg.infoScreenText, s, sizeof( cg.infoScreenText ) );

	trap_UpdateScreen();
}

/*
===================
CG_LoadingItem
===================
*/
void CG_LoadingItem( int itemNum ) {
	gitem_t		*item;

	item = &bg_itemlist[itemNum];

	// Tr!Force: [CGameGeneral] Restored loading icons
	if ( jkcvar_cg_loadingIcons.integer && item->icon && loadingItemIconCount < MAX_LOADING_ITEM_ICONS ) {
		loadingItemIcons[loadingItemIconCount++] = item->giTag == WP_BRYAR_PISTOL ? cgs.media.weaponIcons[WP_BRYAR_PISTOL] : trap_R_RegisterShaderNoMip( item->icon );
	}

	CG_LoadingString( CG_GetStripEdString("INGAME",item->classname) );
}

/*
===================
CG_LoadingClient
===================
*/
void CG_LoadingClient( int clientNum ) {
	const char		*info;
	char			personality[MAX_QPATH];

	info = CG_ConfigString( CS_PLAYERS + clientNum );

if (jkcvar_cg_loadingIcons.integer) // Tr!Force: [CGameGeneral] Restored loading icons
{
	char			model[MAX_QPATH];
	char			iconName[MAX_QPATH];
	char			*skin;
	if ( loadingPlayerIconCount < MAX_LOADING_PLAYER_ICONS ) {
		Q_strncpyz( model, Info_ValueForKey( info, "model" ), sizeof( model ) );
		skin = Q_strrchr( model, '/' );
		if ( skin ) {
			*skin++ = '\0';
		} else {
			skin = "default";
		}

		Com_sprintf( iconName, MAX_QPATH, "models/players/%s/icon_%s.tga", model, skin );
		
		loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		if ( !loadingPlayerIcons[loadingPlayerIconCount] ) {
			Com_sprintf( iconName, MAX_QPATH, "models/players/characters/%s/icon_%s.tga", model, skin );
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		}
		if ( !loadingPlayerIcons[loadingPlayerIconCount] ) {
			Com_sprintf( iconName, MAX_QPATH, "models/players/%s/icon_%s.tga", DEFAULT_MODEL, "default" );
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		}
		if ( loadingPlayerIcons[loadingPlayerIconCount] ) {
			loadingPlayerIconCount++;
		}
	}
}
	Q_strncpyz( personality, Info_ValueForKey( info, "n" ), sizeof(personality) );
	Q_CleanStr( personality, (qboolean)(jk2startversion == VERSION_1_02) );

	if( cgs.gametype == GT_SINGLE_PLAYER ) {
		trap_S_RegisterSound( va( "sound/player/announce/%s.wav", personality ));
	}

	CG_LoadingString( personality );
}


/*
====================
CG_DrawInformation

Draw all the status / pacifier stuff during level loading
====================
*/
#define UI_INFOFONT (UI_BIGFONT)
void CG_DrawInformation( void ) {
	const char	*s;
	const char	*info;
	const char	*sysInfo;
	const float	x = 0.5f * cgs.screenWidth;
	int			y;
	int			value, valueNOFP;
	qhandle_t	levelshot;
	char		buf[1024];
	int			iPropHeight = 18;	// I know, this is total crap, but as a post release asian-hack....  -Ste
	const char *temp;	// Tr!Force: [CGameGeneral] Save space for loading icons

	info = CG_ConfigString( CS_SERVERINFO );
	sysInfo = CG_ConfigString( CS_SYSTEMINFO );

	s = Info_ValueForKey( info, "mapname" );

	// Tr!Force: [CGameGeneral] Custom levelshots check
	levelshot = trap_R_RegisterShaderNoMip( va( "levelshots/%s", s ) );
	if ( levelshot ) {
		trap_R_SetColor( NULL );
		CG_DrawPic(0, 0, cgs.screenWidth, cgs.screenHeight, levelshot);
	} else {
		levelshot = trap_R_RegisterShaderNoMip("menu/art/unknownmap");
		trap_R_SetColor( NULL );
		CG_FillRect(0, 0, cgs.screenWidth, cgs.screenHeight, colorBlack);
		CG_DrawPic((cgs.screenWidth - (cgs.screenHeight * ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT))) / 2, 0, cgs.screenHeight * ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT), cgs.screenHeight, levelshot);
	}

	CG_LoadBar();

	// the first 150 rows are reserved for the client connection
	// screen to write into
	if ( cg.infoScreenText[0] ) {
		const char *psLoading = CG_GetStripEdString("MENUS3", "LOADING_MAPNAME");
		UI_DrawProportionalString( x, (cgs.screenHeight*0.27f)-2-32, va(/*"Loading... %s"*/ psLoading, cg.infoScreenText),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
	} else {
		const char *psAwaitingSnapshot = CG_GetStripEdString("MENUS3", "AWAITING_SNAPSHOT");
		UI_DrawProportionalString( x, (cgs.screenHeight*0.27f)-2-32, /*"Awaiting snapshot..."*/psAwaitingSnapshot,
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
	}

	// draw info string information

	y = (cgs.screenHeight*0.375f)-32;

	// don't print server lines if playing a local game
	trap_Cvar_VariableStringBuffer( "sv_running", buf, sizeof( buf ) );
	if ( !atoi( buf ) ) {
		// server hostname
		Q_strncpyz(buf, Info_ValueForKey( info, "sv_hostname" ), 1024);
		Q_CleanStr(buf, (qboolean)(jk2startversion == VERSION_1_02));
		if ( jkcvar_cg_loadingIcons.integer ) { // Tr!Force: [CGameGeneral] Save space for loading icons
			temp = Info_ValueForKey(sysInfo, "sv_pure");
			if ( temp[0] == '1' ) {
				Q_strcat( buf, MAX_INFO_VALUE, va(" (%s)", CG_GetStripEdString("JKMENUS", "PURE_SERVER")) );
			}
		}
		UI_DrawProportionalString( x, y, buf,
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;

		// pure server
		if ( !jkcvar_cg_loadingIcons.integer ) {  // Tr!Force: [CGameGeneral] Save space for loading icons
			s = Info_ValueForKey( sysInfo, "sv_pure" );
			if ( s[0] == '1' ) {
				const char *psPure = CG_GetStripEdString("INGAMETEXT", "PURE_SERVER");
				UI_DrawProportionalString( x, y, psPure,
					UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
				y += iPropHeight;
			}
		}

		// server-specific message of the day
		s = CG_ConfigString( CS_MOTD );
		if ( s[0] ) {
			UI_DrawProportionalString( x, y, s,
				UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
			y += iPropHeight;
		}

		// Tr!Force: [UIGeneral] Check motd
		if (!CG_Cvar_Get("jk_ui_hideMotd"))
		{	// display global MOTD at bottom (mirrors ui_main UI_DrawConnectScreen
			char motdString[1024];
			trap_Cvar_VariableStringBuffer( "cl_motdString", motdString, sizeof( motdString ) );

			if (motdString[0])
			{
				UI_DrawProportionalString( x, cgs.screenHeight-55, motdString,
					UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
			}
		}

		// some extra space after hostname and motd
		y += 10;
	}

	// map-specific message (long map name)
	s = CG_ConfigString( CS_MESSAGE );
	if ( s[0] ) {
		UI_DrawProportionalString( x, y, s,
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
	}

	// cheats warning
	if ( !jkcvar_cg_loadingIcons.integer ) {  // Tr!Force: [CGameGeneral] Save space for loading icons
		s = Info_ValueForKey( sysInfo, "sv_cheats" );
		if ( s[0] == '1' ) {
			UI_DrawProportionalString( x, y, CG_GetStripEdString("INGAMETEXT", "CHEATSAREENABLED"),
				UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
			y += iPropHeight;
		}
	}

	// game type
	switch ( cgs.gametype ) {
	case GT_FFA:
		s = "Free For All";
		break;
	case GT_HOLOCRON:
		s = "Holocron FFA";
		break;
	case GT_JEDIMASTER:
		s = "Jedi Master";
		break;
	case GT_SINGLE_PLAYER:
		s = "Single Player";
		break;
	case GT_TOURNAMENT:
		s = "Duel";
		break;
	case GT_TEAM:
		s = "Team FFA";
		break;
	case GT_SAGA:
		s = "N/A";
		break;
	case GT_CTF:
		s = "Capture The Flag";
		break;
	case GT_CTY:
		s = "Capture The Ysalamiri";
		break;
	default:
		s = "Unknown Gametype";
		break;
	}
	if ( jkcvar_cg_loadingIcons.integer ) { // Tr!Force: [CGameGeneral] Save space for loading icons
		temp = Info_ValueForKey(sysInfo, "sv_cheats");
		if ( temp[0] == '1' ) {
			s = va("%s (%s)", s, CG_GetStripEdString("JKMENUS", "CHEATSAREENABLED"));
		}
	}
	UI_DrawProportionalString( x, y, s,
		UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
	y += iPropHeight;
		
	value = atoi( Info_ValueForKey( info, "timelimit" ) );
	if ( value ) {
		UI_DrawProportionalString( x, y,
			va( "%s %i", CG_GetStripEdString("INGAMETEXT", "TIMELIMIT"), value ),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
	}

	if (cgs.gametype < GT_CTF ) {
		value = atoi( Info_ValueForKey( info, "fraglimit" ) );
		if ( value ) {
			UI_DrawProportionalString( x, y,
				va( "%s %i", CG_GetStripEdString("INGAMETEXT", "FRAGLIMIT"), value ),
				UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
			y += iPropHeight;
		}

		if (cgs.gametype == GT_TOURNAMENT)
		{
			value = atoi( Info_ValueForKey( info, "duel_fraglimit" ) );
			if ( value ) {
				UI_DrawProportionalString( x, y,
					va( "%s %i", CG_GetStripEdString("INGAMETEXT", "WINLIMIT"), value ),
					UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
				y += iPropHeight;
			}
		}
	}

	if (cgs.gametype >= GT_CTF) {
		value = atoi( Info_ValueForKey( info, "capturelimit" ) );
		if ( value ) {
			UI_DrawProportionalString( x, y, va( "%s %i", CG_GetStripEdString("INGAMETEXT", "CAPTURELIMIT"), value ),
				UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
			y += iPropHeight;
		}
	}

	if (cgs.gametype >= GT_TEAM)
	{
		value = atoi( Info_ValueForKey( info, "g_forceBasedTeams" ) );
		if ( value ) {
			UI_DrawProportionalString( x, y, CG_GetStripEdString("INGAMETEXT", "FORCEBASEDTEAMS"),
				UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
			y += iPropHeight;
		}
	}

	valueNOFP = atoi( Info_ValueForKey( info, "g_forcePowerDisable" ) );

	value = atoi( Info_ValueForKey( info, "g_maxForceRank" ) );
	if ( value && !valueNOFP ) {
		char fmStr[1024];
		char *text;

		trap_SP_GetStringTextString("INGAMETEXT_MAXFORCERANK",fmStr, sizeof(fmStr));

		if ( value >= 0 && value < (int)ARRAY_LEN(forceMasteryLevels) ) {
			text = va( "%s %s", fmStr, CG_GetStripEdString("INGAMETEXT", forceMasteryLevels[value]) );
		} else {
			text = va( "%s %i", fmStr, value );
		}

		Q_strncpyz( buf, text, MAX_INFO_VALUE );
		if ( jkcvar_cg_loadingIcons.integer ) { // Tr!Force: [CGameGeneral] Save space for loading icons
			value = cgs.gametype == GT_TOURNAMENT ? atoi( Info_ValueForKey( info, "g_duelWeaponDisable" ) ) : atoi( Info_ValueForKey( info, "g_weaponDisable" ) );
			if ( cgs.gametype != GT_JEDIMASTER && value ) {
				Q_strcat( buf, MAX_INFO_VALUE, va(" (%s)", CG_GetStripEdString("INGAMETEXT", "SABERONLYSET")) );
			}
		}
		UI_DrawProportionalString( x, y, buf, UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
	}
	else if (!valueNOFP)
	{
		char fmStr[1024];
		char text[1024];
		trap_SP_GetStringTextString("INGAMETEXT_MAXFORCERANK",fmStr, sizeof(fmStr));

		Q_strncpyz( text, CG_GetStripEdString("INGAMETEXT", forceMasteryLevels[7]), sizeof(text) );
		if ( jkcvar_cg_loadingIcons.integer ) { // Tr!Force: [CGameGeneral] Save space for loading icons
			value = cgs.gametype == GT_TOURNAMENT ? atoi( Info_ValueForKey( info, "g_duelWeaponDisable" ) ) : atoi( Info_ValueForKey( info, "g_weaponDisable" ) );
			if ( cgs.gametype != GT_JEDIMASTER && value ) {
				Q_strcat( text, MAX_INFO_VALUE, va(" (%s)", CG_GetStripEdString("INGAMETEXT", "SABERONLYSET")) );
			}
		}
		UI_DrawProportionalString( x, y, va( "%s %s", fmStr, text ),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
	}

	if ( !jkcvar_cg_loadingIcons.integer ) { // Tr!Force: [CGameGeneral] Save space for loading icons
		if (cgs.gametype == GT_TOURNAMENT)
		{
			value = atoi( Info_ValueForKey( info, "g_duelWeaponDisable" ) );
		}
		else
		{
			value = atoi( Info_ValueForKey( info, "g_weaponDisable" ) );
		}
		if ( cgs.gametype != GT_JEDIMASTER && value ) {
			UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "SABERONLYSET") ),
				UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
			y += iPropHeight;
		}
	}

	if ( valueNOFP ) {
		char text[1024];
		Q_strncpyz( text, CG_GetStripEdString("INGAMETEXT", "NOFPSET"), sizeof(text) );
		if ( jkcvar_cg_loadingIcons.integer ) { // Tr!Force: [CGameGeneral] Save space for loading icons
			value = cgs.gametype == GT_TOURNAMENT ? atoi( Info_ValueForKey( info, "g_duelWeaponDisable" ) ) : atoi( Info_ValueForKey( info, "g_weaponDisable" ) );
			if ( cgs.gametype != GT_JEDIMASTER && value ) {
				Q_strcat( text, MAX_INFO_VALUE, va(" (%s)", CG_GetStripEdString("INGAMETEXT", "SABERONLYSET")) );
			}
		}
		UI_DrawProportionalString( x, y, va( "%s", text),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
	}


	// Display the rules based on type
		y += iPropHeight;
	switch ( cgs.gametype ) {
	case GT_FFA:					
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_FFA_1")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		break;
	case GT_HOLOCRON:
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_HOLO_1")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_HOLO_2")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		break;
	case GT_JEDIMASTER:
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_JEDI_1")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_JEDI_2")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		break;
	case GT_SINGLE_PLAYER:
		break;
	case GT_TOURNAMENT:
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_DUEL_1")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_DUEL_2")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		break;
	case GT_TEAM:
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_TEAM_1")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_TEAM_2")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		break;
	case GT_SAGA:
		break;
	case GT_CTF:
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_CTF_1")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_CTF_2")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		break;
	case GT_CTY:
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_CTY_1")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		UI_DrawProportionalString( x, y, va( "%s", (char *)CG_GetStripEdString("INGAMETEXT", "RULES_CTY_2")),
			UI_CENTER|UI_INFOFONT|UI_DROPSHADOW, colorWhite );
		y += iPropHeight;
		break;
	default:
		break;
	}
				   
	// draw the icons of things as they are loaded
	if (jkcvar_cg_loadingIcons.integer) CG_DrawLoadingIcons(y); // Tr!Force: [CGameGeneral] Restored loading icons
}

/*
===================
CG_LoadBar
===================
*/
void CG_LoadBar(void)
{
	const int numticks = 9, tickwidth = 40, tickheight = 8;
	const int tickpadx = 20, tickpady = 12;
	const int capwidth = 8;
	const float barwidth = numticks*tickwidth+tickpadx*2+capwidth*2;
	const float barleft = 0.5f * (cgs.screenWidth - barwidth);
	const int barheight = tickheight + tickpady*2, bartop = cgs.screenHeight-barheight;
	const float capleft = barleft+tickpadx;
	const float tickleft = capleft+capwidth;
	const float ticktop = bartop+tickpady;

	trap_R_SetColor( colorWhite );
	// Draw background
	CG_DrawPic(barleft, bartop, barwidth, barheight, cgs.media.loadBarLEDSurround);

	// Draw left cap (backwards)
	CG_DrawPic(tickleft, ticktop, -capwidth, tickheight, cgs.media.loadBarLEDCap);

	// Draw bar
	CG_DrawPic(tickleft, ticktop, tickwidth*cg.loadLCARSStage, tickheight, cgs.media.loadBarLED);

	// Draw right cap
	CG_DrawPic(tickleft+tickwidth*cg.loadLCARSStage, ticktop, capwidth, tickheight, cgs.media.loadBarLEDCap);
}

