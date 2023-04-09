code

equ trap_Print									  -1	; CG_PRINT 
equ trap_Error									  -2	; CG_ERROR 
equ trap_Milliseconds							  -3	; CG_MILLISECONDS 
equ trap_Cvar_Register							  -4	; CG_CVAR_REGISTER 
equ trap_Cvar_Update							  -5	; CG_CVAR_UPDATE 
equ trap_Cvar_Set								  -6	; CG_CVAR_SET 
equ trap_Cvar_VariableStringBuffer				  -7	; CG_CVAR_VARIABLESTRINGBUFFER 
equ trap_Argc									  -8	; CG_ARGC 
equ trap_Argv									  -9	; CG_ARGV 
equ trap_Args									 -10	; CG_ARGS 
equ trap_FS_FOpenFile							 -11	; CG_FS_FOPENFILE 
equ trap_FS_Read								 -12	; CG_FS_READ 
equ trap_FS_Write								 -13	; CG_FS_WRITE 
equ trap_FS_FCloseFile							 -14	; CG_FS_FCLOSEFILE 
equ trap_SendConsoleCommand						 -15	; CG_SENDCONSOLECOMMAND 
equ trap_AddCommand								 -16	; CG_ADDCOMMAND 
equ trap_RemoveCommand							 -17	; CG_REMOVECOMMAND 
equ trap_SendClientCommand						 -18	; CG_SENDCLIENTCOMMAND 
equ trap_UpdateScreen							 -19	; CG_UPDATESCREEN 
equ trap_CM_LoadMap								 -20	; CG_CM_LOADMAP 
equ trap_CM_NumInlineModels						 -21	; CG_CM_NUMINLINEMODELS 
equ trap_CM_InlineModel							 -22	; CG_CM_INLINEMODEL 
equ trap_CM_TempBoxModel						 -23	; CG_CM_TEMPBOXMODEL 
equ trap_CM_TempCapsuleModel					 -24	; CG_CM_TEMPCAPSULEMODEL 
equ trap_CM_PointContents						 -25	; CG_CM_POINTCONTENTS 
equ trap_CM_TransformedPointContents			 -26	; CG_CM_TRANSFORMEDPOINTCONTENTS 
equ trap_CM_BoxTrace							 -27	; CG_CM_BOXTRACE 
equ trap_CM_CapsuleTrace						 -28	; CG_CM_CAPSULETRACE 
equ trap_CM_TransformedBoxTrace					 -29	; CG_CM_TRANSFORMEDBOXTRACE 
equ trap_CM_TransformedCapsuleTrace				 -30	; CG_CM_TRANSFORMEDCAPSULETRACE 
equ trap_CM_MarkFragments						 -31	; CG_CM_MARKFRAGMENTS 
equ trap_S_MuteSound							 -32	; CG_S_MUTESOUND 
equ trap_S_StartSound							 -33	; CG_S_STARTSOUND 
equ trap_S_StartLocalSound						 -34	; CG_S_STARTLOCALSOUND 
equ trap_S_ClearLoopingSounds					 -35	; CG_S_CLEARLOOPINGSOUNDS 
equ trap_S_AddLoopingSound						 -36	; CG_S_ADDLOOPINGSOUND 
equ trap_S_UpdateEntityPosition					 -37	; CG_S_UPDATEENTITYPOSITION 
equ trap_S_AddRealLoopingSound					 -38	; CG_S_ADDREALLOOPINGSOUND 
equ trap_S_StopLoopingSound						 -39	; CG_S_STOPLOOPINGSOUND 
equ trap_S_Respatialize							 -40	; CG_S_RESPATIALIZE 
equ trap_S_RegisterSound						 -41	; CG_S_REGISTERSOUND 
equ trap_S_StartBackgroundTrack					 -42	; CG_S_STARTBACKGROUNDTRACK 
equ trap_R_LoadWorldMap							 -43	; CG_R_LOADWORLDMAP 
equ trap_R_RegisterModel						 -44	; CG_R_REGISTERMODEL 
equ trap_R_RegisterSkin							 -45	; CG_R_REGISTERSKIN 
equ trap_R_RegisterShader						 -46	; CG_R_REGISTERSHADER 
equ trap_R_RegisterShaderNoMip					 -47	; CG_R_REGISTERSHADERNOMIP 
equ trap_R_RegisterFont							 -48	; CG_R_REGISTERFONT 
equ trap_R_Font_StrLenPixels					 -49	; CG_R_FONT_STRLENPIXELS 
equ trap_R_Font_StrLenChars						 -50	; CG_R_FONT_STRLENCHARS 
equ trap_R_Font_HeightPixels					 -51	; CG_R_FONT_STRHEIGHTPIXELS 
equ trap_R_Font_DrawString						 -52	; CG_R_FONT_DRAWSTRING 

