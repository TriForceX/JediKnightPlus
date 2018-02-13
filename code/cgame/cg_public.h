// Copyright (C) 1999-2000 Id Software, Inc.
//


#define	CMD_BACKUP			64	
#define	CMD_MASK			(CMD_BACKUP - 1)
// allow a lot of command backups for very fast systems
// multiple commands may be combined into a single packet, so this
// needs to be larger than PACKET_BACKUP


#define	MAX_ENTITIES_IN_SNAPSHOT	256

// snapshots are a view of the server at a given time

// Snapshots are generated at regular time intervals by the server,
// but they may not be sent if a client's rate level is exceeded, or
// they may be dropped by the network.
typedef struct {
	int				snapFlags;			// SNAPFLAG_RATE_DELAYED, etc
	int				ping;

	int				serverTime;		// server time the message is valid for (in msec)

	byte			areamask[MAX_MAP_AREA_BYTES];		// portalarea visibility bits

	playerState_t	ps;						// complete information about the current player at this time

	int				numEntities;			// all of the entities that need to be presented
	entityState_t	entities[MAX_ENTITIES_IN_SNAPSHOT];	// at the time of this snapshot

	int				numServerCommands;		// text based server commands to execute when this
	int				serverCommandSequence;	// snapshot becomes current
} snapshot_t;

typedef struct {
	int				snapFlags;			// SNAPFLAG_RATE_DELAYED, etc
	int				ping;

	int				serverTime;		// server time the message is valid for (in msec)

	byte			areamask[MAX_MAP_AREA_BYTES];		// portalarea visibility bits

	playerState_1_02_t	ps;						// complete information about the current player at this time

	int				numEntities;			// all of the entities that need to be presented
	entityState_t	entities[MAX_ENTITIES_IN_SNAPSHOT];	// at the time of this snapshot

	int				numServerCommands;		// text based server commands to execute when this
	int				serverCommandSequence;	// snapshot becomes current
} snapshot_1_02_t;

enum {
  CGAME_EVENT_NONE,
  CGAME_EVENT_TEAMMENU,
  CGAME_EVENT_SCOREBOARD,
  CGAME_EVENT_EDITHUD
};


/*
==================================================================

functions imported from the main executable

==================================================================
*/

#define	CGAME_IMPORT_API_VERSION	5

