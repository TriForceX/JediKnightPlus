//bg_multiversion.c - This file contains multiversion variables and functions that are shared accross all modules...
#include "../game/bg_multiversion.h"

#ifdef JK2_GAME
#include "../game/g_local.h"
#elif JK2_CGAME
#include "../cgame/cg_local.h"
#elif JK2_UI
#include "../ui/ui_local.h"
#endif

mvversion_t	jk2version      = VERSION_UNDEF;  // Current engine gameversion
mvversion_t	jk2gameplay     = VERSION_UNDEF;  // Current gameplay to apply
mvversion_t	jk2startversion = VERSION_UNDEF;  // Gameversion the engine had when the module was loaded

#ifndef JK2_UI // Not defined for UI, but still in bg_* for game and cgame
/* Additional functions */
extern stringID_table_t animTable_1_02[MAX_ANIMATIONS_1_02+1];
extern stringID_table_t animTable[MAX_ANIMATIONS+1];
//int animMappingTable_1_04_to_1_02[MAX_TOTALANIMATIONS];
//int animMappingTable_1_02_to_1_04[MAX_TOTALANIMATIONS_1_02];

#ifdef MV_GENERATE_ANIMATION_MAPPING_TABLE
void MV_DumpAnimationMappingTable( void )
{
	fileHandle_t dumpFile;
	char		 line[512];
	int			 i, j;

	trap_FS_FOpenFile("animMappingTableDump.txt", &dumpFile, FS_WRITE);
	if ( !dumpFile )
	{
		trap_FS_FCloseFile(dumpFile);
		return;
	}

	Q_strncpyz( line, "animNumber_1_02_t animMappingTable_1_04_to_1_02[MAX_TOTALANIMATIONS] = \n"
					"{\n", sizeof(line) );
	trap_FS_Write( line, strlen(line), dumpFile );

	for ( i = 0; i < MAX_TOTALANIMATIONS; i++ )
	{
		for ( j = 0; j < sizeof(animTable_1_02); j++ )
		{
			if ( animTable_1_02[j].id == animMappingTable_1_04_to_1_02[i] )
			{
				Q_strncpyz( line, va("	%s, // %s\n", animTable_1_02[j].name, animTable[i].name), sizeof(line) ); // It's only an assumption that animTable[i].name is the right 1.04 name. :/
				trap_FS_Write( line, strlen(line), dumpFile );
				break;
			}
		}
	}

	Q_strncpyz( line, "};\n\n", sizeof(line) );
	trap_FS_Write( line, strlen(line), dumpFile );

	Q_strncpyz( line, "animNumber_t animMappingTable_1_02_to_1_04[MAX_TOTALANIMATIONS_1_02] = \n"
					"{\n", sizeof(line) );
	trap_FS_Write( line, strlen(line), dumpFile );

	for ( i = 0; i < MAX_TOTALANIMATIONS_1_02; i++ )
	{
		for ( j = 0; j < sizeof(animTable); j++ )
		{
			if ( animTable[j].id == animMappingTable_1_02_to_1_04[i] )
			{
				Q_strncpyz( line, va("	%s, // %s\n", animTable[j].name, animTable_1_02[i].name), sizeof(line) ); // It's only an assumption that animTable_1_02[i].name is the right 1.02 name. :/
				trap_FS_Write( line, strlen(line), dumpFile );
				break;
			}
		}
	}

	Q_strncpyz( line, "};\n\n", sizeof(line) );
	trap_FS_Write( line, strlen(line), dumpFile );

	trap_FS_FCloseFile( dumpFile );
	return;
}
#endif // #ifdef MV_GENERATE_ANIMATION_MAPPING_TABLE

