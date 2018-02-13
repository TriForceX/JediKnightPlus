//g_multiversion.c - This file contains functions to decide which of the modified syscalls we have to call for which version, as well as other version-specific things...

/* Includes */
#include "g_local.h"
#include "../game/g_multiversion.h"
#include "../game/bg_multiversion.h"

/*
TODO:
	- 1.02 tournament-behaviour (is it actually different?)
*/

/* Variables */
playerState_1_02_t g_ps[MAX_CLIENTS];

/* 1.02 functions */
//qboolean	trap_G2API_GetBoltMatrix_NoRecNoRot_1_02	(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale);
int			trap_G2API_InitGhoul2Model_1_02				(void **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin, qhandle_t customShader, int modelFlags, int lodBias);
int			trap_G2API_AddBolt_1_02						(void *ghoul2, int modelIndex, const char *boneName);
void		trap_G2API_SetBoltInfo_1_02					(void *ghoul2, int modelIndex, int boltInfo);
int			trap_G2API_CopyGhoul2Instance_1_02			(void *g2From, void *g2To, int modelIndex);
void		trap_G2API_CopySpecificGhoul2Model_1_02		(void *g2From, int modelFrom, void *g2To, int modelTo);
void		trap_G2API_DuplicateGhoul2Instance_1_02		(void *g2From, void **g2To);
qboolean	trap_G2API_HasGhoul2ModelOnIndex_1_02		(void *ghlInfo, int modelIndex);
qboolean	trap_G2API_RemoveGhoul2Model_1_02			(void *ghlInfo, int modelIndex);
void		trap_G2API_CleanGhoul2Models_1_02			(void **ghoul2Ptr);
//void		trap_G2API_CollisionDetect_1_02				(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, vec3_t rayStart, vec3_t rayEnd, vec3_t scale, int traceFlags, int useLod, float fRadius);
qboolean	trap_G2API_SetBoneAngles_1_02				(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime);
void		trap_G2API_GetGLAName_1_02					(void *ghoul2, int modelIndex, char *fillBuf);
qboolean	trap_G2API_SetBoneAnim_1_02					(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime);

/* 1.03 & 1.04 functions */
qboolean	trap_G2API_GetBoltMatrix_NoRecNoRot_1_04	(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale);
int			trap_G2API_InitGhoul2Model_1_04				(void **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin, qhandle_t customShader, int modelFlags, int lodBias);
int			trap_G2API_AddBolt_1_04						(void *ghoul2, int modelIndex, const char *boneName);
void		trap_G2API_SetBoltInfo_1_04					(void *ghoul2, int modelIndex, int boltInfo);
int			trap_G2API_CopyGhoul2Instance_1_04			(void *g2From, void *g2To, int modelIndex);
void		trap_G2API_CopySpecificGhoul2Model_1_04		(void *g2From, int modelFrom, void *g2To, int modelTo);
void		trap_G2API_DuplicateGhoul2Instance_1_04		(void *g2From, void **g2To);
qboolean	trap_G2API_HasGhoul2ModelOnIndex_1_04		(void *ghlInfo, int modelIndex);
qboolean	trap_G2API_RemoveGhoul2Model_1_04			(void *ghlInfo, int modelIndex);
void		trap_G2API_CleanGhoul2Models_1_04			(void **ghoul2Ptr);
void		trap_G2API_CollisionDetect_1_04				(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, vec3_t rayStart, vec3_t rayEnd, vec3_t scale, int traceFlags, int useLod, float fRadius);
qboolean	trap_G2API_SetBoneAngles_1_04				(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime);
void		trap_G2API_GetGLAName_1_04					(void *ghoul2, int modelIndex, char *fillBuf);
qboolean	trap_G2API_SetBoneAnim_1_04					(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime);

