/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
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

// Movement stuff
#define JK_JETPACK_JKA			( 1 << 0 )
#define JK_WEAPON_STAND			( 1 << 1 )
#define JK_DISRUPTOR_WALK		( 1 << 2 )

// Jetpack stuff
#define JK_JETPACK_ACTIVE		( 1 << 28 )
#define JK_JETPACK_FLAMING		( 1 << 29 )

// Other stuff
#define JK_PASS_THROUGH			( 1 << 30 )

/*
=====================================================================
Player / world information
=====================================================================
*/

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
	DIMENSION_CHEAT		= ( 1 << 6 )

} jkmod_dimensions_t;

#endif //__JK_PUBLIC_H__