void MV_BuildAnimationMappingTable( void )
{
#ifdef MV_GENERATE_ANIMATION_MAPPING_TABLE
	qboolean found = qfalse;
	int i, j;

	// 1.04 to 1.02
	Com_Printf("Building animationMappingTable 1.04 to 1.02!\n");
	for ( i = 0; i < MAX_TOTALANIMATIONS; i++ )
	{
		found = qfalse;
		for ( j = 0; j < MAX_TOTALANIMATIONS_1_02; j++ )
		{
			if ( !Q_stricmp(va("%s_1_02", animTable[i].name), animTable_1_02[j].name) )
			{
				animMappingTable_1_04_to_1_02[animTable[i].id] = animTable_1_02[j].id;
				found = qtrue;
			}
		}
		if ( !found ) animMappingTable_1_04_to_1_02[i] = 0;
	}

#ifdef _DEBUG
	for ( i = 0; i < MAX_TOTALANIMATIONS; i++ )
	{
		Com_Printf("%s to %s\n", animTable[i].name, animTable_1_02[animMappingTable_1_04_to_1_02[i]].name);
	}
#endif
	
	// 1.02 to 1.04
	Com_Printf("Building animationMappingTable 1.02 to 1.04!\n");
	for ( i = 0; i < MAX_TOTALANIMATIONS_1_02; i++ )
	{
		found = qfalse;
		for ( j = 0; j < MAX_TOTALANIMATIONS; j++ )
		{
			if ( !Q_stricmp(animTable_1_02[i].name, va("%s_1_02", animTable[j].name)) )
			{
				animMappingTable_1_02_to_1_04[animTable_1_02[i].id] = animTable[j].id;
				found = qtrue;
			}
		}
		if ( !found ) animMappingTable_1_02_to_1_04[i] = 0;
	}

#ifdef _DEBUG
	for ( i = 0; i < MAX_TOTALANIMATIONS_1_02; i++ )
	{
		Com_Printf("%s to %s\n", animTable_1_02[i].name, animTable[animMappingTable_1_02_to_1_04[i]].name);
	}
#endif

	Com_Printf("AnimTable built!\n");
#endif // #ifdef MV_GENERATE_ANIMATION_MAPPING_TABLE
}

extern int forcePowerNeeded_1_02[NUM_FORCE_POWER_LEVELS][NUM_FORCE_POWERS];
extern int forcePowerNeeded_1_04[NUM_FORCE_POWER_LEVELS][NUM_FORCE_POWERS];
extern int (*forcePowerNeeded)[NUM_FORCE_POWERS];
void MV_SetGamePlay( mvversion_t version )
{ /* Set weaponData (ammo costs, ...), forcePowerNeeded (force costs) and the gameplay value according to the new version. */
#ifdef JK2_GAME
	gentity_t *saberent;
	gentity_t *ent;
	int		   i;
#endif // #ifdef JK2_GAME

	/* Find the settings for the version we want to switch to */
	switch ( version )
	{
		case VERSION_1_02:
			weaponData = weaponData_1_02;
			forcePowerNeeded = forcePowerNeeded_1_02;
			jk2gameplay = VERSION_1_02;
			break;
		case VERSION_1_03:
			weaponData = weaponData_1_03;
			forcePowerNeeded = forcePowerNeeded_1_04;
			jk2gameplay = VERSION_1_03;
			break;
		default:
		case VERSION_1_04:
			weaponData = weaponData_1_04;
			forcePowerNeeded = forcePowerNeeded_1_04;
			jk2gameplay = VERSION_1_04;
			break;
	}
	
#ifdef JK2_GAME
	/* Client specific changes */
	for ( i = 0; i < MAX_CLIENTS; i++ )
	{
		ent = &g_entities[i];

		if ( ent && ent->client && ent->inuse )
		{
			/* Adjust the size of the saberbox (8.0f in 1.02, 16.0f in 1.03 and 1.04). The define for SABER_BOX_SIZE already contains the check for the current version, so we can just use the same code for all 3 versions here. */
			saberent = &g_entities[ent->client->ps.saberEntityNum];
			if (saberent)
			{
				VectorSet( saberent->r.mins, -SABER_BOX_SIZE, -SABER_BOX_SIZE, -SABER_BOX_SIZE );
				VectorSet( saberent->r.maxs, SABER_BOX_SIZE, SABER_BOX_SIZE, SABER_BOX_SIZE );
			}
		}
	}
	
	for ( i = 0; i < MAX_CLIENTS; i++ ) if ( g_entities[i].client && g_entities[i].inuse ) ClientUserinfoChanged(i); // Update every client's userinfo to reflect the gameplay change (assigning the value to every player opens the possibility to intoduce per-player gameplay in the future)
#endif // #ifdef JK2_GAME
}
#endif // #ifndef JK2_UI // Not defined for UI, but still in bg_* for game and cgame