/* Function Pointers */
qboolean	(*trap_G2API_GetBoltMatrix_NoRecNoRot)		(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale);
int			(*trap_G2API_InitGhoul2Model)				(void **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin, qhandle_t customShader, int modelFlags, int lodBias);
int			(*trap_G2API_AddBolt)						(void *ghoul2, int modelIndex, const char *boneName);
void		(*trap_G2API_SetBoltInfo)					(void *ghoul2, int modelIndex, int boltInfo);
int			(*trap_G2API_CopyGhoul2Instance)			(void *g2From, void *g2To, int modelIndex);
void		(*trap_G2API_CopySpecificGhoul2Model)		(void *g2From, int modelFrom, void *g2To, int modelTo);
void		(*trap_G2API_DuplicateGhoul2Instance)		(void *g2From, void **g2To);
qboolean	(*trap_G2API_HasGhoul2ModelOnIndex)			(void *ghlInfo, int modelIndex);
qboolean	(*trap_G2API_RemoveGhoul2Model)				(void *ghlInfo, int modelIndex);
void		(*trap_G2API_CleanGhoul2Models)				(void **ghoul2Ptr);
void		(*trap_G2API_CollisionDetect)				(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, vec3_t rayStart, vec3_t rayEnd, vec3_t scale, int traceFlags, int useLod, float fRadius);
qboolean	(*trap_G2API_SetBoneAngles)					(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime);
void		(*trap_G2API_GetGLAName)					(void *ghoul2, int modelIndex, char *fillBuf);
qboolean	(*trap_G2API_SetBoneAnim)					(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime);

/* JK2MultiVersionFunctions: these functions decide which syscalls we have to call for which version... */
qboolean trap_G2API_GetBoltMatrix_NoRecNoRot_DoesNotExistIn_1_02(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale)
{
	G_Printf("MVSDK: trap_G2API_GetBoltMatrix_NoRecNoRot called in 1.02 mode\n");
	return qfalse;
}

void trap_G2API_CollisionDetect_DoesNotExistIn_1_02(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, vec3_t rayStart, vec3_t rayEnd, vec3_t scale, int traceFlags, int useLod, float fRadius)
{
	G_Printf("MVSDK: trap_G2API_CollisionDetect called in 1.02 mode");
}

void MV_SetGameVersion( mvversion_t version )
{
	switch ( version )
	{
		case VERSION_1_02:
			MV_BuildAnimationMappingTable(); // Only need this in 1.02 mode!

			trap_G2API_GetBoltMatrix_NoRecNoRot		= trap_G2API_GetBoltMatrix_NoRecNoRot_DoesNotExistIn_1_02;
			trap_G2API_InitGhoul2Model				= trap_G2API_InitGhoul2Model_1_02;
			trap_G2API_AddBolt						= trap_G2API_AddBolt_1_02;
			trap_G2API_SetBoltInfo					= trap_G2API_SetBoltInfo_1_02;
			trap_G2API_CopyGhoul2Instance			= trap_G2API_CopyGhoul2Instance_1_02;
			trap_G2API_CopySpecificGhoul2Model		= trap_G2API_CopySpecificGhoul2Model_1_02;
			trap_G2API_DuplicateGhoul2Instance		= trap_G2API_DuplicateGhoul2Instance_1_02;
			trap_G2API_HasGhoul2ModelOnIndex		= trap_G2API_HasGhoul2ModelOnIndex_1_02;
			trap_G2API_RemoveGhoul2Model			= trap_G2API_RemoveGhoul2Model_1_02;
			trap_G2API_CleanGhoul2Models			= trap_G2API_CleanGhoul2Models_1_02;
			trap_G2API_CollisionDetect				= trap_G2API_CollisionDetect_DoesNotExistIn_1_02;
			trap_G2API_SetBoneAngles				= trap_G2API_SetBoneAngles_1_02;
			trap_G2API_GetGLAName					= trap_G2API_GetGLAName_1_02;
			trap_G2API_SetBoneAnim					= trap_G2API_SetBoneAnim_1_02;
			break;
		case VERSION_1_03:
		case VERSION_1_04:
		default:
			trap_G2API_GetBoltMatrix_NoRecNoRot		= trap_G2API_GetBoltMatrix_NoRecNoRot_1_04;
			trap_G2API_InitGhoul2Model				= trap_G2API_InitGhoul2Model_1_04;
			trap_G2API_AddBolt						= trap_G2API_AddBolt_1_04;
			trap_G2API_SetBoltInfo					= trap_G2API_SetBoltInfo_1_04;
			trap_G2API_CopyGhoul2Instance			= trap_G2API_CopyGhoul2Instance_1_04;
			trap_G2API_CopySpecificGhoul2Model		= trap_G2API_CopySpecificGhoul2Model_1_04;
			trap_G2API_DuplicateGhoul2Instance		= trap_G2API_DuplicateGhoul2Instance_1_04;
			trap_G2API_HasGhoul2ModelOnIndex		= trap_G2API_HasGhoul2ModelOnIndex_1_04;
			trap_G2API_RemoveGhoul2Model			= trap_G2API_RemoveGhoul2Model_1_04;
			trap_G2API_CleanGhoul2Models			= trap_G2API_CleanGhoul2Models_1_04;
			trap_G2API_CollisionDetect				= trap_G2API_CollisionDetect_1_04;
			trap_G2API_SetBoneAngles				= trap_G2API_SetBoneAngles_1_04;
			trap_G2API_GetGLAName					= trap_G2API_GetGLAName_1_04;
			trap_G2API_SetBoneAnim					= trap_G2API_SetBoneAnim_1_04;
			break;
	}

	MV_SetGamePlay( version ); // Default gameplay of that version...
}

