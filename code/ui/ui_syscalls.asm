code

equ trap_Print								  -2	; UI_PRINT 
equ trap_Error								  -1	; UI_ERROR 
equ trap_Milliseconds						  -3	; UI_MILLISECONDS 
equ trap_Cvar_Register						 -51	; UI_CVAR_REGISTER 
equ trap_Cvar_Update						 -52	; UI_CVAR_UPDATE 
equ trap_Cvar_Set							  -4	; UI_CVAR_SET 
equ trap_Cvar_VariableValue					  -5	; UI_CVAR_VARIABLEVALUE 
equ trap_Cvar_VariableStringBuffer			  -6	; UI_CVAR_VARIABLESTRINGBUFFER 
equ trap_Cvar_SetValue						  -7	; UI_CVAR_SETVALUE 
equ trap_Cvar_Reset							  -8	; UI_CVAR_RESET 
equ trap_Cvar_Create						  -9	; UI_CVAR_CREATE 
equ trap_Cvar_InfoStringBuffer				 -10	; UI_CVAR_INFOSTRINGBUFFER 
equ trap_Argc								 -11	; UI_ARGC 
equ trap_Argv								 -12	; UI_ARGV 
equ trap_Cmd_ExecuteText					 -13	; UI_CMD_EXECUTETEXT 
equ trap_FS_FOpenFile						 -14	; UI_FS_FOPENFILE 
equ trap_FS_Read							 -15	; UI_FS_READ 
equ trap_FS_Write							 -16	; UI_FS_WRITE 
equ trap_FS_FCloseFile						 -17	; UI_FS_FCLOSEFILE 
equ trap_FS_GetFileList						 -18	; UI_FS_GETFILELIST 
equ trap_R_RegisterModel					 -19	; UI_R_REGISTERMODEL 
equ trap_R_RegisterSkin						 -20	; UI_R_REGISTERSKIN 
equ trap_R_RegisterFont						 -57	; UI_R_REGISTERFONT 
equ trap_R_Font_StrLenPixels				 -58	; UI_R_FONT_STRLENPIXELS 
equ trap_R_Font_StrLenChars					 -59	; UI_R_FONT_STRLENCHARS 
equ trap_R_Font_HeightPixels				 -60	; UI_R_FONT_STRHEIGHTPIXELS 
equ trap_R_Font_DrawString					 -61	; UI_R_FONT_DRAWSTRING 

;1.04
equ trap_Language_IsAsian_1_04				 -62	; UI_LANGUAGE_ISASIAN 
equ trap_Language_UsesSpaces_1_04			 -63	; UI_LANGUAGE_USESSPACES 
equ trap_AnyLanguage_ReadCharFromString_1_04 -64	; UI_ANYLANGUAGE_READCHARFROMSTRING 

;1.02
equ trap_AnyLanguage_ReadCharFromString_1_02 -62	; UI_ANYLANGUAGE_READCHARFROMSTRING 

equ trap_R_RegisterShaderNoMip				 -21	; UI_R_REGISTERSHADERNOMIP 
equ trap_R_ClearScene						 -22	; UI_R_CLEARSCENE 
equ trap_R_AddRefEntityToScene				 -23	; UI_R_ADDREFENTITYTOSCENE 
equ trap_R_AddPolyToScene					 -24	; UI_R_ADDPOLYTOSCENE 
equ trap_R_AddLightToScene					 -25	; UI_R_ADDLIGHTTOSCENE 
equ trap_R_RenderScene						 -26	; UI_R_RENDERSCENE 
equ trap_R_SetColor							 -27	; UI_R_SETCOLOR 
equ trap_R_DrawStretchPic					 -28	; UI_R_DRAWSTRETCHPIC 

;1.04
equ trap_R_ModelBounds_1_04					 -65	; UI_R_MODELBOUNDS 

;1.02
equ trap_R_ModelBounds_1_02					 -63	; UI_R_MODELBOUNDS 

