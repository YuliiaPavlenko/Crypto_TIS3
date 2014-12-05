#pragma once

typedef unsigned long long uint64;

union block64
{		
	uint64 data;
	struct
	{
		unsigned long lo;
		unsigned long hi;
	};
};