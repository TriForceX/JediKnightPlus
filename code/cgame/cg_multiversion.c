//cg_multiversion.c - This file contains functions to decide which of the modified syscalls we have to call for which version, as well as other version-specific things...

/* Includes */
#include "cg_local.h"
#include "../cgame/cg_multiversion.h"
#include "../game/bg_multiversion.h"

/*
TODO:
	- Find any use of "trap_Language_IsAsian" and add 1.02 behaviour for 1.02 mode
	- Find any use of "trap_Language_UsesSpaces" and add 1.02 behaviour for 1.02 mode
	- Find any use of "trap_Language_ReadCharFromString" and add 1.02 behaviour for 1.02 mode
	- Compare the difference between "trap_Language_ReadCharFromString" in 1.02 and later
*/

/* Variables */
/* */

/* 1.02 functions */
//qboolean	trap_Language_IsAsian_1_02					(void);
//qboolean	trap_Language_UsesSpaces_1_02				(void);
unsigned	int trap_AnyLanguage_ReadCharFromString_1_02(const char **ppText);
qboolean	trap_G2API_SetRootSurface_1_02				(void *ghoul2, const int modelIndex, const char *surfaceName);
qboolean	trap_G2API_SetSurfaceOnOff_1_02				(void *ghoul2, const char *surfaceName, const int flags);
qboolean	trap_G2API_SetNewOrigin_1_02				(void *ghoul2, const int boltIndex);
void		trap_CG_RegisterSharedMemory_1_02			(char *memory);

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
//void		trap_G2API_CollisionDetect_1_02				(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale, int traceFlags, int useLod, float fRadius );
qboolean	trap_G2API_SetBoneAngles_1_02				(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime);
void		trap_G2API_GetGLAName_1_02					(void *ghoul2, int modelIndex, char *fillBuf);
qboolean	trap_G2API_SetBoneAnim_1_02					(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime);

/* 1.03 & 1.04 functions */
qboolean	trap_Language_IsAsian_1_04					(void);
qboolean	trap_Language_UsesSpaces_1_04				(void);
unsigned	int trap_AnyLanguage_ReadCharFromString_1_04(const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation/* = NULL*/);
qboolean	trap_G2API_SetRootSurface_1_04				(void *ghoul2, const int modelIndex, const char *surfaceName);
qboolean	trap_G2API_SetSurfaceOnOff_1_04				(void *ghoul2, const char *surfaceName, const int flags);
qboolean	trap_G2API_SetNewOrigin_1_04				(void *ghoul2, const int boltIndex);
void		trap_CG_RegisterSharedMemory_1_04			(char *memory);

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
void		trap_G2API_CollisionDetect_1_04				(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale, int traceFlags, int useLod, float fRadius );
qboolean	trap_G2API_SetBoneAngles_1_04				(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime);
void		trap_G2API_GetGLAName_1_04					(void *ghoul2, int modelIndex, char *fillBuf);
qboolean	trap_G2API_SetBoneAnim_1_04					(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime);

/* Functions pointers */
qboolean	(*trap_Language_IsAsian)					(void);
qboolean	(*trap_Language_UsesSpaces)					(void);
//unsigned	int (*trap_AnyLanguage_ReadCharFromString)	(const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation/* = NULL*/);
qboolean	(*trap_G2API_SetRootSurface)				(void *ghoul2, const int modelIndex, const char *surfaceName);
qboolean	(*trap_G2API_SetSurfaceOnOff)				(void *ghoul2, const char *surfaceName, const int flags);
qboolean	(*trap_G2API_SetNewOrigin)					(void *ghoul2, const int boltIndex);
void		(*trap_CG_RegisterSharedMemory)				(char *memory);

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
void		(*trap_G2API_CollisionDetect)				(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale, int traceFlags, int useLod, float fRadius );
qboolean	(*trap_G2API_SetBoneAngles)					(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime);
void		(*trap_G2API_GetGLAName)					(void *ghoul2, int modelIndex, char *fillBuf);
qboolean	(*trap_G2API_SetBoneAnim)					(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime);

/* Functions that do not exist in 1.02 get a dummy here */
qboolean trap_Language_IsAsian_DoesNotExistIn_1_02(void)
{
	CG_Printf("MVSDK: trap_Language_IsAsian called in 1.02 mode\n");
	return qfalse;
}

qboolean trap_Language_UsesSpaces_DoesNotExistIn_1_02(void)
{
	CG_Printf("MVSDK: trap_Language_UsesSpaces called in 1.02 mode\n");
	return qfalse;
}

qboolean trap_G2API_GetBoltMatrix_NoRecNoRot_DoesNotExistIn_1_02(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale)
{
	CG_Printf("MVSDK: trap_G2API_GetBoltMatrix_NoRecNoRot called in 1.02 mode\n");
	return qfalse;
}

void trap_G2API_CollisionDetect_DoesNotExistIn_1_02(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale, int traceFlags, int useLod, float fRadius)
{
	CG_Printf("MVSDK: trap_G2API_CollisionDetect called in 1.02 mode");
}

void MV_SetGameVersion( mvversion_t version, qboolean setGameplay )
{
	switch ( version )
	{
		case VERSION_1_02:
			MV_BuildAnimationMappingTable(); // Only need this in 1.02 mode!

			/* The first two don't exist in 1.02 */
			trap_Language_IsAsian					= trap_Language_IsAsian_DoesNotExistIn_1_02;
			trap_Language_UsesSpaces				= trap_Language_UsesSpaces_DoesNotExistIn_1_02;
			//trap_AnyLanguage_ReadCharFromString		= trap_AnyLanguage_ReadCharFromString_1_02;
			trap_G2API_SetRootSurface				= trap_G2API_SetRootSurface_1_02;
			trap_G2API_SetSurfaceOnOff				= trap_G2API_SetSurfaceOnOff_1_02;
			trap_G2API_SetNewOrigin					= trap_G2API_SetNewOrigin_1_02;
			trap_CG_RegisterSharedMemory			= trap_CG_RegisterSharedMemory_1_02;

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
			trap_Language_IsAsian					= trap_Language_IsAsian_1_04;
			trap_Language_UsesSpaces				= trap_Language_UsesSpaces_1_04;
			//trap_AnyLanguage_ReadCharFromString		= trap_AnyLanguage_ReadCharFromString_1_04;
			trap_G2API_SetRootSurface				= trap_G2API_SetRootSurface_1_04;
			trap_G2API_SetSurfaceOnOff				= trap_G2API_SetSurfaceOnOff_1_04;
			trap_G2API_SetNewOrigin					= trap_G2API_SetNewOrigin_1_04;
			trap_CG_RegisterSharedMemory			= trap_CG_RegisterSharedMemory_1_04;

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

	if ( setGameplay ) MV_SetGamePlay( version ); // Default gameplay of that version...
}
