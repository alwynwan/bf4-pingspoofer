#include "Misc/Common.h"
#include "PolyHook/PolyHook.hpp"
vars_s vars;

#define OFFSET_SETPING	0x140222750 //40 57 48 83 EC 30 48 C7 44 24 ? ? ? ? ? 48 89 5C 24 ? 48 89 74 24 ? 8B F2 48 8B F9 33 DB

PLH::Detour* set_ping_detour;
typedef int( __fastcall* set_ping_fn )( __int64 a1, unsigned int ping );
set_ping_fn o_set_ping;

int __fastcall hk_set_ping( __int64 a1, unsigned int ping ) {
	return o_set_ping( a1, vars.misc.ping_spoof ? vars.misc.ping_value : ping );
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD Reason, LPVOID lpReserved ) {
	if ( Reason == DLL_PROCESS_ATTACH ) {
		DisableThreadLibraryCalls( hModule );

		set_ping_detour = new PLH::Detour( );
		set_ping_detour->SetupHook( reinterpret_cast< uint8_t* >( OFFSET_SETPING ), reinterpret_cast< uint8_t* >( &hk_set_ping ) );
		set_ping_detour->Hook( );
		o_set_ping = set_ping_detour->GetOriginal<set_ping_fn>( );
	}
	else if ( Reason == DLL_PROCESS_DETACH ) {
		set_ping_detour->UnHook( );
	}

	return true;
}