equ trap_UpdateScreen						 -29	; UI_UPDATESCREEN 
equ trap_CM_LerpTag							 -30	; UI_CM_LERPTAG 
equ trap_S_StartLocalSound					 -33	; UI_S_STARTLOCALSOUND 
equ trap_S_RegisterSound					 -32	; UI_S_REGISTERSOUND 
equ trap_Key_KeynumToStringBuf_real			 -34	; UI_KEY_KEYNUMTOSTRINGBUF 
equ trap_Key_GetBindingBuf_real				 -35	; UI_KEY_GETBINDINGBUF 
equ trap_Key_SetBinding_real				 -36	; UI_KEY_SETBINDING 
equ trap_Key_IsDown_real					 -37	; UI_KEY_ISDOWN 
equ trap_Key_GetOverstrikeMode				 -38	; UI_KEY_GETOVERSTRIKEMODE 
equ trap_Key_SetOverstrikeMode				 -39	; UI_KEY_SETOVERSTRIKEMODE 
equ trap_Key_ClearStates					 -40	; UI_KEY_CLEARSTATES 
equ trap_Key_GetCatcher						 -41	; UI_KEY_GETCATCHER 
equ trap_Key_SetCatcher						 -42	; UI_KEY_SETCATCHER 
equ trap_GetClipboardData					 -43	; UI_GETCLIPBOARDDATA 
equ trap_GetClientState						 -45	; UI_GETCLIENTSTATE 
equ trap_GetGlconfig						 -44	; UI_GETGLCONFIG 
equ trap_GetConfigString					 -46	; UI_GETCONFIGSTRING 

;1.04
equ trap_LAN_GetServerCount_1_04			 -76	; UI_LAN_GETSERVERCOUNT 
equ trap_LAN_GetServerAddressString_1_04	 -77	; UI_LAN_GETSERVERADDRESSSTRING 
equ trap_LAN_GetServerInfo_1_04				 -78	; UI_LAN_GETSERVERINFO 
equ trap_LAN_GetServerPing_1_04				 -93	; UI_LAN_GETSERVERPING 

;1.02
equ trap_LAN_GetServerCount_1_02			 -74	; UI_LAN_GETSERVERCOUNT 
equ trap_LAN_GetServerAddressString_1_02	 -75	; UI_LAN_GETSERVERADDRESSSTRING 
equ trap_LAN_GetServerInfo_1_02				 -76	; UI_LAN_GETSERVERINFO 
equ trap_LAN_GetServerPing_1_02				 -91	; UI_LAN_GETSERVERPING 

equ trap_LAN_GetPingQueueCount				 -47	; UI_LAN_GETPINGQUEUECOUNT 

;1.04
equ trap_LAN_ServerStatus_1_04				 -92	; UI_LAN_SERVERSTATUS 
equ trap_LAN_SaveCachedServers_1_04			 -83	; UI_LAN_SAVECACHEDSERVERS 
equ trap_LAN_LoadCachedServers_1_04			 -82	; UI_LAN_LOADCACHEDSERVERS 
equ trap_LAN_ResetPings_1_04				 -81	; UI_LAN_RESETPINGS 

;1.02
equ trap_LAN_ServerStatus_1_02				 -90	; UI_LAN_SERVERSTATUS 
equ trap_LAN_SaveCachedServers_1_02			 -81	; UI_LAN_SAVECACHEDSERVERS 
equ trap_LAN_LoadCachedServers_1_02			 -80	; UI_LAN_LOADCACHEDSERVERS 
equ trap_LAN_ResetPings_1_02				 -79	; UI_LAN_RESETPINGS 

equ trap_LAN_ClearPing						 -48	; UI_LAN_CLEARPING 
equ trap_LAN_GetPing						 -49	; UI_LAN_GETPING 
equ trap_LAN_GetPingInfo					 -50	; UI_LAN_GETPINGINFO 