#ifndef JK2_GAME

// MVSDK: Multiversion keys...
static mvKeyconversion_t mvKeyconversion[] =
{
	{ K_TAB, A_TAB },
	{ K_ENTER, A_ENTER },
	{ K_ESCAPE, A_ESCAPE },
	{ K_SPACE, A_SPACE },

	{ K_BACKSPACE, A_BACKSPACE },

	//{ K_COMMAND, A_COMMAND },
	{ K_CAPSLOCK, A_CAPSLOCK },
	//{ K_POWER, A_POWER },
	{ K_PAUSE, A_PAUSE },

	{ K_UPARROW, A_CURSOR_UP },
	{ K_DOWNARROW, A_CURSOR_DOWN },
	{ K_LEFTARROW, A_CURSOR_LEFT },
	{ K_RIGHTARROW, A_CURSOR_RIGHT },

	{ K_ALT, A_ALT },
	{ K_ALT, A_ALT2 }, // 1.02 only knows ALT, 1.03+ knows ALT and ALTGR
	{ K_CTRL, A_CTRL },
	{ K_SHIFT, A_SHIFT },
	{ K_INS, A_INSERT },
	{ K_DEL, A_DELETE },
	{ K_PGDN, A_PAGE_DOWN },
	{ K_PGUP, A_PAGE_UP },
	{ K_HOME, A_HOME },
	{ K_END, A_END },

	{ K_F1, A_F1 },
	{ K_F2, A_F2 },
	{ K_F3, A_F3 },
	{ K_F4, A_F4 },
	{ K_F5, A_F5 },
	{ K_F6, A_F6 },
	{ K_F7, A_F7 },
	{ K_F8, A_F8 },
	{ K_F9, A_F9 },
	{ K_F10, A_F10 },
	{ K_F11, A_F11 },
	{ K_F12, A_F12 },
	/*
	{ K_F13, A_F13 },
	{ K_F14, A_F14 },
	{ K_F15, A_F15 },
	*/

	{ K_KP_HOME, A_KP_7 },
	{ K_KP_UPARROW, A_KP_8 },
	{ K_KP_PGUP, A_KP_9 },
	{ K_KP_LEFTARROW, A_KP_4 },
	{ K_KP_5, A_KP_5 },
	{ K_KP_RIGHTARROW, A_KP_6 },
	{ K_KP_END, A_KP_1 },
	{ K_KP_DOWNARROW, A_KP_2 },
	{ K_KP_PGDN, A_KP_3 },
	{ K_KP_ENTER, A_KP_ENTER },
	{ K_KP_INS, A_KP_0 },
	{ K_KP_DEL, A_KP_PERIOD },
	{ K_KP_SLASH, A_DIVIDE },
	{ K_KP_MINUS, A_KP_MINUS },
	{ K_KP_PLUS, A_KP_PLUS },
	{ K_KP_NUMLOCK, A_NUMLOCK },
	{ K_KP_STAR, A_MULTIPLY },
	//{ K_KP_EQUALS, A_KP_EQUALS },

	{ K_MOUSE1, A_MOUSE1 },
	{ K_MOUSE2, A_MOUSE2 },
	{ K_MOUSE3, A_MOUSE3 },
	{ K_MOUSE4, A_MOUSE4 },
	{ K_MOUSE5, A_MOUSE5 },

	{ K_MWHEELDOWN, A_MWHEELDOWN },
	{ K_MWHEELUP, A_MWHEELUP },

	{ K_JOY1, A_JOY1 },
	{ K_JOY2, A_JOY2 },
	{ K_JOY3, A_JOY3 },
	{ K_JOY4, A_JOY4 },
	{ K_JOY5, A_JOY5 },
	{ K_JOY6, A_JOY6 },
	{ K_JOY7, A_JOY7 },
	{ K_JOY8, A_JOY8 },
	{ K_JOY9, A_JOY9 },
	{ K_JOY10, A_JOY10 },
	{ K_JOY11, A_JOY11 },
	{ K_JOY12, A_JOY12 },
	{ K_JOY13, A_JOY13 },
	{ K_JOY14, A_JOY14 },
	{ K_JOY15, A_JOY15 },
	{ K_JOY16, A_JOY16 },
	{ K_JOY17, A_JOY17 },
	{ K_JOY18, A_JOY18 },
	{ K_JOY19, A_JOY19 },
	{ K_JOY20, A_JOY20 },
	{ K_JOY21, A_JOY21 },
	{ K_JOY22, A_JOY22 },
	{ K_JOY23, A_JOY23 },
	{ K_JOY24, A_JOY24 },
	{ K_JOY25, A_JOY25 },
	{ K_JOY26, A_JOY26 },
	{ K_JOY27, A_JOY27 },
	{ K_JOY28, A_JOY28 },
	{ K_JOY29, A_JOY29 },
	{ K_JOY30, A_JOY30 },
	{ K_JOY31, A_JOY31 },
	// { K_JOY32, A_JOY0 }, //FIXME: 1.02 has JOY32, 1.04 has JOY0, but they're not really mapped

	{ K_AUX1, A_AUX1 },
	{ K_AUX2, A_AUX2 },
	{ K_AUX3, A_AUX3 },
	{ K_AUX4, A_AUX4 },
	{ K_AUX5, A_AUX5 },
	{ K_AUX6, A_AUX6 },
	{ K_AUX7, A_AUX7 },
	{ K_AUX8, A_AUX8 },
	{ K_AUX9, A_AUX9 },
	{ K_AUX10, A_AUX10 },
	{ K_AUX11, A_AUX11 },
	{ K_AUX12, A_AUX12 },
	{ K_AUX13, A_AUX13 },
	{ K_AUX14, A_AUX14 },
	{ K_AUX15, A_AUX15 },
	{ K_AUX16, A_AUX16 },

	{ K_LAST_KEY, MAX_KEYS },
};
static int mvKeyconversionCount = sizeof(mvKeyconversion) / sizeof(mvKeyconversion[0]);