equ trap_Language_IsAsian_1_04					 -53	; CG_LANGUAGE_ISASIAN 
equ trap_Language_UsesSpaces_1_04				 -54	; CG_LANGUAGE_USESSPACES 
equ trap_AnyLanguage_ReadCharFromString_1_04	 -55	; CG_ANYLANGUAGE_READCHARFROMSTRING 
equ trap_AnyLanguage_ReadCharFromString_1_02	 -53	; CG_ANYLANGUAGE_READCHARFROMSTRING 

equ trap_R_ClearScene							-201	; CG_R_CLEARSCENE 
equ trap_R_AddRefEntityToScene					-202	; CG_R_ADDREFENTITYTOSCENE 
equ trap_R_AddPolyToScene						-203	; CG_R_ADDPOLYTOSCENE 
equ trap_R_AddPolysToScene						-204	; CG_R_ADDPOLYSTOSCENE 
equ trap_R_LightForPoint						-205	; CG_R_LIGHTFORPOINT 
equ trap_R_AddLightToScene						-206	; CG_R_ADDLIGHTTOSCENE 
equ trap_R_AddAdditiveLightToScene				-207	; CG_R_ADDADDITIVELIGHTTOSCENE 
equ trap_R_RenderScene							-208	; CG_R_RENDERSCENE 
equ trap_R_SetColor								-209	; CG_R_SETCOLOR 
equ trap_R_DrawStretchPic						-210	; CG_R_DRAWSTRETCHPIC 
equ trap_R_ModelBounds							-211	; CG_R_MODELBOUNDS 
equ trap_R_LerpTag								-212	; CG_R_LERPTAG 
equ trap_R_DrawRotatePic						-213	; CG_R_DRAWROTATEPIC 
equ trap_R_DrawRotatePic2						-214	; CG_R_DRAWROTATEPIC2 
equ trap_R_RemapShader							-215	; CG_R_REMAP_SHADER 
equ trap_R_GetLightStyle						-216	; CG_R_GET_LIGHT_STYLE 
equ trap_R_SetLightStyle						-217	; CG_R_SET_LIGHT_STYLE 
equ trap_R_GetBModelVerts						-218	; CG_R_GET_BMODEL_VERTS 
equ trap_FX_AddLine								-219	; CG_FX_ADDLINE 
equ trap_GetGlconfig							-220	; CG_GETGLCONFIG 
equ trap_GetGameState							-221	; CG_GETGAMESTATE 
equ trap_GetCurrentSnapshotNumber				-222	; CG_GETCURRENTSNAPSHOTNUMBER 
equ trap_GetSnapshot							-223	; CG_GETSNAPSHOT 
equ trap_GetServerCommand						-224	; CG_GETSERVERCOMMAND 
equ trap_GetCurrentCmdNumber					-225	; CG_GETCURRENTCMDNUMBER 
equ trap_GetUserCmd								-226	; CG_GETUSERCMD 
equ trap_SetUserCmdValue						-227	; CG_SETUSERCMDVALUE 
equ trap_SetClientForceAngle					-228	; CG_SETCLIENTFORCEANGLE 
equ trap_SetClientTurnExtent					-229	; CG_SETCLIENTTURNEXTENT 
equ trap_OpenUIMenu								-230	; CG_OPENUIMENU 
equ trap_MemoryRemaining						-233	; CG_MEMORY_REMAINING 
equ trap_Key_IsDown_real						-234	; CG_KEY_ISDOWN 
equ trap_Key_GetCatcher							-235	; CG_KEY_GETCATCHER 
equ trap_Key_SetCatcher							-236	; CG_KEY_SETCATCHER 
equ trap_Key_GetKey_real						-237	; CG_KEY_GETKEY 
equ trap_PC_AddGlobalDefine						-238	; CG_PC_ADD_GLOBAL_DEFINE 
equ trap_PC_LoadSource							-239	; CG_PC_LOAD_SOURCE 
equ trap_PC_FreeSource							-240	; CG_PC_FREE_SOURCE 
equ trap_PC_ReadToken							-241	; CG_PC_READ_TOKEN 
equ trap_PC_SourceFileAndLine					-242	; CG_PC_SOURCE_FILE_AND_LINE 
equ trap_PC_LoadGlobalDefines					-243	; CG_PC_LOAD_GLOBAL_DEFINES 
equ trap_PC_RemoveAllGlobalDefines				-244	; CG_PC_REMOVE_ALL_GLOBAL_DEFINES 
equ trap_S_StopBackgroundTrack					-245	; CG_S_STOPBACKGROUNDTRACK 
equ trap_RealTime								-246	; CG_REAL_TIME 
equ trap_SnapVector								-247	; CG_SNAPVECTOR 
equ trap_CIN_PlayCinematic						-248	; CG_CIN_PLAYCINEMATIC 
equ trap_CIN_StopCinematic						-249	; CG_CIN_STOPCINEMATIC 
equ trap_CIN_RunCinematic						-250	; CG_CIN_RUNCINEMATIC 
equ trap_CIN_DrawCinematic						-251	; CG_CIN_DRAWCINEMATIC 
equ trap_CIN_SetExtents							-252	; CG_CIN_SETEXTENTS 
equ trap_GetEntityToken							-253	; CG_GET_ENTITY_TOKEN 
equ trap_R_inPVS								-254	; CG_R_INPVS 
equ trap_FX_RegisterEffect						-255	; CG_FX_REGISTER_EFFECT 
equ trap_FX_PlaySimpleEffect					-256	; CG_FX_PLAY_SIMPLE_EFFECT 
equ trap_FX_PlayEffect							-257	; CG_FX_PLAY_EFFECT 
equ trap_FX_PlayEntityEffect					-258	; CG_FX_PLAY_ENTITY_EFFECT 
equ trap_FX_PlaySimpleEffectID					-259	; CG_FX_PLAY_SIMPLE_EFFECT_ID 
equ trap_FX_PlayEffectID						-260	; CG_FX_PLAY_EFFECT_ID 
equ trap_FX_PlayEntityEffectID					-261	; CG_FX_PLAY_ENTITY_EFFECT_ID 
equ trap_FX_PlayBoltedEffectID					-262	; CG_FX_PLAY_BOLTED_EFFECT_ID 
equ trap_FX_AddScheduledEffects					-263	; CG_FX_ADD_SCHEDULED_EFFECTS 
equ trap_FX_InitSystem							-264	; CG_FX_INIT_SYSTEM 
equ trap_FX_FreeSystem							-265	; CG_FX_FREE_SYSTEM 
equ trap_FX_AdjustTime							-266	; CG_FX_ADJUST_TIME 
equ trap_FX_AddPoly								-267	; CG_FX_ADDPOLY 
equ trap_FX_AddBezier							-268	; CG_FX_ADDBEZIER 
equ trap_FX_AddPrimitive						-269	; CG_FX_ADDPRIMITIVE 
equ trap_FX_AddSprite							-270	; CG_FX_ADDSPRITE 
equ trap_SP_Print								-271	; CG_SP_PRINT 
equ trap_SP_GetStringTextString					-272	; CG_SP_GETSTRINGTEXTSTRING 
equ trap_SP_Register							-273	; CG_SP_REGISTER 
equ trap_ROFF_Clean								-274	; CG_ROFF_CLEAN 
equ trap_ROFF_UpdateEntities					-275	; CG_ROFF_UPDATE_ENTITIES 
equ trap_ROFF_Cache								-276	; CG_ROFF_CACHE 
equ trap_ROFF_Play								-277	; CG_ROFF_PLAY 
equ trap_ROFF_Purge_Ent							-278	; CG_ROFF_PURGE_ENT 
equ trap_G2_ListModelSurfaces					-279	; CG_G2_LISTSURFACES 
equ trap_G2_ListModelBones						-280	; CG_G2_LISTBONES 
equ trap_G2_SetGhoul2ModelIndexes				-281	; CG_G2_SETMODELS 
equ trap_G2_HaveWeGhoul2Models					-282	; CG_G2_HAVEWEGHOULMODELS 
equ trap_G2API_GiveMeVectorFromMatrix			-283	; CG_G2_GIVEMEVECTORFROMMATRIX 
equ trap_G2API_GetBoltMatrix					-284	; CG_G2_GETBOLT 
equ trap_G2API_GetBoltMatrix_NoReconstruct		-285	; CG_G2_GETBOLT_NOREC 

