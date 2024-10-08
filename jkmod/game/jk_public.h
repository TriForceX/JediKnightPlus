/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Public header
=====================================================================
*/

#ifndef __JK_PUBLIC_H__
#define __JK_PUBLIC_H__

/*
=====================================================================
Global definitions
=====================================================================
*/

// Player stuff
#define JK_EMOTE_IN				( 1 << 0 )
#define JK_CHAT_IN				( 1 << 1 )
#define JK_ANTI_STUCK			( 1 << 2 )

// Tweaks stuff
#define JK_JETPACK_JKA			( 1 << 0 )
#define JK_WEAPON_STAND			( 1 << 1 )
#define JK_DISRUPTOR_WALK		( 1 << 2 )
#define JK_USE_STAND			( 1 << 3 )
#define JK_SPECTATOR_NOCLIP		( 1 << 4 )
#define JK_RESIST_PUSH			( 1 << 5 )
#define JK_DUAL_MOVES			( 1 << 6 )
#define JK_GHOST_SABERS			( 1 << 7 )
#define JK_CHAIR_FORCE			( 1 << 8 )
#define JK_TELEPORT_MOVING		( 1 << 9 )
#define JK_FORCECHANGE_MOVING	( 1 << 10 )

// Bot actions
#define JK_BOT_STATIC			( 1 << 0 )
#define JK_BOT_ATTACK			( 1 << 1 )
#define JK_BOT_ATTACK_ALT		( 1 << 2 )
#define JK_BOT_CROUCH			( 1 << 3 )
#define JK_BOT_JUMP				( 1 << 4 )
#define JK_BOT_TAUNT			( 1 << 5 )
#define JK_BOT_TALK				( 1 << 6 )
#define JK_BOT_GOD				( 1 << 7 )
#define JK_BOT_CONTROL			( 1 << 8 )

// Map fixes stuff
#define JK_MAP_SPVOTE			( 1 << 0 )
#define JK_MAP_FXDAMAGE			( 1 << 1 )
#define JK_MAP_FXANGLES			( 1 << 2 )
#define JK_MAP_TURRET			( 1 << 3 )
#define JK_MAP_SPEAKERSOUND		( 1 << 4 )
#define JK_MAP_SPEAKERFLAGS		( 1 << 5 )
#define JK_MAP_SPMUSIC			( 1 << 6 )
#define JK_MAP_LOOPINGSOUND		( 1 << 7 )
#define JK_MAP_FUNCSTATIC		( 1 << 8 )
#define JK_MAP_BRUSHMODEL		( 1 << 9 )
#define JK_MAP_FUNCUSABLE		( 1 << 10 )
#define JK_MAP_SPAWNPRINT		( 1 << 11 )
#define JK_MAP_SPELEVATORS		( 1 << 12 )
#define JK_MAP_SPDOORUSE		( 1 << 13 )
#define JK_MAP_SPDOORTRIGGER	( 1 << 14 )
#define JK_MAP_SPECDOORSTUCK	( 1 << 15 )

// Entity flags
#define JK_BODY_PUSH			( 1 << 18 )
#define JK_JETPACK_ACTIVE		( 1 << 27 )
#define JK_JETPACK_FLAMING		( 1 << 28 )
#define JK_PASS_THROUGH			( 1 << 29 )

// Config strings
#define JK_CS_PAUSE				( 31 )
#define JK_CS_TELEPORTS			( CS_MAX_BASE + 1 )
#define JK_CS_GAME_TYPE			( CS_MAX_BASE + 2 )

// Utilities
#define VAL(x) #x
#define STR(x) VAL(x)
#define CVAR_NONE				(0x00000000u)
#define CONTENTS_NONE			(0x00000000u)
#define VALIDSTRING(a)			((a != NULL) && (a[0] != '\0'))
#define VALIDCVAR(a)			((a != NULL) && (a[0] != '\0') && (*a != '0'))
#define VALIDTEXT(a)			((a != NULL) && (a[0] != '\0') && (a[0] != ' '))
#define VALIDDIGIT(a)			((a >= '0' && a <= '9'))
#define VALIDAMOUNT(a)			((abs(a) > 999 ? 999 : abs(a)))
#define INFINITE_VALUE			0x7fff
#define ARRAY_CLEAR(a)			memset(a, 0, sizeof(a))
#define VERSION_FLOAT(x,y,z)	atof(va("%d.%02d%02d", x, y, z))

/*
=====================================================================
Player / world information
=====================================================================
*/

// Get normalised vector for the axis requested
void JKMod_BG_GiveMeVectorFromMatrix(mdxaBone_t *boltMatrix, int flags, vec3_t vec);

// Check dual saber anims
qboolean JKMod_BG_SpinningDualAnim(int anim);

// Dimension options
typedef enum
{
	DIMENSION_ALL		= -1,

	/* List */
	DIMENSION_FREE		= ( 1 << 0 ),
	DIMENSION_DUEL		= ( 1 << 1 ),
	DIMENSION_GUNS		= ( 1 << 2 ),
	DIMENSION_RACE		= ( 1 << 3 ),
	DIMENSION_SABER		= ( 1 << 4 ),
	DIMENSION_INSTA		= ( 1 << 5 ),
	DIMENSION_CHEAT		= ( 1 << 6 ),
	DIMENSION_FORCE		= ( 1 << 7 ),
	DIMENSION_PRIVATE	= ( 1 << 8 ),

	/* Total */
	DIMENSION_MAX		= 9

} jkmod_dimensions_t;

// Generic map objects
typedef enum
{
	/* List */
	GENERIC_PUSHABLE	= ( 1 << 0 ),
	GENERIC_USABLE		= ( 1 << 1 ),
	GENERIC_CONVERTER	= ( 1 << 2 ),
	GENERIC_SERVERSIDE	= ( 1 << 3 ),

	/* Total */
	GENERIC_MAX = 4

} jkmod_objects_t;

// Letter case style
typedef enum 
{
	/* List */
	CASE_NORMAL,
	CASE_UPPER,
	CASE_LOWER,

	/* Total */
	CASE_MAX

} jkmod_letterCase_t;

#endif //__JK_PUBLIC_H__