typedef enum {
	CG_PRINT = 0,
	CG_ERROR,
	CG_MILLISECONDS,
	CG_CVAR_REGISTER,
	CG_CVAR_UPDATE,
	CG_CVAR_SET,
	CG_CVAR_VARIABLESTRINGBUFFER,
	CG_ARGC,
	CG_ARGV,
	CG_ARGS,
	CG_FS_FOPENFILE,
	CG_FS_READ,
	CG_FS_WRITE,
	CG_FS_FCLOSEFILE,
	CG_SENDCONSOLECOMMAND,
	CG_ADDCOMMAND,
	CG_REMOVECOMMAND,
	CG_SENDCLIENTCOMMAND,
	CG_UPDATESCREEN,
	CG_CM_LOADMAP,
	CG_CM_NUMINLINEMODELS,
	CG_CM_INLINEMODEL,
	CG_CM_TEMPBOXMODEL,
	CG_CM_TEMPCAPSULEMODEL,
	CG_CM_POINTCONTENTS,
	CG_CM_TRANSFORMEDPOINTCONTENTS,
	CG_CM_BOXTRACE,
	CG_CM_CAPSULETRACE,
	CG_CM_TRANSFORMEDBOXTRACE,
	CG_CM_TRANSFORMEDCAPSULETRACE,
	CG_CM_MARKFRAGMENTS,
	CG_S_MUTESOUND,
	CG_S_STARTSOUND,
	CG_S_STARTLOCALSOUND,
	CG_S_CLEARLOOPINGSOUNDS,
	CG_S_ADDLOOPINGSOUND,
	CG_S_UPDATEENTITYPOSITION,
	CG_S_ADDREALLOOPINGSOUND,
	CG_S_STOPLOOPINGSOUND,
	CG_S_RESPATIALIZE,
	CG_S_REGISTERSOUND,
	CG_S_STARTBACKGROUNDTRACK,
	CG_R_LOADWORLDMAP,
	CG_R_REGISTERMODEL,
	CG_R_REGISTERSKIN,
	CG_R_REGISTERSHADER,
	CG_R_REGISTERSHADERNOMIP,
	CG_R_REGISTERFONT,
	CG_R_FONT_STRLENPIXELS,
	CG_R_FONT_STRLENCHARS,
	CG_R_FONT_STRHEIGHTPIXELS,
	CG_R_FONT_DRAWSTRING,
	CG_LANGUAGE_ISASIAN,
	CG_LANGUAGE_USESSPACES,
	CG_ANYLANGUAGE_READCHARFROMSTRING,

	CGAME_MEMSET = 100,
	CGAME_MEMCPY,
	CGAME_STRNCPY,
	CGAME_SIN,
	CGAME_COS,
	CGAME_ATAN2,
	CGAME_SQRT,
	CGAME_MATRIXMULTIPLY,
	CGAME_ANGLEVECTORS,
	CGAME_PERPENDICULARVECTOR,
	CGAME_FLOOR,
	CGAME_CEIL,
	CGAME_ACOS,
	CGAME_ASIN,

	CG_R_CLEARSCENE = 200,
	CG_R_ADDREFENTITYTOSCENE,
	CG_R_ADDPOLYTOSCENE,
	CG_R_ADDPOLYSTOSCENE,
	CG_R_LIGHTFORPOINT,
	CG_R_ADDLIGHTTOSCENE,
	CG_R_ADDADDITIVELIGHTTOSCENE,
	CG_R_RENDERSCENE,
	CG_R_SETCOLOR,
	CG_R_DRAWSTRETCHPIC,
	CG_R_MODELBOUNDS,
	CG_R_LERPTAG,
	CG_R_DRAWROTATEPIC,
	CG_R_DRAWROTATEPIC2,
	CG_R_REMAP_SHADER,
	CG_R_GET_LIGHT_STYLE,
	CG_R_SET_LIGHT_STYLE,
	CG_R_GET_BMODEL_VERTS,
	CG_FX_ADDLINE,

	CG_GETGLCONFIG,
	CG_GETGAMESTATE,
	CG_GETCURRENTSNAPSHOTNUMBER,
	CG_GETSNAPSHOT,
	CG_GETSERVERCOMMAND,
	CG_GETCURRENTCMDNUMBER,
	CG_GETUSERCMD,
	CG_SETUSERCMDVALUE,
	CG_SETCLIENTFORCEANGLE,
	CG_SETCLIENTTURNEXTENT,
	CG_OPENUIMENU,
	CG_TESTPRINTINT,
	CG_TESTPRINTFLOAT,
	CG_MEMORY_REMAINING,
	CG_KEY_ISDOWN,
	CG_KEY_GETCATCHER,
	CG_KEY_SETCATCHER,
	CG_KEY_GETKEY,

 	CG_PC_ADD_GLOBAL_DEFINE,
	CG_PC_LOAD_SOURCE,
	CG_PC_FREE_SOURCE,
	CG_PC_READ_TOKEN,
	CG_PC_SOURCE_FILE_AND_LINE,
	CG_PC_LOAD_GLOBAL_DEFINES,
	CG_PC_REMOVE_ALL_GLOBAL_DEFINES,

	CG_S_STOPBACKGROUNDTRACK,
	CG_REAL_TIME,
	CG_SNAPVECTOR,
	CG_CIN_PLAYCINEMATIC,
	CG_CIN_STOPCINEMATIC,
	CG_CIN_RUNCINEMATIC,
	CG_CIN_DRAWCINEMATIC,
	CG_CIN_SETEXTENTS,

	CG_GET_ENTITY_TOKEN,
	CG_R_INPVS,
	CG_FX_REGISTER_EFFECT,
	CG_FX_PLAY_SIMPLE_EFFECT,
	CG_FX_PLAY_EFFECT,
	CG_FX_PLAY_ENTITY_EFFECT,
	CG_FX_PLAY_SIMPLE_EFFECT_ID,
	CG_FX_PLAY_EFFECT_ID,
	CG_FX_PLAY_ENTITY_EFFECT_ID,
	CG_FX_PLAY_BOLTED_EFFECT_ID,
	CG_FX_ADD_SCHEDULED_EFFECTS,
	CG_FX_INIT_SYSTEM,
	CG_FX_FREE_SYSTEM,
	CG_FX_ADJUST_TIME,
	CG_FX_ADDPOLY,
	CG_FX_ADDBEZIER,
	CG_FX_ADDPRIMITIVE,
	CG_FX_ADDSPRITE,

	CG_SP_PRINT,
	CG_SP_GETSTRINGTEXTSTRING,
	CG_SP_REGISTER,

	CG_ROFF_CLEAN,
	CG_ROFF_UPDATE_ENTITIES,
	CG_ROFF_CACHE,
	CG_ROFF_PLAY,
	CG_ROFF_PURGE_ENT,

/*
Ghoul2 Insert Start
*/
	CG_G2_LISTSURFACES,
	CG_G2_LISTBONES,
	CG_G2_SETMODELS,
	CG_G2_HAVEWEGHOULMODELS,
	CG_G2_GIVEMEVECTORFROMMATRIX,
	CG_G2_GETBOLT,
	CG_G2_GETBOLT_NOREC,
	CG_G2_GETBOLT_NOREC_NOROT,
	CG_G2_INITGHOUL2MODEL,
	CG_G2_COLLISIONDETECT,
	CG_G2_CLEANMODELS,
	CG_G2_ANGLEOVERRIDE,
	CG_G2_PLAYANIM,
	CG_G2_GETGLANAME,
	CG_G2_COPYGHOUL2INSTANCE,
	CG_G2_COPYSPECIFICGHOUL2MODEL,
	CG_G2_DUPLICATEGHOUL2INSTANCE,
	CG_G2_HASGHOUL2MODELONINDEX,
	CG_G2_REMOVEGHOUL2MODEL,
	CG_G2_ADDBOLT,
	CG_G2_SETBOLTON,
	CG_G2_SETROOTSURFACE,
	CG_G2_SETSURFACEONOFF,
	CG_G2_SETNEWORIGIN,

	CG_SET_SHARED_BUFFER,
/*
Ghoul2 Insert End
*/
} cgameImport_t;


