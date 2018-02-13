//ui_multiversion.c - This file contains functions to decide which of the modified syscalls we have to call for which version, as well as other version-specific things...

/* Includes */
#include "ui_local.h"
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
//qboolean		trap_Language_IsAsian_1_02					(void);
//qboolean		trap_Language_UsesSpaces_1_02				(void);
unsigned int	trap_AnyLanguage_ReadCharFromString_1_02	(const char **ppText);

void			trap_R_ModelBounds_1_02						(clipHandle_t model, vec3_t mins, vec3_t maxs);
int				trap_LAN_GetServerCount_1_02				(int source);
void			trap_LAN_GetServerAddressString_1_02		(int source, int n, char *buf, int buflen);
void			trap_LAN_GetServerInfo_1_02					(int source, int n, char *buf, int buflen);
int				trap_LAN_GetServerPing_1_02					(int source, int n);
int				trap_LAN_ServerStatus_1_02					(const char *serverAddress, char *serverStatus, int maxLen);
void			trap_LAN_SaveCachedServers_1_02				();
void			trap_LAN_LoadCachedServers_1_02				();
void			trap_LAN_ResetPings_1_02					(int n);
void			trap_LAN_MarkServerVisible_1_02				(int source, int n, qboolean visible);
int				trap_LAN_ServerIsVisible_1_02				(int source, int n);
qboolean		trap_LAN_UpdateVisiblePings_1_02			(int source);
int				trap_LAN_AddServer_1_02						(int source, const char *name, const char *addr);
void			trap_LAN_RemoveServer_1_02					(int source, const char *addr);
int				trap_LAN_CompareServers_1_02				(int source, int sortKey, int sortDir, int s1, int s2);
int				trap_PC_AddGlobalDefine_1_02				(char *define);
int				trap_PC_LoadSource_1_02						(const char *filename);
int				trap_PC_FreeSource_1_02						(int handle);
int				trap_PC_ReadToken_1_02						(int handle, pc_token_t *pc_token);
int				trap_PC_SourceFileAndLine_1_02				(int handle, char *filename, int *line);
int				trap_PC_LoadGlobalDefines_1_02				(const char* filename);

void			trap_PC_RemoveAllGlobalDefines_1_02			(void);

void			trap_S_StopBackgroundTrack_1_02				(void);
void			trap_S_StartBackgroundTrack_1_02			(const char *intro, const char *loop, qboolean bReturnWithoutStarting);
int				trap_RealTime_1_02							(qtime_t *qtime);
int				trap_CIN_PlayCinematic_1_02					(const char *arg0, int xpos, int ypos, int width, int height, int bits);
e_status		trap_CIN_StopCinematic_1_02					(int handle);
e_status		trap_CIN_RunCinematic_1_02					(int handle);
void			trap_CIN_DrawCinematic_1_02					(int handle);
void			trap_CIN_SetExtents_1_02					(int handle, int x, int y, int w, int h);
void			trap_R_RemapShader_1_02						(const char *oldShader, const char *newShader, const char *timeOffset);

/* 1.03 & 1.04 functions */
qboolean		trap_Language_IsAsian_1_04					(void);
qboolean		trap_Language_UsesSpaces_1_04				(void);
unsigned int	trap_AnyLanguage_ReadCharFromString_1_04	(const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation);

