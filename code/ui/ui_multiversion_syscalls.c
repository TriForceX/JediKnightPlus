//ui_multiversion_syscalls.c

#include "ui_local.h"
#include "ui_multiversion_syscalls.h"

void trap_Key_KeynumToStringBuf( int keynum, char *buf, int buflen )
{
	trap_Key_KeynumToStringBuf_real( Key_GetProtocolKey(jk2version, keynum), buf, buflen );
}

void trap_Key_GetBindingBuf( int keynum, char *buf, int buflen )
{
	trap_Key_GetBindingBuf_real( Key_GetProtocolKey(jk2version, keynum), buf, buflen );
}

void trap_Key_SetBinding( int keynum, const char *binding )
{
	trap_Key_SetBinding_real( Key_GetProtocolKey(jk2version, keynum), binding );
}

qboolean trap_Key_IsDown( int keynum )
{
	return trap_Key_IsDown_real( Key_GetProtocolKey(jk2version, keynum) );
}