typedef enum {
	CG_PRINT_1_02 = 0,
	CG_ERROR_1_02,
	CG_MILLISECONDS_1_02,
	CG_CVAR_REGISTER_1_02,
	CG_CVAR_UPDATE_1_02,
	CG_CVAR_SET_1_02,
	CG_CVAR_VARIABLESTRINGBUFFER_1_02,
	CG_ARGC_1_02,
	CG_ARGV_1_02,
	CG_ARGS_1_02,
	CG_FS_FOPENFILE_1_02,
	CG_FS_READ_1_02,
	CG_FS_WRITE_1_02,
	CG_FS_FCLOSEFILE_1_02,
	CG_SENDCONSOLECOMMAND_1_02,
	CG_ADDCOMMAND_1_02,
	CG_REMOVECOMMAND_1_02,
	CG_SENDCLIENTCOMMAND_1_02,
	CG_UPDATESCREEN_1_02,
	CG_CM_LOADMAP_1_02,
	CG_CM_NUMINLINEMODELS_1_02,
	CG_CM_INLINEMODEL_1_02,
	CG_CM_TEMPBOXMODEL_1_02,
	CG_CM_TEMPCAPSULEMODEL_1_02,
	CG_CM_POINTCONTENTS_1_02,
	CG_CM_TRANSFORMEDPOINTCONTENTS_1_02,
	CG_CM_BOXTRACE_1_02,
	CG_CM_CAPSULETRACE_1_02,
	CG_CM_TRANSFORMEDBOXTRACE_1_02,
	CG_CM_TRANSFORMEDCAPSULETRACE_1_02,
	CG_CM_MARKFRAGMENTS_1_02,
	CG_S_MUTESOUND_1_02,
	CG_S_STARTSOUND_1_02,
	CG_S_STARTLOCALSOUND_1_02,
	CG_S_CLEARLOOPINGSOUNDS_1_02,
	CG_S_ADDLOOPINGSOUND_1_02,
	CG_S_UPDATEENTITYPOSITION_1_02,
	CG_S_ADDREALLOOPINGSOUND_1_02,
	CG_S_STOPLOOPINGSOUND_1_02,
	CG_S_RESPATIALIZE_1_02,
	CG_S_REGISTERSOUND_1_02,
	CG_S_STARTBACKGROUNDTRACK_1_02,
	CG_R_LOADWORLDMAP_1_02,
	CG_R_REGISTERMODEL_1_02,
	CG_R_REGISTERSKIN_1_02,
	CG_R_REGISTERSHADER_1_02,
	CG_R_REGISTERSHADERNOMIP_1_02,
	CG_R_REGISTERFONT_1_02,
	CG_R_FONT_STRLENPIXELS_1_02,
	CG_R_FONT_STRLENCHARS_1_02,
	CG_R_FONT_STRHEIGHTPIXELS_1_02,
	CG_R_FONT_DRAWSTRING_1_02,
	CG_ANYLANGUAGE_READCHARFROMSTRING_1_02,

	CGAME_MEMSET_1_02 = 100,
	CGAME_MEMCPY_1_02,
	CGAME_STRNCPY_1_02,
	CGAME_SIN_1_02,
	CGAME_COS_1_02,
	CGAME_ATAN2_1_02,
	CGAME_SQRT_1_02,
	CGAME_MATRIXMULTIPLY_1_02,
	CGAME_ANGLEVECTORS_1_02,
	CGAME_PERPENDICULARVECTOR_1_02,
	CGAME_FLOOR_1_02,
	CGAME_CEIL_1_02,
	CGAME_ACOS_1_02,
	CGAME_ASIN_1_02,

	CG_R_CLEARSCENE_1_02 = 200,
	CG_R_ADDREFENTITYTOSCENE_1_02,
	CG_R_ADDPOLYTOSCENE_1_02,
	CG_R_ADDPOLYSTOSCENE_1_02,
	CG_R_LIGHTFORPOINT_1_02,
	CG_R_ADDLIGHTTOSCENE_1_02,
	CG_R_ADDADDITIVELIGHTTOSCENE_1_02,
	CG_R_RENDERSCENE_1_02,
	CG_R_SETCOLOR_1_02,
	CG_R_DRAWSTRETCHPIC_1_02,
	CG_R_MODELBOUNDS_1_02,
	CG_R_LERPTAG_1_02,
	CG_R_DRAWROTATEPIC_1_02,
	CG_R_DRAWROTATEPIC2_1_02,
	CG_R_REMAP_SHADER_1_02,
	CG_R_GET_LIGHT_STYLE_1_02,
	CG_R_SET_LIGHT_STYLE_1_02,
	CG_R_GET_BMODEL_VERTS_1_02,
	CG_FX_ADDLINE_1_02,

	CG_GETGLCONFIG_1_02,
	CG_GETGAMESTATE_1_02,
	CG_GETCURRENTSNAPSHOTNUMBER_1_02,
	CG_GETSNAPSHOT_1_02,
	CG_GETSERVERCOMMAND_1_02,
	CG_GETCURRENTCMDNUMBER_1_02,
	CG_GETUSERCMD_1_02,
	CG_SETUSERCMDVALUE_1_02,
	CG_SETCLIENTFORCEANGLE_1_02,
	CG_SETCLIENTTURNEXTENT_1_02,
	CG_OPENUIMENU_1_02,
	CG_TESTPRINTINT_1_02,
	CG_TESTPRINTFLOAT_1_02,
	CG_MEMORY_REMAINING_1_02,
	CG_KEY_ISDOWN_1_02,
	CG_KEY_GETCATCHER_1_02,
	CG_KEY_SETCATCHER_1_02,
	CG_KEY_GETKEY_1_02,

 	CG_PC_ADD_GLOBAL_DEFINE_1_02,
	CG_PC_LOAD_SOURCE_1_02,
	CG_PC_FREE_SOURCE_1_02,
	CG_PC_READ_TOKEN_1_02,
	CG_PC_SOURCE_FILE_AND_LINE_1_02,
	CG_PC_LOAD_GLOBAL_DEFINES_1_02,
	CG_PC_REMOVE_ALL_GLOBAL_DEFINES_1_02,

	CG_S_STOPBACKGROUNDTRACK_1_02,
	CG_REAL_TIME_1_02,
	CG_SNAPVECTOR_1_02,
	CG_CIN_PLAYCINEMATIC_1_02,
	CG_CIN_STOPCINEMATIC_1_02,
	CG_CIN_RUNCINEMATIC_1_02,
	CG_CIN_DRAWCINEMATIC_1_02,
	CG_CIN_SETEXTENTS_1_02,

	CG_GET_ENTITY_TOKEN_1_02,
	CG_R_INPVS_1_02,
	CG_FX_REGISTER_EFFECT_1_02,
	CG_FX_PLAY_SIMPLE_EFFECT_1_02,
	CG_FX_PLAY_EFFECT_1_02,
	CG_FX_PLAY_ENTITY_EFFECT_1_02,
	CG_FX_PLAY_SIMPLE_EFFECT_ID_1_02,
	CG_FX_PLAY_EFFECT_ID_1_02,
	CG_FX_PLAY_ENTITY_EFFECT_ID_1_02,
	CG_FX_PLAY_BOLTED_EFFECT_ID_1_02,
	CG_FX_ADD_SCHEDULED_EFFECTS_1_02,
	CG_FX_INIT_SYSTEM_1_02,
	CG_FX_FREE_SYSTEM_1_02,
	CG_FX_ADJUST_TIME_1_02,
	CG_FX_ADDPOLY_1_02,
	CG_FX_ADDBEZIER_1_02,
	CG_FX_ADDPRIMITIVE_1_02,
	CG_FX_ADDSPRITE_1_02,

	CG_SP_PRINT_1_02,
	CG_SP_GETSTRINGTEXTSTRING_1_02,
	CG_SP_REGISTER_1_02,

	CG_ROFF_CLEAN_1_02,
	CG_ROFF_UPDATE_ENTITIES_1_02,
	CG_ROFF_CACHE_1_02,
	CG_ROFF_PLAY_1_02,
	CG_ROFF_PURGE_ENT_1_02,

/*
Ghoul2 Insert Start
*/
	CG_G2_LISTSURFACES_1_02,
	CG_G2_LISTBONES_1_02,
	CG_G2_SETMODELS_1_02,
	CG_G2_HAVEWEGHOULMODELS_1_02,
	CG_G2_GIVEMEVECTORFROMMATRIX_1_02,
	CG_G2_GETBOLT_1_02,
	CG_G2_GETBOLT_NOREC_1_02,
	CG_G2_INITGHOUL2MODEL_1_02,
	CG_G2_CLEANMODELS_1_02,
	CG_G2_ANGLEOVERRIDE_1_02,
	CG_G2_PLAYANIM_1_02,
	CG_G2_GETGLANAME_1_02,
	CG_G2_COPYGHOUL2INSTANCE_1_02,
	CG_G2_COPYSPECIFICGHOUL2MODEL_1_02,
	CG_G2_DUPLICATEGHOUL2INSTANCE_1_02,
	CG_G2_HASGHOUL2MODELONINDEX_1_02,
	CG_G2_REMOVEGHOUL2MODEL_1_02,
	CG_G2_ADDBOLT_1_02,
	CG_G2_SETBOLTON_1_02,
	CG_G2_SETROOTSURFACE_1_02,
	CG_G2_SETSURFACEONOFF_1_02,
	CG_G2_SETNEWORIGIN_1_02,

	CG_SET_SHARED_BUFFER_1_02,
} cgameImport_1_02_t;


