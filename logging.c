#include "logging/logging.h"

#include <api/api.h>
#include <threading/threading.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

// === Forward Declarations ===

PRIVATE_FUNC(void, print_log(const char* prefix, const char* message));

// === Public API ===

void logging_log_message(const char* message)
{
	_print_log(NULL, message);
}

void logging_log_formatted(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	_print_log(NULL, buffer);
}

void logging_log_with_ID(const char* module_id, const char* function, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	char prefix[256];
	snprintf(prefix, sizeof(prefix), "%s: %s", module_id, function);
	_print_log(prefix, buffer);
}

// === Static Functions ===

static void _print_log(const char* prefix, const char* message)
{
	static bool initialized = false;
	static threading_critical_section cs = {0};

	if (!initialized)
	{
		threading_critical_section_initialize(&cs);
		initialized = true;
	}

	threading_critical_section_lock(&cs);
	if (strlen(message) > 0)
	{
		static long first_time_ns = 0.0;
		struct timespec ts = {0};
		clock_gettime(0, &ts);
		int64_t time_ns = ts.tv_sec * 1.0e9 + ts.tv_nsec;
		first_time_ns = first_time_ns == 0.0 ? time_ns : first_time_ns;
		int64_t diff_ns = time_ns - first_time_ns;

		if (prefix)
		{
			printf("[%020ld]: %s: %s\n", diff_ns, prefix, message);
		}
		else
		{
			printf("[%020ld]: %s\n", diff_ns, message);
		}
		fflush(stdout);
	}
	threading_critical_section_unlock(&cs);
}