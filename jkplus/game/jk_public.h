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

// Dimension stuff
#define JK_DUEL_IN				1
#define JK_GUNS_IN				2
#define JK_RACE_IN				3

// Player stuff
#define JK_EMOTE_IN				( 1 << 0 )
#define JK_CHAT_IN				( 1 << 1 )

// Movement stuff
#define JK_JETPACK_JKA			( 1 << 0 )

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
	DIMENSION_DUEL = 0,
	DIMENSION_GUNS,
	DIMENSION_RACE

} jkmod_dimensions_t;

#endif //__JK_PUBLIC_H__