void			trap_R_ModelBounds_1_04						(clipHandle_t model, vec3_t mins, vec3_t maxs);
int				trap_LAN_GetServerCount_1_04				(int source);
void			trap_LAN_GetServerAddressString_1_04		(int source, int n, char *buf, int buflen);
void			trap_LAN_GetServerInfo_1_04					(int source, int n, char *buf, int buflen);
int				trap_LAN_GetServerPing_1_04					(int source, int n);
int				trap_LAN_ServerStatus_1_04					(const char *serverAddress, char *serverStatus, int maxLen);
void			trap_LAN_SaveCachedServers_1_04				();
void			trap_LAN_LoadCachedServers_1_04				();
void			trap_LAN_ResetPings_1_04					(int n);
void			trap_LAN_MarkServerVisible_1_04				(int source, int n, qboolean visible);
int				trap_LAN_ServerIsVisible_1_04				(int source, int n);
qboolean		trap_LAN_UpdateVisiblePings_1_04			(int source);
int				trap_LAN_AddServer_1_04						(int source, const char *name, const char *addr);
void			trap_LAN_RemoveServer_1_04					(int source, const char *addr);
int				trap_LAN_CompareServers_1_04				(int source, int sortKey, int sortDir, int s1, int s2);
int				trap_PC_AddGlobalDefine_1_04				(char *define);
int				trap_PC_LoadSource_1_04						(const char *filename);
int				trap_PC_FreeSource_1_04						(int handle);
int				trap_PC_ReadToken_1_04						(int handle, pc_token_t *pc_token);
int				trap_PC_SourceFileAndLine_1_04				(int handle, char *filename, int *line);
int				trap_PC_LoadGlobalDefines_1_04				(const char* filename);

void			trap_PC_RemoveAllGlobalDefines_1_04			(void);

void			trap_S_StopBackgroundTrack_1_04				(void);
void			trap_S_StartBackgroundTrack_1_04			(const char *intro, const char *loop, qboolean bReturnWithoutStarting);
int				trap_RealTime_1_04							(qtime_t *qtime);
int				trap_CIN_PlayCinematic_1_04					(const char *arg0, int xpos, int ypos, int width, int height, int bits);
e_status		trap_CIN_StopCinematic_1_04					(int handle);
e_status		trap_CIN_RunCinematic_1_04					(int handle);
void			trap_CIN_DrawCinematic_1_04					(int handle);
void			trap_CIN_SetExtents_1_04					(int handle, int x, int y, int w, int h);
void			trap_R_RemapShader_1_04						(const char *oldShader, const char *newShader, const char *timeOffset);

/* Function pointers */
qboolean		(*trap_Language_IsAsian)					(void);
qboolean		(*trap_Language_UsesSpaces)					(void);
//unsigned int	(*trap_AnyLanguage_ReadCharFromString)		(const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation);

void			(*trap_R_ModelBounds)						(clipHandle_t model, vec3_t mins, vec3_t maxs);
int				(*trap_LAN_GetServerCount)					(int source);
void			(*trap_LAN_GetServerAddressString)			(int source, int n, char *buf, int buflen);
void			(*trap_LAN_GetServerInfo)					(int source, int n, char *buf, int buflen);
int				(*trap_LAN_GetServerPing)					(int source, int n);
int				(*trap_LAN_ServerStatus)					(const char *serverAddress, char *serverStatus, int maxLen);
void			(*trap_LAN_SaveCachedServers)				();
void			(*trap_LAN_LoadCachedServers)				();
void			(*trap_LAN_ResetPings)						(int n);
void			(*trap_LAN_MarkServerVisible)				(int source, int n, qboolean visible);
int				(*trap_LAN_ServerIsVisible)					(int source, int n);
qboolean		(*trap_LAN_UpdateVisiblePings)				(int source);
int				(*trap_LAN_AddServer)						(int source, const char *name, const char *addr);
void			(*trap_LAN_RemoveServer)					(int source, const char *addr);
int				(*trap_LAN_CompareServers)					(int source, int sortKey, int sortDir, int s1, int s2);
int				(*trap_PC_AddGlobalDefine)					(char *define);
int				(*trap_PC_LoadSource)						(const char *filename);
int				(*trap_PC_FreeSource)						(int handle);
int				(*trap_PC_ReadToken)						(int handle, pc_token_t *pc_token);
int				(*trap_PC_SourceFileAndLine)				(int handle, char *filename, int *line);
int				(*trap_PC_LoadGlobalDefines)				(const char* filename);

void			(*trap_PC_RemoveAllGlobalDefines)			(void);