;1.04
equ trap_LAN_MarkServerVisible_1_04			 -79	; UI_LAN_MARKSERVERVISIBLE 
equ trap_LAN_ServerIsVisible_1_04			 -94	; UI_LAN_SERVERISVISIBLE 
equ trap_LAN_UpdateVisiblePings_1_04		 -80	; UI_LAN_UPDATEVISIBLEPINGS 
equ trap_LAN_AddServer_1_04					 -84	; UI_LAN_ADDSERVER 
equ trap_LAN_RemoveServer_1_04				 -85	; UI_LAN_REMOVESERVER 
equ trap_LAN_CompareServers_1_04			 -95	; UI_LAN_COMPARESERVERS 

;1.02
equ trap_LAN_MarkServerVisible_1_02			 -77	; UI_LAN_MARKSERVERVISIBLE 
equ trap_LAN_ServerIsVisible_1_02			 -92	; UI_LAN_SERVERISVISIBLE 
equ trap_LAN_UpdateVisiblePings_1_02		 -78	; UI_LAN_UPDATEVISIBLEPINGS 
equ trap_LAN_AddServer_1_02					 -82	; UI_LAN_ADDSERVER 
equ trap_LAN_RemoveServer_1_02				 -83	; UI_LAN_REMOVESERVER 
equ trap_LAN_CompareServers_1_02			 -93	; UI_LAN_COMPARESERVERS 

equ trap_MemoryRemaining					 -53	; UI_MEMORY_REMAINING 
equ trap_GetCDKey							 -54	; UI_GET_CDKEY 
equ trap_SetCDKey							 -55	; UI_SET_CDKEY 
equ trap_VerifyCDKey						 -56	; UI_VERIFY_CDKEY 

;1.04
equ trap_PC_AddGlobalDefine_1_04			 -66	; UI_PC_ADD_GLOBAL_DEFINE 
equ trap_PC_LoadSource_1_04					 -67	; UI_PC_LOAD_SOURCE 
equ trap_PC_FreeSource_1_04					 -68	; UI_PC_FREE_SOURCE 
equ trap_PC_ReadToken_1_04					 -69	; UI_PC_READ_TOKEN 
equ trap_PC_SourceFileAndLine_1_04			 -70	; UI_PC_SOURCE_FILE_AND_LINE 
equ trap_PC_LoadGlobalDefines_1_04			 -71	; UI_PC_LOAD_GLOBAL_DEFINES 
equ trap_PC_RemoveAllGlobalDefines_1_04		 -72	; UI_PC_REMOVE_ALL_GLOBAL_DEFINES 
equ trap_S_StopBackgroundTrack_1_04			 -73	; UI_S_STOPBACKGROUNDTRACK 
equ trap_S_StartBackgroundTrack_1_04		 -74	; UI_S_STARTBACKGROUNDTRACK 
equ trap_RealTime_1_04						 -75	; UI_REAL_TIME 
equ trap_CIN_PlayCinematic_1_04				 -86	; UI_CIN_PLAYCINEMATIC 
equ trap_CIN_StopCinematic_1_04				 -87	; UI_CIN_STOPCINEMATIC 
equ trap_CIN_RunCinematic_1_04				 -88	; UI_CIN_RUNCINEMATIC 
equ trap_CIN_DrawCinematic_1_04				 -89	; UI_CIN_DRAWCINEMATIC 
equ trap_CIN_SetExtents_1_04				 -90	; UI_CIN_SETEXTENTS 
equ trap_R_RemapShader_1_04					 -91	; UI_R_REMAP_SHADER 

