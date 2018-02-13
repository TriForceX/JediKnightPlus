//cg_multiversion_syscalls.c

#include "cg_local.h"
#include "cg_multiversion_syscalls.h"

qboolean trap_Key_IsDown( int keynum )
{
	return trap_Key_IsDown_real( Key_GetProtocolKey(jk2version, keynum) );
}

int trap_Key_GetKey( const char *binding )
{
	return Key_GetProtocolKey15( jk2version, trap_Key_GetKey_real(binding) );
}