/*
==================================================================

functions exported to the main executable

==================================================================
*/

typedef enum {
	CG_INIT,
//	void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum )
	// called when the level loads or when the renderer is restarted
	// all media should be registered at this time
	// cgame will display loading status by calling SCR_Update, which
	// will call CG_DrawInformation during the loading process
	// reliableCommandSequence will be 0 on fresh loads, but higher for
	// demos, tourney restarts, or vid_restarts

	CG_SHUTDOWN,
//	void (*CG_Shutdown)( void );
	// oportunity to flush and close any open files

	CG_CONSOLE_COMMAND,
//	qboolean (*CG_ConsoleCommand)( void );
	// a console command has been issued locally that is not recognized by the
	// main game system.
	// use Cmd_Argc() / Cmd_Argv() to read the command, return qfalse if the
	// command is not known to the game

	CG_DRAW_ACTIVE_FRAME,
//	void (*CG_DrawActiveFrame)( int serverTime, stereoFrame_t stereoView, qboolean demoPlayback );
	// Generates and draws a game scene and status information at the given time.
	// If demoPlayback is set, local movement prediction will not be enabled

	CG_CROSSHAIR_PLAYER,
//	int (*CG_CrosshairPlayer)( void );

	CG_LAST_ATTACKER,
//	int (*CG_LastAttacker)( void );

	CG_KEY_EVENT, 
//	void	(*CG_KeyEvent)( int key, qboolean down );

	CG_MOUSE_EVENT,
//	void	(*CG_MouseEvent)( int dx, int dy );
	CG_EVENT_HANDLING,
//	void (*CG_EventHandling)(int type);

	CG_POINT_CONTENTS,
//	int	CG_PointContents( const vec3_t point, int passEntityNum );

	CG_GET_LERP_ORIGIN,
//	void CG_LerpOrigin(int num, vec3_t result);

	CG_GET_LERP_ANGLES,
	CG_GET_MODEL_SCALE,
	CG_GET_GHOUL2,
	CG_GET_MODEL_LIST,

	CG_CALC_LERP_POSITIONS,
//	void CG_CalcEntityLerpPositions(int num);

	CG_TRACE,
//void CG_Trace( trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, 
//					 int skipNumber, int mask );

	CG_GET_ORIGIN,		// int entnum, vec3_t origin
	CG_GET_ANGLES,		// int entnum, vec3_t angle

	CG_GET_BOLT_POS,	// int entnum, vec3_t origin, vec3_t angle

	CG_GET_ORIGIN_TRAJECTORY,		// int entnum
	CG_GET_ANGLE_TRAJECTORY,		// int entnum

	CG_ROFF_NOTETRACK_CALLBACK,		// int entnum, char *notetrack

	CG_IMPACT_MARK,
//void CG_ImpactMark( qhandle_t markShader, const vec3_t origin, const vec3_t dir, 
//				   float orientation, float red, float green, float blue, float alpha,
//				   qboolean alphaFade, float radius, qboolean temporary )

	CG_MAP_CHANGE,

} cgameExport_t;

