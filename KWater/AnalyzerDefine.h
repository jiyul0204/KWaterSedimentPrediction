#pragma once

#include <deque>

typedef struct _ExportArgs
{
	CString szFormat;
	std::deque<INT>* que;
} ExportArgs;