equ trap_G2API_GetBoltMatrix_NoRecNoRot_1_04	-286	; CG_G2_GETBOLT_NOREC_NOROT 
equ trap_G2API_InitGhoul2Model_1_04				-287	; CG_G2_INITGHOUL2MODEL 
equ trap_G2API_CollisionDetect_1_04				-288	; CG_G2_COLLISIONDETECT 
equ trap_G2API_CleanGhoul2Models_1_04			-289	; CG_G2_CLEANMODELS 
equ trap_G2API_SetBoneAngles_1_04				-290	; CG_G2_ANGLEOVERRIDE 
equ trap_G2API_SetBoneAnim_1_04					-291	; CG_G2_PLAYANIM 
equ trap_G2API_GetGLAName_1_04					-292	; CG_G2_GETGLANAME 
equ trap_G2API_CopyGhoul2Instance_1_04			-293	; CG_G2_COPYGHOUL2INSTANCE 
equ trap_G2API_CopySpecificGhoul2Model_1_04		-294	; CG_G2_COPYSPECIFICGHOUL2MODEL 
equ trap_G2API_DuplicateGhoul2Instance_1_04		-295	; CG_G2_DUPLICATEGHOUL2INSTANCE 
equ trap_G2API_HasGhoul2ModelOnIndex_1_04		-296	; CG_G2_HASGHOUL2MODELONINDEX 
equ trap_G2API_RemoveGhoul2Model_1_04			-297	; CG_G2_REMOVEGHOUL2MODEL 
equ trap_G2API_AddBolt_1_04						-298	; CG_G2_ADDBOLT 
equ trap_G2API_SetBoltInfo_1_04					-299	; CG_G2_SETBOLTON 
equ trap_G2API_SetRootSurface_1_04				-300	; CG_G2_SETROOTSURFACE 
equ trap_G2API_SetSurfaceOnOff_1_04				-301	; CG_G2_SETSURFACEONOFF 
equ trap_G2API_SetNewOrigin_1_04				-302	; CG_G2_SETNEWORIGIN 
equ trap_CG_RegisterSharedMemory_1_04			-303	; CG_SET_SHARED_BUFFER 