void			(*trap_S_StopBackgroundTrack)				(void);
void			(*trap_S_StartBackgroundTrack)				(const char *intro, const char *loop, qboolean bReturnWithoutStarting);
int				(*trap_RealTime)							(qtime_t *qtime);
int				(*trap_CIN_PlayCinematic)					(const char *arg0, int xpos, int ypos, int width, int height, int bits);
e_status		(*trap_CIN_StopCinematic)					(int handle);
e_status		(*trap_CIN_RunCinematic)					(int handle);
void			(*trap_CIN_DrawCinematic)					(int handle);
void			(*trap_CIN_SetExtents)						(int handle, int x, int y, int w, int h);
void			(*trap_R_RemapShader)						(const char *oldShader, const char *newShader, const char *timeOffset);

/* Functions that do not exist in 1.02 get a dummy here */
qboolean trap_Language_IsAsian_DoesNotExistIn_1_02(void)
{
	Com_Printf("MVSDK: trap_Language_IsAsian called in 1.02 mode\n");
	return qfalse;
}

qboolean trap_Language_UsesSpaces_DoesNotExistIn_1_02(void)
{
	Com_Printf("MVSDK: trap_Language_UsesSpaces called in 1.02 mode\n");
	return qfalse;
}

/* Additional functions */
void MV_SetGamePlay( mvversion_t version )
{ /* A lot more simple than in CGame or Game */
	switch ( version )
	{
		case VERSION_1_02:
			jk2gameplay = VERSION_1_02;
			break;
		case VERSION_1_03:
			jk2gameplay = VERSION_1_03;
			break;
		default:
		case VERSION_1_04:
			jk2gameplay = VERSION_1_04;
			break;
	}
}

