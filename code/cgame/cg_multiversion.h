//cg_multiversion.h
#ifndef __CG_MULTIVERSION_H_INCLUDED___
#define __CG_MULTIVERSION_H_INCLUDED___

/* Functions */
extern void MV_SetGameVersion( mvversion_t version, qboolean setGameplay );

/* Function Pointers */
extern qboolean	(*trap_Language_IsAsian)					(void);
extern qboolean	(*trap_Language_UsesSpaces)					(void);
//extern unsigned	int (*trap_AnyLanguage_ReadCharFromString)	(const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation/* = NULL*/);
extern qboolean	(*trap_G2API_SetRootSurface)				(void *ghoul2, const int modelIndex, const char *surfaceName);
extern qboolean	(*trap_G2API_SetSurfaceOnOff)				(void *ghoul2, const char *surfaceName, const int flags);
extern qboolean	(*trap_G2API_SetNewOrigin)					(void *ghoul2, const int boltIndex);
extern void		(*trap_CG_RegisterSharedMemory)				(char *memory);

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
extern void		(*trap_G2API_CollisionDetect)				(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale, int traceFlags, int useLod, float fRadius );
extern qboolean	(*trap_G2API_SetBoneAngles)					(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime);
extern void		(*trap_G2API_GetGLAName)					(void *ghoul2, int modelIndex, char *fillBuf);
extern qboolean	(*trap_G2API_SetBoneAnim)					(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime);

extern unsigned int trap_AnyLanguage_ReadCharFromString_1_04( const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation );
extern unsigned int trap_AnyLanguage_ReadCharFromString_1_02( const char **ppText );

#endif //__CG_MULTIVERSION_H_INCLUDED___
