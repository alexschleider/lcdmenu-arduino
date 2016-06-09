#include "PChar.h"
PChar::PChar(const char b) {
	const char c = b;
	//length = strlen(c);
	//if(length > 0) {
		//const char tempchar = c[0];
		str_ptr = (__extension__({static const char __c PROGMEM = c; &__c;}));
		/*for(int i=1; i<length; i++) {
			//const char tempchar = c[i];
			static const char __c PROGMEM = b;
		}*/
	//}
}

char* PChar::get() {
	char ret[length];
	for(int i=0; i<length; i++) {
		ret[i] = pgm_read_byte_near(str_ptr + i);	
	}
	return ret;
}