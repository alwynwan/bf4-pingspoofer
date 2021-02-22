#pragma once

struct vars_s
{
	vars_s() = default;

	struct misc_s
	{
		bool ping_spoof = false;
		int ping_value = 25;
	} misc;
};

extern vars_s vars;