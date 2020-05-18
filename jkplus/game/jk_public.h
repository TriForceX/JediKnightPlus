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

#define JK_EMOTE_IN			( 1 << 0 )		// Emote prediction
#define JK_CHAT_PROTECT		( 1 << 1 )		// Chat protect

/*
=====================================================================
Player / world information
=====================================================================
*/

// Dimensions options
typedef enum
{
	DIMENSION_DUEL = 0,
	DIMENSION_CHAT

} jkmod_dimensions_t;

// Dimension entities
typedef struct jkmod_dimension_ents_s
{
	int entityNum;
	int contents;

} jkmod_dimension_ents_t;

#endif //__JK_PUBLIC_H__