void MV_SetGameVersion( mvversion_t version )
{
	switch ( version )
	{
		case VERSION_1_02:
			/* The first two don't exist in 1.02 */
			trap_Language_IsAsian				= trap_Language_IsAsian_DoesNotExistIn_1_02;
			trap_Language_UsesSpaces			= trap_Language_UsesSpaces_DoesNotExistIn_1_02;

			/*trap_AnyLanguage_ReadCharFromString	= trap_AnyLanguage_ReadCharFromString_1_02;*/

			trap_R_ModelBounds					= trap_R_ModelBounds_1_02;
			trap_LAN_GetServerCount				= trap_LAN_GetServerCount_1_02;
			trap_LAN_GetServerAddressString		= trap_LAN_GetServerAddressString_1_02;
			trap_LAN_GetServerInfo				= trap_LAN_GetServerInfo_1_02;
			trap_LAN_GetServerPing				= trap_LAN_GetServerPing_1_02;
			trap_LAN_ServerStatus				= trap_LAN_ServerStatus_1_02;
			trap_LAN_SaveCachedServers			= trap_LAN_SaveCachedServers_1_02;
			trap_LAN_LoadCachedServers			= trap_LAN_LoadCachedServers_1_02;
			trap_LAN_ResetPings					= trap_LAN_ResetPings_1_02;
			trap_LAN_MarkServerVisible			= trap_LAN_MarkServerVisible_1_02;
			trap_LAN_ServerIsVisible			= trap_LAN_ServerIsVisible_1_02;
			trap_LAN_UpdateVisiblePings			= trap_LAN_UpdateVisiblePings_1_02;
			trap_LAN_AddServer					= trap_LAN_AddServer_1_02;
			trap_LAN_RemoveServer				= trap_LAN_RemoveServer_1_02;
			trap_LAN_CompareServers				= trap_LAN_CompareServers_1_02;
			trap_PC_AddGlobalDefine				= trap_PC_AddGlobalDefine_1_02;
			trap_PC_LoadSource					= trap_PC_LoadSource_1_02;
			trap_PC_FreeSource					= trap_PC_FreeSource_1_02;
			trap_PC_ReadToken					= trap_PC_ReadToken_1_02;
			trap_PC_SourceFileAndLine			= trap_PC_SourceFileAndLine_1_02;
			trap_PC_LoadGlobalDefines			= trap_PC_LoadGlobalDefines_1_02;

			trap_PC_RemoveAllGlobalDefines		= trap_PC_RemoveAllGlobalDefines_1_02;

			trap_S_StopBackgroundTrack			= trap_S_StopBackgroundTrack_1_02;
			trap_S_StartBackgroundTrack			= trap_S_StartBackgroundTrack_1_02;
			trap_RealTime						= trap_RealTime_1_02;
			trap_CIN_PlayCinematic				= trap_CIN_PlayCinematic_1_02;
			trap_CIN_StopCinematic				= trap_CIN_StopCinematic_1_02;
			trap_CIN_RunCinematic				= trap_CIN_RunCinematic_1_02;
			trap_CIN_DrawCinematic				= trap_CIN_DrawCinematic_1_02;
			trap_CIN_SetExtents					= trap_CIN_SetExtents_1_02;
			trap_R_RemapShader					= trap_R_RemapShader_1_02;
			break;
		case VERSION_1_03:
		case VERSION_1_04:
		default:
			trap_Language_IsAsian				= trap_Language_IsAsian_1_04;
			trap_Language_UsesSpaces			= trap_Language_UsesSpaces_1_04;
			/*trap_AnyLanguage_ReadCharFromString	= trap_AnyLanguage_ReadCharFromString_1_04;*/

			trap_R_ModelBounds					= trap_R_ModelBounds_1_04;
			trap_LAN_GetServerCount				= trap_LAN_GetServerCount_1_04;
			trap_LAN_GetServerAddressString		= trap_LAN_GetServerAddressString_1_04;
			trap_LAN_GetServerInfo				= trap_LAN_GetServerInfo_1_04;
			trap_LAN_GetServerPing				= trap_LAN_GetServerPing_1_04;
			trap_LAN_ServerStatus				= trap_LAN_ServerStatus_1_04;
			trap_LAN_SaveCachedServers			= trap_LAN_SaveCachedServers_1_04;
			trap_LAN_LoadCachedServers			= trap_LAN_LoadCachedServers_1_04;
			trap_LAN_ResetPings					= trap_LAN_ResetPings_1_04;
			trap_LAN_MarkServerVisible			= trap_LAN_MarkServerVisible_1_04;
			trap_LAN_ServerIsVisible			= trap_LAN_ServerIsVisible_1_04;
			trap_LAN_UpdateVisiblePings			= trap_LAN_UpdateVisiblePings_1_04;
			trap_LAN_AddServer					= trap_LAN_AddServer_1_04;
			trap_LAN_RemoveServer				= trap_LAN_RemoveServer_1_04;
			trap_LAN_CompareServers				= trap_LAN_CompareServers_1_04;
			trap_PC_AddGlobalDefine				= trap_PC_AddGlobalDefine_1_04;
			trap_PC_LoadSource					= trap_PC_LoadSource_1_04;
			trap_PC_FreeSource					= trap_PC_FreeSource_1_04;
			trap_PC_ReadToken					= trap_PC_ReadToken_1_04;
			trap_PC_SourceFileAndLine			= trap_PC_SourceFileAndLine_1_04;
			trap_PC_LoadGlobalDefines			= trap_PC_LoadGlobalDefines_1_04;

			trap_PC_RemoveAllGlobalDefines		= trap_PC_RemoveAllGlobalDefines_1_04;

			trap_S_StopBackgroundTrack			= trap_S_StopBackgroundTrack_1_04;
			trap_S_StartBackgroundTrack			= trap_S_StartBackgroundTrack_1_04;
			trap_RealTime						= trap_RealTime_1_04;
			trap_CIN_PlayCinematic				= trap_CIN_PlayCinematic_1_04;
			trap_CIN_StopCinematic				= trap_CIN_StopCinematic_1_04;
			trap_CIN_RunCinematic				= trap_CIN_RunCinematic_1_04;
			trap_CIN_DrawCinematic				= trap_CIN_DrawCinematic_1_04;
			trap_CIN_SetExtents					= trap_CIN_SetExtents_1_04;
			trap_R_RemapShader					= trap_R_RemapShader_1_04;
			break;
	}

	MV_SetGamePlay( version ); // Default gameplay of that version...
}
