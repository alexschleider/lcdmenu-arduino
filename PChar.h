#ifndef LCDMenu_PgmChar
#define LCDMenu_PgmChar
#include "WProgram.h"
#include <avr/pgmspace.h>
#include <avr/io.h>

class PChar {
	public:
		PChar(const char b);
		char* get();
	private:
		int length;
		const prog_char* str_ptr;
};

#endif