equ trap_G2API_InitGhoul2Model_1_02				-286	; CG_G2_INITGHOUL2MODEL 
equ trap_G2API_CleanGhoul2Models_1_02			-287	; CG_G2_CLEANMODELS 
equ trap_G2API_SetBoneAngles_1_02				-288	; CG_G2_ANGLEOVERRIDE 
equ trap_G2API_SetBoneAnim_1_02					-289	; CG_G2_PLAYANIM 
equ trap_G2API_GetGLAName_1_02					-290	; CG_G2_GETGLANAME 
equ trap_G2API_CopyGhoul2Instance_1_02			-291	; CG_G2_COPYGHOUL2INSTANCE 
equ trap_G2API_CopySpecificGhoul2Model_1_02		-292	; CG_G2_COPYSPECIFICGHOUL2MODEL 
equ trap_G2API_DuplicateGhoul2Instance_1_02		-293	; CG_G2_DUPLICATEGHOUL2INSTANCE 
equ trap_G2API_HasGhoul2ModelOnIndex_1_02		-294	; CG_G2_HASGHOUL2MODELONINDEX 
equ trap_G2API_RemoveGhoul2Model_1_02			-295	; CG_G2_REMOVEGHOUL2MODEL 
equ trap_G2API_AddBolt_1_02						-296	; CG_G2_ADDBOLT 
equ trap_G2API_SetBoltInfo_1_02					-297	; CG_G2_SETBOLTON 
equ trap_G2API_SetRootSurface_1_02				-298	; CG_G2_SETROOTSURFACE 
equ trap_G2API_SetSurfaceOnOff_1_02				-299	; CG_G2_SETSURFACEONOFF 
equ trap_G2API_SetNewOrigin_1_02				-300	; CG_G2_SETNEWORIGIN 
equ trap_CG_RegisterSharedMemory_1_02			-301	; CG_SET_SHARED_BUFFER 


; hardcoded functions
equ memset										-101	; CGAME_MEMSET 
equ memcpy										-102	; CGAME_MEMCPY 
equ strncpy										-103	; CGAME_STRNCPY 
equ sin											-104	; CGAME_SIN 
equ cos											-105	; CGAME_COS 
equ atan2										-106	; CGAME_ATAN2 
equ sqrt										-107	; CGAME_SQRT 
equ matrixmultiply								-108	; CGAME_MATRIXMULTIPLY 
equ anglevectors								-109	; CGAME_ANGLEVECTORS 
equ perpendicularvector							-110	; CGAME_PERPENDICULARVECTOR 
equ floor										-111	; CGAME_FLOOR 
equ ceil										-112	; CGAME_CEIL 
equ acos										-113	; CGAME_ACOS 
equ asin										-114	; CGAME_ASIN 

; JK2MV Syscalls [Universal]
equ trap_MVAPI_ControlFixes						-704	;MVAPI_CONTROL_FIXES
equ trap_MVAPI_GetVersion						-705    ;MVAPI_GET_VERSION
equ trap_FS_FLock								-709	;MVAPI_FS_FLOCK
equ trap_MVAPI_SetVersion						-710	;MVAPI_SET_VERSION
equ trap_MVAPI_Print							-714	;MVAPI_PRINT

; JK2MV Syscalls [CGame]
equ trap_R_AddRefEntityToScene2					-707	;CG_MVAPI_R_ADDREFENTITYTOSCENE2
equ trap_MVAPI_SetVirtualScreen					-708	;CG_MVAPI_SETVIRTUALSCREEN
equ trap_MVAPI_EnableSubmodelBypass				-713	;CG_MVAPI_ENABLE_SUBMODELBYPASS
