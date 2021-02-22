#pragma once

#define IN_RANGE(x,a,b)    (x >= a && x <= b)
#define GET_BITS( x )    (IN_RANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (IN_RANGE(x,'0','9') ? x - '0' : 0))
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

typedef struct cached_modules_s {
	uintptr_t rangeStart;
	MODULEINFO modInfo;
} cached_modules_t;

class cutil {
public:
	uintptr_t load_module_wait( const char* szModuleName );
	uintptr_t find_pattern( const char* moduleName, const char* pattern );
	char* wstring_to_string( const wchar_t* wstr );
	wchar_t* string_to_wstring( const char* str );

	void save_config( std::string config_name );
	void load_config( std::string config_name );
	bool is_code_ptr( void* ptr );

private:
	std::map<const char*, cached_modules_t> cached_modules;
};

extern cutil util;