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

// Dimension stuff
#define JK_DUEL_IN				( 1 << 0 )
#define JK_CHAT_IN				( 1 << 1 )
#define JK_GUNS_IN				( 1 << 2 )
#define JK_RACE_IN				( 1 << 3 )

// Jetpack stuff
#define JK_JETPACK				( 1 << 0 )
#define JK_JETPACK_THRUST		( 1 << 1 )
#define JK_JETPACK_GROUND		( 1 << 2 )

// Grapple hook stuff
#define JK_HOOK_FIRED			( 1 << 0 )
#define JK_HOOK_ANCHORED		( 1 << 1 )

/*
=====================================================================
Player / world information
=====================================================================
*/

// Dimension options
typedef enum
{
	DIMENSION_DUEL = 0,
	DIMENSION_CHAT,
	DIMENSION_GUNS,
	DIMENSION_RACE

} jkmod_dimensions_t;

#endif //__JK_PUBLIC_H__