void MV_VersionMagic( qboolean revert )
{
	if ( jk2version == VERSION_1_02 || jk2startversion == VERSION_1_02 )
	{ // Do the magic
		static const size_t section1 = (size_t)((char *)&((playerState_t*)NULL)->forceRestricted);
		static const size_t section2 = (size_t)((char *)(&((gclient_t*)NULL)->ps) + sizeof(playerState_t) - (char *)&((gclient_t*)NULL)->ps.saberIndex);
		static const size_t section3 = (size_t)((char *)&((playerState_1_02_t*)NULL)->saberIndex);
		static const size_t section4 = (size_t)((char *)(&((playerState_1_02_t*)NULL)[1]) - (char *)&((playerState_1_02_t*)NULL)->saberIndex);

		gentity_t *ent;
		gentity_t *entEnd = &g_entities[level.num_entities];
		playerState_t *ps;
		int i;

		if ( !revert )
		{ // 1.02 to 1.04
			for ( i = 0; i < level.maxclients; i++ )
			{
				ent = &g_entities[i];
				if ( !ent->inuse ) continue;

				ps = &ent->client->ps;

				if ( jk2version == VERSION_1_02 && !mvStructConversionDisabled )
				{
					/* Convert the 1.02 struct from the engine into a 1.04 struct we can handle internally */
					memcpy( ps, &g_ps[i], section1);
					// memset( &(g_clients[i].ps.forceRestricted), 0, ((size_t)&(g_clients[i].ps.saberIndex) - (size_t)&(g_clients[i].ps.forceRestricted)) ); // We don't need this, as the engine only got the 1.02 struct. By not doing this we can these internally...
					memcpy( &ps->saberIndex, &g_ps[i].saberIndex, section2);
				}

				if ( jk2startversion == VERSION_1_02 )
				{
					/* Convert the animations */
					/* NOTE: When converting from 1.02 to 1.04 we have to convert the playerState struct BEFORE the animations. When converting from 1.04 to 1.02 we have to convert the playerState struct AFTER the animations. */
					ps->legsAnim = MV_MapAnimation104( ps->legsAnim );
					ps->legsAnimExecute = MV_MapAnimation104( ps->legsAnimExecute );
					ps->torsoAnim = MV_MapAnimation104( ps->torsoAnim );
					ps->torsoAnimExecute = MV_MapAnimation104( ps->torsoAnimExecute );

					/* Only convert forceDodgeAnim if it really is an animation (forceHandExtend being either HANDEXTEND_TAUNT or HANDEXTEND_DODGE) */
					if ( ps->forceHandExtend == HANDEXTEND_TAUNT || ps->forceHandExtend == HANDEXTEND_DODGE )
					{
						ps->forceDodgeAnim = MV_MapAnimation104( ps->forceDodgeAnim );
					}

					/* The following two seem to be unused, but maybe custom cgames make use of them (well, fullAnimExecute seems to not even be set at least once - could probably just leave that one out) */
					ps->fullAnimExecute = MV_MapAnimation104( ps->fullAnimExecute );
					ps->saberAttackSequence = MV_MapAnimation104( ps->saberAttackSequence );

					/* Convert the saberblocks */
					if (ps->saberBlocked > BLOCKED_NONE) {
						ps->saberBlocked++;
					}
				}
			}

			if ( jk2startversion != VERSION_1_02 ) return;

			for ( ent = g_entities; ent < entEnd; ent++ )
			{
				if ( !ent->inuse ) continue;

				ent->s.torsoAnim = MV_MapAnimation104( ent->s.torsoAnim );
				ent->s.legsAnim = MV_MapAnimation104( ent->s.legsAnim );
			}
		}
		else	// revert
		{
			for ( i = 0; i < MAX_CLIENTS; i++ )
			{
				ent = &g_entities[i];
				if ( !ent->inuse ) continue;

				ps = &ent->client->ps;

				if ( jk2startversion == VERSION_1_02 )
				{
					/* Convert the animations */
					/* NOTE: When converting from 1.02 to 1.04 we have to convert the playerState struct BEFORE the animations. When converting from 1.04 to 1.02 we have to convert the playerState struct AFTER the animations. */
					ps->legsAnim = MV_MapAnimation102( ps->legsAnim );
					ps->legsAnimExecute = MV_MapAnimation102( ps->legsAnimExecute );
					ps->torsoAnim = MV_MapAnimation102( ps->torsoAnim );
					ps->torsoAnimExecute = MV_MapAnimation102( ps->torsoAnimExecute );

					/* Only convert forceDodgeAnim if it really is an animation (forceHandExtend being either HANDEXTEND_TAUNT or HANDEXTEND_DODGE) */
					if ( ps->forceHandExtend == HANDEXTEND_TAUNT || ps->forceHandExtend == HANDEXTEND_DODGE )
					{
						ps->forceDodgeAnim = MV_MapAnimation102( ps->forceDodgeAnim );
					}

					/* The following two seem to be unused, but maybe custom cgames make use of them (well, fullAnimExecute seems to not even be set at least once - could probably just leave that one out) */
					ps->fullAnimExecute = MV_MapAnimation104( ps->fullAnimExecute );
					ps->saberAttackSequence = MV_MapAnimation104( ps->saberAttackSequence );

					/* Convert the saberblocks */
					if (ps->saberBlocked > BLOCKED_NONE) {
						ps->saberBlocked--;
					}
				}

				if ( jk2version == VERSION_1_02 && !mvStructConversionDisabled )
				{ // 1.04 to 1.02
					/* Convert the 1.04 struct into a 1.02 struct so the engine can handle it */
					memcpy( &g_ps[i], ps, section3);
					memcpy( &g_ps[i].saberIndex, &ps->saberIndex, section4);
				}
			}

			if ( jk2startversion != VERSION_1_02 ) return;

			/* Things that must be converted for all entities (like the torsoAnim and legsAnim, g2animent have them as well) */
			for ( ent = g_entities; ent < entEnd; ent++ )
			{
				if ( !ent->inuse ) continue;

				ent->s.torsoAnim = MV_MapAnimation102( ent->s.torsoAnim );
				ent->s.legsAnim = MV_MapAnimation102( ent->s.legsAnim );
			}
		}
	}
	else
	{ // No magic required for 1.03 and 1.04...
		return;
	}
}
