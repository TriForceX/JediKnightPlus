//g_multiversion.h
#ifndef __G_MULTIVERSION_H_INCLUDED___
#define __G_MULTIVERSION_H_INCLUDED___

/* Enums */
/*
typedef enum
{
	VERSION_UNDEF = 0,
	VERSION_1_02 = 2,
	VERSION_1_03 = 3,
	VERSION_1_04 = 4,
} mvversion_t;
*/
#include "../api/mvapi.h"

/* Variables */
extern playerState_1_02_t g_ps[MAX_CLIENTS];

/* Functions */
extern void MV_SetGameVersion( mvversion_t version );
extern void MV_VersionMagic( qboolean revert );

/* Function pointers */
extern qboolean	(*trap_G2API_GetBoltMatrix_NoRecNoRot)		(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale);
extern int		(*trap_G2API_InitGhoul2Model)				(void **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin, qhandle_t customShader, int modelFlags, int lodBias);
extern int		(*trap_G2API_AddBolt)						(void *ghoul2, int modelIndex, const char *boneName);
extern void		(*trap_G2API_SetBoltInfo)					(void *ghoul2, int modelIndex, int boltInfo);
extern int		(*trap_G2API_CopyGhoul2Instance)			(void *g2From, void *g2To, int modelIndex);
extern void		(*trap_G2API_CopySpecificGhoul2Model)		(void *g2From, int modelFrom, void *g2To, int modelTo);
extern void		(*trap_G2API_DuplicateGhoul2Instance)		(void *g2From, void **g2To);
extern qboolean	(*trap_G2API_HasGhoul2ModelOnIndex)			(void *ghlInfo, int modelIndex);
extern qboolean	(*trap_G2API_RemoveGhoul2Model)				(void *ghlInfo, int modelIndex);
extern void		(*trap_G2API_CleanGhoul2Models)				(void **ghoul2Ptr);
extern void		(*trap_G2API_CollisionDetect)				(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, vec3_t rayStart, vec3_t rayEnd, vec3_t scale, int traceFlags, int useLod, float fRadius);
extern qboolean	(*trap_G2API_SetBoneAngles)					(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime);
extern void		(*trap_G2API_GetGLAName)					(void *ghoul2, int modelIndex, char *fillBuf);
extern qboolean	(*trap_G2API_SetBoneAnim)					(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime);

#endif //__G_MULTIVERSION_H_INCLUDED___
