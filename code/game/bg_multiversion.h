//bg_multiversion.h - This file contains multiversion variables and functions that are shared accross all modules...
#include "../api/mvapi.h"
#include "../game/q_shared.h"
#include "../game/anims.h"

/* Variables */
extern mvversion_t jk2version;
extern mvversion_t jk2gameplay;
extern mvversion_t jk2startversion;

#ifndef JK2_UI // Not defined for UI, but still in bg_* for game and cgame
extern animNumber_1_02_t animMappingTable_1_04_to_1_02[MAX_TOTALANIMATIONS];
extern animNumber_t animMappingTable_1_02_to_1_04[MAX_TOTALANIMATIONS_1_02];
#endif // #ifndef JK2_UI

/* Functions */
#ifndef JK2_UI // Not defined for UI, but still in bg_* for game and cgame
#ifdef MV_GENERATE_ANIMATION_MAPPING_TABLE
extern void MV_DumpAnimationMappingTable( void );
#endif // #ifdef MV_GENERATE_ANIMATION_MAPPING_TABLE
extern void MV_BuildAnimationMappingTable( void );
extern void MV_SetGamePlay( mvversion_t version );

/* Macros */
#define MV_MapAnimation104(anim) (animMappingTable_1_02_to_1_04[(anim)&~ANIM_TOGGLEBIT] | ((anim)&ANIM_TOGGLEBIT))
#define MV_MapAnimation102(anim) (animMappingTable_1_04_to_1_02[(anim)&~ANIM_TOGGLEBIT] | ((anim)&ANIM_TOGGLEBIT))


//extern int MV_VersionMagic_g2ModelParts( int limbType );
#ifndef MV_VersionMagic_g2ModelParts
#ifdef JK2_GAME
	#define MV_VersionMagic_g2ModelParts( limbType ) ( jk2startversion != VERSION_1_02 ? limbType : (limbType >= G2_MODELPART_RHAND ? limbType-1 : limbType) )
#elif JK2_CGAME
	#define MV_VersionMagic_g2ModelParts( limbType ) ( jk2startversion != VERSION_1_02 ? limbType : (limbType >= G2_MODELPART_RHAND ? limbType+1 : limbType) )
#endif // #ifdef JK2_GAME
#endif // #ifndef MV_VersionMagic_g2ModelParts

#endif // #ifndef JK2_UI

#ifndef JK2_GAME
extern int Key_GetProtocolKey(mvversion_t version, int key16);
extern int Key_GetProtocolKey15(mvversion_t version, int key15);
#endif // #ifndef JK2_GAME

