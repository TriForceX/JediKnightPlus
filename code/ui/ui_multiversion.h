//ui_multiversion.h
#ifndef __UI_MULTIVERSION_H_INCLUDED___
#define __UI_MULTIVERSION_H_INCLUDED___

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

/* Variables */
/* */

/* Functions */
extern void MV_SetGameVersion( mvversion_t version, qboolean setGameplay );
extern void MV_SetGamePlay( mvversion_t version );

/* Function pointers */
extern qboolean		(*trap_Language_IsAsian)					(void);
extern qboolean		(*trap_Language_UsesSpaces)					(void);
//extern unsigned int	(*trap_AnyLanguage_ReadCharFromString)		(const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation);

extern void			(*trap_R_ModelBounds)						(clipHandle_t model, vec3_t mins, vec3_t maxs);
extern int			(*trap_LAN_GetServerCount)					(int source);
extern void			(*trap_LAN_GetServerAddressString)			(int source, int n, char *buf, int buflen);
extern void			(*trap_LAN_GetServerInfo)					(int source, int n, char *buf, int buflen);
extern int			(*trap_LAN_GetServerPing)					(int source, int n);
extern int			(*trap_LAN_ServerStatus)					(const char *serverAddress, char *serverStatus, int maxLen);
extern void			(*trap_LAN_SaveCachedServers)				();
extern void			(*trap_LAN_LoadCachedServers)				();
extern void			(*trap_LAN_ResetPings)						(int n);
extern void			(*trap_LAN_MarkServerVisible)				(int source, int n, qboolean visible);
extern int			(*trap_LAN_ServerIsVisible)					(int source, int n);
extern qboolean		(*trap_LAN_UpdateVisiblePings)				(int source);
extern int			(*trap_LAN_AddServer)						(int source, const char *name, const char *addr);
extern void			(*trap_LAN_RemoveServer)					(int source, const char *addr);
extern int			(*trap_LAN_CompareServers)					(int source, int sortKey, int sortDir, int s1, int s2);
extern int			(*trap_PC_AddGlobalDefine)					(char *define);
extern int			(*trap_PC_LoadSource)						(const char *filename);
extern int			(*trap_PC_FreeSource)						(int handle);
extern int			(*trap_PC_ReadToken)						(int handle, pc_token_t *pc_token);
extern int			(*trap_PC_SourceFileAndLine)				(int handle, char *filename, int *line);
extern int			(*trap_PC_LoadGlobalDefines)				(const char* filename);

extern void			(*trap_PC_RemoveAllGlobalDefines)			(void);

extern void			(*trap_S_StopBackgroundTrack)				(void);
extern void			(*trap_S_StartBackgroundTrack)				(const char *intro, const char *loop, qboolean bReturnWithoutStarting);
extern int			(*trap_RealTime)							(qtime_t *qtime);
extern int			(*trap_CIN_PlayCinematic)					(const char *arg0, int xpos, int ypos, int width, int height, int bits);
extern e_status		(*trap_CIN_StopCinematic)					(int handle);
extern e_status		(*trap_CIN_RunCinematic)					(int handle);
extern void			(*trap_CIN_DrawCinematic)					(int handle);
extern void			(*trap_CIN_SetExtents)						(int handle, int x, int y, int w, int h);
extern void			(*trap_R_RemapShader)						(const char *oldShader, const char *newShader, const char *timeOffset);

extern unsigned int trap_AnyLanguage_ReadCharFromString_1_04( const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation );
extern unsigned int trap_AnyLanguage_ReadCharFromString_1_02( const char **ppText );

#endif //__UI_MULTIVERSION_H_INCLUDED___
