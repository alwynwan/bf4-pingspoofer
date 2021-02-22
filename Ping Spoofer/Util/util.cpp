#include "../Misc/Common.h"

cutil util;

uintptr_t cutil::load_module_wait( const char * szModuleName ) {
	uintptr_t module_handle = NULL;
	while ( !module_handle ) {
		module_handle = reinterpret_cast< uintptr_t >( GetModuleHandleA( szModuleName ) );
		if ( !module_handle )
			Sleep( 50 );
	}
	return module_handle;
}

uintptr_t cutil::find_pattern( const char* moduleName, const char* pattern ) {
	auto pat = pattern;
	uintptr_t first_match = 0;

	if ( cached_modules.find( moduleName ) == cached_modules.end( ) ) {
		cached_modules[ moduleName ].rangeStart = reinterpret_cast< uintptr_t >( GetModuleHandleA( moduleName ) );
		GetModuleInformation( GetCurrentProcess( ), reinterpret_cast< HMODULE >( cached_modules[ moduleName ].rangeStart ), &cached_modules[ moduleName ].modInfo, sizeof( MODULEINFO ) );
	}

	uintptr_t rangeEnd = cached_modules[ moduleName ].rangeStart + cached_modules[ moduleName ].modInfo.SizeOfImage;
	for ( uintptr_t pCur = cached_modules[ moduleName ].rangeStart; pCur < rangeEnd; pCur++ ) {
		if ( !*pat )
			return first_match;

		if ( *PBYTE( pat ) == '\?' || *reinterpret_cast< BYTE* >( pCur ) == GET_BYTE( pat ) ) {
			if ( !first_match )
				first_match = pCur;

			if ( !pat[ 2 ] )
				return first_match;

			if ( *PWORD( pat ) == '\?\?' || *( PBYTE ) pat != '\?' )
				pat += 3;

			else
				pat += 2; //one ?
		}
		else {
			pat = pattern;
			first_match = 0;
		}
	}
	return NULL;
}

char* cutil::wstring_to_string( const wchar_t* wstr ) {
	auto length = wcslen( wstr );
	auto c = std::make_unique<char[ ]>( length + 1 );
	size_t converted = 0;
	wcstombs_s( &converted, c.get( ), length + 1, wstr, length );

	return c.get( );
}

wchar_t* cutil::string_to_wstring( const char* str )
{
	auto length = _mbstrlen( str );
	auto wc = std::make_unique<wchar_t[ ]>( length + 1 );
	size_t converted = 0;
	mbstowcs_s( &converted, wc.get( ), length + 1, str, length );

	return wc.get( );
}

void cutil::save_config( std::string config_name ) { }

void cutil::load_config( std::string config_name ) { }

bool cutil::is_code_ptr( void* ptr )
{
	constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

	MEMORY_BASIC_INFORMATION out;
	VirtualQuery( ptr, &out, sizeof out );

	return out.Type
		&& !( out.Protect & ( PAGE_GUARD | PAGE_NOACCESS ) )
		&& out.Protect & protect_flags;
}