// CG_POINT_CONTENTS
typedef struct
{
	vec3_t		mPoint;			// input
	int			mPassEntityNum;	// input
} TCGPointContents;

// CG_GET_BOLT_POS
typedef struct
{
	vec3_t		mPoint;			// output
	vec3_t		mAngle;			// output
	int			mEntityNum;		// input
} TCGBoltPos;

// CG_IMPACT_MARK
typedef struct
{
	int		mHandle;
	vec3_t	mPoint;
	vec3_t	mAngle;
	float	mRotation;
	float	mRed;
	float	mGreen;
	float	mBlue;
	float	mAlphaStart;
	float	mSizeStart;
} TCGImpactMark;

// CG_GET_LERP_ORIGIN
// CG_GET_LERP_ANGLES
// CG_GET_MODEL_SCALE
typedef struct
{
	int			mEntityNum;		// input
	vec3_t		mPoint;			// output
} TCGVectorData;

// CG_TRACE
typedef struct
{
	trace_t mResult;					// output
	vec3_t	mStart, mMins, mMaxs, mEnd;	// input
	int		mSkipNumber, mMask;			// input
} TCGTrace;

// CG_FX_CAMERASHAKE
typedef struct
{
	vec3_t	mOrigin;					// input
	float	mIntensity;					// input
	int		mRadius;					// input
	int		mTime;						// input
} TCGCameraShake;

// CG_MISC_ENT
typedef struct
{
	char	mModel[MAX_QPATH];			// input
	vec3_t	mOrigin, mAngles, mScale;	// input
} TCGMiscEnt;

typedef struct
{
	refEntity_t		ent;				// output
	void			*ghoul2;			// input
	int				modelIndex;			// input
	int				boltIndex;			// input
	vec3_t			origin;				// input
	vec3_t			angles;				// input
	vec3_t			modelScale;			// input
} TCGPositionOnBolt;

#define	MAX_CG_SHARED_BUFFER_SIZE		2048

//----------------------------------------------
