//ui_multiversion_syscalls.h

void trap_Key_KeynumToStringBuf_real( int keynum, char *buf, int buflen );
void trap_Key_GetBindingBuf_real( int keynum, char *buf, int buflen );
void trap_Key_SetBinding_real( int keynum, const char *binding );
qboolean trap_Key_IsDown_real( int keynum );