int Key_GetProtocolKey_New(mvversion_t version, int key, qboolean to15, qboolean invert) {
	int i;

	// We don't need to convert anything if we're not dealing with 1.02, cause internally we use the 1.03/1.04 values
	if ( version != VERSION_1_02 )
		return key;

	// Char events don't need conversion
	if ( key & K_CHAR_FLAG )
		return key;

	for ( i = 0; i < mvKeyconversionCount; i++ )
	{ // Find matching key
		if ( (key == (int)mvKeyconversion[i].key16 && to15) || (key == (int)mvKeyconversion[i].key15 && !to15) )
		{ // Found a match
			return (to15 ? (int)mvKeyconversion[i].key15 : (int)mvKeyconversion[i].key16);
		}
	}

	// Prevent double entries for 1.02 (Example: if 1.02 asks for K_CTRL it will be as if it asked for A_CTRL, if 1.02 asks for something that has the same number as A_CTRL it will count as A_CTRL, too: the CTRL key is handled twice. Solution: check if key would get altered by the inverse replacement).
	if ( !invert && Key_GetProtocolKey_New( version, key, (qboolean)!to15, qtrue ) != key ) return -1;

	// Limit the maximum
	if ( (to15 && key >= K_LAST_KEY) || (!to15 && key >= MAX_KEYS) ) return -1;

	// Return the key unmodified
	return key;
}

int Key_GetProtocolKey(mvversion_t version, int key16) {
	// Converts key16 to key15 (if not on 1.02)
	return Key_GetProtocolKey_New(version, key16, qtrue, qfalse);
}

int Key_GetProtocolKey15(mvversion_t version, int key15) {
	// Converts key15 to key16 (if not on 1.02)
	return Key_GetProtocolKey_New(version, key15, qfalse, qfalse);
}
#endif // #ifndef JK2_GAME