;1.02
equ trap_PC_AddGlobalDefine_1_02			 -64	; UI_PC_ADD_GLOBAL_DEFINE 
equ trap_PC_LoadSource_1_02					 -65	; UI_PC_LOAD_SOURCE 
equ trap_PC_FreeSource_1_02					 -66	; UI_PC_FREE_SOURCE 
equ trap_PC_ReadToken_1_02					 -67	; UI_PC_READ_TOKEN 
equ trap_PC_SourceFileAndLine_1_02			 -68	; UI_PC_SOURCE_FILE_AND_LINE 
equ trap_PC_LoadGlobalDefines_1_02			 -69	; UI_PC_LOAD_GLOBAL_DEFINES 
equ trap_PC_RemoveAllGlobalDefines_1_02		 -70	; UI_PC_REMOVE_ALL_GLOBAL_DEFINES 
equ trap_S_StopBackgroundTrack_1_02			 -71	; UI_S_STOPBACKGROUNDTRACK 
equ trap_S_StartBackgroundTrack_1_02		 -72	; UI_S_STARTBACKGROUNDTRACK 
equ trap_RealTime_1_02						 -73	; UI_REAL_TIME 
equ trap_CIN_PlayCinematic_1_02				 -84	; UI_CIN_PLAYCINEMATIC 
equ trap_CIN_StopCinematic_1_02				 -85	; UI_CIN_STOPCINEMATIC 
equ trap_CIN_RunCinematic_1_02				 -86	; UI_CIN_RUNCINEMATIC 
equ trap_CIN_DrawCinematic_1_02				 -87	; UI_CIN_DRAWCINEMATIC 
equ trap_CIN_SetExtents_1_02				 -88	; UI_CIN_SETEXTENTS 
equ trap_R_RemapShader_1_02					 -89	; UI_R_REMAP_SHADER 

equ trap_SP_Register						-201	; UI_SP_REGISTER 
equ trap_SP_GetStringTextString				-202	; UI_SP_GETSTRINGTEXTSTRING 
equ trap_G2API_SetBoneAngles				-203	; UI_G2_ANGLEOVERRIDE 


; hardcoded functions
equ memset									-101	; UI_MEMSET 
equ memcpy									-102	; UI_MEMCPY 
equ strncpy									-103	; UI_STRNCPY 
equ sin										-104	; UI_SIN 
equ cos										-105	; UI_COS 
equ atan2									-106	; UI_ATAN2 
equ sqrt									-107	; UI_SQRT 
equ matrixmultiply							-116	; UI_MATRIXMULTIPLY 
equ anglevectors							-109	; UI_ANGLEVECTORS 
equ perpendicularvector						-110	; UI_PERPENDICULARVECTOR 
equ floor									-108	; UI_FLOOR 
equ ceil									-111	; UI_CEIL 
equ acos									-114	; UI_ACOS 
equ asin									-115	; UI_ASIN 


; JK2MV Syscalls [Universal]
equ trap_MVAPI_ControlFixes					-704	;MVAPI_CONTROL_FIXES
equ trap_MVAPI_GetVersion					-705    ;MVAPI_GET_VERSION
equ trap_FS_FLock							-709	;MVAPI_FS_FLOCK
equ trap_MVAPI_SetVersion					-710	;MVAPI_SET_VERSION
equ trap_MVAPI_Print						-714	;MVAPI_PRINT

; JK2MV Syscalls [UI]
equ trap_R_AddRefEntityToScene2				-707	;UI_MVAPI_R_ADDREFENTITYTOSCENE2
equ trap_MVAPI_SetVirtualScreen				-708	;UI_MVAPI_SETVIRTUALSCREEN

; JK2MV Syscalls [mvmenu]
equ trap_CL_ContinueCurrentDownload         -301    ; UI_MVAPI_CONTINUE_DOWNLOAD
equ trap_FS_GetDLList                       -302    ; UI_MVAPI_GETDLLIST
equ trap_FS_RMDLPrefix                      -303    ; UI_MVAPI_RMDLPREFIX
equ trap_UI_DeleteDLFile                    -304    ; UI_MVAPI_DELDLFILE
